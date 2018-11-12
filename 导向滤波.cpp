
#include <iostream>  
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/imgproc/imgproc.hpp"      
using namespace std;
using namespace cv;
// �����˲��� 
cv::Mat guidedfilter(Mat &srcImage, Mat &srcClone, int r, double eps)
{
	// ת��Դͼ����Ϣ
	srcImage.convertTo(srcImage, CV_64FC1);
	srcClone.convertTo(srcClone, CV_64FC1);
	int nRows = srcImage.rows;
	int nCols = srcImage.cols;
	cv::Mat boxResult;
	// ����һ: �����ֵ
	cv::boxFilter(cv::Mat::ones(nRows, nCols, srcImage.type()),
		boxResult, CV_64FC1, cv::Size(r, r));
	// ���ɵ����ֵmean_I    
	cv::Mat mean_I;
	cv::boxFilter(srcImage, mean_I, CV_64FC1, cv::Size(r, r));
	// ����ԭʼ��ֵmean_p   
	cv::Mat mean_p;
	cv::boxFilter(srcClone, mean_p, CV_64FC1, cv::Size(r, r));
	// ���ɻ���ؾ�ֵmean_Ip 
	cv::Mat mean_Ip;
	cv::boxFilter(srcImage.mul(srcClone), mean_Ip,
		CV_64FC1, cv::Size(r, r));
	cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p);
	// ��������ؾ�ֵmean_II 
	cv::Mat mean_II;
	cv::boxFilter(srcImage.mul(srcImage), mean_II, CV_64FC1, cv::Size(r, r));
	// ��������������ϵ��   
	cv::Mat var_I = mean_II - mean_I.mul(mean_I);
	cv::Mat var_Ip = mean_Ip - mean_I.mul(mean_p);
	// ���������������ϵ��a��b 
	cv::Mat a = cov_Ip / (var_I + eps);
	cv::Mat b = mean_p - a.mul(mean_I);
	// �����ģ�����ϵ��a��b��ֵ    
	cv::Mat mean_a;
	cv::boxFilter(a, mean_a, CV_64FC1, cv::Size(r, r));
	mean_a = mean_a / boxResult;
	cv::Mat mean_b;
	cv::boxFilter(b, mean_b, CV_64FC1, cv::Size(r, r));
	mean_b = mean_b / boxResult;
	//�����壺����������� 
	cv::Mat resultMat = mean_a.mul(srcImage) + mean_b;
	return resultMat;
}
int main()
{
	cv::Mat srcImage = cv::imread("..\\images\\flower3.jpg");
	if (srcImage.empty())
		return-1;
	cv::Mat srcGray(srcImage.size(), CV_8UC1);
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	// ͨ������ 
	vector<Mat> vSrcImage, vResultImage;
	split(srcImage, vSrcImage);
	Mat resultMat;
	for (int i = 0; i < 3; i++)
	{
		// ��ͨ��ת���ɸ���������
		Mat tempImage;
		vSrcImage[i].convertTo(tempImage, CV_64FC1, 1.0 / 255.0);
		Mat p = tempImage.clone();
		// �ֱ���е����˲�       
		Mat resultImage = guidedfilter(tempImage, p, 4, 0.01);
		vResultImage.push_back(resultImage);
	}
	// ͨ������ϲ�   
	merge(vResultImage, resultMat);
	cv::imshow("srcImage", srcImage);
	cv::imshow("resultMat", resultMat);
	cv::waitKey(0);
	return 0;
}
