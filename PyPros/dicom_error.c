// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include <stdlib.h>
// //#include <jpeglib.h>
// #include <jpeglib.h>
// void write_dicom_tag(FILE *file, uint16_t group, uint16_t element, const char *value);
// void write_dicom_meta_header(FILE *file);
// void write_pixel_data(FILE *file, uint8_t *pixel_data, int width, int height);

// // DICOM Header Structure
// struct DICOM_Header {
//     char prefix[128];    // Empty 128 bytes
//     char magic[4];       // "DICM"
// };

// // DICOM Tag Structure
// struct DICOM_Tag {
//     uint16_t group;
//     uint16_t element;
//     uint32_t length;
//     char value[64];
// };
// void write_dicom_meta_header(FILE *file) {
//     uint8_t preamble[128] = {0}; // 128 bytes of zero
//     fwrite(preamble, 1, 128, file);
//     fwrite("DICM", 1, 4, file); // Write DICM identifier
//     write_dicom_tag(file, 0x0002, 0x0000, "\x00\x00\x00\x00"); // File Meta Information Group Length
//     write_dicom_tag(file, 0x0002, 0x0010, "1.2.840.10008.1.2.1"); // Transfer Syntax UID (Explicit VR Little Endian)
// }



// // Function to Write a DICOM Tag
// void write_dicom_tag(FILE *file, uint16_t group, uint16_t element, const char *value) {
//     uint32_t length = strlen(value);
//     fwrite(&group, 2, 1, file);
//     fwrite(&element, 2, 1, file);
//     fwrite(&length, 4, 1, file);
//     fwrite(value, length, 1, file);
// }

// // Function to Convert JPG to Grayscale and Get Raw Data
// uint8_t *convert_jpg_to_raw(const char *jpg_filename, int *width, int *height) {
//     FILE *jpg = fopen(jpg_filename, "rb");
// if (!jpg) {
//     printf("Error: Cannot open JPG file: %s\n", jpg_filename);
//     return NULL;
// } else {
//     printf("opened image: %s\n",jpg_filename);
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

// int main() {
//     const char *jpg_filename = "test.jpg";
//     char *dicom_name = "output.dcm";
//     FILE *dicom_filename = fopen(dicom_name, "wb");
//     if(!dicom_filename){
//         perror("error with dicom file");
//     }

//     int width, height;
//     write_dicom_meta_header(dicom_filename);

//     // Convert JPG to RAW grayscale
//     uint8_t *raw_data = convert_jpg_to_raw(jpg_filename, &width, &height);
//     if (!raw_data) {
//         return -1;
//     }

//     FILE *dicom = fopen(dicom_name, "wb");
//     if (!dicom) {
//         printf("Error: Cannot create DICOM file.\n");
//         free(raw_data);
//         return -1;
//     }

//     // Create and Write DICOM Header
//     // struct DICOM_Header header = {0};
//     // memcpy(header.magic, "DICM", 4);
//     // fwrite(&header, sizeof(header), 1, dicom);

//     // Add Metadata Tags
//     // First, write the meta header
// // write_dicom_meta_header(dicom);

// // Then, write DICOM metadata in ascending order
// // Image Type
// // write_dicom_tag(dicom, 0x0008, 0x0060, "OT"); // Modality (OT = Other)
// // write_dicom_tag(dicom, 0x0010, 0x0010, "John Doe "); // Patient Name (Ensure even length)
// // write_dicom_tag(dicom, 0x0010, 0x0020, "123456 "); // Patient ID (Ensure even length)

// // // Finally, write Pixel Data at the very end
// // write_pixel_data(dicom, raw_pixel_data, width, height);
//     write_dicom_tag(dicom, 0x0008, 0x0008, "ORIGINAL RIMARY AXIAL"); 
//     write_dicom_tag(dicom, 0x0008, 0x0060, "OT");
//     write_dicom_tag(dicom, 0x0010, 0x0010, "John Doe");  // Patient Name
//     write_dicom_tag(dicom, 0x0010, 0x0020, "12345");  // Patient ID
//       // Modality (OT = Other)
    
//     // Pixel Data Tag (7FE0,0010)
//     uint16_t pixel_data_group = 0x7FE0;
//     uint16_t pixel_data_element = 0x0010;
//     uint32_t pixel_data_length = width * height;
    
