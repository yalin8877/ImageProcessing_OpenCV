#include "ColorMoment.h"
#include <opencv2/opencv.hpp>
#include <stdlib.h>  
#include <math.h>

using namespace std;
using namespace cv;

ColorMoment::ColorMoment(IplImage** hsv_planes)
{
	h_plane=hsv_planes[0];
	s_plane=hsv_planes[1];
	v_plane=hsv_planes[2];
};

ColorMoment::~ColorMoment()
{
	h_plane=NULL;
	s_plane=NULL;
	v_plane=NULL;
}

void ColorMoment::calMean()
{
	int width=h_plane->width; 
	int height=h_plane->height;
	//cout<<width<<endl;
	//cout<<height<<endl<<endl;

	h_mean=0;
	s_mean=0;
	v_mean=0;

	//int max=-500;int min=500;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));

			//if(temp>max)
			//	max=temp;
			//if(temp<min)
			//	min=temp;

			h_mean+=temp;
		}
	}

	//cout<<max<<endl;
	//cout<<min<<endl<<endl;

	h_mean=h_mean/(width*height);
	cout<<"mean of H:"<<(int)h_mean<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(s_plane->imageData+i*s_plane->widthStep+j));

			//if(temp>max)
			//	max=temp;
			//if(temp<min)
			//	min=temp;

			s_mean+=temp;
		}
	}

	//cout<<max<<endl;
	//cout<<min<<endl<<endl;

	s_mean=s_mean/(width*height);
	cout<<"mean of S:"<<(int)s_mean<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(v_plane->imageData+i*v_plane->widthStep+j));

			//if(temp>max)
			//	max=temp;
			//if(temp<min)
			//	min=temp;

			v_mean+=temp;
		}
	}

	//cout<<max<<endl;
	//cout<<min<<endl<<endl;

	v_mean=v_mean/(width*height);
	cout<<"mean of V:"<<(int)v_mean<<endl<<endl;
}

void ColorMoment::calVariance()
{
	int width=h_plane->width; 
	int height=h_plane->height;

	h_variance=0;
	s_variance=0;
	v_variance=0;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			h_variance+=(temp-h_mean)*(temp-h_mean);
		}
	}
	h_variance=sqrt((float)(h_variance/(width*height)));
	cout<<"variance of H:"<<(int)h_variance<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			s_variance+=(temp-s_mean)*(temp-s_mean);
		}
	}
	s_variance=sqrt((float)(s_variance/(width*height)));
	cout<<"variance of S:"<<(int)s_variance<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			v_variance+=(temp-v_mean)*(temp-v_mean);
		}
	}
	v_variance=sqrt((float)(v_variance/(width*height)));
	cout<<"variance of V:"<<(int)v_variance<<endl<<endl;
}

void ColorMoment::calSkewness()
{
	int width=h_plane->width; 
	int height=h_plane->height;

	h_skewness=0;
	s_skewness=0;
	v_skewness=0;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			h_skewness+=pow((float)(temp-h_mean),3);
		}
	}
	h_skewness=(h_skewness>=0)?pow(h_skewness/(width*height),(float)(1.0/3.0)):-pow(-h_skewness/(width*height),(float)(1.0/3.0));
	/*if(_isnan(h_skewness))
		h_skewness=0;*/
	cout<<"skewness of H:"<<(int)h_skewness<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			s_skewness+=pow((float)(temp-s_mean),3);
		}
	}
	s_skewness=(s_skewness>=0)?pow(s_skewness/(width*height),(float)(1.0/3.0)):-pow(-s_skewness/(width*height),(float)(1.0/3.0));
	/*if(_isnan(s_skewness))
		s_skewness=0;*/
	cout<<"skewness of S:"<<(int)s_skewness<<endl;

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			int temp=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			v_skewness+=pow((float)(temp-v_mean),3);
		}
	}
	v_skewness=(v_skewness>=0)?pow(v_skewness/(width*height),(float)(1.0/3.0)):-pow(-v_skewness/(width*height),(float)(1.0/3.0));
	/*if(_isnan(v_skewness))
		v_skewness=0;*/
	cout<<"skewness of V:"<<(int)v_skewness<<endl;
}