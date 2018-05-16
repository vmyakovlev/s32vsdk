# Sub-directories where more CMakeLists.txt exist
#arithmetic_kernels  comparison_kernels  display_kernels            filtering_kernels  morphology_kernels        optical_flow_kernel   resizing_kernels
#    conversion_kernels  feature_detection_kernels  geometry_kernels   object_detection_kernels  optimization_kernels  statistics_kernels


file (GLOB KERNEL_DIRS "${VISION_SDK}/kernels/apu/*kernel*")
message("Extern Kernel dirs: " ${KERNEL_DIRS})

add_subdirectory(${KERNEL_DIRS})
#export (EXPORT KERNEL_DIRS)
# or rather glob for all libraries and add them to the library list.


