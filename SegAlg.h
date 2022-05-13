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
#pragma once
float ostu1(Mat t, const Mat image)
{
	float mg = 0;
	float totalNum = 0;
	float S = 0;
	float pk = 0;
	float mk = 0;
	cv::sort(t, t, SORT_EVERY_ROW + 0);
	int m_start = -1;
	int m_end = -1;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			totalNum += image.at<float>(i, j);
		}
	}
	Mat gray = image / totalNum;
	for (int i = 0; i < gray.rows; i++) {
		for (int j = 0; j < gray.cols; j++) {
			mg += (float)(j + 1) * gray.at<float>(i, j);
		}
	}
	for (int i1 = 0; i1 < t.rows; i1++) {
		for (int j1 = 0; j1 < t.cols + 1; j1++) {
			if (j1 == 0) {
				m_start = 0;
			}
			else {
				m_start = (int)(t.at<float>(i1, j1 - 1) + 1);
				if (m_start > 255)
					m_start = 255;
			}
			if (j1 == t.cols) {
				m_end = 255;
			}
			else {
				m_end = (int)(t.at<float>(i1, j1));
			}
			pk = 0;
			mk = 0;
			for (int i = m_start; i < m_end; i++)
			{

				pk += gray.at<float>(0, i);
				mk += (float)(i + 1) * gray.at<float>(0, i);
			}
			if (pk == 0) {
				mk = 0;
			}
			else
				mk = mk / pk;
			if (m_start == m_end) {
				mk = 0;
				pk = 0;
			}
			S += pk * pow((mk - mg), 2);
		}
	}
	return -S;
}
float Kapur(Mat t, const Mat image)
{
	float W = 0;
	double H = 0;
	float inter = 0;
	float totalNum = 0;
	cv::sort(t, t, SORT_EVERY_ROW + 0);
	int m_start = -1;
	int m_end = -1;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			totalNum += image.at<float>(i, j);
		}
	}
	Mat gray = image / totalNum;
	for (int i1 = 0; i1 < t.rows; i1++) {
		for (int j1 = 0; j1 < t.cols + 1; j1++) {
			if (j1 == 0) {
				m_start = 0;
			}
			else {
				m_start = (int)(t.at<float>(i1, j1 - 1) + 1);
				if (m_start > 255)
					m_start = 255;
			}
			if (j1 == t.cols) {
				m_end = 255;
			}
			else {
				m_end = (int)(t.at<float>(i1, j1));
			}
			inter = 0;
			W = 0;
			for (int i = m_start; i < m_end; i++) {
				W += gray.at<float>(0, i);
			}
			for (int i = m_start; i < m_end; i++)
			{

				if (W != 0) {
					if (gray.at<float>(0, i) != 0 && W != 0) {

						inter = inter - (gray.at<float>(0, i) * log(gray.at<float>(0, i) / W) / W);
					}
				}
			}
			if (inter != INFINITE && W != 0) {
				H += inter;
			}

		}
	}
	return (float)-H;
}
float ostu2(Mat t, const Mat image)
{
	float mg = 0;
	float totalNum = 0;
	float S_within = 0;
	float S_between = 0;
	float pk = 0;
	float mk = 0;
	float si = 0;
	float sg = 0;
	cv::sort(t, t, SORT_EVERY_ROW + 0);
	int m_start = -1;
	int m_end = -1;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			totalNum += image.at<float>(i, j);
		}
	}
	Mat gray = image / totalNum;
	for (int i = 0; i < gray.rows; i++) {
		for (int j = 0; j < gray.cols; j++) {
			mg += (float)(j + 1) * gray.at<float>(i, j);
		}
	}
	for (int i = 0; i < 256; i++) {
		sg = sg + image.at<float>(0, i) * pow((i + 1 - mg), 2);
	}
	for (int i1 = 0; i1 < t.rows; i1++) {
		for (int j1 = 0; j1 < t.cols + 1; j1++) {
			if (j1 == 0) {
				m_start = 0;
			}
			else {
				m_start = (int)(t.at<float>(i1, j1 - 1) + 1);
				if (m_start > 255)
					m_start = 255;
			}
			if (j1 == t.cols) {
				m_end = 255;
			}
			else {
				m_end = (int)(t.at<float>(i1, j1));
			}
			pk = 0;
			mk = 0;
			si = 0;
			for (int i = m_start; i < m_end; i++)
			{

				pk += gray.at<float>(0, i);
				mk += (float)(i + 1) * gray.at<float>(0, i);
			}
			mk = mk / pk;
			if (m_start == m_end) {
				mk = 0;
				pk = 0;
			}
			for (int i = m_start; i < m_end; i++)
			{
				si = si + pow((image.at<float>(0, i) - mk), 2);
			}
			S_between += pk * pow((mk - mg), 2);
			S_within += si / sg;
		}
	}
	return -S_between / S_within;
}