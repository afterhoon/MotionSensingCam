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

	return moved > 1000 ? moved : 0;
}

int main()
{
	int cnt = 0;
	int nThreshold = 50;
	int start = 0, end = 0;

	CvCapture *capture = cvCaptureFromFile("test2.mp4");
	if (!capture) {
		printf("ERROR!!!! Can't find video file\n");
		return 0;
	}

	int width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	CvSize size = cvSize(width, height);


	IplImage *prevImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *currImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *diffImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *frame = NULL;

	system("pause");
	while (1) {
		frame = cvQueryFrame(capture);
		if (!frame)    break;
		cnt++;

		if (isCatch(diffImage) > 0 && start == 0) start = cnt;
		if (isCatch(diffImage) == 0 && start != 0) {
			end = cnt;
			start = 0;
			end = 0;
		}

		//printf("%d       %d\n", t, isCatch(diffImage));
		if (start > 0) printf("■");
		else printf("□");

		cvCvtColor(frame, currImage, CV_BGR2GRAY);
		cvThreshold(currImage, currImage, nThreshold, 255, CV_THRESH_BINARY);

		cvSub(currImage, prevImage, diffImage, 0);

		cvShowImage("prevImage", prevImage);
		cvShowImage("currImage", currImage);
		cvShowImage("diffImage", diffImage);

		char chKey = cvWaitKey(10);
		if (chKey == 27) {    // Esc
			break;
		}

		if (cnt == 300) cvWaitKey(0);

		cvCvtColor(frame, prevImage, CV_BGR2GRAY);
		cvThreshold(prevImage, prevImage, nThreshold, 255, CV_THRESH_BINARY);
	}

	cvDestroyAllWindows();
	cvReleaseImage(&currImage);
	cvReleaseImage(&diffImage);
	cvReleaseCapture(&capture);
	return 0;
}

