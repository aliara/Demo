#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../LineDetect/Header.h"

#define uint8_t unsigned char


uint8_t DetectDormInit( int argc, char *argv[] );
void DetectDormHandler(cv::Mat&);
void DetectDormEnd(void);

uint8_t LineDetectInit( int argc, char *argv[] );
void LineDetectHandler(IplImage** draw);
void LineDetectEnd(void);




int main( int argc, char** argv )
{
	std::string main_window_name = "Demo Output Aliara";
	IplImage * pImgLines;
	cv::Mat MatLines;
	cv::Mat MatEyes;
	cv::Mat BigMat = cv::Mat::zeros(480 +  CROP_HEIGHT, 640, CV_8UC3);
	int ch=0;
	uint8_t status;

	cv::namedWindow(main_window_name,CV_WINDOW_AUTOSIZE);
	cv::moveWindow(main_window_name, 0, 0);

	cv::namedWindow("video",CV_WINDOW_AUTOSIZE);

	status = DetectDormInit(argc, argv);

	if(status != 0)
	{
		return 0;
	}

	status = LineDetectInit( argc, argv );

	if(status != 0)
	{
		return 0;
	}

	while(ch != 'x')
	{
		DetectDormHandler(MatEyes);
		LineDetectHandler(&pImgLines);
		cv::Mat MatLines = cv::Mat(pImgLines, false);
		imshow(main_window_name, MatEyes);
		imshow("video", MatLines);
		ch=cv::waitKey(10);
	}

	DetectDormEnd();
	LineDetectEnd();
}


