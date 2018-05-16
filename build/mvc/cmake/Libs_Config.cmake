
cmake_minimum_required (VERSION 2.6)

set (EMU_LIBS 		"${VISION_SDK}/tools/emu")
set (APU_LIB_DIR	"${EMU_LIBS}/apu")
set (ACF_LIB_DIR   	"${EMU_LIBS}/acf")
set (GENERAL_LIBS 	"${VISION_SDK}/libs")
set (ARM_LIBS 		"${GENERAL_LIBS}/")
set (ARM_COMMON_LIB	"${ARM_LIBS}/utils/common")
set (ARM_Img_Vid_LIB "${ARM_LIBS}/io/image_video_io")
set (EXT_KERNEL_DIR	"${VISION_SDK}/kernels/apu")
#set (ARM_OAL_LIB	"${ARM_LIBS}/utils/oal")
function(CREATE_LIB)
		
    set(Crt_Lib_Dir ${ARGV0})
	#set(SRC_DIR ${ARGV1} )
	#set(HEADER_DIR ${ARGV2})
	
	set(SRC_DIR "src" )
	set(HEADER_DIR "include")
	
#	message("emu lib dir : " ${Crt_Lib_Dir})
	get_filename_component(LibName ${Crt_Lib_Dir} NAME)

	set(LibName ${LibName} PARENT_SCOPE)
	
	#set(OWN_KERNEL_SRC "${Crt_Lib_Dir}/src")
	set(OWN_KERNEL_SRC "${Crt_Lib_Dir}/${SRC_DIR}")
	set(OWN_KERNEL_HEADER "${Crt_Lib_Dir}/$(HEADER_DIR)")

	file( GLOB srcFilesCPP "${OWN_KERNEL_SRC}/*.cpp")
	file( GLOB srcFilesC "${OWN_KERNEL_SRC}/*.c")
	file( GLOB headerFilesH_fromSrc "${OWN_KERNEL_SRC}/*.h")
	file( GLOB headerFilesHPP_fromSrc "${OWN_KERNEL_SRC}/*.hpp")
	file( GLOB headerFilesH_fromHeader "${OWN_KERNEL_HEADER}/*.h")
	file( GLOB headerFilesHPP_fromHeader "${OWN_KERNEL_HEADER}/*.hpp")

	set (srcFiles "")
	list(APPEND srcFiles ${srcFilesCPP} ${srcFilesC})
	#message("Lib ${LibName} sources: ${srcFiles}")
	set (headerFiles "")
	list(APPEND headerFiles ${headerFilesHPP_fromSrc} ${headerFilesH_fromSrc} ${headerFilesHPP_fromHeader} ${headerFilesH_fromHeader})

	#message("own kernel cat src files: " ${srcFiles})

	LIST(APPEND GEN_DEPENDENCIES ${LibName})
	
	set(GEN_DEPENDENCIES ${GEN_DEPENDENCIES} PARENT_SCOPE)

	#set(LibFound "")
	#find_library(LibFound ${LibName} "${Crt_Lib_Dir}/${VS_PROJ_DIR}/Release" )
	#message ("Lib ${LibName} found " ${LibFound})

	#add_library(${LibName} STATIC IMPORTED)
	add_library(${LibName} STATIC ${headerFiles} ${srcFiles})

	source_group("Header Files" FILES ${headerFiles} )
	source_group("Source Files" FILES ${srcFiles})
	
	#target_compile_definitions(${LibName} PUBLIC ACF_KERNEL_IMPLEMENTATION)

	# Creates a folder "cmake/proj" and adds target 
	# project (<LibName>.vcproj) under it
	#set_property(TARGET ${LibName} PROPERTY FOLDER "${LIB_SOURCE_DIR}/build-deskwin32/mvc")
	#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_DEBUG "${Crt_Lib_Dir}/${VS_PROJ_DIR}/Debug")
	#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_RELEASE "${Crt_Lib_Dir}/${VS_PROJ_DIR}/Release")

	# Adds logic to INSTALL.vcproj to copy sobelDemo.exe to destination directory
	#install (TARGETS ${LibName} 
	#		ARCHIVE DESTINATION ${LIB_SOURCE_DIR}/build-deskwin32/mvc)
	
endfunction(CREATE_LIB)