//     fwrite(&pixel_data_group, 2, 1, dicom);
//     fwrite(&pixel_data_element, 2, 1, dicom);
//     fwrite(&pixel_data_length, 4, 1, dicom);

//     // Append Pixel Data
//     fwrite(raw_data, 1, pixel_data_length, dicom);

//     fclose(dicom);
//     free(raw_data);

//     printf("DICOM file created: %s\n", dicom_name);
//     return 0;
// }
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>

void write_uint16(FILE *file, uint16_t value) {
    uint8_t bytes[2] = { value & 0xFF, (value >> 8) & 0xFF };
    fwrite(bytes, 1, 2, file);
}

void write_uint32(FILE *file, uint32_t value) {
    uint8_t bytes[4] = { value & 0xFF, (value >> 8) & 0xFF, (value >> 16) & 0xFF, (value >> 24) & 0xFF };
    fwrite(bytes, 1, 4, file);
}

void write_tag(FILE *file, uint16_t group, uint16_t element) {
    write_uint16(file, group);
    write_uint16(file, element);
}

void write_string(FILE *file, const char *str, size_t length) {
    fwrite(str, 1, length, file);
    if (length % 2 != 0) {
        char pad = ' ';
        fwrite(&pad, 1, 1, file);
    }
}

// Convert JPG to raw grayscale data


int main() {
    const char *jpg_filename = "test.jpg";
    const char *dicom_filename = "output.dcm";

    int width, height;
    uint8_t *raw_data = convert_jpg_to_raw(jpg_filename, &width, &height);
    if (!raw_data) return -1;

    FILE *dicom = fopen(dicom_filename, "wb");
    if (!dicom) {
        free(raw_data);
        return -1;
    }

    uint8_t preamble[128] = {0};
    fwrite(preamble, sizeof(preamble), 1, dicom);
    fwrite("DICM", 4, 1, dicom);

    long meta_start = ftell(dicom);

    // File Meta Group Length (Placeholder)
    write_tag(dicom, 0x0002, 0x0000);
    fwrite("UL", 2, 1, dicom);
    write_uint16(dicom, 4);
    long meta_length_pos = ftell(dicom);
    write_uint32(dicom, 0);

    // Transfer Syntax
    write_tag(dicom, 0x0002, 0x0010);
    fwrite("UI", 2, 1, dicom);
    write_uint16(dicom, 0);
    const char *transfer_syntax = "1.2.840.10008.1.2.1";  // Explicit VR Little Endian
    write_uint32(dicom, strlen(transfer_syntax));
    write_string(dicom, transfer_syntax, strlen(transfer_syntax));

    long meta_end = ftell(dicom);
    uint32_t meta_length = meta_end - meta_start - 12;
    fseek(dicom, meta_length_pos, SEEK_SET);
    write_uint32(dicom, meta_length);
    fseek(dicom, meta_end, SEEK_SET);

    // Patient Name
    write_tag(dicom, 0x0010, 0x0010);
    fwrite("PN", 2, 1, dicom);
    const char *patient_name = "Doe^John";
    write_uint16(dicom, strlen(patient_name));
    write_string(dicom, patient_name, strlen(patient_name));

    // Image Attributes
    write_tag(dicom, 0x0028, 0x0010);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, height);

    write_tag(dicom, 0x0028, 0x0011);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, width);

    write_tag(dicom, 0x0028, 0x0100);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, 8);

    write_tag(dicom, 0x0028, 0x0101);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, 8);

    write_tag(dicom, 0x0028, 0x0102);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, 7);

    write_tag(dicom, 0x0028, 0x0103);
    fwrite("US", 2, 1, dicom);
    write_uint16(dicom, 2);
    write_uint16(dicom, 0);

    // Pixel Data (7FE0,0010)
    write_tag(dicom, 0x7FE0, 0x0010);
    fwrite("OW", 2, 1, dicom);
    write_uint16(dicom, 0);
    uint32_t pixel_length = width * height;
    write_uint32(dicom, pixel_length);
    fwrite(raw_data, 1, pixel_length, dicom);
    if (pixel_length % 2 != 0) {
        uint8_t padding = 0;
        fwrite(&padding, 1, 1, dicom);
    }

    fclose(dicom);
    free(raw_data);

    printf("DICOM file created: %s\n", dicom_filename);
    return 0;
}
