#include "load.h"

uint8_t Imgdata[IMG_ROW][IMG_COL];//图片数据
#define showImg PerImg
cv::String folder = "F:\\Desktop\\smart_car\\image\\img0";//默认播放文件夹的路径
std::vector<cv::String> imagePathList;//存放图片的序列
uint8_t img_error = 0;

/**
 * @描述: 读取一张图像在Mat矩阵，转换为数组
 * @传入参数 {String} addr
 * @返回 {*}
 */
void Imgread_Mat2Buffer(cv::String addr)
{
	cv::Mat img(IMG_ROW, IMG_COL, CV_8UC1);
	int i, j;
	img = cv::imread(addr, 0);
	if (img.rows != IMG_ROW || img.cols != IMG_COL)
	{
		printf_s("图片大小不适合！！！\n");
		img_error = 1;
		return;
	}
	for (i = 0; i < IMG_ROW; i++)
	{
		for (j = 0; j < IMG_COL; j++)
		{
			Imgdata[i][j] = img.at<uchar>(i, j);
		}
	}
	cv::imshow("befor", img);
	ImageDeal_main();
}

/**
 * @描述: 将数组读取为Mat矩阵，并显示
 * 后续需要添加处理部分显示彩色标注
 * @返回 {*}
 */

void Imshow_Buffer2Mat()
{
	int i, j;
	cv::Point location;
	cv::Mat show(IMG_ROW, IMG_COL, CV_8UC3);
	for (i = 0; i < IMG_ROW; i++)
	{
		for (j = 0; j < IMG_COL; j++)
		{
			location.y = i;
			location.x = j;
			if (showImg[i][j] == BLACK)
			{
				cv::circle(show, location, 0, cv::Scalar(0, 0, 0), -1);
			}
			if (showImg[i][j] == WHITE)
			{
				cv::circle(show, location, 0, cv::Scalar(255, 255, 255), -1);
			}
			if (showImg[i][j] == BLUE)
			{
				cv::circle(show, location, 0, cv::Scalar(255, 0, 0), -1);
			}
			if (showImg[i][j] == GREEN)
			{
				cv::circle(show, location, 0, cv::Scalar(0, 255, 0), -1);
			}
			if (showImg[i][j] == RED)
			{
				cv::circle(show, location, 0, cv::Scalar(0, 0, 255), -1);
			}
			else {
				cv::circle(show, location, 0, cv::Scalar(showImg[i][j],showImg[i][j], showImg[i][j]), -1);
			}
		}
	}
	cv::resize(show, show, cv::Size(188 * 4, 120 * 4), cv::INTER_LINEAR);
	cv::imshow("after", show);
}

/**
 * @描述: 读取文件夹的图片，中间通过算法处理数据并显示结果
 * @返回 {*}
 */
void Read_folder(cv::String folder_addr,int FramTime)
{
	cv::glob(folder_addr, imagePathList);
	for (int i = 0; i < imagePathList.size(); i++)
	{
		std::cout << imagePathList[i] << "\n";
		Imgread_Mat2Buffer(imagePathList[i]);
		Imshow_Buffer2Mat();
		cv::waitKey(FramTime);
	}
	reset_staus();
	cv::waitKey(1000);
	cv::destroyAllWindows();
}