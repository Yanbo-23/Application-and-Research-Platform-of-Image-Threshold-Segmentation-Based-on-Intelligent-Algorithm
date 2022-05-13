#ifndef _PLOT_H
#define _PLOT_H
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/highgui/highgui_c.h>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
using namespace cv;
using namespace std;
struct LineType
{
	char type;
	bool is_need_lined;
	Scalar color;
};

class CPlot
{
public:
	void DrawAxis(IplImage* image, float THEMIN); 
	void DrawData(IplImage* image); 
	int window_height; 
	int window_width;
	vector< vector<CvPoint2D64f> >dataset;	
	vector<LineType> lineTypeSet; 
	CvScalar Color_0;
	CvScalar Color_1;
	CvScalar Color_2;
	IplImage* Figure;
	bool yJudge;
	float yMax;
	float yMin;
	float yScale;
	bool xJudge;
	float xMax;
	float xMin;
	float xScale;
	int border_size;
	template<class T>
	void plot(T* y, size_t Cnt, CvScalar color, char type = '*', bool is_need_lined = true);
	template<class T>
	void plot(T* x, T* y, size_t Cnt, CvScalar color, char type = '*', bool is_need_lined = true);
	void xlabel(string xlabel_name, CvScalar label_color);
	void ylabel(string ylabel_name, CvScalar label_color);
	void clear();
	void title(string title_name, CvScalar title_color);
	CPlot();
	~CPlot();
};
CPlot::CPlot()
{
	this->border_size = 40; 
	this->window_height = WINDOW_HEIGHT;
	this->window_width = WINDOW_WIDTH;
	this->Figure = cvCreateImage(cvSize(this->window_height, this->window_width), IPL_DEPTH_8U, 3);
	memset(Figure->imageData, 255, sizeof(unsigned char) * Figure->widthStep * Figure->height);
	this->Color_0 = cvScalar(CV_RGB(255, 255, 255)); 
	this->Color_1 = cvScalar(CV_RGB(0, 0, 0));
	this->Color_2 = cvScalar(CV_RGB(255, 0, 0)); 
	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
}

CPlot::~CPlot()
{
	this->clear();
	cvReleaseImage(&(this->Figure));
}


void CPlot::clear()
{
	this->dataset.clear();
	this->lineTypeSet.clear();
}

void CPlot::title(string title_name, CvScalar title_color = cvScalar(0, 0, 0))
{
	int chw = 6, chh = 10;
	IplImage* image = this->Figure;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 2, 0.7, 0, 1, CV_AA);
	int x = (this->window_width - 2 * this->border_size) / 2 + this->border_size - (title_name.size() / 2.0) * chw;
	int y = this->border_size / 2;
	cvPutText(image, title_name.c_str(), cvPoint(x, y), &font, title_color);
}

void CPlot::xlabel(string xlabel_name, CvScalar label_color = cvScalar(0, 0, 0))
{
	int chw = 6, chh = 10;
	int bs = this->border_size;
	int h = this->window_height;
	int w = this->window_width;
	float y_ref = this->yMin;
	if ((this->yMax > 0) && (this->yMin <= 0))
	{
		y_ref = 0;
	}
	int x_axis_pos = h - bs - cvRound((y_ref - this->yMin) * this->yScale);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5, 0.7, 0, 1, CV_AA);
	int x = this->window_width - this->border_size - chw * xlabel_name.size();
	int y = x_axis_pos + bs / 1.5;
	cvPutText(this->Figure, xlabel_name.c_str(), cvPoint(x, y), &font, label_color);
}
void CPlot::ylabel(string yLabelName, CvScalar labelColor = cvScalar(0, 0, 0))
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5, 0.7, 0, 1, CV_AA);
	int x = this->border_size;
	int y = this->border_size;
	cvPutText(this->Figure, yLabelName.c_str(), cvPoint(x, y), &font, labelColor);
}

