#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
using namespace cv;
using namespace std;
char* source_window = "Camera image";
int main(int argc, char** argv) {
	Mat src;
	IplImage *srcImage;
	VideoCapture InputVideo(0);
	if (!InputVideo.isOpened()) {
		cout << "Could not open reference" << 0 << endl;
		return -1;
	}
	else
		cout << "Connect camera device index" << 0 << endl;
	while (1) {
		InputVideo.read(src);
		srcImage = cvCreateImage(cvSize(src.cols, src.rows), 8, 3);
		IplImage ipltemp = src;
		cvCopy(&ipltemp, srcImage);
		namedWindow(source_window, CV_WINDOW_AUTOSIZE);
		imshow(source_window, src);
		namedWindow("IplImage", CV_WINDOW_AUTOSIZE);
		cvShowImage("IplImage", srcImage);
		waitKey(50);
	}
	cvReleaseImage(&srcImage);
	return (0);
}
