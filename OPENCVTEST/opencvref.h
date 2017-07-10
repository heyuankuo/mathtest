#pragma once

#ifdef _DEBUG

#ifdef _OPENCV_3_2
#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>
#pragma comment(lib, "opencv_calib3d320d.lib")
#pragma comment(lib, "opencv_core320d.lib")
#pragma comment(lib, "opencv_features2d320d.lib")
#pragma comment(lib, "opencv_flann320d.lib")
#pragma comment(lib, "opencv_highgui320d.lib")
#pragma comment(lib, "opencv_imgcodecs320d.lib")
#pragma comment(lib, "opencv_imgproc320d.lib")
#pragma comment(lib, "opencv_ml320d.lib")
#pragma comment(lib, "opencv_objdetect320d.lib")
#pragma comment(lib, "opencv_photo320d.lib")
#pragma comment(lib, "opencv_shape320d.lib")
#pragma comment(lib, "opencv_stitching320d.lib")
#pragma comment(lib, "opencv_superres320d.lib")
#pragma comment(lib, "opencv_ts320d.lib")
#pragma comment(lib, "opencv_video320d.lib")
#pragma comment(lib, "opencv_videoio320d.lib")
#pragma comment(lib, "opencv_videostab320d.lib")

#endif

#else
#pragma comment(lib, "opencv_calib3d320.lib")
#pragma comment(lib, "opencv_core320.lib")
#pragma comment(lib, "opencv_features2d320.lib")
#pragma comment(lib, "opencv_flann320.lib")
#pragma comment(lib, "opencv_highgui320.lib")
#pragma comment(lib, "opencv_imgcodecs320.lib")
#pragma comment(lib, "opencv_imgproc320.lib")
#pragma comment(lib, "opencv_ml320.lib")
#pragma comment(lib, "opencv_objdetect320.lib")
#pragma comment(lib, "opencv_photo320.lib")
#pragma comment(lib, "opencv_shape320.lib")
#pragma comment(lib, "opencv_stitching320.lib")
#pragma comment(lib, "opencv_superres320.lib")
#pragma comment(lib, "opencv_ts320.lib")
#pragma comment(lib, "opencv_video320.lib")
#pragma comment(lib, "opencv_videoio320.lib")
#pragma comment(lib, "opencv_videostab320.lib")
#endif

using namespace cv;