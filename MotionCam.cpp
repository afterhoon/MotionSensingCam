#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
using namespace cv;
using namespace std;
char* source_window = "Camera image";

typedef struct point {
	int start;
	int end;
} Point1;

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

int main(int argc, char** argv) {
	Point1 arr[10];
	int count = 0;
	int start = 0, end = 0;
	Mat src;
	IplImage *frame;
	VideoCapture InputVideo(0);
	if (!InputVideo.isOpened()) {
		cout << "Could not open reference" << 0 << endl;
		return -1;
	}
	else
		cout << "Connect camera device index" << 0 << endl;

	InputVideo.read(src);
	CvSize size = cvSize(src.cols, src.rows);
	IplImage *prevImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *currImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *diffImage = cvCreateImage(size, IPL_DEPTH_8U, 1);

	int t = 0;    // frame count
	int nThreshold = 50;

	while (1) {
		InputVideo.read(src);
		frame = cvCreateImage(cvSize(src.cols, src.rows), 8, 3);
		IplImage ipltemp = src;
		cvCopy(&ipltemp, frame);
		
		if (!frame)    break;
		t++;
		//std::cout << (isCatch(diffImage) ? "MOVE!! " : "XXXXXX ") << t << "       " << isCatch(diffImage) << std::endl;

		if (isCatch(diffImage) > 0 && start == 0) {
			arr[count].start = t;
			start = t;
		}
		if (isCatch(diffImage) == 0 && start != 0) {
			arr[count].end = t;
			count++;
			end = t;
			start = 0;
			end = 0;
			printf("START: %5d   END: %5d\n", arr[count - 1].start, arr[count - 1].end);
		}

//		if (start > 0) printf("■");
//		else printf("□");


		// cvCvtColor 함수를 사용하여 cvQueryFrame 함수로 획득한 frame을 그레이 스케일 영상으로 변환하여 grayImage에 저장한다.
		cvCvtColor(frame, currImage, CV_BGR2GRAY);

		// cvAbsDiff 함수로 현재의 입력 비디오 프레임과 그레이 스케일 영상인 grayImage와 배경 영상인 bkgImage와의 차이의 절대값을 계산하여 diffImage에 저장한다.
		cvAbsDiff(currImage, prevImage, diffImage);

		// diffImage에서 0인 화소는 변화가 없는 화소이며, 값이 크면 클수록 배경 영상과의 차이가 크게 일어난 화소이다.
		// cvThreshold 함수를 사용하여 cvThreshold=50 이상인 화소만을 255로 저장하고, 임계값 이하인 값은 0으로 저장한다.
		// 임계값은 실험 또는 자동으로 적절히 결정해야 한다.
		cvThreshold(diffImage, diffImage, nThreshold, 255, CV_THRESH_BINARY);

		cvShowImage("prevImage", prevImage);
		cvShowImage("currImage", currImage);
		cvShowImage("diffImage", diffImage);

		char chKey = cvWaitKey(10);
		if (chKey == 27) {    // Esc
			break;
		}

		cvCvtColor(frame, prevImage, CV_BGR2GRAY);
	}
	cvReleaseImage(&frame);
	return (0);
}
