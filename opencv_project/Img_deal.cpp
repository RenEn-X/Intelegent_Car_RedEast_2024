#include "Img_deal.h"

//
// Created by RUPC on 2022/9/20.
//

#define PER_IMG Imgdata        // SimBinImage:用于透视变换的图像
#define ImageUsed *PerImg_ip   //*PerImg_ip定义使用的图像，ImageUsed为用于巡线和识别的图像
typedef unsigned char uint8_t; // 无符号  8 bits
uint8_t *PerImg_ip[RESULT_ROW][RESULT_COL];
uint8_t PerImg[RESULT_ROW][RESULT_COL];
void ImagePerspective_Init(void)
{

    static uint8_t BlackColor = 0;
    double change_un_Mat[3][3] = { {-0.044987,0.041236,-1.569150},{-0.000305,0.000802,-1.333432},{-0.000007,0.000425,-0.060192} };
    for (int i = 0; i < RESULT_COL; i++)
    {
        for (int j = 0; j < RESULT_ROW; j++)
        {
            int local_x = (int)((change_un_Mat[0][0] * i + change_un_Mat[0][1] * j + change_un_Mat[0][2]) / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j + change_un_Mat[2][2]));
            int local_y = (int)((change_un_Mat[1][0] * i + change_un_Mat[1][1] * j + change_un_Mat[1][2]) / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j + change_un_Mat[2][2]));
            if (local_x >= 0 && local_y >= 0 && local_y < USED_ROW && local_x < USED_COL)
            {
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else
            {
                PerImg_ip[j][i] = &BlackColor; //&PER_IMG[0][0];
            }
        }
    }
}

/*完成摄像头初始化后，调用一次ImagePerspective_Init，此后，直接调用ImageUsed   即为透视结果*/
static inline void show_PerImg()
{
    for (uint8_t i = 0; i < IMG_ROW; i++)
        for (uint8_t j = 0; j < IMG_COL; j++)
        {
            PerImg[i][j] = *PerImg_ip[i][j];
        }
}
void ImageDeal_main()
{
    show_PerImg();
}
void reset_staus()
{
}