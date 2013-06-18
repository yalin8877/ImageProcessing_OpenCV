#pragma once
#include <opencv2/opencv.hpp>

using namespace std;

class ColorHistogram
{
private:
	IplImage** planes;
public:
	ColorHistogram(IplImage** hs_planes){planes=hs_planes;};
	~ColorHistogram(void);
	bool Show();
};
