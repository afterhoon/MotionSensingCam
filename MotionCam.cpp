/*
2014_05_09
배경 차영상에 의한 이동 물체 검출
*/

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int isCatch(IplImage *image) {
	int height, width;
	uchar *data;
	int i, j;
	int moved = 0;

	height = image->height;
	width = image->width;
	data = (uchar *)image->imageData;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			if (data[i*width + j] == 255) moved++;

	return moved;
}

int main()
{
	int start = 0, end = 0;

	// 움직임을 검출할 영상을 포인터 capture에 저장한다.
	CvCapture *capture = cvCaptureFromFile("test2.mp4");
	if (!capture) {
		std::cout << "The video file was not found" << std::endl;
		return 0;
	}

	// 영상의 사이즈를 구한다.
	int width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	CvSize size = cvSize(width, height);


	// cvCreateImage 함수를 사용하여 이전이미지 prevImage와 현재이미지 currImage 그리고 차영상을 위한 diffImage를 생성한다.
	IplImage *prevImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *currImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *diffImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *frame = NULL;

	int t = 0;    // frame count
	int nThreshold = 50;	// Threshold 값

	while (1) {
		// capture로부터 프레임을 획득하여 포인터 frame에 저장한다.
		frame = cvQueryFrame(capture);
		if (!frame)    break;
		t++;
		//std::cout << t << "       " << isCatch(diffImage) << std::endl;

		if (isCatch(diffImage) > 0 && start == 0) start = t;
		if (isCatch(diffImage) == 0 && start != 0) {
			end = t;
			start = 0;
			end = 0;
		}

		// 영상에서 움직임이 있는지 없는지를 도스창에 출력
		if (start > 0) std::cout << "■";
		else std::cout << "□";


		// cvCvtColor 함수를 사용하여 cvQueryFrame 함수로 획득한 frame을 그레이 스케일 영상으로 변환하여 currImage에 저장한다.
		cvCvtColor(frame, currImage, CV_BGR2GRAY);

		cvThreshold(currImage, currImage, nThreshold, 255, CV_THRESH_BINARY);

		// cvAbsDiff 함수로 현재의 입력 비디오 프레임과 그레이 스케일 영상인 grayImage와 배경 영상인 bkgImage와의 차이의 절대값을 계산하여 diffImage에 저장한다.
		//cvAbsDiff(currImage, prevImage, diffImage);
		cvSub(currImage, prevImage, diffImage, 0);


		// diffImage에서 0인 화소는 변화가 없는 화소이며, 값이 크면 클수록 이전 영상과의 차이가 크게 일어난 화소이다.
		// cvThreshold 함수를 사용하여 cvThreshold=50 이상인 화소만을 255로 저장하고, 임계값 이하인 값은 0으로 저장한다.
		// 임계값은 실험 또는 자동으로 적절히 결정해야 한다.
		//cvThreshold(diffImage, diffImage, nThreshold, 255, CV_THRESH_BINARY);

		cvShowImage("prevImage", prevImage);
		cvShowImage("currImage", currImage);
		cvShowImage("diffImage", diffImage);

		char chKey = cvWaitKey(10);
		if (chKey == 27) {    // Esc
			break;
		}

		cvCvtColor(frame, prevImage, CV_BGR2GRAY);
		cvThreshold(prevImage, prevImage, nThreshold, 255, CV_THRESH_BINARY);
	}

	cvDestroyAllWindows();
	cvReleaseImage(&currImage);
	cvReleaseImage(&diffImage);
	cvReleaseCapture(&capture);
	return 0;
}
