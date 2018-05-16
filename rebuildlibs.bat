@echo off
set StartFolder=%CD%

call:process isp\graphs\h264dec_4stream\build-v234ce-gnu-linux-d\
call:process isp\graphs\h264dec_single_stream\build-v234ce-gnu-linux-d\
call:process isp\graphs\h264enc\build-v234ce-gnu-linux-d\
call:process isp\graphs\imx224_exp_ctrl\build-v234ce-gnu-linux-d\
call:process isp\graphs\imx224_exp_ctrl\build-v234ce-gnu-sa-d\
call:process isp\graphs\imx224_surround\build-v234ce-gnu-linux-d\
call:process isp\graphs\imx224_surround\build-v234ce-gnu-sa-d\
call:process isp\graphs\isp_generic\build-v234ce-gnu-linux-d\
call:process isp\graphs\isp_generic\build-v234ce-gnu-sa-d\
call:process isp\graphs\jpeg_4stream\build-v234ce-gnu-linux-d\
call:process isp\graphs\jpeg_4stream\build-v234ce-gnu-sa-d\
call:process isp\graphs\jpeg_single_stream\build-v234ce-gnu-linux-d\
call:process isp\graphs\jpeg_single_stream\build-v234ce-gnu-sa-d\
call:process isp\graphs\mipi_simple\build-v234ce-gnu-linux-d\
call:process isp\graphs\mipi_simple\build-v234ce-gnu-sa-d\
call:process isp\graphs\mipi_stereo_ftf\build-v234ce-gnu-linux-d\
call:process isp\graphs\mipi_stereo_ftf\build-v234ce-gnu-sa-d\
call:process isp\graphs\ov10635_surround\build-v234ce-gnu-linux-d\
call:process isp\graphs\ov10635_surround\build-v234ce-gnu-sa-d\
call:process isp\graphs\ov10640_12bcomb_surround\build-v234ce-gnu-linux-d\
call:process isp\graphs\ov10640_12bcomb_surround\build-v234ce-gnu-sa-d\
call:process isp\graphs\ov10640_default\build-v234ce-gnu-linux-d\
call:process isp\graphs\ov10640_default\build-v234ce-gnu-sa-d\
call:process isp\graphs\rgb_yuv_gs8\build-v234ce-gnu-linux-d\
call:process isp\graphs\rgb_yuv_gs8\build-v234ce-gnu-sa-d\
call:process isp\graphs\sony_dualexp\build-v234ce-gnu-linux-d\
call:process isp\graphs\sony_dualexp\build-v234ce-gnu-sa-d\
call:process isp\graphs\viu_simple\build-v234ce-gnu-linux-d\
call:process isp\graphs\viu_simple\build-v234ce-gnu-sa-d\
call:process isp\graphs\yuv_grey_pyramid\build-v234ce-gnu-linux-d\
call:process isp\graphs\yuv_grey_pyramid\build-v234ce-gnu-sa-d\

call:process kernels\apu\apexcv_base_arithmetic\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_base_color_conversion\build-apu-nxp-sa-d\
    REM call:process kernels\apu\apexcv_base_filter\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_base_histogram\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_base_image_filters\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_base_integral_image\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_base_interpolation\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_gdc_ldw2\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_aggcf\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_blockmatching\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_brief\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_canny\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_fast\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_gftt_corners\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_harris_corners\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_histogram_equalization\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_hog\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_hough\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_laplacian_pyramid\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_lbp\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_lkpyramid\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_lktracker\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_orb\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_pyramid\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_remap\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_resize\build-apu-nxp-sa-d\
call:process kernels\apu\apexcv_pro_tmo\build-apu-nxp-sa-d\
call:process kernels\apu\sample_arithmetic_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_box_generic_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_comparison_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_conversion_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_display_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_feature_detection_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_filtering_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_geometry_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_morphology_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_object_detection_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_optimization_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_resizing_kernels\build-apu-nxp-sa-d\
call:process kernels\apu\sample_statistics_kernels\build-apu-nxp-sa-d\

call:process libs\apex\acf\build-v234ce-gnu-linux-d\
call:process libs\apex\acf\build-v234ce-gnu-sa-d\
call:process libs\apex\drivers\user\build-v234ce-gnu-linux-d\
call:process libs\apex\drivers\user\build-v234ce-gnu-sa-d\
call:process libs\apex\icp\build-v234ce-gnu-linux-d\
call:process libs\apex\icp\build-v234ce-gnu-sa-d\
call:process libs\apexcv_base\apexcv_core\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\apexcv_core\build-v234ce-gnu-sa-d\
	REM at this point you have to clean and rebuild kernels\apu\apexcv_base_kernels\build-apu-nxp-sa-d\
call:process libs\apexcv_base\arithmetic\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\arithmetic\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\arithmetic\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_base\color_conversion\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\color_conversion\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\color_conversion\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_base\histogram\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\histogram\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\histogram\graphs\build-apu-nxp-sa-d\
	REM call:process libs\apexcv_base\histogram_equalization\build-v234ce-gnu-linux-d\
	REM call:process libs\apexcv_base\histogram_equalization\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\histogram_equalization\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_base\image_filters\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\image_filters\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\image_filters\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_base\integral_image\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\integral_image\build-v234ce-gnu-sa-d\
	REM call:process libs\apexcv_base\integral_image\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_base\interpolation\build-v234ce-gnu-linux-d\
