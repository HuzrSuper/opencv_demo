
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/core/core.hpp" 
#include <opencv2/opencv.hpp> 
#include <iostream>
using namespace cv;
using namespace std;
Mat img, inpaintMask;
Point prevPt(-1, -1);
// 鼠标点击绘制噪声
static void onMouse(int event, int x, int y, int flags, void*)
{
	// 根据鼠标响应确定事件
	if (event == CV_EVENT_LBUTTONUP || !(flags &
		CV_EVENT_FLAG_LBUTTON))
		prevPt = Point(-1, -1);
	// 左键按下
	else if (event == CV_EVENT_LBUTTONDOWN)
		prevPt = Point(x, y);
	// 鼠标移动和左键事件
	else if (event == CV_EVENT_MOUSEMOVE &&
		(flags & CV_EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (prevPt.x < 0)
			prevPt = pt;
		// 绘制鼠标移动轨迹
		line(inpaintMask, prevPt, pt, Scalar::all(255), 5, 8, 0);
		line(img, prevPt, pt, Scalar::all(255), 5, 8, 0);
		prevPt = pt;
		imshow("image", img);
	}
}
int main()
{
	cv::Mat src = imread("flower3.jpg");
	if (src.empty())
	{
		return 0;
	}
	img = src.clone();
	inpaintMask = Mat::zeros(img.size(), CV_8U);
	cv::imshow("srcImage", img);
	// 鼠标事件回调函数
	setMouseCallback("srcImage", onMouse, 0);
	// 完成图像污点修复
	for (;;)
	{
		char c = (char)waitKey();
		if (c == 27)
			break;
		if (c == 'r')
		{
			inpaintMask = Scalar::all(0);
			src.copyTo(img);
			cv::imshow("srcImage", img);
		}
		if (c == 'i' || c == ' ')
		{
			Mat resMat;
			cv::inpaint(img, inpaintMask, resMat,
				3, CV_INPAINT_TELEA);
			cv::imshow("resMat", resMat);
		}
	}
	return 0;
}
