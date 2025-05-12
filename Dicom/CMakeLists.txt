cmake_minimum_required(VERSION 3.0)

project(dicom)

find_package(JPEG REQUIRED)

include_directories(${JPEG_LIBRARIES})

set(C_STANDARD_REQUIRED 11)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Dicom.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES} pthread)