#include <iostream>
#include < fstream>
#include <windows.h>
#include "opencvref.h"

using namespace cv;
using std::ifstream;
using std::ios_base;

int main(int argc, char *argv[])
{
	// 数据可视化
	int width = 512, height = 512;
	Mat image = Mat::zeros(height, width, CV_8UC3);

	// 设置训练数据
	float labels[4] = { 1, -1, -1, -1 };
	Mat labelsMat(3, 1, CV_32SC1, labels);

	// 样本
	float trainingData[4][2] = { 501,	10,
		255,	10,
		501,	255,
		10,		501 };

	Mat trainingDataMat(3, 2, CV_32FC1, trainingData);

	// 设置SVM参数
	Ptr<ml::SVM> svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setGamma(3);
	svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6));

	// 训练
	svm->train(trainingDataMat, ml::ROW_SAMPLE, labelsMat);

	Vec3b green(0, 255, 0), blue(255, 0, 0);
	// Show the decision regions given by the SVM
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1, 2) << i, j);
			float response = svm->predict(sampleMat);

			if (response == 1)
				image.at<Vec3b>(j, i) = green;
			else if (response == -1)
				image.at<Vec3b>(j, i) = blue;
		}

	// Show the training data
	/*int thickness = -1;
	int lineType = 8;
	circle(image, Point(501, 10), 5, Scalar(0, 0, 0), thickness, lineType);
	circle(image, Point(255, 10), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(image, Point(10, 501), 5, Scalar(255, 255, 255), thickness, lineType);*/

	// Show support vectors
	//thickness = 2;
	//lineType = 8;
	//int c = svm->get_support_vector_count();

	//for (int i = 0; i < c; ++i)
	//{
	//	const float* v = SVM.get_support_vector(i);
	//	circle(image, Point((int)v[0], (int)v[1]), 6, Scalar(128, 128, 128), thickness, lineType);
	//}

	//cv::imwrite("result.png", image);        // save the image 

	//imshow("SVM Simple Example", image); // show it to the user
	//waitKey(0);
}