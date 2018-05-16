##############################
#
# Visual Studio												|	CMake Command
#															|
# Solution file (.sln) 										|	project 
# Project file (.vcproj) 									|	target name in the command add_executable or add_library 
# executable (.exe) 										|	add_executable 
# static library (.lib) 									|	add_library 
# dynamic library (.dll)									|	add_library(SHARED) 
# Source Folders 											|	source_group 
# Project Folders 											|	set_property(TARGET <target> PROPERTY FOLDER <name>) 
# Properties->C/C++->Preprocessor->Preprocessor Definitions |	add_definitions 
# Properties->C/C++->General->Additional Include Directories|	include_directories 
# Properties->Linker->General->Additional Library Directories|  link_directories 
# Properties->Linker->Input->Additional Dependencies 		|   target_link_libraries 


cmake_minimum_required (VERSION 2.6)
enable_language(CXX)

set (PRJ_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
get_filename_component (Prj_DIR ${PRJ_SRC_DIR} ABSOLUTE)
get_filename_component(PDir ${Prj_DIR} NAME)
message("Project Name: " ${PDir})

################### Open CV package ####################
#find_package(OpenCV ${OpenCV_Version}  REQUIRED)
find_package(OpenCV REQUIRED)
message("OpenCV_VERSION: " ${OpenCV_VERSION})
add_library(OpenCV SHARED IMPORTED GLOBAL)

#message("main OpenCV_INCLUDE_DIRS "  ${OpenCV_INCLUDE_DIRS})

#Parses a unix- or windows-style command-line string “<args>” and stores a semicolon-separated list of the arguments in <var>. 
#The entire command line must be given in one “<args>” argument.
#separate_arguments(<var> WINDOWS_COMMAND "<args>")

set (TARGET_NAME ${PDir})

###########################################################
# !!!! this shall be filled in by the code generator !!!!
# set (KERNELS_DIR "kernels")
# set (GRAPHS_DIR "graphs")
# set (SRC_DIR 	"src")
# set (MAIN 		"main.cpp")


# list (APPEND EXT_KERNEL_DEPEND "arithmetic_kernels" "optimization_kernels" )
######### end of code generator section #########################
include("CMakeLists_generated.txt")
set(BUILD_DIR "${VISION_SDK}/build/mvc/cmake")
set (OpenCV_DIR "${VISION_SDK}/3rdparty/ocv/win32-x86/vc12/lib")

#Add the pathes for the kernel directories and other project constants 
#add_subdirectory($BUILD_DIR)

set (VS_PROJ_DIR "/build-deskwin32/mvc")
set (PROJECT_BINARY_DIR "${PRJ_SRC_DIR}/${VS_PROJ_DIR}")

#message ("PROJECT_SRC_DIR: " ${PRJ_SRC_DIR})
#message ("PROJECT_BIN_DIR: " ${PROJECT_BINARY_DIR})


#Adds the given files as dependencies to source_file
#ADD_FILE_DEPENDENCIES(source_file depend_files...)

#set(CMAKE_REQUIRED_DEFINITIONS (-DMyProjectRoot=${VISION_SDK}) )


#CMAKE_REQUIRED_FLAGS = string of compile command line flags
#CMAKE_REQUIRED_INCLUDES = list of include directories
#CMAKE_REQUIRED_LIBRARIES = list of libraries to link
#CMAKE_REQUIRED_QUIET = execute quietly without messages


# Maps to Visual Studio solution file (${TARGET_NAME}.sln)
# The solution will have all targets (exe, lib, dll) 
# as Visual Studio projects (.vcproj)
project (${TARGET_NAME})

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and options. 
# Here it is setting the Visual Studio warning level to 4
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")


# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

#add the include policy
cmake_policy(SET CMP0011 NEW) #CMP0011 NO_POLICY_SCOPE  is also possible
cmake_policy(SET CMP0015 NEW)

#Add the pathes for the kernel directories and other project constants 
include("${BUILD_DIR}/Libs_Config.cmake")
include("${BUILD_DIR}/Kernels_Libs.cmake")
include("${BUILD_DIR}/GatherOwnKernels.cmake")


## those below could be also in the the above CMakeLists.txt
#include("${APU_LIB_DIR}/CMakeLists.txt")
include("CMakeLists_APU_lib.txt")
#include("${ACF_LIB_DIR}/CMakeLists.txt")
include("CMakeLists_ACF_lib.txt")

#include("${ARM_COMMON_LIB}/CMakeLists.txt")
include("CMakeLists_common_lib.txt")
#include("${ARM_Img_Vid_LIB}/CMakeLists.txt")
include("CMakeLists_img_video_io_lib.txt")
#create_lib(${ARM_COMMON_LIB} "src" "include")
#create_lib(${ARM_Img_Vid_LIB} "src" "include")


#list(APPEND UtilityLibs $<TARGET_OBJECTS:APU_Lib> $<TARGET_OBJECTS:ACF_Lib> $<TARGET_OBJECTS:common> $<TARGET_OBJECTS:image_video_io> )

#set (GENERATED_DEPEND "") # gather all generated kernel libraries

# Sub-directories where more CMakeLists.txt exist
set (OWN_KERNEL_DIR	"${PRJ_SRC_DIR}/${KERNELS_DIR}")
set(own_kernel_include_directories "")
#generate all kernels" projects
#if (EXISTS "${OWN_KERNEL_DIR}" AND EXISTS "${OWN_KERNEL_DIR}/CMakeLists.txt")
if (EXISTS ${OWN_KERNEL_DIR} AND IS_DIRECTORY ${OWN_KERNEL_DIR})
	#include ("${OWN_KERNEL_DIR}/CMakeLists.txt")
	gather_own_kernel_libs(${OWN_KERNEL_DIR})
endif()

set (OWN_SRC_DIR 		"${PRJ_SRC_DIR}/${SRC_DIR}")
set (OWN_GRAPH_DIR 	"${PRJ_SRC_DIR}/${GRAPHS_DIR}")

# Collect sources into the variable SIMPLE_FUNCTION_SOURCES
file (GLOB processes_header_files_h   "${OWN_SRC_DIR}/*.h")
file (GLOB processes_header_files_hpp "${OWN_SRC_DIR}/*.hpp")
file (GLOB processes_source_files "${OWN_SRC_DIR}/*.cpp")
file (GLOB GRAPHS_HPP			  "${OWN_GRAPH_DIR}/*.hpp")
file (GLOB GRAPHS_H				  "${OWN_GRAPH_DIR}/*.h")

set(processes_header_files "")
list(APPEND processes_header_files ${processes_header_files_hpp} ${processes_header_files_h})

set (GRAPHS "")
list(APPEND GRAPHS ${GRAPHS_HPP} ${GRAPHS_H})


# Set Properties->General->Configuration Type to Application(.exe)
# Creates ${TARGET_NAME}.exe with the listed sources (main.cxx)
# Adds sources to the Solution Explorer
#add_executable (${TARGET_NAME} "${OWN_SRC_DIR}/${MAIN}" ${processes_source_files}  ${processes_header_files_hpp}  ${GRAPHS} ${UtilityLibs} ${extKernObjLibs})
#add_executable (${TARGET_NAME} ${processes_source_files}  ${processes_header_files_hpp}  ${GRAPHS} ${UtilityLibs} ${extKernObjLibs})
add_executable (${TARGET_NAME} ${processes_header_files} ${processes_source_files} ${GRAPHS})

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("Header Files" FILES ${processes_header_files})
source_group("Source Files" FILES ${processes_source_files})
source_group("Graphs" FILES ${GRAPHS})


foreach(extUtil ${GEN_DEPENDENCIES}) 	
	target_link_libraries ("${TARGET_NAME}" "${extUtil}")	
	add_dependencies(${TARGET_NAME} "${extUtil}")
	#link_directories($<$<CONFIG:Debug>:"${EXT_KERNEL_DIR}/Debug":"${EXT_KERNEL_DIR}/Release">) 	
endforeach()

foreach(extKern ${EXT_KERNEL_DEPEND}) 
	set(crtDir "${EXT_KERNEL_DIR}/${extKern}")
	#include("${crtDir}/CMakeLists.txt")
	create_kernel_lib(${crtDir})
	#list(APPEND extKernObjLibs $<TARGET_OBJECTS:${extKern}>)
	
	#message("main ext kernel crtDir : " ${crtDir})
	# set(crtSrcDir "${crtDir}/src")
	# set(crtLib "${extKern}.lib")	
	#target_include_directories(${TARGET_NAME} PUBLIC ${crtSrcDir})

	#add_library(${crtLib} STATIC IMPORTED GLOBAL) 
	
	target_link_libraries ("${TARGET_NAME}" "${LibName}")	
	add_dependencies(${TARGET_NAME} "${LibName}")
	# target_link_libraries ("${TARGET_NAME}" debug "${LibName}_acfd")	
	# target_link_libraries (${TARGET_NAME} optimized "${LibName}_acf")
	# target_link_libraries ("${TARGET_NAME}" debug "${LibName}_apud")	
	# target_link_libraries (${TARGET_NAME} optimized "${LibName}_apu")
	
	# set(EXT_ROOT_DIR "${EXT_KERNEL_DIR}/${extKern}${VS_PROJ_DIR}/")
	link_directories($<$<CONFIG:Debug>:"${EXT_KERNEL_DIR}/Debug":"${EXT_KERNEL_DIR}/Release">) 	
	#set_property(LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${EXT_KERNEL_DIR}/Debug":"${EXT_KERNEL_DIR}/Release">) 
	
	# message("Adding dependency on: " ${LibName})
	# add_dependencies(${TARGET_NAME} "${LibName}_acf")
	# add_dependencies(${TARGET_NAME} "${LibName}_apu")
endforeach()


# if (DEFINED extKernObjLibs)
# set_target_properties( ${TARGET_NAME} ${extKernObjLibs} 
    # PROPERTIES
    # ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build-deskwin32/cmake"
    # LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build-deskwin32/cmake"
    # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build-deskwin32/cmake"
# )
# endif()

#target_compile_definitions(${TARGET_NAME} PUBLIC "MyProjectRoot=${VISION_SDK}")


# Adds logic to INSTALL.vcproj to copy ${TARGET_NAME}.exe to destination directory
install (TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
		 
add_definitions(-DAPEX2_EMULATE)		 
target_include_directories(${TARGET_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} ${VISION_SDK}/include)

# linking libs
target_link_libraries(${TARGET_NAME} optimized ${OpenCV_LIBS})
#link_directories(${OpenCV_LIB_DIR_OPT} ${OpenCV_LIB_DIR})
# set (cvModulesDbg "")
# foreach(module ${OpenCV_LIBS})
	 # list(APPEND cvModulesDbg "${module}d")
# endforeach()
# target_link_libraries(${TARGET_NAME} debug ${cvModulesDbg})
# link_directories(${OpenCV_LIB_DIR_OPT} ${OpenCV_LIB_DIR_DBG})
################################################################################

###################### Own directories and sources
# Properties->C/C++->General->Additional Include Directories
target_include_directories (${TARGET_NAME} PUBLIC ${PRJ_SRC_DIR} "${OWN_GRAPH_DIR}" "${OWN_SRC_DIR}")

# Creates a folder "cmake/proj" and adds target 
# project (${TARGET_NAME}.vcproj) under it
#set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "${PRJ_SRC_DIR}/build-deskwin32/cmake/proj")

##########################################################
########## additional emulation libraries
#include_directories("${ARM_COMMON_LIB}/include")
include_directories("${VISION_SDK}/include" ${OpenCV_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PUBLIC "${APU_LIB_DIR}/src" "${ACF_LIB_DIR}/src" "${ARM_Img_Vid_LIB}/src")

# Properties->Linker->Input->Additional Dependencies
#target_link_libraries (${TARGET_NAME} debug APU_libd ACF_libd commond image_video_iod)
#target_link_libraries (${TARGET_NAME} optimized APU_lib ACF_lib common image_video_io)

link_directories($<$<CONFIG:Debug>:"${APU_LIB_DIR}${VS_PROJ_DIR}/Debug":"${APU_LIB_DIR}${VS_PROJ_DIR}/Release">)
#set_property(TARGET ${TARGET_NAME} PROPERTY LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${APU_LIB_DIR}${VS_PROJ_DIR}/Debug":"${APU_LIB_DIR}${VS_PROJ_DIR}/Release">)

link_directories($<$<CONFIG:Debug>:"${ACF_LIB_DIR}${VS_PROJ_DIR}/Debug":"${ACF_LIB_DIR}${VS_PROJ_DIR}/Release">)
#set_property(TARGET ${TARGET_NAME} PROPERTY LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${ACF_LIB_DIR}${VS_PROJ_DIR}/Debug":"${ACF_LIB_DIR}${VS_PROJ_DIR}/Release">)

link_directories($<$<CONFIG:Debug>:"${ARM_COMMON_LIB}${VS_PROJ_DIR}/Debug":"${ARM_COMMON_LIB}${VS_PROJ_DIR}/Release">)
#set_property(TARGET ${TARGET_NAME} PROPERTY LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${ARM_COMMON_LIB}${VS_PROJ_DIR}/Debug":"${ARM_COMMON_LIB}${VS_PROJ_DIR}/Release">)

link_directories($<$<CONFIG:Debug>:"${ARM_Img_Vid_LIB}${VS_PROJ_DIR}/Debug":"${ARM_Img_Vid_LIB}${VS_PROJ_DIR}/Release">)
#set_property(TARGET ${TARGET_NAME} PROPERTY LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${ARM_Img_Vid_LIB}${VS_PROJ_DIR}/Debug":"${ARM_Img_Vid_LIB}${VS_PROJ_DIR}/Release">)

###############################################
############# own kernel libraries (the includes are resolved in each of the kernel CMakeLists.txt
foreach(kn ${OWN_KERNELS})
	
	# message ("linking exe to library: " ${kn})
	target_link_libraries(${TARGET_NAME} debug ${kn})
	target_link_libraries(${TARGET_NAME} optimized ${kn})
	set(OWN_KN_ROOT_DIR "${kn}${VS_PROJ_DIR}/")
	link_directories($<$<CONFIG:Debug>:"${OWN_KN_ROOT_DIR}/Debug":"${OWN_KN_ROOT_DIR}/Release">) 	
	#set_property(LINK_LIBRARIES DIRECTORY $<$<CONFIG:Debug>:"${OWN_KN_ROOT_DIR}/Debug":"${OWN_KN_ROOT_DIR}/Release">) 
endforeach()
	
#################################################
########### externally loaded kernel libraries
##
#set (EXT_KERNEL_DEP "")
#message("EXT_KERNEL_DIR in main: " ${EXT_KERNEL_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${EXT_KERNEL_DIR})
# foreach(extKern ${EXT_KERNEL_DEPEND}) 
	# set(crtDir "${EXT_KERNEL_DIR}/${extKern}")
	
	# message("main ext kernel crtDir : " ${crtDir})
	# set(crtSrcDir "${crtDir}/src")
	# target_include_directories(${TARGET_NAME} PUBLIC ${crtSrcDir})
# endforeach()

####################
#These are the dependencies to the generated kernels
#message ("gen dependencies: " ${GENERATED_DEPEND})
if (DEFINED GENERATED_DEPEND)
	add_dependencies(${TARGET_NAME} ${GENERATED_DEPEND})
endif()
		 
