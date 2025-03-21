#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>

void write_dicom_tag(FILE *dicom,uint16_t group,uint16_t element,const char *vr,char *value);
void write_dicom_numeric(FILE *dicom,uint16_t group,uint16_t element,const char *vr,uint32_t value);
void write_pixel_data(FILE *dicom, uint8_t *data, int width, int height);
// DICOM Header Structure
struct DICOM_Header {
    char prefix[128];    // Empty 128 bytes
    char magic[4];       // "DICM"
};

// DICOM Tag Structure
struct DICOM_Tag {
    uint16_t group;
    uint16_t element;
    uint32_t length;
    char value[64];
};

uint8_t *convert_jpg_to_raw(const char *jpg_filename, int *width, int *height) {
    FILE *jpg = fopen(jpg_filename, "rb");
    if (!jpg) return NULL;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, jpg);
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_GRAYSCALE;
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;

    uint8_t *raw_data = malloc((*width) * (*height) * 2);  // Allocate 16-bit buffer
    uint8_t *buffer = malloc(*width);

    for (int row = 0; row < *height; row++) {
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        for (int col = 0; col < *width; col++) {
            //raw_data[row * (*width) + col] = buffer[col] << 8;  // Convert 8-bit to 16-bit
            uint16_t pixel_value = buffer[col];
            raw_data[2 * (row * (*width) + col)] = (uint8_t)(pixel_value & 0xFF);  // Low byte
            raw_data[2 * (row * (*width) + col) + 1] = (uint8_t)(pixel_value >> 8); 
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(jpg);
    free(buffer);
    
    return raw_data;
}

int main() {
    const char *jpg_filename = "depak.jpg";
    const char *dicom_filename = "output.dcm";

    int width, height;

    // Convert JPG to RAW grayscale
    uint8_t *raw_data = convert_jpg_to_raw(jpg_filename, &width, &height);
    if (!raw_data) {
        return -1;
    } else {
        printf("converted");
    }

    FILE *dicom = fopen(dicom_filename, "wb");
    if (!dicom) {
        printf("Error: Cannot create DICOM file.\n");
        free(raw_data);
        return -1;
    }

    // Create and Write DICOM Header
    struct DICOM_Header head ={0};
    memcpy(head.magic,"DICM",4);
    fwrite(&head, sizeof(head), 1, dicom);

    // Add Metadata Tags
    uint32_t metalen = 0;
    long metalen_pos = ftell(dicom);

    write_dicom_numeric(dicom,0x0002,0x0000,"UL",metalen);
    write_dicom_tag(dicom,0x0002,0x0001,"OB","\x00\x01");
    // Change this in your code
    write_dicom_tag(dicom, 0x0002, 0x0002, "UI", "1.2.840.10008.5.1.4.1.1.7");  // Secondary Capture Image Storage 
    write_dicom_tag(dicom,0x0002,0x0003,"UI","1.2.840.10008.1.2.1");
    write_dicom_tag(dicom,0x0002,0x0010,"UI","1.2.840.10008.1.2.1");
    write_dicom_tag(dicom,0x0002,0x0013,"SH","ACSIA_TECH");
    // Add this before writing pixel data
    // Secondary Capture Image Storage

    long current_pos = ftell(dicom);
    metalen = current_pos - metalen_pos - 12; // 12 bytes for the tag, VR, and length field
    fseek(dicom, metalen_pos + 8, SEEK_SET); // moving +8 pos to skip tag and VR
    fwrite(&metalen, 4, 1, dicom);
    fseek(dicom, current_pos, SEEK_SET);

    printf("wrote meta data");
    write_dicom_tag(dicom, 0x0008, 0x0016, "UI", "1.2.840.10008.5.1.4.1.1.7"); 
    write_dicom_tag(dicom, 0x0008, 0x0060,"CS", "OT");  
    write_dicom_tag(dicom, 0x0010, 0x0010,"PN", "BLAZE");  
    write_dicom_tag(dicom, 0x0010, 0x0020,"LO", "C332");
    
// Add this before writing pixel data
    write_dicom_numeric(dicom, 0x0028, 0x0002, "US", 1); 
    write_dicom_tag(dicom, 0x0028, 0x0004, "CS", "MONOCHROME2"); 
    write_dicom_numeric(dicom,0x0028,0x0010,"US",height);   //rows
    write_dicom_numeric(dicom,0x0028,0x0011,"US",width);     //cols
   // write_dicom_tag(dicom,0x7FE0,0x0010,"OW",raw_data); 

     // Add these before writing pixel data
          // Samples per Pixel
     write_dicom_numeric(dicom, 0x0028, 0x0100, "US", 16);     // Bits Allocated
     write_dicom_numeric(dicom, 0x0028, 0x0101, "US", 16);     // Bits Stored
     write_dicom_numeric(dicom, 0x0028, 0x0102, "US", 15);     // High Bit
     write_dicom_numeric(dicom, 0x0028, 0x0103, "US", 0); 
                                                                   // Pixel Representation (0=unsigned)
     write_pixel_data(dicom, raw_data, width, height);
    
   
    fclose(dicom);
    free(raw_data);

    printf("DICOM file created: %s\n", dicom_filename);
    return 0;
}
void write_dicom_tag(FILE *dicom,uint16_t group,uint16_t element,const char *vr,char *value){
    uint16_t len = strlen(value);
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    fwrite(vr, 2, 1, dicom);
    if (strcmp(vr, "OB") == 0 || strcmp(vr, "OW") == 0 || strcmp(vr, "OF") == 0 || 
        strcmp(vr, "SQ") == 0 || strcmp(vr, "UT") == 0 || strcmp(vr, "UN") == 0) {
        uint16_t reserved = 0;
        uint32_t length32 = len;
        fwrite(&reserved, 2, 1, dicom);
        fwrite(&length32, 4, 1, dicom);
    } else {
        //Other VRs have 2-byte length
        fwrite(&len, 2, 1, dicom);
    }
    fwrite(value, len, 1, dicom); 
}
void write_dicom_numeric(FILE *dicom, uint16_t group, uint16_t element, const char *vr, uint32_t value) {
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    fwrite(vr, 2, 1, dicom);
    
    // For OB, OW, OF, SQ, UT, and UN, add reserved 2 bytes and 4-byte length
    if (strcmp(vr, "OB") == 0 || strcmp(vr, "OW") == 0 || strcmp(vr, "OF") == 0 || 
        strcmp(vr, "SQ") == 0 || strcmp(vr, "UT") == 0 || strcmp(vr, "UN") == 0) {
        uint16_t reserved = 0;
        uint32_t length32 = 4;
        fwrite(&reserved, 2, 1, dicom);
        fwrite(&length32, 4, 1, dicom);
    } else {
        // Determine correct length based on VR type
        uint16_t len = 4;  // Default for UL, FL, etc.
        
        if (strcmp(vr, "US") == 0 || strcmp(vr, "SS") == 0) {
            len = 2;  // 2 bytes for US and SS
        }
        
        fwrite(&len, 2, 1, dicom);
    }
    
    // Write the value with the correct size based on VR
    if (strcmp(vr, "US") == 0 || strcmp(vr, "SS") == 0) {
        uint16_t shortVal = (uint16_t)value;  // Cast to 16-bit for US/SS
        fwrite(&shortVal, 2, 1, dicom);
    } else {
        fwrite(&value, 4, 1, dicom);  // Write 4 bytes for other types
    }
}

void write_pixel_data(FILE *dicom, uint8_t *data, int width, int height) {
    // Write tag
    uint16_t group = 0x7FE0;
    uint16_t element = 0x0010;
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    
    // For 16-bit data, use OW value representation
    const char *vr = "OW";
    fwrite(vr, 2, 1, dicom);
    
    // Calculate data length - ensure it's a multiple of 2
    uint32_t data_length = width * height * 2; // 16-bit data (2 bytes per pixel)
    
    // Use explicit length encoding instead of undefined length
    uint16_t reserved = 0;
    fwrite(&reserved, 2, 1, dicom);
    fwrite(&data_length, 4, 1, dicom);
    
    // Write the actual pixel data
    fwrite(data, 1, data_length, dicom); // order of arguments: fwrite(ptr, size, count, stream)
    
    
    if (data_length % 2 != 0) {
        uint8_t padding = 0;
        fwrite(&padding, 1, 1, dicom);
    }
}