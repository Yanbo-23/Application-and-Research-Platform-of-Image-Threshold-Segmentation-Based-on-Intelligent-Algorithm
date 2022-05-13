#pragma once
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <stdio.h>
#include <math.h>
#include <fstream>
using namespace cv;
using namespace std;

float SSIM(cv::Mat image_ref, cv::Mat image_obj)
{
	double C1 = 6.5025, C2 = 58.5225;

	int width = image_ref.cols;
	int height = image_ref.rows;
	double mean_x = 0;
	double mean_y = 0;
	double sigmaX = 0;
	double sigmaY = 0;
	double sigmaXY = 0;
	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			mean_x += image_ref.at<uchar>(v, u);
			mean_y += image_obj.at<uchar>(v, u);

		}
	}
	mean_x = mean_x / width / height;
	mean_y = mean_y / width / height;
	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			sigmaX += (image_ref.at<uchar>(v, u) - mean_x) * (image_ref.at<uchar>(v, u) - mean_x);
			sigmaY += (image_obj.at<uchar>(v, u) - mean_y) * (image_obj.at<uchar>(v, u) - mean_y);
			sigmaXY += abs((image_ref.at<uchar>(v, u) - mean_x) * (image_obj.at<uchar>(v, u) - mean_y));
		}
	}
	sigmaX = sigmaX / (width * height - 1);
	sigmaY = sigmaY / (width * height - 1);
	sigmaXY = sigmaXY / (width * height - 1);
	double fenzi = (2 * mean_x * mean_y + C1) * (2 * sigmaXY + C2);
	double fenmu = (mean_x * mean_x + mean_y * mean_y + C1) * (sigmaX + sigmaY + C2);
	double ssim = fenzi / fenmu;
	return ssim;

}
float SD(vector<float> x)
{
	float Sum = 0;
	float SD=0;
	for (int i = 0; i < x.size(); i++)
		Sum += x[i];
	float avg= Sum / x.size();
	for (int i = 0; i < x.size(); i++)
		SD+= pow(x[i] - avg, 2) / x.size();
	return pow(SD, 0.5);
}

template<typename T>
T SumVector(vector<T>& vec)
{
	T res = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		res += vec[i];
	}
	return res;
}



using namespace std;
void fileEmpty(const string fileName)
{
	fstream file(fileName, ios::out);
}