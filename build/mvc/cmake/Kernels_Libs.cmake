function(CREATE_KERNEL_LIB)

  #set(oneValueArgs DIR)
  #cmake_parse_arguments(ExtKernel_Lib "${oneValueArgs}" )
  set(Kernel_Lib_DIR ${ARGV0})
  if (EXISTS ${Kernel_Lib_DIR} AND IS_DIRECTORY ${Kernel_Lib_DIR})
	#message("Ext Kernel Dir: " ${Kernel_Lib_DIR})
	get_filename_component(LibName ${Kernel_Lib_DIR} NAME)
	#message("Ext Kernel Lib Name: " ${LibName})

	set(LibName ${LibName} PARENT_SCOPE)
	
	#get_filename_component(LibName ${Kernel_Lib_DIR} NAME)


	set(OWN_KERNEL_SRC "${Kernel_Lib_DIR}/src")

	file( GLOB srcFilesCPP "${OWN_KERNEL_SRC}/*.cpp")
	file( GLOB srcFilesC "${OWN_KERNEL_SRC}/*.c")
	file( GLOB headerFilesH "${OWN_KERNEL_SRC}/*.h")
	file( GLOB headerFilesHPP "${OWN_KERNEL_SRC}/*.hpp")
	

	set (srcFiles "")
	list(APPEND srcFiles ${srcFilesCPP} ${srcFilesC})
	set (headerFiles "")
	list(APPEND headerFiles ${headerFilesHPP} ${headerFilesH} "${VISION_SDK}/include/common_stringify_macros.h")

	LIST(APPEND EXT_KERNEL_DEPEND ${LibName})
	set(EXT_KERNEL_DEPEND ${EXT_KERNEL_DEPEND} PARENT_SCOPE)

	# set(LibFound "")
	# find_library(LibFound "${LibName}_acf" PATHS "${Kernel_Lib_DIR}/${VS_PROJ_DIR}" PATH_SUFFIXES "${LibName}_acf_Release" "${LibName}_acf_Debug" )
	# message ("Lib ${LibName}_acf found " ${LibFound})
	# message ("searched in path " "${Kernel_Lib_DIR}/${VS_PROJ_DIR}")

	#add_library("${LibName}_apu" STATIC IMPORTED)
	add_library(${LibName} STATIC ${headerFiles} ${srcFiles})
	source_group("Header Files" FILES ${headerFiles} )
	source_group("Source Files" FILES ${srcFiles})

	target_compile_options(${LibName} PUBLIC "/Tp") # compile as C++
	target_compile_definitions(${LibName} PUBLIC ACF_KERNEL_IMPLEMENTATION ACF_KERNEL_METADATA)

	target_include_directories(${LibName} PUBLIC "${APU_LIB_DIR}/src")
	target_include_directories(${LibName} PUBLIC "${ACF_LIB_DIR}/src")

	#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_DEBUG "${Kernel_Lib_DIR}/${VS_PROJ_DIR}/Debug")
	#set_property(TARGET ${LibName} PROPERTY IMPORTED_LOCATION_RELEASE "${Kernel_Lib_DIR}/${VS_PROJ_DIR}/Release")

	# Creates a folder "cmake/proj" and adds target 
	# project (<LibName>.vcproj) under it
	#set_property(TARGET ${LibName} PROPERTY FOLDER "${LIB_SOURCE_DIR}/build-deskwin32/mvc")

	# Adds logic to INSTALL.vcproj to copy sobelDemo.exe to destination directory
	#install (TARGETS ${LibName} 
	#		ARCHIVE DESTINATION ${LIB_SOURCE_DIR}/build-deskwin32/mvc)
  endif()	
endfunction(CREATE_KERNEL_LIB)