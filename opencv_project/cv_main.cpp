#include "cv_main.h"
#include "load.h"

int input, runing_flag = 1;
int frame_delay;
char input_str[1000];
cv::String img_addr;

int main()
{
	while (runing_flag)
	{
		printf_s("���ǻ���opencv������һ���������\n");
		printf_s("�����������Խ�������Ҫ���еĲ���\n");
		printf_s("1��������ͼƬ\n");
		printf_s("2�����������ļ��е�ͼƬ\n");
		printf_s("3������Ĭ���ļ���\n");
		printf_s("4���˳�\n");
		std::cin >> input;
		switch (input)
		{
		case 1:
			printf_s("������ͼƬ��ַ\n");
			std::cin >> input_str;
			img_addr = input_str;
			Imgread_Mat2Buffer(img_addr);
			Imshow_Buffer2Mat();
			cv::waitKey(0);
			cv::destroyAllWindows();
			break;
		case 2:
			printf_s("�������ļ��е�ַ\n");
			std::cin >> input_str;
			img_addr = input_str;
			Read_folder(img_addr, 1);
			break;
		case 3:
			printf_s("������ÿ��ͼƬ��������0Ϊ���°�������л���\n");
			std::cin >> frame_delay;
			Read_folder(folder, frame_delay);
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