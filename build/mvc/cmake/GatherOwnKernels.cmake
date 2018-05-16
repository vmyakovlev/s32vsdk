# Collect sources into the variable MY_FILTER_KERNEL_SOURCES without
# having to explicitly list each header and source file.
#
# CMake documentation states "We do not recommend using GLOB to collect a list of
# source files from your source tree. If no CMakeLists.txt file changes when a 
# source is added or removed then the generated build system cannot know when to 
# ask CMake to regenerate".




function(CREATE_OWN_KERNEL_LIB)

  set(OWN_KERNEL_DIR ${ARGV0})

  #cmake_minimum_required (VERSION 2.6)

  # This file should be in $VISION_SDK/tests/<projName>/kernels/<some_kernel_category>/
  get_filename_component(CrtKernelLib ${OWN_KERNEL_DIR} NAME)

  set(OWN_KERNEL_SRC "${OWN_KERNEL_DIR}/src")

  if ((EXISTS ${OWN_KERNEL_SRC} ) AND (IS_DIRECTORY  ${OWN_KERNEL_SRC}) )
	#CMAKE_REQUIRED_DEFINITIONS = (-DMyProjectRoot=${VISION_SDK})

	#message("crt lib source dir: " ${OWN_KERNEL_SRC})

	# Maps to Visual Studio solution file (sobelDemo.sln)
	# The solution will have all targets (exe, lib, dll) 
	# as Visual Studio projects (.vcproj)
	#project (${CrtKernelLib})


	file( GLOB srcFilesCPP "${OWN_KERNEL_SRC}/*.cpp")
	file( GLOB srcFilesC "${OWN_KERNEL_SRC}/*.c")
	file( GLOB headerFilesH "${OWN_KERNEL_SRC}/*.h")
	file( GLOB headerFilesHPP "${OWN_KERNEL_SRC}/*.hpp")

	set (srcFiles "")
	list(APPEND srcFiles ${srcFilesCPP} ${srcFilesC})
	set (headerFiles "")
	list(APPEND headerFiles ${headerFilesHPP} ${headerFilesH} "${VISION_SDK}/include/common_stringify_macros.h")

	#message("own kernel cat src files: " ${srcFiles})
	#message("own kernel cat H files: " ${headerFiles})

	LIST(APPEND OWN_KERNELS ${CrtKernelLib})
	set(OWN_KERNELS ${OWN_KERNELS} PARENT_SCOPE)
	
	add_library(${CrtKernelLib} STATIC ${headerFiles} ${srcFiles})
	source_group("Header Files" FILES ${headerFles} )
	source_group("Source Files" FILES ${srcFiles})

	target_compile_options(${CrtKernelLib} BEFORE PRIVATE "/Tp") # compile as C++
	target_compile_definitions(${CrtKernelLib} PUBLIC ACF_KERNEL_IMPLEMENTATION ACF_KERNEL_METADATA)

	target_include_directories(${CrtKernelLib} PUBLIC ${OWN_KERNEL_SRC} "${APU_LIB_DIR}/src" "${ACF_LIB_DIR}/src")

	# Creates a folder "cmake/proj" and adds target 
	# project (<CrtKernelLib>.vcproj) under it
	# set_property(TARGET ${CrtKernelLib} PROPERTY FOLDER "${LIB_SOURCE_DIR}/build-deskwin32/cmake/proj/")

	# Adds logic to INSTALL.vcproj to copy ${CrtKernelLib}.exe to destination directory
	# install (TARGETS ${CrtKernelLib} EXPORT "${CrtKernelLib}d"
			 # ARCHIVE DESTINATION "${LIB_SOURCE_DIR}${VS_PROJ_DIR}/Debug" CONFIGURATIONS Debug)
	# install (TARGETS ${CrtKernelLib}
			 # ARCHIVE DESTINATION  "${LIB_SOURCE_DIR}${VS_PROJ_DIR}/Release"  CONFIGURATIONS Release)
  endif()
endfunction(CREATE_OWN_KERNEL_LIB)

function(GATHER_OWN_KERNEL_LIBS)
    set(OWN_KERNEL_DIR ${ARGV0})
	#message ("gather own kernels in: " ${OWN_KERNEL_DIR})
	#file (GLOB crt_own_kernels "${OWN_KERNEL_DIR}/*/CMakeLists.txt")
	file (GLOB crt_own_kernels "${OWN_KERNEL_DIR}/*")

	foreach(kndir ${crt_own_kernels} )
	    if(IS_DIRECTORY ${kndir}) 
			#message("crt kndir. "  ${kndir})
			#get_filename_component(dirPath ${kndir} DIRECTORY)
			#get_filename_component(dirName ${dirPath} NAME)
			get_filename_component(dirName ${kndir} NAME)

			list(APPEND own_kernels  ${dirName})
		endif()
	endforeach()

	set(own_kernels ${own_kernels} PARENT_SCOPE)

	foreach(kd ${crt_own_kernels})
	#	include("${kd}/CMakeLists.txt")
		#message("crt kd. "  ${kd})
		#include("${kd}")
		create_own_kernel_lib(${kd})
		#get_filename_component(dirPath ${kd} DIRECTORY)
		#list(APPEND own_kernel_include_directories "${dirPath}/src")
		list(APPEND own_kernel_include_directories "${kd}/src")
	endforeach()
	set(own_kernel_include_directories ${own_kernel_include_directories} PARENT_SCOPE)

	set(OWN_KERNELS ${OWN_KERNELS} PARENT_SCOPE)

endfunction(GATHER_OWN_KERNEL_LIBS)