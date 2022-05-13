#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random> 
#include <windows.h>
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
int imgShow(Mat img)
{
	imshow(" image_gray", img);   
	waitKey(0);  
	return 0;
}
void showHis(Mat grayImage) {

	int channels[] = { 0 };

	const int histSize[] = { 256 };

	float range[] = { 0,256 };


	const float* ranges[] = { range };


	Mat hist;

	calcHist(&grayImage, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);


	double maxValue = 0;

	minMaxLoc(hist, 0, &maxValue, 0, 0);

	int rows = cvRound(maxValue);

	Mat histImage = Mat::zeros(rows, 256, CV_8UC1);

	for (int i = 0; i < 256; i++)
	{

		int temp = (int)(hist.at<float>(i, 0));

		if (temp)
		{
			histImage.col(i).rowRange(Range(rows - temp, rows)) = 255;
		}
	}

	Mat resizeImage;
	resize(histImage, resizeImage, Size(512, 512));
	imgShow(resizeImage);

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

	RNG rng = RNG(rand());

	rng.fill(a, RNG::UNIFORM, 0.f, 1.f);

	return a;


}
min_struct min(Mat m) {
	min_struct result;
	double minVal, maxVal;
	int  minIdx[2] = {}, maxIdx[2] = {};	
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
float Sphere(Mat x, Mat h)
{
	pow(x, 2, x);
	float s = cv::sum(x)[0];
	return s;

}

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
	float alpha_max = 50;
	float alpha_min = 1;
	float lemada_max = 20;
	float lemada_min = 1;
	float alpha;
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
		alpha= 2;
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
		for (iter = 0; iter < Iterations; iter++) {
			if (alpha < alpha_min)
				alpha = alpha_max;

			oldfBest = fBest;

			oldBest = Best.clone();
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			int flag = 0;
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
			value = fitness(p, State.clone(), h);
			if (value.fBest < fBest) {
				fBest = value.fBest;
				Best = value.Best.clone();
				flag = 1;
			}
			else
				flag = 0;
			if (flag == 1) {
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
			oldBest = Best.clone();
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			flag = 0;
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
				flag = 1;
			}
			else
				flag = 0;

			if (flag == 1) {
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
			oldBest = Best.clone();
			Best = oldBest.clone();
			fBest = p(Best.clone(), h);
			flag = 0;
			n = Best.total();
			y = repmat(Best.t(), 1, SE) + alpha * (1.0 / n / (norm(Best))) * ((rand(SE * n, n) * 2 - 1) * Best.t(), n, SE);
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
				flag = 1;
			}
			else
				flag = 0;

			if (flag == 1) {
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
			history.at<float>(iter) = fBest;
			alpha = alpha / fc;
		}
	}
};
class PSO
{
public:
	pf p;
	int SE;
	int Dim;
	int Iterations;
	Mat h;
	float low;
	float up;
	float w = 1;
	float wdamp = 0.99;
	float c1 = 1.5;
	float c2 = 2.0;
	Mat VarSize = (Mat_ < float >(1, 2) << 1, Dim);
	Mat Best;
	float fBest;
	Mat history;
	PSO(pf p, int SE, int Dim, int Iterations, Mat h, float low, float up)
	{
		this->p = p;
		this->SE = SE;
		this->Dim = Dim;
		this->Iterations = Iterations;
		this->h = h.clone();
		this->low = low;
		this->up = up;
	}
	void Set_w(float w)
	{
		this->w = w;

	}
	void Set_wdamp(float wdamp)
	{
		this->wdamp = wdamp;
	}
	void Set_c1(float c1)
	{
		this->c1 = c1;
	}
	void Set_c2(float c2)
	{
		this->c2 = c2;
	}
	void PSOMain();
private:
	struct empty_particle {
		Mat Position;
		float Cost;
		Mat Velocity;
		Mat BestPosition;
		float BestCost;

	};
	float GlobalBestCost;
	Mat GlobalBestPosition;


};

