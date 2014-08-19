#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"
#include "rs232.hpp"


/** Constantes **/
#define ANCHO 640
#define ALTO 480
#define FPS 30

/** Prototipos */
uint8_t detectAndDisplay( cv::Mat frame );
int RS232_OpenComport(int, int);
int RS232_SendByte(int, unsigned char);
void RS232_CloseComport(int);
int promedio (cv::Mat);
void variarLuz(int);



cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;
std::string face_window_name = "Realidad";
cv::RNG rng(12345);
cv::Mat debugImage;
cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
cv::Mat frame;
std::vector<cv::Rect> faces;
int contHist=0;
cv::VideoCapture eyes_capture;


uint8_t
DetectDormInit( int argc, char *argv[] )
{
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n"); return -1; };
	if(debugL1)
	{
		cv::namedWindow(face_window_name,CV_WINDOW_NORMAL);
		cv::moveWindow(face_window_name, 700, 0);
	}
	eyes_capture.set(CV_CAP_PROP_FRAME_WIDTH, ANCHO);
	eyes_capture.set(CV_CAP_PROP_FRAME_HEIGHT, ALTO);
	eyes_capture.set(CV_CAP_PROP_FPS, FPS);
	eyes_capture = cv::VideoCapture(atoi(argv[1]));


	if( !eyes_capture.isOpened() )
	{
		std::cout <<"Camara interior apagada"<<std::endl;
		return 1;
	}



	return 0;

}


void
DetectDormHandler( cv::Mat& MatEyes )
{
	eyes_capture >> frame;
	if( !frame.empty() )
	{
		cv::flip(frame, frame, 1);
		frame.copyTo(debugImage);
		detectAndDisplay( frame );
		MatEyes = debugImage;
	}
	else
	{
		printf(" --(!) No captured frame -- Break!\n");
	}
}


void
DetectDormEnd()
{
	if(rs232) variarLuz(0);
//	releaseCornerKernels();
	eyes_capture.release();
	//cvReleaseCapture( &eyes_capture );
}







