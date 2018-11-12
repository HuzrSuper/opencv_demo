
#include <opencv2/opencv.hpp>
int main()
{
	// ͼ���ȡ����֤
	cv::Mat srcImage = cv::imread("flower3.jpg");
	if (!srcImage.data)
		return 1;
	// �洢��ɫֱ��ͼ��ͼ��ͨ������
	cv::Mat colorHeqImage;
	std::vector<cv::Mat> BGR_plane;
	// ��BGRͨ�����з���
	cv::split(srcImage, BGR_plane);
	// �ֱ��BGR����ֱ��ͼ���⻯
	for (int i = 0; i < BGR_plane.size(); i++)
		cv::equalizeHist(BGR_plane[i], BGR_plane[i]);
	// �ϲ���Ӧ����ͨ��
	cv::merge(BGR_plane, colorHeqImage);
	cv::imshow("srcImage", srcImage);
	cv::imshow("colorHeqImage", colorHeqImage);
	cv::waitKey(0);
	return 0;
}