void PSO::PSOMain()
{
	float VelMax = 0.1 * (up - low);
	float VelMin = -VelMax;
	struct empty_particle* particle = new struct empty_particle[SE];
	GlobalBestCost = 9999999;
	for (int i = 0; i < SE; i++)
	{
		particle[i].Position = rand(1, Dim).clone() * (up - low) + low;
		particle[i].Velocity = Mat::zeros(1, Dim, CV_32F);
		particle[i].Cost = p(particle[i].Position.clone(), h);
		particle[i].BestPosition = particle[i].Position.clone();
		particle[i].BestCost = particle[i].Cost;
		if (particle[i].BestCost < GlobalBestCost) {
			GlobalBestPosition = particle[i].BestPosition.clone();
			GlobalBestCost = particle[i].BestCost;
		}

	}


	Mat BestCost = Mat::zeros(Iterations, 1, CV_32F);
	for (int it = 0; it < Iterations; it++) {

		for (int i = 0; i < SE; i++) {

			try{
				particle[i].Velocity = w * particle[i].Velocity.clone()
				+ c1 * rand(1, Dim).mul(particle[i].BestPosition.clone() - particle[i].Position.clone())
				+ c2 * rand(1, Dim).mul(GlobalBestPosition.clone() - particle[i].Position.clone());
			}
		catch (exception e)
		{
			
		}
			for (int j = 0; j < Dim; j++) {
				particle[i].Velocity.at<float>(j) = max(particle[i].Velocity.at<float>(j), VelMin);
				particle[i].Velocity.at<float>(j) = min(particle[i].Velocity.at<float>(j), VelMax);
			}
			particle[i].Position = particle[i].Position.clone() + particle[i].Velocity.clone();

			for (int j = 0; j < Dim; j++) {
				particle[i].Position.at<float>(j) = max(particle[i].Position.at<float>(j), low);
				particle[i].Position.at<float>(j) = min(particle[i].Position.at<float>(j), up);
			}

			particle[i].Cost = p(particle[i].Position.clone(), h);

			if (particle[i].Cost < particle[i].BestCost) {

				particle[i].BestPosition = particle[i].Position.clone();
				particle[i].BestCost = particle[i].Cost;
			}
			if (particle[i].BestCost < GlobalBestCost) {
				GlobalBestCost = particle[i].BestCost;
				GlobalBestPosition = particle[i].BestPosition.clone();
			}

		}

		BestCost.at<float>(it) = GlobalBestCost;
		w = w * wdamp;
		cout << it + 1 << endl;

	}
	history = BestCost.clone();
	Best = GlobalBestPosition.clone();
	min_struct theMin = min(BestCost);
	fBest = theMin.minum;
	for (int i = 0; i < Dim; i++)
		cv::sort(Best, Best, 0);

	delete[] particle;

}


class GA
{
public:
	pf p;
	int SE;
	int Dim;
	int Iterations;
	Mat h;
	float low;
	float up;
	float etac;
	float etam;
	float Pc; 
	float Pm; 
	Mat Best;
	float fBest;
	Mat history;

	GA(pf p, int SE, int Dim, int Iterations, Mat h, float low, float up)
	{
		this->p = p;
		this->SE = SE;
		this->Dim = Dim;
		this->Iterations = Iterations;
		this->h = h.clone();
		this->low = low;
		this->up = up;
		etac = 20;
		etam = 20;
		Pc = 0.8;
		Pm = 0.2;
	}
	void Set_etac(float etac)
	{
		this->etac = etac;

	}
	void Set_etam(float etam)
	{
		this->etam = etam;
	}
	void Set_Pc(float Pc)
	{
		this->Pc = Pc;
	}
	void Set_Pm(float Pm)
	{
		this->Pm = Pm;
	}
	void GAMain();
private:
	struct empty_particle {
		Mat Position;
		float Cost;
		Mat Velocity;
		Mat BestPosition;
		float BestCost;

	};
	float GlobalBestCost;
	Mat GlobalBestPosition;
	Mat Candidate;
	Mat CandidateObj;
	Mat MatingPool;

};


