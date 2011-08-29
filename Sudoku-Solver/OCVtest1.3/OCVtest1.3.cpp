	#include <stdafx.h>
	#include <stdio.h>
	#include <stack>

    #include "cv.h"
    #include "highgui.h"

	#include <iostream>
	#include <assert.h>

	using namespace std;
	using namespace cv;
     
	void DetectBiggest(CvContourScanner, CvSeq*, int);

    int _tmain(int argc, _TCHAR* argv[])
{
	cvNamedWindow( "Contours 2", 1 );
	IplImage* img_8uc1 = cvLoadImage( "sudoku.jpg", 0 );
	IplImage* img_edge = cvCreateImage( cvGetSize(img_8uc1), 8, 1 );
	IplImage* img_8uc3 = cvCreateImage( cvGetSize(img_8uc1), 8, 3 );

	try{
	cvThreshold( img_8uc1, img_edge, 128, 255, CV_THRESH_BINARY );
	}

	catch( cv::Exception& e )
	{
		const char* err_msg = e.what();
		cout << "Exception raised: " << err_msg << '\n';
	}

	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;

	/*int Nc = cvFindContours(
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_LIST );

	*/
	CvContourScanner scanner = cvStartFindContours(
	img_edge,
	storage,
	sizeof(CvContour),
	CV_RETR_EXTERNAL,
	CV_CHAIN_APPROX_SIMPLE
	);

	int n=0;
	//printf( "Total Contours Detected: %d\n", Nc );
	CvScalar red = CV_RGB(250,0,0);
	CvScalar blue = CV_RGB(0,0,250);

	CvSeq* biggest=NULL;
	DetectBiggest(scanner, biggest,(img_edge->height + img_edge->width));

	CvSeq* c=biggest;
	for( int i=0; i<c->total; ++i ){
		CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, c, i );
		printf(" (%d,%d)\n", p->x, p->y );
	}
	/*
	for( CvSeq* c=first_contour; c!=NULL; c=c->h_next ){*/
		cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
		cvDrawContours(
			img_8uc3,
			c,
			red,		// Red
			blue,		// Blue
			1,			// Vary max_level and compare results
			2,
			8 );
		/*printf( "Contour #%dn", n );
		cvShowImage( "Contours 2", img_8uc3 );
		printf( " %d elements:\n", c->total );
		for( int i=0; i<c->total; ++i ){
			CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, c, i );
			printf(" (%d,%d)\n", p->x, p->y );
		}
		cvWaitKey();
		n++;
	}
	*/

	printf( "Finished all contours.\n");
	//cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
	cvShowImage( "Contours 2", img_8uc3 );
	cvWaitKey();	

	cvDestroyWindow( "Contours 2" );

	cvReleaseImage( &img_8uc1 );
	cvReleaseImage( &img_8uc3 );
	cvReleaseImage( &img_edge );

	return 0;
}

void DetectBiggest(CvContourScanner inp, CvSeq* biggest, int heightwidth)
{
	CvSeq* c;
	CvSeq* cnew;
	CvMemStorage* storage = cvCreateMemStorage();

	while( (c = cvFindNextContour( inp )) != NULL ){
		try {

			double len = cvContourPerimeter( c );
			double q = heightwidth/4;
			cout<<len<<"<"<<q<<"\n";
			if( len < 1 ) {
			cvSubstituteContour( inp, NULL );
			} else {

			cnew = cvApproxPoly(
			c,
			sizeof(CvContour),
			storage,
			CV_POLY_APPROX_DP,
			4,
			0
			);

			if(fabs(cvContourArea(cnew,CV_WHOLE_SEQ)) >= fabs(cvContourArea(biggest,CV_WHOLE_SEQ))||biggest==NULL){
				biggest=cnew;

			cout<<"biggest:"<<cvContourArea(biggest)<<"\n";
			}
			}
		}

		catch( cv::Exception& e )
	{
		const char* err_msg = e.what();
		cout << "Exception raised: " << err_msg << '\n';
	}
	}

	


}