uint8_t findEyes(cv::Mat frame_gray, cv::Rect face)
{
	uint8_t dev = 255;
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;
	;

	if (kSmoothFaceImage)
	{
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur( faceROI, faceROI, cv::Size( 0, 0 ), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth/100.0);
	int eye_region_height = face.width * (kEyePercentHeight/100.0);
	int eye_region_top = face.height * (kEyePercentTop/100.0);
	cv::Rect leftEyeRegion(face.width*(kEyePercentSide/100.0),eye_region_top,eye_region_width,eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide/100.0), eye_region_top,eye_region_width,eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI,leftEyeRegion,"Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI,rightEyeRegion,"Right Eye");

	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	cv::Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	rectangle(debugFace,leftRightCornerRegion,200);
	rectangle(debugFace,leftLeftCornerRegion,200);
	rectangle(debugFace,rightLeftCornerRegion,200);
	rectangle(debugFace,rightRightCornerRegion,200);
	//std::cout<<rightPupil.y<<"	"<<faces[0].y<<std::endl;
	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x+faces[0].x;
	rightPupil.y += rightEyeRegion.y+faces[0].y;
	leftPupil.x += leftEyeRegion.x+faces[0].x;
	leftPupil.y += leftEyeRegion.y+faces[0].y;
	// draw eye centers
	//circle(debugFace, rightPupil, 3, 1234);


	if(rightPupil.y>pupilDetUmb+rightEyeRegion.y+faces[0].y)
	{
		if(abs(rightPupil.y-leftPupil.y)<=alturaOjos)
		{
			cv::putText(debugImage,"Ojos abiertos",cvPoint(30,50),1, 0.8, cvScalar(0,255,0), 1, CV_AA);
			circle(debugImage, rightPupil, 3, cv::Scalar(0,0,255));
			contHist=0;
		}
		dev=1;

	}
	else
	{
		if(contHist>=persistCuadros)
		{
			cv::putText(debugImage,"Ojos cerrados",cvPoint(30,70),1, 0.8, cvScalar(0,0,255), 1, CV_AA);
			dev=2;
//			contHist=0;
		}
		else
		{
			cv::putText(debugImage,"Ojos abiertos",cvPoint(30,50),1, 0.8, cvScalar(0,255,0), 1, CV_AA);
			contHist ++;
		}
	}
	if(leftPupil.y>pupilDetUmb+leftEyeRegion.y+faces[0].y)
	{

		if(abs(rightPupil.y-leftPupil.y)<=alturaOjos)
		{
			cv::putText(debugImage,"Ojos abiertos",cvPoint(30,50),1, 0.8, cvScalar(0,255,0), 1, CV_AA);
			//circle(debugFace, leftPupil, 3, 1234);
			circle(debugImage, leftPupil, 3, cv::Scalar(0,0,255));
			contHist=0;
		}
		dev=1;

	}
	else
	{
		if(contHist>=persistCuadros)
		{
			cv::putText(debugImage,"Ojos cerrados",cvPoint(30,70),1, 0.8, cvScalar(0,0,255), 1, CV_AA);
			dev=2;
//			contHist=0;
		}
		else
		{
			cv::putText(debugImage,"Ojos abiertos",cvPoint(30,50),1, 0.8, cvScalar(0,255,0), 1, CV_AA);
			contHist ++;
		}
	}
	//-- Find Eye Corners
	if (kEnableEyeCorner)
	{
		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;
		cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		rightLeftCorner.x += rightLeftCornerRegion.x;
		rightLeftCorner.y += rightLeftCornerRegion.y;
		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;
		circle(faceROI, leftRightCorner, 3, 200);
		circle(faceROI, leftLeftCorner, 3, 200);
		circle(faceROI, rightLeftCorner, 3, 200);
		circle(faceROI, rightRightCorner, 3, 200);
	}
//	if(debugL1)
//		{
			circle(faceROI, rightPupil, 3, 255);
//		}


	if(debugL1)
	{
		imshow(face_window_name, faceROI);
	}
	//  cv::Rect roi( cv::Point( 0, 0 ), faceROI.size());
	//  cv::Mat destinationROI = debugImage( roi );
	//  faceROI.copyTo( destinationROI );
	return dev;
}









cv::Mat findSkin (cv::Mat &frame)
{
	cv::Mat input;
	cv::Mat output = cv::Mat(frame.rows,frame.cols, CV_8U);
	cvtColor(frame, input, CV_BGR2YCrCb);
	for (int y = 0; y < input.rows; ++y)
	{
		const cv::Vec3b *Mr = input.ptr<cv::Vec3b>(y);
		//    uchar *Or = output.ptr<uchar>(y);
		cv::Vec3b *Or = frame.ptr<cv::Vec3b>(y);
		for (int x = 0; x < input.cols; ++x)
		{
			cv::Vec3b ycrcb = Mr[x];
			//      Or[x] = (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) ? 255 : 0;
			if(skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0)
			{
				Or[x] = cv::Vec3b(0,0,0);
			}
		}
	}
	return output;
}





/**
 * @function detectAndDisplay
 */
uint8_t detectAndDisplay( cv::Mat frame )
{
	uint8_t dev= -1;

	//cv::Mat frame_gray;

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(frame, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];

	//cvtColor( frame, frame_gray, CV_BGR2GRAY );
	//equalizeHist( frame_gray, frame_gray );
	//cv::pow(frame_gray, CV_64F, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150) );
	//  findSkin(debugImage);
	if(rs232) variarLuz(intensidad);
	for( unsigned i = 0; i < faces.size(); i++ )
	{
		rectangle(debugImage, faces[i], 1234);
	}

	//-- Show what you got
	if (faces.size() > 0)
	{
		//		  rectangle(debugImage, faces[i], 1234);
		dev = findEyes(frame_gray, faces[0]);
	}
	if (faces.size()==0)
	{
		cv::putText(debugImage, "No se detecto la cara", cvPoint(30,30), 1, 0.8, cvScalar(200,200,250), 1, 0);
		dev=0;
	}
	return dev;

}

void variarLuz(int intensidad)
{
	union byteint
		{
		    uint8_t b[sizeof (int)];
		    int i;
		};
		byteint bi;
		bi.i=20;
	std::cout<<"Puerto serie abierto: "<< RS232_OpenComport(3,38400)<<std::endl;
		bi.i=170;
		RS232_SendByte(3, bi.b[0]);
		bi.i=5;
		RS232_SendByte(3, bi.b[0]);
		bi.i=1;
		RS232_SendByte(3, bi.b[0]);
		bi.i=intensidad;
		RS232_SendByte(3, bi.b[0]);
		bi.i=238;
		RS232_SendByte(3, bi.b[0]);
		RS232_CloseComport(3);

}