void GA::GAMain()
{
	history = Mat::zeros(1, Iterations, CV_32F);
	Mat	f = Mat::zeros(SE, 1, CV_32F);

	Mat OffspringObj = Mat::zeros(SE, 1, CV_32F);

	Mat lb = repmat((Mat_ < float >(1, 1) << low), 1, Dim);
	Mat ub = repmat((Mat_ < float >(1, 1) << up), 1, Dim);
	int	D = lb.total();

	Mat	P = repmat(lb, SE, 1) + repmat((ub - lb), SE, 1).mul(rand(SE, D));
	Mat Parent = Mat::zeros(SE, Dim, CV_32F);
	std::vector<int> indx(SE);
	min_struct minV;
	Mat offspring;
	Mat CombinedPopulation;
	Mat temp;
	Mat ind;
	float r;
	float beta;
	float delta;
	unsigned seed;
	for (int p_ = 0; p_ < SE; p_++)
		f.at<float>(p_) = p(P.rowRange(p_, p_ + 1).clone(), h);
	for (int t = 0; t < Iterations; t++) {
		MatingPool = Mat::zeros(SE, 1, CV_32F);
		for (int o = 0; o < SE; o++)
			indx[o] = o;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(indx.begin(), indx.end(), std::default_random_engine(seed));

		for (int i = 0; i < SE - 1; i++) { 
			Candidate = (Mat_ < float >(1, 2) << indx[i], indx[i + 1]);
			CandidateObj = (Mat_ < float >(1, 2) << f.at<float>(indx[i]), f.at<float>(indx[i + 1]));
			minV = min(CandidateObj);
			MatingPool.at<float>(i) = Candidate.at<float>(minV.index);
		}
		Candidate = (Mat_ < float >(1, 2) << indx[SE - 1], indx[0]);
		CandidateObj = (Mat_ < float >(1, 2) << f.at<float>(indx[SE - 1]), f.at<float>(indx[0]));
		minV = min(CandidateObj);
		MatingPool.at<float>(SE - 1) = Candidate.at<float>(minV.index);

		for (int i = 0; i < SE; i++) {
			if (i == 0)
				Parent = P.rowRange(MatingPool.at<float>(i), MatingPool.at<float>(i) + 1).clone();
			if (i > 0)
				vconcat(Parent, P.rowRange(MatingPool.at<float>(i), MatingPool.at<float>(i) + 1).clone(), Parent);
		}

		for (int o = 0; o < SE; o++)
			indx[o] = o;
		seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(indx.begin(), indx.end(), std::default_random_engine(seed));
		for (int i = 0; i < SE; i++)
			Parent.at<float>(i) = Parent.at<float>(indx[i]);
		offspring = Mat::zeros(SE, Dim, CV_32F);
		for (int I = 0; I < SE; I += 2) {

			r = rand(1, 1).at<float>(0);

			if (r < Pc) {
				for (int J = 0; J < Dim; J++) {

					r = rand(1, 1).at<float>(0);

					if (r <= 0.5)
						beta = pow((2.0 * r), (1 / (etac + 1)));
					else
						beta = pow((1.0 / (2 * (1 - r))), (1 / (etac + 1)));

					offspring.at<float>(I, J) = 0.5 * (((1 + beta) * Parent.at<float>(I, J)) + (1 - beta) * Parent.at<float>(I + 1, J));
					offspring.at<float>(I + 1, J) = 0.5 * (((1 - beta) * Parent.at<float>(I, J)) + (1 + beta) * Parent.at<float>(I + 1, J));
				}
			}
			else {
				for (int J = 0; J < Dim; J++)
				{
					offspring.at<float>(I, J) = Parent.at<float>(I, J);
					offspring.at<float>(I + 1, J) = Parent.at<float>(I + 1, J);
				}
			}
		}

		for (int I = 0; I < SE; I++) {
			r = rand(1, 1).at<float>(0);
			if (r < Pm) {
				for (int J = 0; J < Dim; J++) {
					r = rand(1, 1).at<float>(0);
					if (r < 0.5)
						delta = pow((2 * r), (1 / (etam + 1)) - 1); 
					else
						delta = 1 - pow((2 * (1 - r)), (1 / (etam + 1))); 						
					offspring.at<float>(I, J) = offspring.at<float>(I, J) + (ub.at<float>(J) - lb.at<float>(J)) * delta; 
				}
			}
		}
		for (int a = 0; a < offspring.size[0]; a++) {
			for (int b = 0; b < offspring.size[1]; b++) {
				if (offspring.at<float>(a, b) < low)
					offspring.at<float>(a, b) = low;
				if (offspring.at<float>(a, b) > up)
					offspring.at<float>(a, b) = up;
			}
		}

		for (int j = 0; j < SE; j++)
			OffspringObj.at<float>(j) = p(offspring.rowRange(j, j + 1).clone(), h);

		vconcat(P, offspring, CombinedPopulation);
		vconcat(f, OffspringObj, temp);
		cv::sortIdx(temp, ind, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);
		cv::sort(temp, f, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);

		f = f.rowRange(0, SE);
		for (int x = 0; x < SE; x++)
			for (int y = 0; y < Dim; y++)
				P.at<float>(x, y) = CombinedPopulation.at<float>(ind.at<int>(x), y);
		history.at<float>(t) = f.at<float>(0);
		cout << t + 1 << endl;
	}
	fBest = f.at<float>(0);
	Best = P.rowRange(0, 1);

}