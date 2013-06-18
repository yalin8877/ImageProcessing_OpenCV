#include <iostream>
#include <opencv2/opencv.hpp>
#include "ColorHistogram.h"
#include "ColorMoment.h"
#include "CCV.h"
#include "ColorCorrelogram.h"

using namespace std;
using namespace cv;

#define cvCvtPixToPlane cvSplit
//* smooth the image */
void smooth(IplImage* plane)
{
	for(int i=1;i<plane->height-1;i++)
	{
		for(int j=1;j<plane->width-1;j++)
		{
			char target=*(plane->imageData+i*plane->widthStep+j);
			char left=*(plane->imageData+i*plane->widthStep+j-1);
			char right=*(plane->imageData+i*plane->widthStep+j+1);
			char up=*(plane->imageData+(i-1)*plane->widthStep+j);
			char down=*(plane->imageData+(i+1)*plane->widthStep+j);
			char upleft=*(plane->imageData+(i-1)*plane->widthStep+j-1);
			char upright=*(plane->imageData+(i-1)*plane->widthStep+j+1);
			char downleft=*(plane->imageData+(i+1)*plane->widthStep+j-1);
			char downright=*(plane->imageData+(i+1)*plane->widthStep+j+1);
			*(plane->imageData+i*plane->widthStep+j)=(target*8+left+right+up+down+upleft+upright+downleft+downright)/16;
		}
	}
}

int main( int argc, char** argv )
{
	cout<<"Choose from the following images:"<<endl;
	cout<<"lena.jpg, lena2.jpg, panda.jpg, car.jpg, blank.jpg, twocolor.jpg, random.jpg"<<endl;
	cout<<"(You can also put your images file into the \"images\" and choose them)"<<endl;
	string file;
	cin>>file;
	file="..\\images\\"+file;
	IplImage * src=NULL;
	while((src=cvLoadImage(file.c_str()))==NULL)
	{
		cout<<"Valid input! Please try again:\n";
		cin>>file;
		file="..\\images\\"+file;
	}

	//* convert to HSV */
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane , v_plane };

	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	//cvShowImage("h",h_plane);
	//cvShowImage("s",s_plane);
	//cvShowImage("v",v_plane);

	//* Color Histogram */
	ColorHistogram* histogram= new ColorHistogram(planes);
	histogram->Show();
	
	//* Color Moment */
	cout<<"*********************** Color Moment **********************"<<endl<<endl;
	ColorMoment* moment=new ColorMoment(planes); 
	moment->calMean();
	moment->calVariance();
	moment->calSkewness();

	//* CCV */
	cout<<"\n*************************** CCV ***************************"<<endl<<endl;
	smooth(h_plane);
	smooth(s_plane);
	smooth(v_plane);

	int threshold=hsv->height*hsv->width/100;
	cout<<"The threshold for CCV is "<<threshold<<endl<<endl;
	CCV* ccv=new CCV(planes,threshold);
	ccv->calcCCV();

	//* Color Correlogram */
	cout<<"\n*********************** Color Correlogram ******************"<<endl<<endl;
	cout<<"The distance range is from 1 to 10"<<endl<<endl;
	ColorCorrelogram* correlogram=new ColorCorrelogram(planes);
	correlogram->calcCorrelogram();

	cvWaitKey(0);
}

