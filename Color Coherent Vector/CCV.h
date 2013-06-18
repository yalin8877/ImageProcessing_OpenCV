#pragma once
#include <opencv2/opencv.hpp>
#include "ColorQuantization.h"

#define Num_ColorBin 32

using namespace std;
class CCV
{
private:
	IplImage* h_plane;
	IplImage* s_plane;
	IplImage* v_plane;

	IplImage* quantized;

	int threshold;
	//int bin_size;
	//int counter;

	//* record status of each pixel */
	//* initial value:-2; traversed:-1; incoherence:0; coherence:1 */
	int* coh_Lables;

public:
	int coherence[Num_ColorBin];
	int n_coherence[Num_ColorBin];

private:
	//void traverse_count(int pi,int pj);
	//void lable(int lable);
	void coherenceLable();

public:
	CCV(IplImage** planes, int th);
	~CCV(void);
	void calcCCV();
};
