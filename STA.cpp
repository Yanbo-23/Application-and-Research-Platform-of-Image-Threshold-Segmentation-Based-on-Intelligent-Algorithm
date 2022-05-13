#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random> 
#include <windows.h>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>
#include<cmath>
using namespace cv;
using namespace std;
typedef float (*pf)(Mat, Mat);

struct fitness_struct
{
	Mat Best;
	float fBest;
};
struct min_struct
{
	float minum;
	int index;
};

//定义函数指针

int show_Data(Mat a)
{
	int sum = 0;
	for (int r = 0; r < a.rows; r++)
	{
		for (int c = 0; c < a.cols; c++)
		{
			sum += a.at<int>(r, c);
			cout << a.at<int>(r, c) << ",";
		}
		cout << endl;
	}
	return sum;

}

int showHis(Mat image) {
	Mat image_gray, hist;   //定义输入图像，灰度图像, 直方图
	if (image.empty())
	{
		cout << "Wrong" << endl;
		return -1;
	}

	cvtColor(image, image_gray, COLOR_BGR2GRAY);  //灰度化

	//获取图像直方图
	int histsize = 256;
	float ranges[] = { 0,256 };
	const float* histRanges = { ranges };
	calcHist(&image_gray, 1, 0, Mat(), hist, 1, &histsize, &histRanges, true, false);

	//创建直方图显示图像
	int hist_h = 300;//直方图的图像的高
	int hist_w = 512; //直方图的图像的宽
	int bin_w = hist_w / histsize;//直方图的等级
	Mat histImage(hist_h, hist_w, CV_8U, Scalar(0, 0, 0));//绘制直方图显示的图像

	//绘制并显示直方图
	normalize(hist, hist, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化直方图
	for (int i = 1; i < histsize; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
	}
	imshow("histImage", histImage);

	waitKey(0);  //暂停，保持图像显示，等待按键结束
	/*show_data(histimage);*/
	return 0;
}

int imgShow(Mat img)
{
	imshow(" image_gray", img);   //显示灰度图像
	waitKey(0);  //暂停，保持图像显示，等待按键结束
	return 0;
}
Mat repmat(Mat matrix, int column, int row)
{
	Mat New_h = matrix;
	Mat New_v;
	for (int i = 1; i < row; i++)
		cv::hconcat(New_h, matrix, New_h);
	New_v = New_h;
	for (int i = 1; i < column; i++)
		cv::vconcat(New_v, New_h, New_v);
	return New_v;
}

Mat randn(int row, int column, int left, int right) {
	RNG rng = RNG(rand());
	Mat_<int>randuM(row, column);
	rng.fill(randuM, RNG::UNIFORM, left, right + 1);
	return randuM;
}



Mat rand(int row, int column) {
	Mat a(row, column, CV_32F);
	// 实例化一个随机数发生器
	RNG rng = RNG(rand());
	// 使用fill方法
	rng.fill(a, RNG::UNIFORM, 0.f, 1.f);
	// 输出结果
	return a;


}
min_struct min(Mat m) {
	min_struct result;
	double minVal, maxVal;
	int  minIdx[2] = {}, maxIdx[2] = {};	// minnimum Index, maximum Index
	minMaxIdx(m, &minVal, &maxVal, minIdx, maxIdx);
	result.minum = minVal;
	if (m.size[0] == 1 || m.size[1] == 1)
		result.index = max(minIdx[0], minIdx[1]);
	return result;
}

double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}
Mat normrnd(int row, int column) {
	Mat X = Mat::ones(row, column, CV_32F);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			X.at<float>(i, j) = float(gaussrand());
		}
	}
	return X;

}
void kron(cv::InputArray m1, cv::InputArray m2, cv::OutputArray mkron)
{
	int ma = m1.rows(), na = m1.cols(), mb = m2.rows(), nb = m2.cols();

	CV_Assert(m1.type() == m2.type() &&
		(m1.type() == CV_32F || m1.type() == CV_64F || m1.type() == CV_32S || m1.type() == CV_16S));
	cv::Mat K(ma * mb, na * nb, m1.type());
	cv::Mat tm1 = m1.getMat();
	cv::Mat tm2 = m2.getMat();
	cv::Rect roi;
	switch (m1.type())
	{
	case CV_64F:
	{
		double* _data = (double*)tm1.data;

		for (int i = 0; i < ma; i++)
		{
			for (int j = 0; j < na; j++)
			{
				roi.y = i * mb, roi.x = j * nb;
				roi.width = nb, roi.height = mb;
				tm2.convertTo(K(roi), m1.type(), _data[i * na + j]);
			}
		}
		K.copyTo(mkron);
		break;
	}

	case CV_32F:
	{
		float* _data = (float*)tm1.data;

		for (int i = 0; i < ma; i++)
		{
			for (int j = 0; j < na; j++)
			{
				roi.y = i * mb, roi.x = j * nb;
				roi.width = nb, roi.height = mb;
				tm2.convertTo(K(roi), m1.type(), _data[i * na + j]);
			}
		}
		K.copyTo(mkron);
		break;
	}
	case CV_32S:
	{
		int* _data = (int*)tm1.data;

		for (int i = 0; i < ma; i++)
		{
			for (int j = 0; j < na; j++)
			{
				roi.y = i * mb, roi.x = j * nb;
				roi.width = nb, roi.height = mb;
				tm2.convertTo(K(roi), m1.type(), _data[i * na + j]);
			}
		}
		K.copyTo(mkron);
		break;
	}
	case CV_16S:
	{
		short* _data = (short*)tm1.data;

		for (int i = 0; i < ma; i++)
		{
			for (int j = 0; j < na; j++)
			{
				roi.y = i * mb, roi.x = j * nb;
				roi.width = nb, roi.height = mb;
				tm2.convertTo(K(roi), m1.type(), _data[i * na + j]);
			}
		}
		K.copyTo(mkron);
		break;
	}
	default:
		break;
	}
}
////------------------------------------------------------------------- fun---------------------------------------------------------------------
float Sphere(Mat x, Mat h)
{
	pow(x, 2, x);
	float s = cv::sum(x)[0];
	cout << h << endl;
	return s;

}