call:process libs\apexcv_base\interpolation\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\affine\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\affine\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\affine\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\aggcf\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\aggcf\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\aggcf\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\aggcf_pd\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\aggcf_pd\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\blockmatching\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\blockmatching\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\blockmatching\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\brief\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\brief\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\brief\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\canny\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\canny\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\canny\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\fast\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\fast\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\fast\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\gftt_corners\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\gftt_corners\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\gftt_corners\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\histogram_equalization\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\histogram_equalization\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\histogram_equalization\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\hog\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\hog\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\hog\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\hough\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\hough\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\hough\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\image_pyramid\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\image_pyramid\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\image_pyramid\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\image_pyramid_multi\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\image_pyramid_multi\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\image_pyramid_multi\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\laplacian_pyramid\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\laplacian_pyramid\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\laplacian_pyramid\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\lbp\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\lbp\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\lbp\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\lkpyramid\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\lkpyramid\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\lkpyramid\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\lktracker\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\lktracker\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\lktracker\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\orb\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\orb\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\orb\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\remap\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\remap\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\remap\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\resize\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\resize\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\resize\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\tmo\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\tmo\build-v234ce-gnu-sa-d\
call:process libs\apexcv_pro\tmo\graphs\build-apu-nxp-sa-d\
call:process libs\apexcv_pro\util\build-v234ce-gnu-linux-d\
call:process libs\apexcv_pro\util\build-v234ce-gnu-sa-d\
call:process libs\io\dcu\build-v234ce-gnu-linux-d\
call:process libs\io\dcu\build-v234ce-gnu-sa-d\
call:process libs\io\frame_io\build-v234ce-gnu-linux-d\
call:process libs\io\frame_io\build-v234ce-gnu-sa-d\
call:process libs\io\gdi\build-v234ce-gnu-linux-d\
call:process libs\io\gdi\build-v234ce-gnu-sa-d\
call:process libs\io\i2c\build-v234ce-gnu-sa-d\
call:process libs\io\image_camera_io\build-v234ce-gnu-linux-d\
call:process libs\io\image_video_io\build-v234ce-gnu-linux-d\
call:process libs\io\image_video_io\build-v234ce-gnu-sa-d\
call:process libs\io\qspi\build-v234ce-gnu-sa-d\
call:process libs\io\sdi\build-v234ce-gnu-linux-d\
call:process libs\io\sdi\build-v234ce-gnu-sa-d\
call:process libs\io\semihost\build-v234ce-gnu-linux-d\
call:process libs\io\semihost\build-v234ce-gnu-sa-d\
call:process libs\io\uartlinflex_io\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\cam_generic\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\cam_generic\user\build-v234ce-gnu-linux-d\
call:process libs\isp\cam_generic\user\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\csi\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\csi\user\build-v234ce-gnu-linux-d\
call:process libs\isp\csi\user\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\fdma\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\fdma\user\build-v234ce-gnu-linux-d\
call:process libs\isp\fdma\user\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\h264dec\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\h264dec\user\build-v234ce-gnu-linux-d\
	REM call:process libs\isp\h264enc\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\h264enc\user\build-v234ce-gnu-linux-d\
	REM call:process libs\isp\jpegdec\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\jpegdec\user\build-v234ce-gnu-linux-d\
	REM call:process libs\isp\sequencer\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\sequencer\user\build-v234ce-gnu-linux-d\
call:process libs\isp\sequencer\user\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\sram\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\sram\user\build-v234ce-gnu-linux-d\
call:process libs\isp\sram\user\build-v234ce-gnu-sa-d\
	REM call:process libs\isp\viu\kernel\build-v234ce-gnu-linux-d\
call:process libs\isp\viu\user\build-v234ce-gnu-linux-d\
	REM call:process libs\startup\v234ce_m4_standalone\
call:process libs\startup\v234ce_standalone\build-v234ce-gnu-sa-d\
call:process libs\utils\common\build-v234ce-gnu-linux-d\
call:process libs\utils\common\build-v234ce-gnu-sa-d\
call:process libs\utils\communications\build-v234ce-gnu-linux-d\
call:process libs\utils\communications\build-v234ce-gnu-sa-d\
call:process libs\utils\h264encoder\build-v234ce-gnu-linux-d\
call:process libs\utils\log\build-v234ce-gnu-linux-d\
call:process libs\utils\log\build-v234ce-gnu-sa-d\
call:process libs\utils\matrix\build-v234ce-gnu-linux-d\
call:process libs\utils\matrix\build-v234ce-gnu-sa-d\
call:process libs\utils\multithread_app\build-v234ce-gnu-linux-d\
call:process libs\utils\neon\build-v234ce-gnu-linux-d\
call:process libs\utils\neon\build-v234ce-gnu-sa-d\
	REM call:process libs\utils\oal\kernel\build-v234ce-gnu-linux-d\
call:process libs\utils\oal\user\build-v234ce-gnu-linux-d\
call:process libs\utils\oal\user\build-v234ce-gnu-sa-d\
call:process libs\utils\umat\build-v234ce-gnu-linux-d\
call:process libs\utils\umat\build-v234ce-gnu-sa-d\
call:process libs\webserver\build-v234ce-gnu-linux-d\
call:process libs\webserver\build-v234ce-gnu-sa-d\

EXIT

:process
pushd %1
call %StartFolder%\consolmake.bat
popd
mkdir results\%1
if     exist %1*.a copy /b /y %1*.a  results\%1
if not exist %1*.a copy /b /y %1*.h* results\%1