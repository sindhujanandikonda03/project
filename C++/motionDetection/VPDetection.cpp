// VPDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

const int MAX_COUNT = 500;

int _tmain(int argc, _TCHAR* argv[])
{	
    CvCapture *input;
	CvVideoWriter *output;

	IplImage* gray, *prev_gray;
	IplImage* frame, *temp;
	IplImage *pyramid, *prev_pyramid;
	IplImage * eig_image, *tmp_image;
	CvSize fsize;
	int fps;

	input = cvCaptureFromFile("test1.mov");
    if (input == NULL)
	{
		/* Either the video didn't exist OR it uses a codec OpenCV doesn't support. */
		fprintf(stderr, "Error: Can't open video.\n");
		return -1;
	}
	
	frame = cvQueryFrame( input );
	// get the frames per second
	fps = cvGetCaptureProperty(input, CV_CAP_PROP_FPS);

	output = cvCreateVideoWriter("out.avi", CV_FOURCC('M', 'P', '4', '2'), fps, cvGetSize(frame), 1);

	if(!output) 
	{
		cvReleaseCapture(&input);
		fprintf(stderr, "Error: Can't output.\n");
		return -1;
	}
	
	fsize =  cvGetSize(frame);
	gray = cvCreateImage(fsize,  8, 1);
	prev_gray = cvCreateImage(fsize,  8, 1);

	pyramid = cvCreateImage( fsize, IPL_DEPTH_8U, 1 );
    prev_pyramid = cvCreateImage( fsize, IPL_DEPTH_8U, 1 );
	
	cvNamedWindow("Optical Flow", 1);
    //cvShowImage("Optical Flow",frame);
	///////////////////////////////////////////////////////////////////////////////////////////
	int corner_count = MAX_COUNT;
	// feature points
	CvPoint2D32f* featuresA = new CvPoint2D32f[ MAX_COUNT ];
	CvPoint2D32f* featuresB = new CvPoint2D32f[ MAX_COUNT ];
	CvPoint2D32f* swap_points =0;
	bool firstframe = true;

	int win_size = 20;
	char* status = 0;
	int i, k;
	
	status = (char*)cvAlloc(MAX_COUNT);
	
	while(true) {
		frame = cvQueryFrame(input);

		if(!frame) {
			fprintf(stderr, "Error: Null Frame.\n");
			break;
		}

		cvCvtColor(frame, gray, CV_BGR2GRAY);
		if(firstframe) 
		{
			eig_image = cvCreateImage( cvGetSize(gray), IPL_DEPTH_32F, 1 );
	        tmp_image = cvCreateImage( cvGetSize(gray), IPL_DEPTH_32F, 1 );
			cvGoodFeaturesToTrack(gray,eig_image, tmp_image,featuresA, &corner_count, 0.01, 10, 0, 3, 0, 0.04);
			cvFindCornerSubPix(gray, featuresA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
			cvReleaseImage( &eig_image );
            cvReleaseImage( &tmp_image );
			firstframe = false;
 		}
        
		if(!prev_gray)    cvCopyImage(gray, prev_gray);

		cvCalcOpticalFlowPyrLK(prev_gray, gray, prev_pyramid, pyramid, featuresA, featuresB, corner_count, cvSize(win_size, win_size), 5, status, 0, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), 0);
		
		CV_SWAP( prev_gray, gray, temp);
		CV_SWAP( prev_pyramid, pyramid, temp);
		CV_SWAP( featuresA, featuresB, swap_points );		
		
		for(i = k = 0; i < MAX_COUNT; i++) 
		{
			if(!status[i])   continue;
			if((abs(featuresA[i].x-featuresB[i].x)+(abs(featuresA[i].y-featuresB[i].y))<2))   continue;
			
			cvCircle( frame, cvPointFrom32f(featuresB[i]), 3, CV_RGB(0,255,0), -1, 8,0);

		}
		
		//cvReleaseImage(&prev_pyramid); 
        //cvReleaseImage(&pyramid); 
		//free(featuresA); 
        //free(featuresB); 
        //free(status);
                
        cvShowImage("Optical Flow",frame);
	    cvWriteFrame(output, frame);

	    int c = cvWaitKey(10);
        if( (char)c == 27 )      break; 
     }

	cvReleaseVideoWriter(&output);
	cvReleaseCapture(&input);

	return 0;

}