void CPlot::DrawAxis(IplImage* image, float THEMIN)
{
	CvScalar axis_color = this->Color_1;
	int isOver = this->border_size;
	int h = this->window_height;
	int w = this->window_width;
	int gh = h - isOver * 2;
	int gw = w - isOver * 2;
	float y_ = this->yMin;
	if ((this->yMax > 0) && (this->yMin <= 0))
	{
		y_ = 0;
	}
	int x_axis_pos = h - isOver - cvRound((y_ - this->yMin) * this->yScale);
	cvLine(image, cvPoint(isOver, x_axis_pos),
		cvPoint(w - isOver, x_axis_pos),
		axis_color);
	cvLine(image, cvPoint(isOver, h - isOver),
		cvPoint(isOver, h - isOver - gh),
		axis_color);

	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 0.55, 0.7, 0, 1, CV_AA);

	int chw = 6, chh = 10;
	char text[16];

	if ((this->yMax - y_) > 0.05 * (this->yMax - this->yMin))
	{
		_snprintf_s(text, sizeof(text) - 1, "%.1f", this->yMax);
		cvPutText(image, text, cvPoint(isOver, isOver / 2), &font, this->Color_2);
	}
	if ((y_ - this->yMin) > 0.05 * (this->yMax - this->yMin))
	{
		_snprintf_s(text, sizeof(text) - 1, "%.1f", this->yMin);
		cvPutText(image, text, cvPoint(isOver, h - isOver / 2), &font, this->Color_2);
	}

	float y_scale_pixes = chh * 2;
	for (int i = 0; i < ceil((x_axis_pos - isOver) / y_scale_pixes) + 1; i++)
	{
		_snprintf_s(text, sizeof(text) - 1, "%.1f", i * y_scale_pixes / this->yScale + THEMIN);
		cvPutText(image, text, cvPoint(isOver / 5, x_axis_pos - i * y_scale_pixes), &font, this->Color_1);
	}
	for (int i = 1; i < ceil((h - x_axis_pos - isOver) / y_scale_pixes) + 1; i++)
	{
		_snprintf_s(text, sizeof(text) - 1, "%.1f", -i * y_scale_pixes / this->yScale);
		cvPutText(image, text, cvPoint(isOver / 5, x_axis_pos + i * y_scale_pixes), &font, this->Color_1);
	}

	_snprintf_s(text, sizeof(text) - 1, "%.1f", this->xMax);
	cvPutText(image, text, cvPoint(w - isOver / 2 - strlen(text) * chw, x_axis_pos), &font, this->Color_2);

	_snprintf_s(text, sizeof(text) - 1, "%.1f", this->xMin);
	cvPutText(image, text, cvPoint(isOver, x_axis_pos), &font, this->Color_2);

	float x_scale_pixes = chw * 4;
	for (int i = 1; i < ceil(gw / x_scale_pixes) + 1; i++)
	{
		_snprintf_s(text, sizeof(text) - 1, "%.0f", this->xMin + i * x_scale_pixes / this->xScale);
		cvPutText(image, text, cvPoint(isOver + i * x_scale_pixes - isOver / 4, x_axis_pos + chh), &font, this->Color_1);
	}
}

       
void CPlot::DrawData(IplImage* image)
{
	int bs = this->border_size;
	for (size_t i = 0; i < this->dataset.size(); i++)
	{
		for (size_t j = 0; j < this->dataset[i].size(); j++)
		{
			if (this->dataset[i][j].x < this->xMin)
			{
				this->xMin = this->dataset[i][j].x;
			}
			else if (this->dataset[i][j].x > this->xMax)
			{
				this->xMax = this->dataset[i][j].x;
			}

			if (this->dataset[i][j].y < this->yMin)
			{
				this->yMin = this->dataset[i][j].y;
			}
			else if (this->dataset[i][j].y > this->yMax)
			{
				this->yMax = this->dataset[i][j].y;
			}
		}
	}
	float x_range = this->xMax - this->xMin;
	float y_range = this->yMax - this->yMin;
	this->xScale = (image->width - bs * 2) / x_range;
	this->yScale = (image->height - bs * 2) / y_range;
	memset(image->imageData, 255, sizeof(unsigned char) * Figure->widthStep * Figure->height);
	this->DrawAxis(image, yMin);
	float tempX, tempY;
	CvPoint prev_point, current_point;
	int radius = 3;
	int slope_radius = (int)(radius * 1.414 / 2 + 0.5);
	for (size_t i = 0; i < this->dataset.size(); i++)
	{
		for (size_t j = 0; j < this->dataset[i].size(); j++)
		{
			tempX = (int)((this->dataset[i][j].x - this->xMin) * this->xScale);
			tempY = (int)((this->dataset[i][j].y - this->yMin) * this->yScale);
			current_point = cvPoint(bs + tempX, image->height - (tempY + bs));

			if (this->lineTypeSet[i].type == 'l')
			{
				if (j >= 1)
				{
					cvLine(image, prev_point, current_point, cvScalar(lineTypeSet[i].color), 1, CV_AA);
				}
				prev_point = current_point;
			}
			else if (this->lineTypeSet[i].type == '.')
			{
				cvCircle(image, current_point, 1, cvScalar(lineTypeSet[i].color), -1, 8);
				if (lineTypeSet[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						cvLine(image, prev_point, current_point, cvScalar(lineTypeSet[i].color), 1, CV_AA);
					}
					prev_point = current_point;
				}
			}

		}
	}
}
class Plot : public CPlot
{
public:
	template<class T>
	void plot(vector<T> Y, CvScalar color, char type = '*', bool is_need_lined = true);
	template<class T>
	void plot(vector< Point_<T> > p, CvScalar color, char type = '*', bool is_need_lined = true);
	Mat figure()
	{
		return cvarrToMat(this->Figure);
	}
	Plot(float x_min, float x_max, float y_min, float y_max) {
		this->xMin = x_min;
		this->xMax = x_max;
		this->yMin = y_min;
		this->yMin = y_max;
	}
};

template<class T>
void Plot::plot(vector<T> Y, CvScalar color, char type, bool is_need_lined)
{
	T tempX, tempY;
	vector<CvPoint2D64f>data;
	for (int i = 0; i < Y.size(); i++)
	{
		tempX = i;
		tempY = Y[i];
		data.push_back(cvPoint2D64f((float)tempX, (float)tempY));
	}
	this->dataset.push_back(data);
	LineType LT;
	LT.type = type;
	LT.color = color;
	LT.is_need_lined = is_need_lined;
	this->lineTypeSet.push_back(LT);
	this->DrawData(this->Figure);
}
#endif
