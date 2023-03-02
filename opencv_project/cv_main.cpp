#include "cv_main.h"
#include "load.h"

int input, runing_flag = 1;
char input_str[1000];
cv::String img_addr;

int main()
{
	while (runing_flag)
	{
		printf_s("这是基于opencv开发的一个仿真代码\n");
		printf_s("请输入数字以进行你想要进行的操作\n");
		printf_s("1、处理单张图片\n");
		printf_s("2、播放整个文件夹的图片\n");
		printf_s("3、播放默认文件夹\n");
		printf_s("4、退出\n");
		std::cin >> input;
		switch (input)
		{
		case 1:
			printf_s("请输入图片地址\n");
			std::cin >> input_str;
			img_addr = input_str;
			Imgread_Mat2Buffer(img_addr);
			Imshow_Buffer2Mat();
			cv::waitKey(1000);
			cv::destroyAllWindows();
			break;
		case 2:
			printf_s("请输入文件夹地址\n");
			std::cin >> input_str;
			img_addr = input_str;
			Read_folder(img_addr, 1);
			break;
		case 3:
			Read_folder(folder, 1);
			break;
		case 4:
			runing_flag = 0;
			break;
		default:
			break;
		}
	}
	cv::waitKey(0);
	return 0;
}