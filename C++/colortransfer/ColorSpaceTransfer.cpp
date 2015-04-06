/* This program implements the algorithm described in the paper 
 * "Color Transfer between Images". To run it, openCV need to be
 * installed and configured with VisualStudio. 
*/

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

bool colorTransfer(IplImage *img1,IplImage *img2,IplImage *&dst)
{
    double sum=0;
    double m1[3],m2[3],d1[3],d2[3];
    int i,x,y;
    IplImage *src1=cvCloneImage(img1),*src2=cvCloneImage(img2);
    int npixs1=src1->width*src1->height;
    int npixs2=src2->width*src2->height;
	//cvCvtColor(img1,src1,CV_RGB2Lab);
	//cvCvtColor(img2,src2,CV_RGB2Lab);
    //Calculate mean value and standard deviations of each channel
    for(i=0;i<3;i++)
    {
        m1[i]=0;
        for(x=0;x<src1->height;x++)
        {
            uchar *ptr=(uchar*)(src1->imageData+x*src1->widthStep);
            for(y=0;y<src1->width;y++)
            {
                m1[i]+=ptr[3*y+i];
            }
        }
        m1[i]/=npixs1;
    }
    for(i=0;i<3;i++)
    {
        d1[i]=0;
        for(x=0;x<src1->height;x++)
        {
            uchar *ptr=(uchar*)(src1->imageData+x*src1->widthStep);
            for(y=0;y<src1->width;y++)
            {
                d1[i]+=(ptr[3*y+i]-m1[i])*(ptr[3*y+i]-m1[i]);
            }
        }
        d1[i]/=npixs1;
        d1[i]=sqrt(d1[i]);
    }
    for(i=0;i<3;i++)
    {
        m2[i]=0;
        for(x=0;x<src2->height;x++)
        {
            uchar *ptr=(uchar*)(src2->imageData+x*src2->widthStep);
            for(y=0;y<src2->width;y++)
            {
                m2[i]+=ptr[3*y+i];
            }
        }
        m2[i]/=npixs2;
    }
    for(i=0;i<3;i++)
    {
        d2[i]=0;
        for(x=0;x<src2->height;x++)
        {
            uchar *ptr=(uchar*)(src2->imageData+x*src2->widthStep);
            for(y=0;y<src2->width;y++)
            {
                d2[i]+=(ptr[3*y+i]-m2[i])*(ptr[3*y+i]-m2[i]);
            }
        }
        d2[i]/=npixs2;
        d2[i]=sqrt(d2[i]);
    }
    
    double rate[3];
    for(i=0;i<3;i++)
    {
        rate[i]=d2[i]/d1[i];
        for(x=0;x<src1->height;x++)
        {
            uchar *ptr=(uchar*)(src1->imageData+x*src1->widthStep);
            for(y=0;y<src1->width;y++)
            {
                double tmp=ptr[3*y+i];
                int t=(int)((tmp-m1[i])*rate[i]+m2[i]);
                if(t<0)                //handle boundary pixels
                {
                    t=0;
                }
                if(t>255)
                {
                    t=255;
                }
                ptr[3*y+i]=t;
                
            }
        }
    }

    dst=cvCloneImage(src1);
	//cvCvtColor(src1,dst,CV_Lab2RGB);
    return true;
    
}

int _tmain(int argc, _TCHAR* argv[])
{
	//IplImage *src_img = cvLoadImage("006.jpg");
	//IplImage *target_img, *result_img;
	
	//CvSize img_sz = cvGetSize( src_img );
	//result_img = cvCreateImage(img_sz,src_img->depth,3);
	/* RGB to Lab color space */
	//cvCvtColor(src_img, result_img, CV_BGR2Lab);
	//cvCvtColor(src_img, result_img, CV_RGB2Lab);
	/*cvCvtColor(src_img, result_img,CV_BGR2Luv);
        cvNamedWindow("Image:",1);
		cvNamedWindow("Rst",1);
        cvShowImage("Image:",src_img);
		cvShowImage("Rst",result_img);

        cvWaitKey();
        cvDestroyWindow("Image:");
        cvReleaseImage(&src_img);
		cvDestroyWindow("Rst");
        cvReleaseImage(&result_img);*/
	cvNamedWindow("src");
    cvNamedWindow("tar");
    cvNamedWindow("lab");
	cvNamedWindow("XYZ");
	cvNamedWindow("YCrCb");
    IplImage * img1,*img2,*dst;

    //Load Image
    img1=cvLoadImage("src3.jpg");
    img2=cvLoadImage("targt3.jpg");
    cvShowImage("src",img1);
    cvShowImage("tar",img2);
    dst=cvCloneImage(img1);
	//RGB
    colorTransfer(img1,img2,dst);
	cvShowImage("lab",dst);
	//Lab
	/*cvCvtColor(img2,img2,CV_RGB2Lab);
	colorTransfer(img1,img2,dst);
	cvCvtColor(dst,dst,CV_Lab2RGB);*/
	//HSV
	/*cvCvtColor(img2,img2,CV_RGB2HSV);
	colorTransfer(img1,img2,dst);
	cvCvtColor(dst,dst,CV_HSV2RGB);*/
	//HLS
	/*cvCvtColor(img2,img2,CV_RGB2HLS);
	colorTransfer(img1,img2,dst);
	cvCvtColor(dst,dst,CV_HLS2RGB);*/
	//XYZ-ok
	cvCvtColor(img2,img2,CV_RGB2XYZ);
	colorTransfer(img1,img2,dst);
	cvCvtColor(dst,dst,CV_XYZ2RGB);
	cvShowImage("XYZ",dst);
	//YCrCb
	cvCvtColor(img2,img2,CV_RGB2YCrCb);
	colorTransfer(img1,img2,dst);
	cvCvtColor(dst,dst,CV_YCrCb2RGB);
    cvShowImage("YCrCb",dst);
   
	cvWaitKey(60000);
    cvDestroyAllWindows();


	return 0;
}

