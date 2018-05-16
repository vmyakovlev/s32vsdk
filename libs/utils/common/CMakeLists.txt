
cmake_minimum_required (VERSION 2.6)

set (LibName "image_video_io")
#get_filename_component(LibName ${CMAKE_CURRENT_LIST_DIR} NAME)

set(img_vid_io_KERNEL_SRC "${CMAKE_CURRENT_LIST_DIR}/src")
set(img_vid_io_KERNEL_HEADER "${CMAKE_CURRENT_LIST_DIR}/src")

#message ("common lib dir " ${img_vid_io_KERNEL_SRC})
#CMAKE_REQUIRED_DEFINITIONS = (-DMyProjectRoot=${VISION_SDK})

# Maps to Visual Studio solution file (LibName.sln)
# The solution will have all targets (exe, lib, dll) 
# as Visual Studio projects (.vcproj)
#project (${LibName})

#file( GLOB srcFilesCPP "${OWN_KERNEL_SRC}/*.cpp")
#file( GLOB srcFilesC "${OWN_KERNEL_SRC}/*.c")
#file( GLOB headerFilesH "${OWN_KERNEL_HEADER}/*.h")
#file( GLOB headerFilesHPP "${OWN_KERNEL_HEADER}/*.hpp")

set (srcFilesC "")
set (headerFilesHPP "")

list(APPEND img_vid_io_srcFilesCPP "${img_vid_io_KERNEL_SRC}/image_video_io.cpp")
list(APPEND img_vid_io_headerFilesH "${VISION_SDK}/io_image_video.h")

set (img_vid_io_srcFiles "")
list(APPEND img_vid_io_srcFiles ${img_vid_io_srcFilesCPP} ${img_vid_io_srcFilesC})
set (img_vid_io_headerFiles "")
list(APPEND img_vid_io_headerFiles ${img_vid_io_headerFilesHPP} ${img_vid_io_headerFilesH})

#message("common src files: " ${img_vid_io_srcFiles})
#message("own kernel cat H files: " ${headerFiles})

LIST(APPEND GEN_DEPENDENCIES ${LibName})

# set(LibFound "")
# find_library(LibFound ${LibName} "${CMAKE_CURRENT_LIST_DIR}/${VS_PROJ_DIR}/Release"  )
# message ("Lib ${LibName} found " ${LibFound})

#add_library(${LibName} STATIC IMPORTED)
#target_sources(${LibName} INTERFACE ${srcFiles} ${headerFiles})
#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/${VS_PROJ_DIR}/Debug")
#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_LIST_DIR}/${VS_PROJ_DIR}/Release")
#unset(LibFound)

add_library(${LibName} STATIC ${img_vid_io_srcFiles} ${img_vid_io_headerFiles} )
source_group("Source Files" FILES ${img_vid_io_srcFiles})
source_group("Header Files" FILES ${img_vid_io_headerFiles} )

# Creates a folder "c${LIB_SOURCE_DIR}/build-deskwin32/mvc" and adds target 
# project (<LibName>.vcproj) under it
#set_property(TARGET ${LibName} PROPERTY FOLDER "${LIB_SOURCE_DIR}/build-deskwin32/mvc")

# Adds logic to INSTALL.vcproj to copy ${LibName}.lib to destination directory
#install (TARGETS ${LibName} 
#		ARCHIVE DESTINATION ${LIB_SOURCE_DIR}/build-deskwin32/mvc)
