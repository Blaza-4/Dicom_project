/*  
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
void write_dicom_tag(FILE *dicom,uint16_t group,uint16_t element,char *value);
void write_dicom_numeric(FILE *dicom,uint16_t group,uint16_t element,uint32_t value);
struct Dicom_header
{
   char header[128];
   char prefix[4];
};
int main(){
    FILE *dicom = fopen("result.dcm","wb");
    if(!dicom){
        perror("dicom file failed ");
    }
    struct Dicom_header head ={0};
    memcpy(head.prefix,"DICM",4);
    fwrite(&head, sizeof(head), 1, dicom);
    uint32_t metalen = 130;
    write_dicom_numeric(dicom,0x0002,0x0000,metalen);
    write_dicom_numeric(dicom,0x0002,0x0001,0x0001);
    write_dicom_tag(dicom,0x0002,0x0002,"1.2.840.10008.1.3.10");
    write_dicom_tag(dicom,0x0002,0x0003,"1.2.840.10008.1.2.1");
    write_dicom_tag(dicom,0x0002,0x0010,"1.2.840.10008.1.2.4.70");

    write_dicom_tag(dicom, 0x0008, 0x0060, "OT");  
    write_dicom_tag(dicom, 0x0010, 0x0010, "John Doe");  
    write_dicom_tag(dicom, 0x0010, 0x0020, "1234");

    fclose(dicom);
}
void write_dicom_tag(FILE *dicom,uint16_t group,uint16_t element,char *value){
    int len = strlen(value);
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    fwrite(&len, 4, 1, dicom);
    fwrite(value, len, 1, dicom);
}
void write_dicom_numeric(FILE *dicom,uint16_t group,uint16_t element,uint32_t value){
    uint32_t len = 4;
    fwrite(&group,2,1,dicom);
    fwrite(&element, 2, 1, dicom);
    fwrite(&len, 4, 1, dicom);
    fwrite(&value, len, 1, dicom);
}

  */


#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

void write_dicom_tag_explicit(FILE *dicom, uint16_t group, uint16_t element, const char *vr, char *value);
void write_dicom_numeric_explicit(FILE *dicom, uint16_t group, uint16_t element, const char *vr, uint32_t value);

struct Dicom_header {
    char header[128];
    char prefix[4];
};

int main() {
    FILE *dicom = fopen("result.dcm", "wb");
    if (!dicom) {
        perror("dicom file failed ");
        return 1;
    }
    
    // Write file header
    struct Dicom_header head = {0};
    memcpy(head.prefix, "DICM", 4);
    fwrite(&head, sizeof(head), 1, dicom);
    
    // Calculate meta info length properly
    // This is a placeholder - will need to calculate based on actual data
    uint32_t metalen = 0; // We'll calculate this later
    long metalen_pos = ftell(dicom); // Remember position to update later
    
    // Write meta header elements using explicit VR Little Endian
    write_dicom_numeric_explicit(dicom, 0x0002, 0x0000, "UL", metalen);
    write_dicom_tag_explicit(dicom, 0x0002, 0x0001, "OB", "\x00\x01");
    write_dicom_tag_explicit(dicom, 0x0002, 0x0002, "UI", "1.2.840.10008.1.3.10");
    write_dicom_tag_explicit(dicom, 0x0002, 0x0003, "UI", "1.2.840.10008.1.2.1");
    write_dicom_tag_explicit(dicom, 0x0002, 0x0010, "UI", "1.2.840.10008.1.2.4.70");
    
    // Calculate and update meta info length
    long current_pos = ftell(dicom);
    metalen = current_pos - metalen_pos - 12; // 12 bytes for the tag, VR, and length field
    fseek(dicom, metalen_pos + 8, SEEK_SET); // +8 to skip tag and VR
    fwrite(&metalen, 4, 1, dicom);
    fseek(dicom, current_pos, SEEK_SET);
    
    // Now write the dataset elements (these go after meta header)
    // Using the transfer syntax specified in (0002,0010)
    write_dicom_tag_explicit(dicom, 0x0008, 0x0060, "CS", "OT");
    write_dicom_tag_explicit(dicom, 0x0010, 0x0010, "PN", "John Doe");
    write_dicom_tag_explicit(dicom, 0x0010, 0x0020, "LO", "1234");
    
    fclose(dicom);
    return 0;
}

// Function to write tags with explicit VR
void write_dicom_tag_explicit(FILE *dicom, uint16_t group, uint16_t element, const char *vr, char *value) {
    uint16_t len = strlen(value);
    
    // Write tag
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    
    // Write VR (2 bytes)
    fwrite(vr, 2, 1, dicom);
    
    // For OB, OW, OF, SQ, UT, and UN, add reserved 2 bytes and 4-byte length
    if (strcmp(vr, "OB") == 0 || strcmp(vr, "OW") == 0 || strcmp(vr, "OF") == 0 || 
        strcmp(vr, "SQ") == 0 || strcmp(vr, "UT") == 0 || strcmp(vr, "UN") == 0) {
        uint16_t reserved = 0;
        uint32_t length32 = len;
        fwrite(&reserved, 2, 1, dicom);
        fwrite(&length32, 4, 1, dicom);
    } else {
        // Other VRs have 2-byte length
        fwrite(&len, 2, 1, dicom);
    }
    
    // Write value
    fwrite(value, len, 1, dicom);
    
    // Pad if odd length for most VRs
    if (len % 2 != 0 && strcmp(vr, "UI") != 0) {
        char pad = 0;
        fwrite(&pad, 1, 1, dicom);
    }
}

// Function to write numeric values with explicit VR
void write_dicom_numeric_explicit(FILE *dicom, uint16_t group, uint16_t element, const char *vr, uint32_t value) {
    // Write tag
    fwrite(&group, 2, 1, dicom);
    fwrite(&element, 2, 1, dicom);
    
    // Write VR (2 bytes)
    fwrite(vr, 2, 1, dicom);
    
    // For OB, OW, OF, SQ, UT, and UN, add reserved 2 bytes and 4-byte length
    if (strcmp(vr, "OB") == 0 || strcmp(vr, "OW") == 0 || strcmp(vr, "OF") == 0 || 
        strcmp(vr, "SQ") == 0 || strcmp(vr, "UT") == 0 || strcmp(vr, "UN") == 0) {
        uint16_t reserved = 0;
        uint32_t length32 = 4;
        fwrite(&reserved, 2, 1, dicom);
        fwrite(&length32, 4, 1, dicom);
    } else {
        // Other VRs have 2-byte length
        uint16_t len = 4;
        fwrite(&len, 2, 1, dicom);
    }
    
    // Write value
    fwrite(&value, 4, 1, dicom);
}