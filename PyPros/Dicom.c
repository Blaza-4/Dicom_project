#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>
//edited from linux
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

// Function to Write a DICOM Tag
void write_dicom_tag(FILE *file, uint16_t group, uint16_t element, const char *value) {
    uint32_t length = strlen(value);
    fwrite(&group, 2, 1, file);
    fwrite(&element, 2, 1, file);
    fwrite(&length, 4, 1, file);
    fwrite(value, length, 1, file);
}

// Function to Convert JPG to Grayscale and Get Raw Data
// uint8_t *convert_jpg_to_raw(const char *jpg_filename, int *width, int *height) {
//     FILE *jpg = fopen(jpg_filename, "rb");
// if (!jpg) {
//     printf("Error: Cannot open JPG file: %s\n", jpg_filename);
//     return NULL;
// }


//     struct jpeg_decompress_struct cinfo;
//     struct jpeg_error_mgr jerr;
//     cinfo.err = jpeg_std_error(&jerr);
//     jpeg_create_decompress(&cinfo);
//     jpeg_stdio_src(&cinfo, jpg);
//     jpeg_read_header(&cinfo, TRUE);
//     jpeg_start_decompress(&cinfo);

//     *width = cinfo.output_width;
//     *height = cinfo.output_height;
//     int row_stride = cinfo.output_width * cinfo.output_components;
//     uint8_t *raw_data = (uint8_t *)malloc((*width) * (*height));

//     uint8_t *buffer = (uint8_t *)malloc(row_stride);
//     int row = 0;

//     while (cinfo.output_scanline < cinfo.output_height) {
//         jpeg_read_scanlines(&cinfo, &buffer, 1);
//         for (int i = 0; i < *width; i++) {
//             uint8_t gray = (buffer[i * 3] + buffer[i * 3 + 1] + buffer[i * 3 + 2]) / 3;
//             raw_data[row * (*width) + i] = gray;
//         }
//         row++;
//     }

//     jpeg_finish_decompress(&cinfo);
//     jpeg_destroy_decompress(&cinfo);
//     fclose(jpg);
//     free(buffer);
    
//     return raw_data;
// }
uint16_t *convert_jpg_to_raw(const char *jpg_filename, int *width, int *height) {
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

    uint16_t *raw_data = malloc((*width) * (*height) * 2);  // Allocate 16-bit buffer
    uint8_t *buffer = malloc(*width);

    for (int row = 0; row < *height; row++) {
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        for (int col = 0; col < *width; col++) {
            raw_data[row * (*width) + col] = buffer[col] << 8;  // Convert 8-bit to 16-bit
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(jpg);
    free(buffer);
    
    return raw_data;
}

int main() {
    const char *jpg_filename = "test.jpg";
    const char *dicom_filename = "output.dcm";

    int width, height;

    // Convert JPG to RAW grayscale
    uint8_t *raw_data = convert_jpg_to_raw(jpg_filename, &width, &height);
    if (!raw_data) {
        return -1;
    }

    FILE *dicom = fopen(dicom_filename, "wb");
    if (!dicom) {
        printf("Error: Cannot create DICOM file.\n");
        free(raw_data);
        return -1;
    }

    // Create and Write DICOM Header
    struct DICOM_Header header = {0};
    strcpy(header.magic, "DICM");
    fwrite(&header, sizeof(header), 1, dicom);

    // Add Metadata Tags
    write_dicom_tag(dicom, 0x0008, 0x0060, "OT");  
    write_dicom_tag(dicom, 0x0010, 0x0010, "John Doe");  // Patient Name
    write_dicom_tag(dicom, 0x0010, 0x0020, "1234");  // Patient ID
    // Modality (OT = Other)
    
    // Pixel Data Tag (7FE0,0010)
    uint16_t pixel_data_group = 0x7FE0;
    uint16_t pixel_data_element = 0x0010;
    uint32_t pixel_data_length = width * height *2;
    
    fwrite(&pixel_data_group, 2, 1, dicom);
    fwrite(&pixel_data_element, 2, 1, dicom);
    fwrite("OW",2,1,dicom);
    fwrite("\x00\x00",2,1,dicom);

    fwrite(&pixel_data_length, 4, 1, dicom);

    // Append Pixel Data
    fwrite(raw_data, 2, width * height , dicom);

    if(pixel_data_length % 2 !=0){
        uint8_t pad =0;
        fwrite(&pad , 1 ,1, dicom);
    }

    fclose(dicom);
    free(raw_data);

    printf("DICOM file created: %s\n", dicom_filename);
    return 0;
}
