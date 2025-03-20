import pydicom
from matplotlib import pyplot as plt
from PIL import Image

# Read the DICOM file
dcm_file = pydicom.dcmread('/home/adin.n@acsiatech.com/Desktop/CAD/output_image.dcm')


# Read the DICOM file


# Print the DICOM dataset to inspect its contents
#print(dcm_file)


# Extract specific information from the header
patient_name = dcm_file.get('PatientName', 'N/A')
patient_id = dcm_file.get('PatientID', 'N/A')
study_date = dcm_file.get('StudyDate', 'N/A')
modality = dcm_file.get('Modality', 'N/A')
image_type = dcm_file.get('ImageType', 'N/A')

# Print extracted information
print("Patient Name:", patient_name)
print("Patient ID:", patient_id)
print("Study Date:", study_date)
print("Modality:", modality)
print("Image Type:", image_type)
# Get pixel data from the DICOM file
#pixel_data = dcm_file.pixel_array

# Convert pixel data to a PIL Image
#image = Image.fromarray(pixel_data)

# Save the image as JPG
#image.save('example.jpg')

# Alternatively, save the image as PNG
# image.save('example.png')

# Show the converted image (optional)
#plt.imshow(pixel_data, cmap=plt.cm.gray)
##plt.axis('off')
#plt.show()


# import os
# import pydicom
# from PIL import Image

# def convert_dicom_to_png(dicom_dir, output_dir):
#     # Loop through all DICOM files in the input directory
#     for filename in os.listdir(dicom_dir):
#         if filename.endswith('.dcm'):
#             # Read the DICOM file
#             dcm_file = pydicom.dcmread(os.path.join(dicom_dir, filename))

#             # Extract pixel data
#             pixel_data = dcm_file.pixel_array

#             # Convert pixel data to image
#             image = Image.fromarray(pixel_data)

#             # Save the image as PNG
#             output_path = os.path.join(output_dir, os.path.splitext(filename)[0] + '.png')
#             image.save(output_path)

#             print(f"Converted {filename} to {output_path}")

# # Example usage
# dicom_directory = '/home/adin.n@acsiatech.com/Music'
# output_directory = '/home/adin.n@acsiatech.com/Music'
# convert_dicom_to_png(dicom_directory, output_directory)