//`````````````````````````````````````````````````````````STA``````````````````````````````````````
fitness_struct fitness(pf p, Mat State, Mat h)
{
	int SE;
	SE = State.size[0];
	State.rowRange(0, 1);
	double minVal, maxVal;
	int    minIdx[2] = {}, maxIdx[2] = {};
	Mat fState = Mat::zeros(SE, 1, CV_32F);
	Mat State_ = State.clone();
	for (int i = 1; i <= SE; i++) {
		fState.at<float>(i - 1) = p(State_.rowRange(i - 1, i).clone(), h);
	}
	min_struct mi = min(fState);
	float fGBest = mi.minum;
	int g = mi.index;
	float fBest = fGBest;
	Mat Best = State.rowRange(g, g + 1).clone();
	fitness_struct result;
	result.Best = Best.clone();
	result.fBest = fBest;
	return result;
}
class STA
{
public:
	pf p;
	int SE;
	int Dim;
	int Iterations;
	Mat h;
	float low;
	float up;
	//---------------------------------------------
	//	 parameter setting
	float alpha_max = 50;
	float alpha_min = 1;
	float lemada_max = 20;
	float lemada_min = 1;
	float alpha = alpha_max;
	float beta = 1;
	float gamma = 1;
	float delta = 1;

	float fc = 1.2;
	int iter = 1;
	float oldfBest;
	Mat Best;
	float fBest;
	Mat history;

	STA(pf p, int SE, int Dim, int Iterations, Mat h, float low, float up)
	{
		this->p = p;
		this->SE = SE;
		this->Dim = Dim;
		this->Iterations = Iterations;
		this->h = h.clone();
		this->low = low;
		this->up = up;
	}

