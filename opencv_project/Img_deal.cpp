#include "Img_deal.h"


 //Created by RUPC on 2022/9/20.


#define PER_IMG Imgdata      // SimBinImage:用于透视变换的图像
#define ImageUsed *PerImg_ip //*PerImg_ip定义使用的图像，ImageUsed为用于巡线和识别的图像
#define Boardsize 100
typedef unsigned char uint8_t; // 无符号  8 bits
uint8_t *PerImg_ip[RESULT_ROW][RESULT_COL];
uint8_t PerImg[RESULT_ROW][RESULT_COL];
uint8_t BlackColor = 0;
uint8_t Lrow[Boardsize], Lcol[Boardsize], Rrow[Boardsize], Rcol[Boardsize], Lsize, Rsize;
void ImagePerspective_Init(void)
{

    double change_un_Mat[3][3] = {{0.117206, -0.101210, 3.725771}, {0.005767, -0.002085, 2.745541}, {0.000099, -0.001054, 0.145553}};
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
            if (PerImg_ip[i][j] == &BlackColor)
                continue;
            else
            {
                PerImg[i][j] = ImageUsed[i][j];
            }
        }
}
static inline uint8_t Find_Start_Point(unsigned char *x1, unsigned char *y1, unsigned char *x2, unsigned char *y2, int block_size, int thres)
{
    unsigned char l_flag = 1, r_flag = 1;
    int half = block_size / 2, x, y;
    for (y = IMG_ROW - 2 - half; l_flag && y > IMG_ROW - 20 - half; y--)
    {
        for (x = IMG_COL / 2; x > 2; x--)
        {
            if (PerImg_ip[y][x - 3] == &BlackColor)
                break;
            if (ImageUsed[y][x + 2] + ImageUsed[y][x + 1] - ImageUsed[y][x - 2] - ImageUsed[y][x - 1] > thres)
            {
                *x1 = x, *y1 = y;
                l_flag = 0;
                break;
            }
        }
    }
    for (y = IMG_ROW - 2 - half; r_flag && y > IMG_ROW - 20 - half; y--)
    {
        for (x = IMG_COL / 2; x < IMG_COL - 2; x++)
        {
            if (PerImg_ip[y][x + 3] == &BlackColor)
                break;
            if (ImageUsed[y][x + 2] + ImageUsed[y][x + 1] - ImageUsed[y][x - 2] - ImageUsed[y][x - 1] < -thres)
            {
                *x2 = x, *y2 = y;
                r_flag = 0;
                break;
            }
        }
    }
    if (!l_flag && !r_flag)
        return 3;//0011
    else if (!r_flag)
        return 2;//0010
    else if (!l_flag)
        return 1;//0001
    else
        return 0;
}
const signed char rowfront[4] = {-1, 0, 1, 0};
const signed char colfront[4] = {0, 1, 0, -1};
const signed char rowleftfront[4] = {-1, -1, 1, 1};
const signed char colleftfront[4] = {-1, 1, 1, -1};
const signed char rowrightfront[4] = {-1, 1, 1, -1};
const signed char colrightfront[4] = {1, 1, -1, -1};
static inline void Find_R(unsigned char rows[], unsigned char cols[], int block_size, unsigned char *size, unsigned char x, unsigned char y, int clip_value)
{
    unsigned char step = 0, dir = 0, turn = 0;
    int half = block_size / 2;
    while (step < *size && turn < 4 && half +1< x && x < IMG_COL - 2 - half && 6 < y && y < IMG_ROW - 1 - half && PerImg_ip[y][x] != &BlackColor && PerImg_ip[y][x - 2] != &BlackColor &&PerImg_ip[y][x+2] != &BlackColor)
    {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++)
        {
            for (int dx = -half; dx <= half; dx++)
            {
                local_thres += ImageUsed[y + dy][x + dx];
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;
        unsigned char front_value = ImageUsed[y + rowfront[dir]][x + colfront[dir]];
        unsigned char frontright_value = ImageUsed[y + rowrightfront[dir]][x + colrightfront[dir]];
        if (front_value < local_thres)
        {
            dir = (dir + 3) % 4;
            turn++;
        }
        else if (frontright_value < local_thres)
        {
            x += colfront[dir];
            y += rowfront[dir];
            rows[step] = y;
            cols[step] = x;
            step++;
            turn = 0;
        }
        else
        {
            y += rowrightfront[dir];
            x += colrightfront[dir];
            dir = (dir + 1) % 4;
            rows[step] = y;
            cols[step] = x;
            step++;
            turn = 0;
        }
    }
    *size = step;
}
static inline void Find_L(unsigned char rows[], unsigned char cols[], int block_size, unsigned char *size, unsigned char x, unsigned char y, int clip_value)
{
    unsigned char step = 0, dir = 0, turn = 0;
    int half = block_size / 2;
    while (step < *size && turn < 4 && half +1< x && x < IMG_COL - 2 - half && 6 < y && y < IMG_ROW - 1 - half && PerImg_ip[y][x] != &BlackColor && PerImg_ip[y][x - 2] != &BlackColor && PerImg_ip[y][x + 2] != &BlackColor)
    {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++)
        {
            for (int dx = -half; dx <= half; dx++)
            {
                local_thres += ImageUsed[y + dy][x + dx];
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;

        unsigned char front_value = ImageUsed[y + rowfront[dir]][x + colfront[dir]];
        unsigned char frontleft_value = ImageUsed[y + rowleftfront[dir]][x + colleftfront[dir]];
        if (front_value < local_thres)
        {
            dir = (dir + 1) % 4;
            turn++;
        }
        else if (frontleft_value < local_thres)
        {
            x += colfront[dir];
            y += rowfront[dir];
            rows[step] = y;
            cols[step] = x;
            step++;
            turn = 0;
        }
        else
        {
            y += rowleftfront[dir];
            x += colleftfront[dir];
            dir = (dir + 3) % 4;
            rows[step] = y;
            cols[step] = x;
            step++;
            turn = 0;
        }
    }
    *size = step;
}
static inline void Draw_Board(uint8_t row[], uint8_t col[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        PerImg[row[i]][col[i]] = BLUE;
    }
}
void ImageDeal_main()
{
    uint8_t lx, ly, rx, ry, return_state;
    return_state = Find_Start_Point(&lx, &ly, &rx, &ry, 3, 100);
    if (return_state & 0x01)
    {
        Lsize = Boardsize;
        Find_L(Lrow, Lcol, 3, &Lsize, lx, ly, 10);
    }
    if (return_state & 0x02)
    {
        Rsize = Boardsize;
        Find_R(Rrow, Rcol, 3, &Rsize, rx, ry, 10);
    }
    show_PerImg();
    
    if (return_state & 0x01)
    {
        PerImg[ly][lx] = BLUE;
        Draw_Board(Lrow, Lcol, Lsize);
        cout << "Lsize: " << (int)Lsize << endl;
    }
    if (return_state & 0x02)
    {
        PerImg[ry][rx] = BLUE;
        Draw_Board(Rrow, Rcol, Rsize);
        cout << "Rsize: " << (int)Rsize << endl;
    }
}
void reset_staus()
{
}