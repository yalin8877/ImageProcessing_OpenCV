#include "CCV.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <queue>

using namespace std;
using namespace cv;

//#define DEBUG_QUANTISITION
//#define DEBUG_COHERENCE

CCV::CCV(IplImage** planes, int th)
{
	h_plane=planes[0];
	s_plane=planes[1];
	v_plane=planes[2];

	threshold=th; 
	//bin_size=bin;

	for(int i=0;i<Num_ColorBin;i++)
	{
		coherence[i]=0;
		n_coherence[i]=0;
	}

	quantized=cvCreateImage( cvGetSize(h_plane), 8, 1 );
	cvZero(quantized);

	//counter=1;

	coh_Lables=new int[h_plane->width*h_plane->height];
	for(int i=0;i<h_plane->width*h_plane->height;i++)
	{
		coh_Lables[i]=-2;
	}

	ColorQuantizition(planes,quantized);
	//cvShowImage("quantized img",quantized);
}

CCV::~CCV(void)
{
}

void CCV::coherenceLable()
{
	int width=quantized->width; 
	int height=quantized->height;
	int stepwidth=quantized->widthStep;

	for(int pi=0;pi<height;pi++)
	{
		for(int pj=0;pj<width;pj++)
		{
			//* go to next pixel if this pixel has been labeled */
			if(coh_Lables[pi*width+pj]!=-2)
				continue;
	
			int color=*(quantized->imageData+pi*stepwidth+pj);

			//* BFS */
			queue<int> pos_i;
			queue<int> pos_j;
			queue<int> pos_i_forlable;
			queue<int> pos_j_forlable;

			pos_i.push(pi);
			pos_j.push(pj);
			pos_i_forlable.push(pi);
			pos_j_forlable.push(pj);

			#ifdef DEBUG_COHERENCE
			cout<<"pos_i.push  "<<pi<<endl;
			cout<<"pos_j.push  "<<pj<<endl;
			cout<<"pos_i_forlable.push  "<<pi<<endl;
			cout<<"pos_j_forlable.push  "<<pj<<endl<<endl;
			#endif
			
			coh_Lables[pi*width+pj]=-1;

			while(!pos_i.empty())
			{
				int i=pos_i.front();
				int j=pos_j.front();
				pos_i.pop();
				pos_j.pop();

				#ifdef DEBUG_COHERENCE
				//* front - pop ? */
				cout<<"pos_i.front  "<<i<<endl;	
				cout<<"pos_j.front  "<<j<<endl;
				cout<<"pos_i.pop  "<<i<<endl;
				cout<<"pos_j.pop  "<<j<<endl<<endl;
				#endif

				//* same color and not visited */
				if(i!=0&&j!=0&&(*(quantized->imageData+(i-1)*stepwidth+j-1))==color&&coh_Lables[(i-1)*width+j-1]==-2)
				{
					coh_Lables[(i-1)*width+j-1]=-1;
					pos_i.push(i-1);
					pos_j.push(j-1);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j-1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i-1<<endl;
					cout<<"pos_j.push  "<<j-1<<endl;
					cout<<"pos_i_forlable.push  "<<i-1<<endl;
					cout<<"pos_j_forlable.push  "<<j-1<<endl<<endl;
					#endif
				}
				if(i!=0&&(*(quantized->imageData+(i-1)*stepwidth+j))==color&&coh_Lables[(i-1)*width+j]==-2)
				{
					coh_Lables[(i-1)*width+j]=-1;
					pos_i.push(i-1);
					pos_j.push(j);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i-1<<endl;
					cout<<"pos_j.push  "<<j<<endl;
					cout<<"pos_i_forlable.push  "<<i-1<<endl;
					cout<<"pos_j_forlable.push  "<<j<<endl<<endl;
					#endif
				}
				if(i!=0&&j!=width-1&&(*(quantized->imageData+(i-1)*stepwidth+j+1))==color&&coh_Lables[(i-1)*width+j+1]==-2)
				{
					coh_Lables[(i-1)*width+j+1]=-1;
					pos_i.push(i-1);
					pos_j.push(j+1);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j+1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i-1<<endl;
					cout<<"pos_j.push  "<<j+1<<endl;
					cout<<"pos_i_forlable.push  "<<i-1<<endl;
					cout<<"pos_j_forlable.push  "<<j+1<<endl<<endl;
					#endif
				}
				if(j!=0&&(*(quantized->imageData+i*stepwidth+j-1))==color&&coh_Lables[i*width+j-1]==-2)
				{
					coh_Lables[i*width+j-1]=-1;
					pos_i.push(i);
					pos_j.push(j-1);
					pos_i_forlable.push(i);
					pos_j_forlable.push(j-1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i<<endl;
					cout<<"pos_j.push  "<<j-1<<endl;
					cout<<"pos_i_forlable.push  "<<i<<endl;
					cout<<"pos_j_forlable.push  "<<j-1<<endl<<endl;
					#endif
				}
				if(j!=width-1&&(*(quantized->imageData+i*stepwidth+j+1))==color&&coh_Lables[i*width+j+1]==-2)
				{
					coh_Lables[i*width+j+1]=-1;
					pos_i.push(i);
					pos_j.push(j+1);
					pos_i_forlable.push(i);
					pos_j_forlable.push(j+1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i<<endl;
					cout<<"pos_j.push  "<<j+1<<endl;
					cout<<"pos_i_forlable.push  "<<i<<endl;
					cout<<"pos_j_forlable.push  "<<j+1<<endl<<endl;
					#endif
				}
				if(i!=height-1&&j!=0&&(*(quantized->imageData+(pi+1)*stepwidth+j-1))==color&&coh_Lables[(pi+1)*width+j-1]==-2)
				{
					coh_Lables[(i+1)*width+j-1]=-1;
					pos_i.push(i+1);
					pos_j.push(j-1);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j-1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i+1<<endl;
					cout<<"pos_j.push  "<<j-1<<endl;
					cout<<"pos_i_forlable.push  "<<i+1<<endl;
					cout<<"pos_j_forlable.push  "<<j-1<<endl<<endl;
					#endif
				}
				if(i!=height-1&&(*(quantized->imageData+(i+1)*stepwidth+j))==color&&coh_Lables[(i+1)*quantized->width+j]==-2)
				{
					coh_Lables[(i+1)*quantized->width+j]=-1;
					pos_i.push(i+1);
					pos_j.push(j);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i+1<<endl;
					cout<<"pos_j.push  "<<j<<endl;
					cout<<"pos_i_forlable.push  "<<i+1<<endl;
					cout<<"pos_j_forlable.push  "<<j<<endl<<endl;
					#endif
				}
				if(i!=height-1&&j!=width-1&&(*(quantized->imageData+(i+1)*stepwidth+j+1))==color&&coh_Lables[(i+1)*width+j+1]==-2)
				{
					coh_Lables[(i+1)*width+j+1]=-1;
					pos_i.push(i+1);
					pos_j.push(j+1);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j+1);
					#ifdef DEBUG_COHERENCE
					cout<<"pos_i.push  "<<i+1<<endl;
					cout<<"pos_j.push  "<<j+1<<endl;
					cout<<"pos_i_forlable.push  "<<i+1<<endl;
					cout<<"pos_j_forlable.push  "<<j+1<<endl<<endl;
					#endif
				}
			}

			//* if number >= threshold, set to 1 ; if else, set to 0 */
			int num=pos_i_forlable.size();
			//cout<<num<<"-"<<color<<"\t";
			while(!pos_i_forlable.empty())
			{
				int i=pos_i_forlable.front();
				int j=pos_j_forlable.front();
				pos_i_forlable.pop();
				pos_j_forlable.pop();

				#ifdef DEBUG_COHERENCE
				cout<<"pos_i_forlable.front  "<<i<<endl;
				cout<<"pos_j_forlable.front  "<<j<<endl;
				cout<<"pos_i_forlable.pop  "<<i<<endl;
				cout<<"pos_j_forlable.pop  "<<j<<endl<<endl;
				#endif

				coh_Lables[i*width+j]=(num>=threshold)?1:0;

				////* this code would cause a result with small bias, why? */
				//int colorBin=*(quantized->imageData+i*quantized->widthStep+j);
				//if(num>=threshold)
				//coherence[colorBin]++;
				//else
				//n_coherence[colorBin]++;
			}
		}
	}
}

////* DFS: this algorithm will cause stack overflow */
//void CCV::traverse_count(int pi,int pj)
//{
//	//if(coh_Lables[pi*quantized->width+pj]!=-2)
//		//return;
//	coh_Lables[pi*quantized->width+pj]=-1;
//	int color=*(quantized->imageData+pi*quantized->widthStep+pj);
//
//	if(pi!=0&&pj!=0&&(*(quantized->imageData+(pi-1)*quantized->widthStep+pj-1))==color)
//	{
//		if(coh_Lables[(pi-1)*quantized->width+pj-1]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi-1)*quantized->width+pj-1]=-1;
//			traverse_count(pi-1,pj-1);
//		}
//	}
//	if(pi!=0&&(*(quantized->imageData+(pi-1)*quantized->widthStep+pj))==color)
//	{
//		if(coh_Lables[(pi-1)*quantized->width+pj]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi-1)*quantized->width+pj]=-1;
//			traverse_count(pi-1,pj);
//		}
//	}
//	if(pi!=0&&pj!=quantized->width-1&&(*(quantized->imageData+(pi-1)*quantized->widthStep+pj+1))==color)
//	{
//		if(coh_Lables[(pi-1)*quantized->width+pj+1]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi-1)*quantized->width+pj+1]=-1;
//			traverse_count(pi-1,pj+1);
//		}
//	}
//	if(pj!=0&&(*(quantized->imageData+pi*quantized->widthStep+pj-1))==color)
//	{
//		if(coh_Lables[pi*quantized->width+pj-1]==-2)
//		{
//			counter++;
//			//coh_Lables[pi*quantized->width+pj-1]=-1;
//			traverse_count(pi,pj-1);
//		}
//	}
//	if(pj!=quantized->width-1&&(*(quantized->imageData+pi*quantized->widthStep+pj+1))==color)
//	{
//		if(coh_Lables[pi*quantized->width+pj+1]==-2)
//		{
//			counter++;
//			//coh_Lables[pi*quantized->width+pj+1]=-1;
//			traverse_count(pi,pj+1);
//		}
//	}
//	if(pi!=quantized->height-1&&pj!=0&&(*(quantized->imageData+(pi+1)*quantized->widthStep+pj-1))==color)
//	{
//		if(coh_Lables[(pi+1)*quantized->width+pj-1]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi+1)*quantized->width+pj-1]=-1;
//			traverse_count(pi+1,pj-1);
//		}
//	}
//	if(pi!=quantized->height-1&&(*(quantized->imageData+(pi+1)*quantized->widthStep+pj))==color)
//	{
//		if(coh_Lables[(pi+1)*quantized->width+pj]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi+1)*quantized->width+pj]=-1;
//			traverse_count(pi+1,pj);
//		}
//	}
//	if(pi!=quantized->height-1&&pj!=quantized->width-1&&(*(quantized->imageData+(pi+1)*quantized->widthStep+pj+1))==color)
//	{
//		if(coh_Lables[(pi+1)*quantized->width+pj+1]==-2)
//		{
//			counter++;
//			//coh_Lables[(pi+1)*quantized->width+pj+1]=-1;
//			traverse_count(pi+1,pj+1);
//		}
//	}
//}
//
//void CCV::lable(int lable)
//{
//	int width=h_plane->width; 
//	int height=h_plane->height;
//
//	for(int i=0;i<height;i++)
//	{
//		for(int j=0;j<width;j++)
//		{
//			if(coh_Lables[i*width+j]==-1)
//				coh_Lables[i*width+j]=lable;
//		}
//	}
//}
//
//void CCV::coherenceLable()
//{
//	int width=h_plane->width; 
//	int height=h_plane->height;
//
//	//int* counter=new int();
//	for(int i=0;i<height;i++)
//	{
//		for(int j=0;j<width;j++)
//		{
//			if(coh_Lables[i*width+j]==0||coh_Lables[i*width+j]==1)
//				continue;
//
//			counter=1;	
//
//			//coh_Lables[i*width+j]=-1;
//
//			traverse_count(i,j);
//			if(counter>=threshold)
//			{
//				lable(1);
//			}
//			else
//			{
//				lable(0);
//			}
//			//cout<<counter<<endl;
//		}
//	}
//}

void CCV::calcCCV()
{
	coherenceLable();

	for(int i=0;i<quantized->height;i++)
	{
		for(int j=0;j<quantized->width;j++)
		{
			//if(coh_Lables[i*quantized->width+j]!=0&&coh_Lables[i*quantized->width+j]!=1)
			//	cout<<coh_Lables[i*quantized->width+j]<<endl;
			int colorBin=*(quantized->imageData+i*quantized->widthStep+j);
			if(coh_Lables[i*quantized->width+j]==1)
				coherence[colorBin]++;
			else
				n_coherence[colorBin]++;
		}
	}

	//* There are several small numbers (smaller after smoothing the image) in coherence[], why? */
	for(int i=0;i<Num_ColorBin;i++)
	{
		if(coherence[i]<threshold)
		{
			n_coherence[i]+=coherence[i];
			coherence[i]=0;
		}
	}

	cout<<"\t coh \t n_coh"<<endl;
	for(int i=0;i<32;i++)
	{
		cout<<"bin "<<i<<":\t"<<coherence[i]<<"\t"<<n_coherence[i]<<endl;
	}
}