	void setAlpha_max(float a)
	{
		alpha_max = a;
	}
	void setAlpha_min(float a)
	{
		alpha_min = a;
	}
	void setBeta(float a)
	{
		beta = a;
	}
	void setGamma(float a)
	{
		gamma = a;
	}
	void setDelta(float a)
	{
		delta = a;
	}
	void setFc(float a)
	{
		fc = a;
	}


	void STAMain()
	{
		history = Mat::zeros(1, Iterations, CV_32F);
		Mat bound = (Mat_ < float >(2, 1) << low, up);
		Mat Range = repmat(bound, 1, Dim);
		Mat Pop_Lb = Range.rowRange(0, 1);
		Mat Pop_Ub = Range.rowRange(1, 2);
		Mat oldBest;
		Mat y;
		Mat newBest;
		Mat temp;
		Mat A;
		Mat index;
		Mat State = rand(SE, Dim).mul(repmat(Pop_Ub - Pop_Lb, SE, 1)) + repmat(Pop_Lb, SE, 1);
		fitness_struct value = fitness(p, State.clone(), h);
		Best = value.Best.clone();
		fBest = value.fBest;
		//	 iterative process
		for (iter = 0; iter < Iterations; iter++) {
			if (alpha < alpha_min)
				alpha = alpha_max;

			oldfBest = fBest;

			////-----------------------------------------[Best, fBest] = expand(funfcn, Best, SE, Range, beta, gamma, h);-----------------------------------------
			oldBest = Best.clone();
			//// expand
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			int flag = 0;

			/////--------------------------------------------------------------------------------------------------
			int n = Best.total();

			y = repmat(Best.t(), 1, SE) + gamma * (normrnd(n, SE).mul(repmat(Best.t(), 1, SE)));
			y = y.t();
			State = y.clone();
			for (int i = 0; i < State.size[0]; i++) {
				for (int j = 0; j < State.size[1]; j++) {
					if (State.at<float>(i, j) < low)
						State.at<float>(i, j) = low;
					if (State.at<float>(i, j) > up)
						State.at<float>(i, j) = up;
				}
			}
			/////--------------------------------------------------------------------------------------------------
			value = fitness(p, State.clone(), h);
			if (value.fBest < fBest) {
				fBest = value.fBest;
				Best = value.Best.clone();
				flag = 1;
			}
			else
				flag = 0;
			if (flag == 1) {
				/////--------------------------------------------------------------------------------------------------
				n = oldBest.total();
				newBest = Best.clone();
				kron(rand(SE, 1), (newBest - oldBest).t(), temp);

				y = repmat(newBest.t(), 1, SE) + beta / (norm(newBest - oldBest)) * (temp.reshape(0, n));
				y = y.t();
				State = y.clone();
				for (int i = 0; i < State.size[0]; i++) {
					for (int j = 0; j < State.size[1]; j++) {
						if (State.at<float>(i, j) < low)
							State.at<float>(i, j) = low;
						if (State.at<float>(i, j) > up)
							State.at<float>(i, j) = up;
					}
				}
				value = fitness(p, State.clone(), h);
				if (value.fBest < fBest) {
					fBest = value.fBest;
					Best = value.Best.clone();
				}

			}
			//////-------------------------------------------[Best, fBest] = axesion(funfcn, Best, SE, Range, beta, delta, h);---------------------
			oldBest = Best.clone();
			//// 
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			flag = 0;

			/////--------------------------------------------------------------------------------------------------
			n = Best.total();
			A = Mat::zeros(SE, n, CV_32F);
			index = randn(1, SE, 1, n);
			for (int i = 0; i < index.total(); i++) {
				index.at<int>(0, i) = round(index.at<int>(0, i) + float(n * i));
			}

			for (int i = 0; i < index.total(); i++) {
				A.at<float>(index.at<int>(i) - 1) = 1;
			}
			A = A.t();

			y = repmat(Best.t(), 1, SE) + delta * normrnd(n, SE).mul(A.mul(repmat(Best.t(), 1, SE)));
			y = y.t();
			State = y.clone();
			/////--------------------------------------------------------------------------------------------------
			for (int i = 0; i < State.size[0]; i++) {
				for (int j = 0; j < State.size[1]; j++) {
					if (State.at<float>(i, j) < low)
						State.at<float>(i, j) = low;
					if (State.at<float>(i, j) > up)
						State.at<float>(i, j) = up;
				}
			}
			/////--------------------------------------------------------------------------------------------------
			value = fitness(p, State.clone(), h);
			if (value.fBest < fBest) {
				fBest = value.fBest;
				Best = value.Best.clone();
				flag = 1;
			}
			else
				flag = 0;

			if (flag == 1) {
				/////--------------------------------------------------------------------------------------------------
				n = oldBest.total();
				newBest = Best.clone();
				kron(rand(SE, 1), (newBest - oldBest).t(), temp);
				y = repmat(newBest.t(), 1, SE) + beta / (norm(newBest - oldBest)) * temp.reshape(0, n);
				y = y.t();
				State = y.clone();
				for (int i = 0; i < State.size[0]; i++) {
					for (int j = 0; j < State.size[1]; j++) {
						if (State.at<float>(i, j) < low)
							State.at<float>(i, j) = low;
						if (State.at<float>(i, j) > up)
							State.at<float>(i, j) = up;
					}
				}
				value = fitness(p, State.clone(), h);
				if (value.fBest < fBest) {
					fBest = value.fBest;
					Best = value.Best.clone();
				}

			}

			////-----------------------------------[Best, fBest] = rotate(funfcn, Best, SE, Range, alpha, beta, h);------------
			oldBest = Best.clone();
			//// 
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			flag = 0;

			/////--------------------------------------------------------------------------------------------------
			n = Best.total();
			y = repmat(Best.t(), 1, SE) + alpha * (1.0 / n / (norm(Best))) * ((rand(SE * n, n) * 2 - 1) * Best.t(), n, SE);
			y = y.t();
			State = y.clone();
			/////--------------------------------------------------------------------------------------------------
			for (int i = 0; i < State.size[0]; i++) {
				for (int j = 0; j < State.size[1]; j++) {
					if (State.at<float>(i, j) < low)
						State.at<float>(i, j) = low;
					if (State.at<float>(i, j) > up)
						State.at<float>(i, j) = up;
				}
			}
			/////--------------------------------------------------------------------------------------------------
			value = fitness(p, State.clone(), h);
			if (value.fBest < fBest) {
				fBest = value.fBest;
				Best = value.Best.clone();
				flag = 1;
			}
			else
				flag = 0;

			if (flag == 1) {
				/////--------------------------------------------------------------------------------------------------
				n = oldBest.total();
				newBest = Best.clone();
				kron(rand(SE, 1), (newBest - oldBest).t(), temp);
				y = repmat(newBest.t(), 1, SE) + beta / (norm(newBest - oldBest)) * temp.reshape(0, n);
				y = y.t();
				State = y.clone();
				for (int i = 0; i < State.size[0]; i++) {
					for (int j = 0; j < State.size[1]; j++) {
						if (State.at<float>(i, j) < low)
							State.at<float>(i, j) = low;
						if (State.at<float>(i, j) > up)
							State.at<float>(i, j) = up;
					}
				}
				value = fitness(p, State.clone(), h);
				if (value.fBest < fBest) {
					fBest = value.fBest;
					Best = value.Best.clone();
				}

			}
			////----------------------------------------------end------------------
			history.at<float>(iter) = fBest;
			alpha = alpha / fc;
			cout << iter + 1 << endl;
		}
	}
};