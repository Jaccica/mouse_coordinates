#include <cv.h>  
#include <highgui.h>  
#include <stdio.h>  
#include <iostream>
#include <vector>  
#include <fstream>  
using namespace std;
using namespace cv;
#define max(a,b)            (((a) > (b)) ? (a) : (b))  
#define min(a,b)            (((a) < (b)) ? (a) : (b))  

//注意参数是有符号短整型，该函数的作用是使i限定为[a,b]区间内  
int bound(short i, short a, short b)
{
	return min(max(i, min(a, b)), max(a, b));
}

CvScalar getInverseColor(CvScalar c)
{
	CvScalar s;
	for (int i = 0; i <= 2; ++i)
	{
		s.val[i] = 255 - c.val[i];
	}
	return s;
}

IplImage* src = 0;
IplImage* dst = 0;
int n = 0;
vector<CvPoint> points;

void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	CvPoint pt;
	CvPoint tmp_pt = { -1,-1 };
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);
	char temp[16];
	CvSize text_size;
	int baseline;

	CvScalar clrPoint = cvScalar(255, 0, 0, 0);
	CvScalar clrText = cvScalar(255, 255, 255, 0);

	if (event == CV_EVENT_MOUSEMOVE)
	{
		cvCopy(dst, src);

		x = bound(x, 0, src->width - 1);
		y = bound(y, 0, src->height - 1);
		pt = cvPoint(x, y);
		cvCircle(src, pt, 2, clrPoint, CV_FILLED, CV_AA, 0);

		sprintf_s(temp, "%d (%d,%d)", n + 1, x, y);
		cvGetTextSize(temp, &font, &text_size, &baseline);
		tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
		tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
		cvPutText(src, temp, tmp_pt, &font, clrText);

		cvShowImage("src", src);
	}
	else if (event == CV_EVENT_LBUTTONDOWN)
	{
		pt = cvPoint(x, y);
		points.push_back(pt); n++;
		cvCircle(src, pt, 2, clrPoint, CV_FILLED, CV_AA, 0);

		sprintf_s(temp, "%d (%d,%d)", n, x, y);
		cvGetTextSize(temp, &font, &text_size, &baseline);
		tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
		tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
		cvPutText(src, temp, tmp_pt, &font, clrText);

		cvCopy(src, dst);
		cvShowImage("src", src);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)
	{
		if (!points.empty())
		{
			cvCopy(dst, src);

			pt = points.back();
			points.pop_back();
			cvCircle(src, pt, 2, getInverseColor(clrPoint), CV_FILLED, CV_AA, 0);

			sprintf_s(temp, "%d (%d,%d)", n, pt.x, pt.y); --n;
			cvGetTextSize(temp, &font, &text_size, &baseline);
			tmp_pt.x = bound(pt.x, 0, src->width - text_size.width);
			tmp_pt.y = bound(pt.y, text_size.height + baseline, src->height - 1 - baseline);
			cvPutText(src, temp, tmp_pt, &font, getInverseColor(clrText));

			cvCopy(src, dst);
			cvShowImage("src", src);
		}
	}
}

int main()
{
	src = cvLoadImage("E:\\cell_layered_picture\\20190429\\1\\cqh0.jpg", 1);
	dst = cvCloneImage(src);

	cvNamedWindow("src", 1);
	cvSetMouseCallback("src", on_mouse, 0);

	cvShowImage("src", src);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	ofstream file("sample.txt");
	if (!file)
	{
		cout << "open file error!";
		return 1;
	}
	vector<CvPoint>::iterator it = points.begin();
	for (; it != points.end(); ++it)
	{
		file << it->x << ',' << it->y << endl;
	}
	file << endl;
	file.close();

	return 0;
}