#pragma once
#include <opencv2/opencv.hpp>

using namespace std;

class ColorMoment
{
private:
	IplImage* h_plane;
	IplImage* s_plane;
	IplImage* v_plane;
public:
	float h_mean;
	float s_mean;
	float v_mean;
	float h_variance;
	float s_variance;
	float v_variance;
	float h_skewness;
	float s_skewness;
	float v_skewness;
public:
	ColorMoment(IplImage** hsv_planes);
	~ColorMoment(void);
	void calMean();
	void calVariance();
	void calSkewness();
};
