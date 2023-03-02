#include "Img_deal.h"

//----------------����������--------------

#define IMG_DATA Imgdata
#define ROW IMG_ROW
#define COL IMG_COL
int  dj_end;
unsigned char stop = 1;//ͣ����־λ����1��ʾͣ��
unsigned char go = 0;//ֱ�б�־λ��1��ʾֱ��
unsigned char deal_flag = 0;//������־λ
unsigned char fork_flag = 0;//�����־λ
unsigned char T_go_flag[2] = { 1,0 };//T�䲹�߱�־λ��0��1��
unsigned char T_go_flag_pin = 0;//T�䲹�߱�ڶ���������־λ��0��1��
unsigned char fork_turn = 1;//1��2��
uint8_t distance_flag = 0;
uint8_t fork_times = 0;
uint8_t Boost_flag = 0;
ALL_enum element_flag = NO_JUGED;
FORK_enum Fork_Flag = NO_FORK;
T_CONNER_enum T_flag = NO_T;
Garage_enmum Garage_flag = GET_OUT;
Roundabout_enmum Roundabout_flag = NO_ROUND;
Roundabout_position_enmum Roundabout_flag_position = NOROUND;
Ramp_enmum ramp_flag = NO_RAMP;
int speed_flag = 254;
int round_row_flag;
int row_lim = 20;
//int i = 0,j = 0;

//********************************************

//Ѳ��
int right_flag = 0;		//�������ڱ�־
int left_flag = 0;
int right_start = 0;	//�ұ��������
int left_start = 0;
int right_end_row = 0;		//�ұ����յ���
int left_end_row = 0;
int right_end_col = 0;      //�ұ����յ���
int left_end_col = 0;
int middle_start = 0;	//���������
int middle_end = 0;
int middleline[240];
int middleline0[240];//��һ����
int middleline_l[240];
int middleline_r[240];
int SUM = 0;
int wild1, wild10;
struct DIV
{
    int Row[240];
    int Col[240];
};

struct AG
{
    float k;
    float SIN;
};

struct APEX
{
    int Apex_Row;
    int Apex_Col;
    int Mark;
};

struct Vector
{
    int star[2];
    int end[2];
};

struct DIV RacingLine;//Ŀ����·
struct DIV left, right;//���ұ���
struct DIV forck_L, forck_R;//�����ж���·
struct DIV T_line;//T���ж���
struct DIV forck_line_L, forck_line_R;//���油����·
struct DIV Round_L, Round_R;//����������·
struct APEX left_apex, right_apex;
struct AG right_ag, left_ag;//Ѱ�������������
struct Vector Vector_r, Vector_l;//��ֹ�������������

int right_line[120] = { 160 };//�˴���Ҫ�ĳ�159������֪��Ϊ������û�����⣬������û����ֻ���󵥱ߵ������ȥѧУ����һ�³�����

int left_line[120] = { 0 };
int max = 0;
int min = 0;
int line[120] = { 0 };
int Threshold;
int mid_row_R[240];
int mid_row_L[240];
int mid_row[240];
int mid_flag = 0;
int L_lenth, R_lenth;
//static int COUNT=0;
float Angle_end;
float Angle_End[10] = { 0 };
float ang_l, ang_r, ang;//����Ƕ��ж�
float L_S, R_S;//�߽緽��
float K_left, K_right;
float CorssCol = 0;
//*********************************************

int TrackWild[120] = {
        2, 3, 5, 6, 7, 8,10,11,12,14,15,16,17,19,20,21,22,24,25,
        26,27,29,30,31,33,34,35,36,38,39,40,41,43,44,45,47,48,49,
        50,52,53,54,55,57,58,59,61,62,63,64,66,67,68,69,71,72,73,
        75,76,77,78,80,81,82,83,85,86,87,89,90,91,92,94,95,96,97,
        99,100,101,103,104,105,106,108,109,110,111,113,114,115,116,118,119,120,122,
        123,124,125,127,128,129,130,132,133,134,136,137,138,139,141,142,143,144,146,
        147,148,150,151,152,153 };

int Thresho;
uint8_t GetOSTU(uint8_t tmImage[IMG_ROW][IMG_COL])
{
    uint16_t i, j;
    uint32_t Amount = 0;
    uint32_t PixelBack = 0;
    uint32_t PixelIntegralBack = 0;
    uint32_t PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;
    uint8_t HistoGram[256] = { 0 };              //  

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 

    for (j = 0; j < IMG_ROW; j++)
    {
        for (i = 0; i < IMG_COL; i++)
        {
            HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)     return (uint8_t)MaxValue;         // ͼ����ֻ��һ����ɫ    
    if (MinValue + 1 == MaxValue)  return (uint8_t)MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
        PixelFore = Amount - PixelBack;         //�������ص���
        OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
        OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
        PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
        MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
        {
            SigmaB = Sigma;
            Threshold = (uint8_t)j;
        }
    }
    return Threshold;                        //���������ֵ;
}

void BinaryImage(uint8_t tmImage[IMG_ROW][IMG_COL], uint8_t ThresholdV)
{
    printf_s("��ֵΪ:%d\r\n", Thresho);
    int i = 0, j = 0;
    for (i = 0; i < IMG_ROW; i++)
    {
        for (j = 0; j < IMG_COL; j++)
        {
            if (tmImage[i][j] >= ThresholdV)
            {
                tmImage[i][j] = WHITE;
            }
            else
            {
                tmImage[i][j] = BLACK;
            }
        }
    }
}

void Deal_Init()
{
    right_flag = 0; left_flag = 0;
    right_start = 254; left_start = 254;
    middle_start = 0; middle_end = 0;
    Thresho = GetOSTU(Imgdata);
    BinaryImage(Imgdata, Thresho);
}

void InitData() //��ʼ���߽���
{
    int pin;
    for (pin = 0; pin < 240; pin++)
    {
        ::left.Row[pin] = 254;//254������Ч����
        ::left.Col[pin] = 254;
        ::right.Row[pin] = 254;
        ::right.Col[pin] = 254;
        if (Fork_Flag == NO_FORK)
        {
            forck_L.Row[pin] = 254;
            forck_L.Col[pin] = 254;
            forck_R.Row[pin] = 254;
            forck_R.Col[pin] = 254;
        }
        if (Roundabout_flag == NO_ROUND)
        {
            Round_L.Row[pin] = 254;
            Round_L.Col[pin] = 254;
            Round_R.Row[pin] = 254;
            Round_R.Col[pin] = 254;
        }
        T_line.Row[pin] = 254;
        T_line.Col[pin] = 254;
        RacingLine.Row[pin] = 254;
        RacingLine.Col[pin] = 254;
        middleline[pin] = 254;
        middleline_l[pin] = 254;
        middleline_r[pin] = 254;
        mid_row_R[pin] = 254;
        mid_row_L[pin] = 254;
        mid_row[pin] = 254;
    }
    speed_flag = 254;
    L_S = 0;
    R_S = 0;
    left_apex.Mark = 254;
    right_apex.Mark = 254;
    Vector_r.star[0] = 254;
    Vector_r.star[1] = 254;
    Vector_r.end[0] = 254;
    Vector_r.end[1] = 254;
    Vector_l.star[0] = 254;
    Vector_l.star[1] = 254;
    Vector_l.end[0] = 254;
    Vector_l.end[1] = 254;
    L_lenth = 0;
    R_lenth = 0;
    fork_flag = 0;
    ang_l = 0;
    ang_r = 0;
    ang = 0;
}

///***************************************************************
//* �������ƣ� Black_White_Zone
//* ����˵���� �ڰ�����ɨ�������ںڰ׵�
//* �������أ� 0��1�������������
//* �� ע��    ���ϵ�����,x��,y��
//***************************************************************
int Black_White_Zone(int x1, int y1, int x2, int y2)
{
    int row, col;
    int point = 0;
    int Black_point = 0;
    int White_point = 0;
    float B_percent, W_percent;
    float k1, k2;
    k1 = 1.0 * (y2 - y1) / (x2 - x1);
    k2 = 1.0 * (y2 - y1) / (x1 - x2);
    for (col = x1; col < x2; col++)
    {
        row = (int)(k1 * (col - x1) + y1);
        if (Imgdata[row][col] == BLACK) Black_point++;
        else White_point++;
        point++;
    }
    for (col = x1; col < x2; col++)
    {
        row = (int)(k2 * (col - x1) + y2);
        if (Imgdata[row][col] == BLACK) Black_point++;
        else White_point++;
        point++;
    }
    row = (y1 + y2) / 2;
    for (col = x1; col < x2; col++)
    {
        if (Imgdata[row][col] == BLACK) Black_point++;
        else White_point++;
        point++;
    }
    col = (x1 + x2) / 2;
    for (row = y1; row < y2; row++)
    {
        if (Imgdata[row][col] == BLACK) Black_point++;
        else White_point++;
        point++;
    }
    B_percent = 1.0 * Black_point / point;
    W_percent = 1.0 * White_point / point;
    if (B_percent > 0.6) return 1;
    else if (W_percent > 0.6) return 2;
    else return 0;
}

///***************************************************************
//* �������ƣ� Cross_col
//* ����˵���� ��ͨ�У������ж�ֱ��
//* �������أ� �׵�ռ�ȣ�float��
//* �� ע��
//***************************************************************
float Cross_col()
{
    float W_percent = 0;
    int i, j;
    int White_point = 0;
    int Black_point = 0;
    int point = 600;
    int col[5] = { 84,90,94,98,102 };
    for (i = 0; i < 5; i++)
    {
        for (j = 119; j > 0; j--)
        {
            if (Imgdata[j][col[i]] == WHITE)
                White_point++;
            else
            {
                Black_point++;
                if (Black_point > 4)
                {
                    Black_point = 0;
                    break;
                }
            }
        }
    }
    W_percent = 1.0 * White_point / point;
    return W_percent;
}
float CrossRow_L(int star)
{
    float W_percent = 0;
    int i, j;
    int White_point = 0;
    int Black_point = 0;
    int point = 470;
    int row[5] = { 0 };
    if (star >= 100) return -1;
    for (i = 0; i < 5; i++) row[i] = star + 5 * i;
    for (i = 0; i < 5; i++)
    {
        for (j = 94; j > 5; j--)
        {
            if (Imgdata[row[i]][j] == WHITE)
                White_point++;
            else if (Imgdata[row[i]][j] == BLACK)
            {
                Black_point++;
                if (Black_point > 4)
                {
                    Black_point = 0;
                    break;
                }
            }
        }
    }
    W_percent = (float)White_point / point;
    return W_percent;
}

float CrossRow_R(int star)
{
    float W_percent = 0;
    int i, j;
    int White_point = 0;
    int Black_point = 0;
    int point = 470;
    int row[5] = { 0 };
    if (star >= 100) return -1;
    for (i = 0; i < 5; i++) row[i] = star + 5 * i;
    for (i = 0; i < 5; i++)
    {
        for (j = 94; j < 183; j++)
        {
            if (Imgdata[row[i]][j] == WHITE)
                White_point++;
            else if (Imgdata[row[i]][j] == BLACK)
            {
                Black_point++;
                if (Black_point > 4)
                {
                    Black_point = 0;
                    break;
                }
            }
        }
    }
    W_percent = (float)White_point / point;
    return W_percent;
}
///***************************************************************
//* �������ƣ� S_jugde
//* ����˵���� ��׼���жϣ���Ϊy
//* �������أ� �߽�����������߱�׼��
//* �� ע��    �жϱ߽��׼���
//***************************************************************
float S_jugde(int row[], int col[], int N)
{
    int xy = 0;
    int x_sq = 0;
    float k, b;
    float S;
    float x_avg = 0;
    float y_avg = 0;
    float x_avg_sq = 0;
    int y_tg;
    int i, num;
    S = 254;
    for (i = 0, num = 0; i < 240; i++)
    {
        if (col[i] == 254 || i == N)
        {
            x_avg = x_avg / num;
            break;
        }
        if (row[i] == row[i + 1]) continue;
        x_avg += col[i];
        num++;
    }
    x_avg_sq = x_avg * x_avg;
    for (i = 0; i < 240; i++)
    {
        if (row[i] == 254 || i == N)
        {
            y_avg = y_avg / num;
            break;
        }
        if (row[i] == row[i + 1]) continue;
        y_avg += row[i];
    }
    for (i = 0; i < 240; i++)
    {
        if (row[i] == 254 || col[i] == 254 || i == N) break;
        if (row[i] == row[i + 1]) continue;
        xy += (row[i] * col[i]);
    }
    for (i = 0; i < 240; i++)
    {
        if (col[i] == 254 || i == N) break;
        if (row[i] == row[i + 1]) continue;
        x_sq += (col[i] * col[i]);
    }
    k = (xy - num * x_avg * y_avg) / (x_sq - num * x_avg_sq);
    b = y_avg - k * x_avg;
    for (i = 0; i < 240; i++)
    {
        if (row[i] == 254 || i == N)
        {
            S = sqrt(S / (num - 1));
            break;
        }
        if (row[i] == row[i + 1]) continue;
        y_tg = k * col[i] + b;
        S += (y_tg - row[i]) * (y_tg - row[i]);
    }
    S = S / sqrt(1.0 * num);
    return S;
}

///***************************************************************
//* �������ƣ� 
//* ����˵���� �������
//* �������أ� 
//* �� ע������ͼ����ָ�ϱ�д    
//***************************************************************
void LeftStartFind()
{
    int row, col;
    for (row = IMG_ROW - 5; row > 40 && ::left.Row[0] == 254; row--)
    {
        for (col = 5; col < IMG_COL - 55 && ::left.Col[0] == 254; col++)
        {
            if (Imgdata[row][col] == BLACK && Imgdata[row][col + 1] == BLACK)
            {
                if (Imgdata[row][col + 2] == WHITE && Imgdata[row][col + 3] == WHITE)
                {
                    ::left.Row[0] = row;
                    ::left.Col[0] = col + 1;
                    Imgdata[row][col + 1] = RED;
                    left_flag = 1;
                }
            }
        }
    }
    left_start = ::left.Row[0];
}

//***************************************************************
//* �������ƣ� 
//* ����˵���� �������
//* �������أ� ��
//* �� ע��    
//***************************************************************
void RightStartFind()
{
    int row, col;
    for (row = IMG_ROW - 5; row > 40 && ::right.Row[0] == 254; row--)
    {
        for (col = IMG_COL - 5; col > 55 && ::right.Col[0] == 254; col--)
        {
            if (Imgdata[row][col] == BLACK && Imgdata[row][col - 1] == BLACK)
            {
                if (Imgdata[row][col - 2] == WHITE && Imgdata[row][col - 3] == WHITE)
                {
                    ::right.Row[0] = row;
                    ::right.Col[0] = col - 1;
                    Imgdata[row][col - 1] = BLUE;
                    right_flag = 1;
                }
            }
        }
    }
    right_start = ::right.Row[0];
}

void left_jump()
{
    int pin;//, i, j;
    int row, col, find;
    //	int row_temp, col_temp;
    //	int whitecounter = 0;
    //	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
    //	int colcheack[12] = {1, 0, -1, -1, 2, -1, -1, 0, 0, 1, -2, 1};
    int colmin, colmax;
    if (::left.Row[0] == 254)
    {
        return;
    }
    left_flag = 1;
    pin = 1;
    for (row = ::left.Row[0] - pin; pin < 240; pin++)
    {
        find = 0;
        row = ::left.Row[0] - pin;
        if (row <= row_lim)break;
        colmin = ::left.Col[pin - 1] - 10;
        colmax = ::left.Col[pin - 1] + 10;
        for (col = colmin; col <= colmax; col++)
        {
            if (col < 5) col = 5;
            if (col < IMG_COL - 5)
            {
                if (Imgdata[row][col] == BLACK && Imgdata[row][col + 1] == BLACK)
                {
                    if (Imgdata[row][col + 2] == WHITE && Imgdata[row][col + 3] == WHITE)
                    {
                        ::left.Row[pin] = row;
                        ::left.Col[pin] = col + 1;
                        Imgdata[row][col + 1] = RED;
                        find = 1;
                        L_lenth++;
                        left_end_row = row;
                        left_end_col = col;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    if (L_lenth <= 5)
    {
        left_flag = 0;
        ::left.Row[0] = 254;
        ::left.Col[0] = 254;
    }

}
void right_jump()
{
    int pin;//, i, j;
    int row, col, find;
    //	int row_temp, col_temp;
    //	int whitecounter = 0;
    int colmin, colmax;
    //	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
    //	int colcheack[12] = {-1, 0, 1, 1, -2, 1, 1, 0, 0, -1, 2, -1};
    if (::right.Row[0] == 254)
    {
        return;
    }
    right_flag = 1;
    for (pin = 1, row = ::right.Row[0] - pin; pin < 240; pin++)
    {
        find = 0;
        row = ::right.Row[0] - pin;
        if (row <= row_lim)break;
        colmin = ::right.Col[pin - 1] - 10;
        colmax = ::right.Col[pin - 1] + 10;
        for (col = colmax; col >= colmin; col--)
        {
            if (col > IMG_COL - 5) col = IMG_COL - 5;
            if (col > 5)
            {
                if (Imgdata[row][col] == BLACK && Imgdata[row][col - 1] == BLACK)
                {
                    if (Imgdata[row][col - 2] == WHITE && Imgdata[row][col - 3] == WHITE)
                    {
                        ::right.Row[pin] = row;
                        ::right.Col[pin] = col - 1;
                        Imgdata[row][col - 1] = BLUE;
                        find = 1;
                        R_lenth++;
                        right_end_row = row;
                        right_end_col = col;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    if (R_lenth <= 5)
    {
        right_flag = 0;
        ::right.Row[0] = 254;
        ::right.Col[0] = 254;
    }


}

//***************************************************************
//* �������ƣ� fixpoint()
//* ����˵����������ʼ������
//* �������أ� 
//* �� ע��    
//***************************************************************
void fixpoint()//���ұ�����ʼ�������
{
    if (::left.Col[0] == 254 && ::left.Row[0] == 254)
    {
        return;
    }
    if (::right.Col[0] == 254 && ::right.Row[0] == 254)
    {
        return;
    }
    float k = 0;
    int UpPointCol = 254, UpPointRow = 254;
    int DownPointCol = 254, DownPointRow = 254;
    int row, col, pin, BP;
    BP = 0;
    if (::left.Row[0] < ::right.Row[0])
    {
        UpPointCol = ::left.Col[0];
        UpPointRow = ::left.Row[0];
        DownPointCol = ::right.Col[0];
        DownPointRow = ::right.Row[0];
    }
    else
    {
        UpPointCol = ::right.Col[0];
        UpPointRow = ::right.Row[0];
        DownPointCol = ::left.Col[0];
        DownPointRow = ::left.Row[0];
    }
    if (UpPointRow != DownPointRow)
    {
        k = (float)(UpPointCol - DownPointCol) / (UpPointRow - DownPointRow);
    }
    for (pin = 0, row = DownPointRow; row >= UpPointRow; pin++, row--)
    {
        col = (int)(DownPointCol - k * pin);
        if (col > -1 && col < IMG_COL && row >-1 && row < IMG_ROW)
        {
            if (Imgdata[row][col] == BLACK)
            {
                BP++;
            }
        }
    }
    if (BP > 8)
    {
        if (::left.Row[0] < ::right.Row[0])
        {
            ::left.Col[0] = 254;
            ::left.Row[0] = 254;
            left_flag = 0;
        }
        else if (::left.Row[0] > ::right.Row[0])
        {
            ::right.Col[0] = 254;
            ::right.Row[0] = 254;
            right_flag = 0;
        }
    }
}

void Left_Apex()
{
    if (::left.Row[0] == 254 && ::left.Col[0] == 254)
    {
        return;
    }
    int pin, col_max = 0;
    int row_flag, col_flag;
    for (pin = 0; pin < 240; pin++)
    {
        if (::left.Row[pin] == 254)
        {
            break;
        }
        if (col_max <= ::left.Col[pin])
        {
            col_max = ::left.Col[pin];
            row_flag = ::left.Row[pin];
            col_flag = ::left.Col[pin];
            left_apex.Mark = pin;
        }
    }
    left_apex.Apex_Row = row_flag;
    left_apex.Apex_Col = col_flag;
    L_S = S_jugde(::left.Row, ::left.Col, 240);
}//Ѱ��������

void Right_Apex()
{
    if (::right.Row[0] == 254 && ::right.Col[0] == 254)
    {
        return;
    }
    int pin;
    int row_flag = 0, col_flag = 0;
    int col_min = 188;
    for (pin = 0; pin < 240; pin++)
    {
        if (::right.Row[pin] == 254)
        {
            break;
        }
        if (::right.Col[pin] <= col_min)
        {
            col_min = ::right.Col[pin];
            row_flag = ::right.Row[pin];
            col_flag = ::right.Col[pin];
            right_apex.Mark = pin;
        }
        //if(::right.Row[pin]<row_min) break;
    }
    right_apex.Apex_Row = row_flag;
    right_apex.Apex_Col = col_flag;
    R_S = S_jugde(::right.Row, ::right.Col, 240);
}//Ѱ��������

//***************************************************************
//* �������ƣ� Get_Center_Line
//* ����˵���� ������
//* �������أ� 
//* �� ע��    line[]������Ҫ�Լ����壬������ʾ�����Ŀ��ȣ�����line[5]��ʾ������������ʵ�ʿ���
//��Ҫ���ǵ�����ͷ������ʱ������ԽԶ������ͼ����ʾ��Խխ�������
//***************************************************************
void Racing_Line()
{
    int pin, i;
    int row, col;
    int flag;
    int mark;
    int l_flag, r_flag;
    l_flag = 0;
    r_flag = 0;
    i = 0;
    if (left_flag == 0 && right_flag == 0)
    {
        return;
    }
    if (left_flag == 0)
    {
        mark = right_apex.Mark + (115 - ::right.Row[5]);
        row = ::right.Row[5];
        i = 5;
        for (pin = 0; pin < mark; pin++)
        {
            flag = 119 - pin;
            if (flag > ::right.Row[5])
            {
                middleline[pin] = ::right.Col[5] - TrackWild[row] / 2;
                col = middleline[pin];
                mid_row[pin] = flag;
                IMG_DATA[flag][col] = GREEN;
            }
            else
            {
                row = ::right.Row[i];
                col = ::right.Col[i] - TrackWild[row] / 2;
                middleline[pin] = col;
                mid_row[pin] = row;
                i++;
                if (row > 5 && row < ROW - 5)
                {
                    if (col > 5 && col < COL - 5)
                    {
                        IMG_DATA[row][col] = GREEN;
                    }
                }
            }
        }
        return;
    }
    else if (right_flag == 0)
    {
        mark = left_apex.Mark + (115 - ::left.Row[5]);
        row = ::left.Row[5];
        i = 5;
        for (pin = 0; pin < mark; pin++)
        {
            flag = 119 - pin;
            if (flag > ::left.Row[5])
            {
                middleline[pin] = ::left.Col[5] + TrackWild[row] / 2;
                col = middleline[pin];
                IMG_DATA[flag][col] = GREEN;
                mid_row[pin] = flag;
            }
            else
            {
                row = ::left.Row[i];
                col = ::left.Col[i] + TrackWild[row] / 2;
                middleline[pin] = col;
                mid_row[pin] = row;
                i++;
                if (row > 5 && row < ROW - 5)
                {
                    if (col > 5 && col < COL - 5)
                    {
                        IMG_DATA[row][col] = GREEN;
                    }
                }
            }
        }
        return;
    }
    if (left_apex.Mark >= right_apex.Mark)
    {
        mark = left_apex.Mark + (115 - ::left.Row[5]);
        row = ::left.Row[5];
        i = 5;
        for (pin = 0; pin < mark; pin++)
        {
            flag = 119 - pin;
            if (flag > ::left.Row[5])
            {
                middleline[pin] = ::left.Col[5] + TrackWild[row] / 2;
                col = middleline[pin];
                mid_row[pin] = flag;
                IMG_DATA[flag][col] = GREEN;
            }
            else
            {
                row = ::left.Row[i];
                col = ::left.Col[i] + TrackWild[row] / 2;
                middleline[pin] = col;
                mid_row[pin] = row;
                i++;
                if (row > 5 && row < ROW - 5)
                {
                    if (col > 5 && col < COL - 5)
                    {
                        IMG_DATA[row][col] = GREEN;
                    }
                }
            }
            l_flag++;
        }
        return;
    }
    else if (left_apex.Mark < right_apex.Mark)
    {
        mark = right_apex.Mark + (115 - ::right.Row[5]);
        row = ::right.Row[5];
        i = 5;
        for (pin = 0; pin < mark; pin++)
        {
            flag = 119 - pin;
            if (flag > ::right.Row[5])
            {
                middleline[pin] = ::right.Col[5] - TrackWild[row] / 2;
                col = middleline[pin];
                mid_row[pin] = flag;
                IMG_DATA[flag][col] = GREEN;
            }
            else
            {
                row = ::right.Row[i];
                col = ::right.Col[i] - TrackWild[row] / 2;
                middleline[pin] = col;
                i++;
                mid_row[pin] = row;
                if (row > 5 && row < ROW - 5)
                {
                    if (col > 5 && col < COL - 5)
                    {
                        IMG_DATA[row][col] = GREEN;
                    }
                }
            }
            r_flag++;
        }
        return;
    }
}

//***************************************************************
//* �������ƣ� void protect()
//* ����˵���� ������ܳ�������ͣ��
//* �������أ� 
//* �� ע��    0��ʾ��ɫ 255��ʾ��ɫ������Լ�˼��һ����ôд
//***************************************************************
void protect()
{
    unsigned char i;
    unsigned char sum;
    int row;
    if (Garage_flag == GET_IN)
    {
        distance_flag = 1;
        row = 60;
    }
    else row = 115;
    sum = 0;
    //    if(Garage_flag!=GET_OUT)
    //    {
    //        if(speed_l<=0 || speed_r<=0)
    //        {
    //            stop=1;
    //            element_flag=NO_JUGED;
    //            Fork_Flag=NO_FORK;
    //            T_flag=NO_T;
    //            Garage_flag=GET_OUT;
    //            Roundabout_flag=NO_ROUND;
    //            Roundabout_flag_position=NOROUND;
    //            return;
    //        }
    //    }
    for (i = 0; i < 188; i++)
    {
        if (IMG_DATA[row][i] == BLACK)
            sum++;
    }
    SUM = sum;
    if (SUM >= 170)
    {
        stop = 1;
        element_flag = NO_JUGED;
        Fork_Flag = NO_FORK;
        T_flag = NO_T;
        Garage_flag = GET_OUT;
        Roundabout_flag = NO_ROUND;
        Roundabout_flag_position = NOROUND;
    }
    return;
}

//***************************************************************
//* �������ƣ� Conner_cheack
//* ����˵���� ������
//* �������أ�
//* �� ע��
//***************************************************************
void Conner_cheack()
{

}

//***************************************************************
//* �������ƣ� Roundabout_Deal
//* ����˵���� ������������
//* �������أ�
//* �� ע��
//***************************************************************
void Roundabout_Deal()
{
    int row, col, find;
    int col_min, col_max;
    float cross_row_L, cross_row_R;
    float cross_col, crossrow_juged;
    float round_l_s, round_r_s;
    cross_col = CorssCol;
    find = 0;
    switch (Roundabout_flag)
    {
    case NO_ROUND:
        cross_row_L = CrossRow_L(65);
        cross_row_R = CrossRow_R(65);
        if (element_flag == IN_JUGED && Roundabout_flag == NO_ROUND)
        {
            Roundabout_flag_position = NOROUND;
            Roundabout_flag = NO_ROUND;
            return;
        }
        if (cross_row_L > 0.9 && cross_row_R < 0.8 && R_S < 0.4)
        {
            Roundabout_flag_position = ROUND_L;
            Roundabout_flag = ROUND_READY;
            element_flag = NO_JUGED;
        }
        else if (cross_row_L < 0.8 && cross_row_R>0.9 && L_S < 0.4)
        {
            Roundabout_flag_position = ROUND_R;
            Roundabout_flag = ROUND_READY;
            element_flag = NO_JUGED;
        }
        if (right_apex.Apex_Col < 78 || left_apex.Apex_Col>110)
        {
            Roundabout_flag_position = NOROUND;
            Roundabout_flag = NO_ROUND;
            return;
        }
        break;
    case ROUND_READY:
        round_l_s = L_S;
        round_r_s = R_S;
        if (right_apex.Apex_Col < 78 || left_apex.Apex_Col>110)
        {
            Roundabout_flag_position = NOROUND;
            Roundabout_flag = NO_ROUND;
            return;
        }
        if (element_flag == IN_JUGED)
        {
            Roundabout_flag_position = NOROUND;
            Roundabout_flag = NO_ROUND;
            return;
        }
        if (CorssCol < 0.75)
        {
            Roundabout_flag = NO_ROUND;
            element_flag = NO_JUGED;
            return;
        }
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            if (right_apex.Apex_Col < 78 || R_lenth < 80 || L_lenth < 20)
            {
                Roundabout_flag_position = NOROUND;
                Roundabout_flag = NO_ROUND;
                return;
            }
            if (round_l_s > 0.9 && round_r_s < 0.4)
            {
                Roundabout_flag = GET_IN_READY;
                element_flag = NO_JUGED;
            }
            else if (round_r_s > 0.5)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            break;
        case ROUND_R:
            if (left_apex.Apex_Col > 110 || L_lenth < 80 || R_lenth < 20)
            {
                Roundabout_flag_position = NOROUND;
                Roundabout_flag = NO_ROUND;
                return;
            }
            if (round_l_s < 0.4 && round_r_s>0.9)
            {
                Roundabout_flag = GET_IN_READY;
                element_flag = NO_JUGED;
            }
            else if (round_l_s > 0.5)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case GET_IN_READY:
        round_l_s = L_S;
        round_r_s = R_S;
        cross_row_L = CrossRow_L(65);
        cross_row_R = CrossRow_R(65);
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            col_min = left_apex.Apex_Col - 10;
            col_max = COL / 2;
            if (round_r_s > 0.5)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            for (row = 50; row > 40; row--)
            {
                for (col = col_min; col < col_max; col++)
                {
                    if (IMG_DATA[row][col] == WHITE && IMG_DATA[row][col + 1] == BLACK)
                    {
                        Round_L.Row[0] = row;
                        Round_L.Col[0] = col + 1;
                        crossrow_juged = CrossRow_L(row);
                        if (crossrow_juged > 0.8 && cross_col > 0.75)
                        {
                            find = 1;
                            
                            Roundabout_flag = GET_IN_ROUND;
                            element_flag = IN_JUGED;
                        }
                    }
                }
            }
            break;
        case ROUND_R:
            col_min = COL / 2;
            col_max = right_apex.Apex_Col + 10;
            if (round_l_s > 0.5)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            for (row = 50; row > 40; row--)
            {
                for (col = col_max; col > col_min; col--)
                {
                    if (IMG_DATA[row][col] == WHITE && IMG_DATA[row][col - 1] == BLACK)
                    {
                        Round_R.Row[0] = row;
                        Round_R.Col[0] = col - 1;
                        crossrow_juged = CrossRow_R(row);
                        if (crossrow_juged > 0.75 && cross_col > 0.75)
                        {
                            find = 1;
                            
                            Roundabout_flag = GET_IN_ROUND;
                            element_flag = IN_JUGED;
                        }
                    }
                }
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case GET_IN_ROUND:
        
        row_lim = 25;
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            if (right_apex.Apex_Col < 47) Roundabout_flag = IN_ROUND;
            break;
        case ROUND_R:
            if (left_apex.Apex_Col > 141) Roundabout_flag = IN_ROUND;
            break;
        case NOROUND:
            break;
        }
        break;
    case IN_ROUND:
        row_lim = 15;
        round_row_flag = 254;
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            if (right_apex.Apex_Col > 94)
            {
                Roundabout_flag = GET_OUT_ROUND;
                element_flag = IN_JUGED;
            }
            break;
        case ROUND_R:
            if (left_apex.Apex_Col < 94)
            {
                Roundabout_flag = GET_OUT_ROUND;
                element_flag = IN_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case GET_OUT_ROUND:
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            cross_row_R = CrossRow_R(75);
            if (cross_row_R > 0.9)
            {
                Roundabout_flag = OUTTING;
                element_flag = IN_JUGED;
            }
            break;
        case ROUND_R:
            cross_row_L = CrossRow_L(75);
            if (cross_row_L > 0.9)
            {
                Roundabout_flag = OUTTING;
                element_flag = IN_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case OUTTING:
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            cross_row_R = CrossRow_R(75);
            if (cross_row_R < 0.8)
            {
                Roundabout_flag = OUT_WAY;
                element_flag = IN_JUGED;
            }
            break;
        case ROUND_R:
            cross_row_L = CrossRow_L(75);
            if (cross_row_L < 0.8)
            {
                Roundabout_flag = OUT_WAY;
                element_flag = IN_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case OUT_WAY:
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            cross_row_L = CrossRow_L(75);
            cross_row_R = CrossRow_R(75);
            if (cross_row_L > 0.9 && cross_row_R < 0.6)
            {
                Roundabout_flag = OUT_ROUND;
                element_flag = IN_JUGED;
            }
            break;
        case ROUND_R:
            cross_row_L = CrossRow_L(75);
            cross_row_R = CrossRow_R(75);
            if (cross_row_R > 0.9 && cross_row_L < 0.6)
            {
                Roundabout_flag = OUT_ROUND;
                element_flag = IN_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    case OUT_ROUND:
        switch (Roundabout_flag_position)
        {
        case ROUND_L:
            cross_row_L = CrossRow_L(75);
            if (cross_row_L < 0.7)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            break;
        case ROUND_R:
            cross_row_R = CrossRow_R(75);
            if (cross_row_R < 0.7)
            {
                Roundabout_flag = NO_ROUND;
                element_flag = NO_JUGED;
            }
            break;
        case NOROUND:
            break;
        }
        break;
    }
}

//**************************************************************
//* �������ƣ� T_Conner_Deal
//* ����˵���� T�䴦������
//* �������أ�
//* �� ע��
//**************************************************************
void T_Conner_Deal()
{
    float L_cross, R_cross, Col_cross;
    int star_row;
    Col_cross = Cross_col();
    star_row = (ROW - (int)ROW * Col_cross) + 5;
    if (star_row > 100) return;
    L_cross = CrossRow_L(star_row);
    R_cross = CrossRow_R(star_row);
    if (element_flag == IN_JUGED)
    {
        if (T_flag == NO_T) return;
    }
    if (T_flag == IN_T)
    {
        speed_flag = 75;
        if (Col_cross > 0.7 || R_lenth > 40 || L_lenth > 40)
        {
            T_flag = NO_T;
            //            if(T_go_flag_pin==1) T_go_flag_pin=0;
            //            else T_go_flag_pin=1;
            element_flag = NO_JUGED;
        }
        else
        {
            //gpio_set(FMQ,1);
            return;
        }
    }
    if ((left_apex.Apex_Row > 80 || right_apex.Apex_Row > 80) && Col_cross < 0.75 && T_flag == NO_T)
    {
        int row_min, row_max, row, col;
        int sum = 0;
        row_min = star_row - 10;
        row_max = star_row + 10;
        for (col = 5; col < COL - 5; col += 5)
        {
            for (row = row_max; row > row_min; row--)
            {
                if (row < 5) break;
                if (row > 115) row = 115;
                if (IMG_DATA[row][col] == WHITE && IMG_DATA[row - 1][col] == WHITE)
                {
                    if (IMG_DATA[row - 2][col] == BLACK && IMG_DATA[row - 3][col] == BLACK)
                    {
                        sum++;
                        break;
                    }
                }
            }
        }
        if (sum > 24)
        {
            element_flag = IN_JUGED;
            T_flag = IN_T;
            //gpio_set(FMQ,1);
        }
    }
}

//***************************************************************
//* �������ƣ� Fork_Deal
//* ����˵���� ���洦������
//* �������أ�
//* �� ע��
//***************************************************************
void Fork_Deal()
{
    int i, j, pin;
    int F_L_flag = 0;
    int F_R_flag = 0;
    int B_L, B_R, B_Top;
    B_L = Black_White_Zone(5, 80, 25, 110);
    B_Top = Black_White_Zone(60, 15, 115, 30);
    B_R = Black_White_Zone(170, 80, 185, 110);
    if (element_flag == IN_JUGED)
    {
        if (Fork_Flag == NO_FORK) return;
    }
    if (Fork_Flag == IN_FORK)
    {
        if (B_Top != 1)
        {
            fork_times++;
            Fork_Flag = NO_FORK;
            element_flag = NO_JUGED;
            return;
        }
    }
    if (B_Top != 1) return;
    for (i = 70; i > 40; i--)
    {
        int min = COL / 2 - 20, max = COL / 2 + 20;
        for (j = min; j < max; j++)
        {
            if (IMG_DATA[i][j] == WHITE && IMG_DATA[i][j + 1] == BLACK)
            {
                forck_L.Row[0] = i;
                forck_L.Col[0] = j + 1;
                F_L_flag = 1;
            }
            if (IMG_DATA[i][j] == BLACK && IMG_DATA[i][j + 1] == WHITE)
            {
                forck_R.Row[0] = i;
                forck_R.Col[0] = j - 1;
                F_R_flag = 1;
            }
        }
        if (F_L_flag == 0 && F_R_flag == 1)
        {
            return;
        }
        if (F_R_flag == 0 && F_L_flag == 1)
        {
            return;
        }
        if (forck_R.Row[0] < forck_L.Row[0]) return;//������2
        if (F_R_flag == 1 && F_L_flag == 1)
        {
            break;
        }
    }
    int find;
    int row, col;
    int colmin, colmax;
    //��Ϊ��
    pin = 1;
    for (row = forck_L.Row[0] - pin; pin < 15; pin++)
    {
        find = 0;
        row = forck_L.Row[0] - pin;
        colmin = forck_L.Col[pin - 1] - 10;
        colmax = forck_L.Col[pin - 1] + 10;
        for (col = colmin; col <= colmax; col++)
        {
            if (col < COL - 5 && col > 5)
            {
                if (IMG_DATA[row][col] == WHITE && IMG_DATA[row][col + 1] == WHITE)
                {
                    if (IMG_DATA[row][col + 2] == BLACK && IMG_DATA[row][col + 3] == BLACK)
                    {
                        forck_L.Row[pin] = row;
                        forck_L.Col[pin] = col + 2;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    //��Ϊ��
    pin = 1;
    for (row = ::right.Row[0] - pin; pin < 15; pin++)
    {
        find = 0;
        row = forck_R.Row[0] - pin;
        colmin = forck_R.Col[pin - 1] - 10;
        colmax = forck_R.Col[pin - 1] + 10;
        for (col = colmax; col >= colmin; col--)
        {
            if (col < COL - 5 && col > 5)
            {
                if (IMG_DATA[row][col] == WHITE && IMG_DATA[row][col - 1] == WHITE)
                {
                    if (IMG_DATA[row][col - 2] == BLACK && IMG_DATA[row][col - 3] == BLACK)
                    {
                        forck_R.Row[pin] = row;
                        forck_R.Col[pin] = col - 2;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    int V_L[2] = { 0 }, V_R[2] = { 0 };
    V_L[0] = forck_L.Row[4] - forck_L.Row[0];
    V_L[1] = forck_L.Col[4] - forck_L.Col[0];
    V_R[0] = forck_R.Row[4] - forck_R.Row[0];
    V_R[1] = forck_R.Col[4] - forck_R.Col[0];
    ang_l = atan(1.0 * V_L[0] / V_L[1]);
    ang_r = atan(1.0 * V_R[0] / V_R[1]);
    ang_l = ang_l * 180 / 3.14;
    ang_r = ang_r * 180 / 3.14 + 180;
    ang = ang_r - ang_l;
    if (ang > 150 && ang < 180)
    {
        float cross_r, cross_l;
        cross_r = CrossRow_R(forck_R.Row[0] - 2);
        cross_l = CrossRow_L(forck_L.Row[0] - 2);
        if (cross_r > 0.6 && cross_l > 0.6)
        {
            element_flag = IN_JUGED;
            Fork_Flag = IN_FORK;
        }
    }
}

//***************************************************************
//* �������ƣ� Garage_Deal
//* ����˵���� ���⺯��
//* �������أ�
//* �� ע��
//***************************************************************
void Garage_Deal()
{
    if (Garage_flag == GET_OUT)
    {
        element_flag = IN_JUGED;
        if (CorssCol > 0.7 || R_lenth > 40 || L_lenth > 40)
        {
            Garage_flag = NO_GARAGE;
            element_flag = NO_JUGED;
            fork_times = 0;
        }
        return;
    }
    else if (Garage_flag != GET_OUT)
    {
        if (element_flag == IN_JUGED) return;
        int row, col;
        int col_min, col_max;
        int T, T0, count, BM_count;
        T = 0, T0 = 0;
        count = 0;
        BM_count = 0;
        for (row = 110; row > 40; row -= 5)
        {
            col_min = COL / 2 - TrackWild[row] / 2;
            col_max = COL / 2 + TrackWild[row] / 2;
            for (col = col_min; col < col_max; col++)
            {
                if (IMG_DATA[row][col] == BLACK && IMG_DATA[row][col + 1] == BLACK && IMG_DATA[row][col + 2] == WHITE && IMG_DATA[row][col + 3] == WHITE)
                {
                    if (T - T0 > -5 && T - T0 < 5) count++;
                    T0 = T;
                    T = 0;
                }
                else
                {
                    T++;
                }
                if (count >= 5)
                {
                    count = 0;
                    BM_count++;
                    break;
                }
            }
        }
        if (BM_count >= 2 && Garage_flag == NO_GARAGE)
        {
            Garage_flag = FIRST;
            element_flag = NO_JUGED;
        }
        else if (BM_count == 0 && Garage_flag == FIRST)
            Garage_flag = READY;
        else if (BM_count >= 1 && Garage_flag == READY)
        {
            Garage_flag = GET_IN;
            element_flag = IN_JUGED;
        }
    }
}

//***************************************************************
//* �������ƣ� RacingLine_L
//* ����˵���� �����Ѱ���ף����Ǹ�ƫ(1/3)
//* �������أ�
//* �� ע��
//***************************************************************
void RacingLine_L(int k)
{
    int pin, i;
    int row, col;
    int flag;
    int mark;
    int l_flag, r_flag;
    l_flag = 0;
    r_flag = 0;
    i = 0;
    mark = L_lenth + (115 - ::left.Row[0]);
    if (mark > 240) mark = 240;
    row = ::left.Row[0];
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (::left.Row[i] == 254) break;
        if (flag > ::left.Row[0])
        {
            middleline[pin] = ::left.Col[0] + TrackWild[row] / k;
            col = middleline[pin];
            IMG_DATA[flag][col] = GREEN;
            mid_row[pin] = flag;
        }
        else
        {
            row = ::left.Row[i];
            col = ::left.Col[i] + TrackWild[row] / k;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }

    }
    return;
}

//***************************************************************
//* �������ƣ� RacingLine_R
//* ����˵���� �ұ���Ѱ���ף����Ǹ�ƫ(1/3)
//* �������أ�
//* �� ע��
//***************************************************************
void RacingLine_R(int k)
{
    int pin, i;
    int row, col;
    int flag;
    int mark;
    int l_flag, r_flag;
    l_flag = 0;
    r_flag = 0;
    i = 0;
    if (left_flag == 0 && right_flag == 0)
    {
        return;
    }
    mark = R_lenth + (115 - ::right.Row[0]);
    if (mark > 240) mark = 240;
    row = ::right.Row[0];
    for (pin = 0; pin < mark; pin++)
    {
        flag = 119 - pin;
        if (::right.Row[i] == 254) break;
        if (flag > ::right.Row[0])
        {
            middleline[pin] = ::right.Col[0] - TrackWild[row] / k;
            col = middleline[pin];
            mid_row[pin] = flag;
            IMG_DATA[flag][col] = GREEN;
        }
        else
        {
            row = ::right.Row[i];
            col = ::right.Col[i] - TrackWild[row] / k;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }
    return;
}

//***************************************************************
//* �������ƣ� Racing_Line_Fork_L
//* ����˵���� ����Ѳ�߲��ԣ�����Ѱ��
//* �������أ�
//* �� ע��
//***************************************************************
void Racing_Line_Fork_L()
{
    int pin, i;
    int row, col;
    int flag;
    int l_flag, r_flag;
    l_flag = 0;
    r_flag = 0;
    i = 0;
    row = forck_L.Row[0];
    
    if (right_apex.Apex_Col < 94)
    {
        RacingLine_R(2);
        return;
    }
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (forck_L.Row[i] == 254) break;
        if (flag > forck_L.Row[0])
        {
            middleline[pin] = forck_L.Col[0] - TrackWild[row] / 2;
            col = middleline[pin];
            mid_row[pin] = flag;
            IMG_DATA[flag][col] = GREEN;
        }
        else
        {
            row = forck_L.Row[i];
            col = forck_L.Col[i] - TrackWild[row] / 2;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }
    return;
}

//***************************************************************
//* �������ƣ� Racing_Line_Fork_R
//* ����˵���� ����Ѳ�߲��ԣ�����Ѱ��
//* �������أ�
//* �� ע��
//***************************************************************
void Racing_Line_Fork_R()
{
    int pin, i;
    int row, col;
    int flag;
    int l_flag, r_flag;
    l_flag = 0;
    r_flag = 0;
    i = 0;
    row = forck_R.Row[0];
    
    if (left_apex.Apex_Col > 94)
    {
        RacingLine_L(2);
        return;
    }
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (forck_R.Row[i] == 254) break;
        if (flag > forck_L.Row[0])
        {
            middleline[pin] = forck_R.Col[0] + TrackWild[row] / 2;
            col = middleline[pin];
            mid_row[pin] = flag;
            IMG_DATA[flag][col] = GREEN;
        }
        else
        {
            row = forck_R.Row[i];
            col = forck_R.Col[i] + TrackWild[row] / 2;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }
    return;
}

//**************************************************************
//* �������ƣ� Raceing_line_T_L
//* ����˵���� T���ߣ�����
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_T_L()
{
    int i, pin;
    if (left_flag == 1)
    {
        RacingLine_L(10);
    }
    else
    {
        right_flag = 1;
        left_flag = 1;
        for (i = 120, pin = 0; i > 60; i--)
        {
            middleline[pin] = 0;
            mid_row[pin] = i;
            pin++;
        }
    }
}

//**************************************************************
//* �������ƣ� Raceing_line_T_R
//* ����˵���� T��������
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_T_R()
{
    int i, pin;
    if (right_flag == 1)
    {
        RacingLine_R(10);
    }
    else
    {
        right_flag = 1;
        left_flag = 1;
        for (i = 120, pin = 0; i > 60; i--)
        {
            middleline[pin] = COL;
            mid_row[pin] = i;
            pin++;
        }
    }
}

//**************************************************************
//* �������ƣ� Raceing_line_G_L
//* ����˵���� ���복�⣨����
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_G_L()
{
    int i, pin;
    //    if(Garage_flag==GET_IN);
    if (left_flag == 0 || Garage_flag == GET_IN)
    {
        right_flag = 1;
        left_flag = 1;
        for (i = 120, pin = 0; i > 60; i--)
        {
            middleline[pin] = 0;
            mid_row[pin] = i;
            pin++;
        }
    }
}

//**************************************************************
//* �������ƣ� Raceing_line_G_R
//* ����˵���� ���복�⣨���ң�
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_G_R()
{
    int i, pin;
    if (Garage_flag == GET_IN)
    {
        speed_flag = 60;
    }
    if (right_flag == 0 || Garage_flag == GET_IN)
    {
        right_flag = 1;
        left_flag = 1;
        for (i = 120, pin = 0; i > 60; i--)
        {
            middleline[pin] = 188;
            mid_row[pin] = i;
            pin++;
        }
    }
}
//**************************************************************
//* �������ƣ� Raceing_line_RoundIn_L
//* ����˵���� �󻷵����벹��
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_RoundIn_L()
{
    int pin, pinn;//, i, j;
    int row, col, find;
    int colmin, colmax;
    int flag, i;
    pinn = 0;
    i = 0;
    if (Round_L.Row[0] == 254)
    {
        return;
    }
    for (pin = 1, row = Round_L.Row[0] - pin; pin < 240; pin++)
    {
        find = 0;
        row = Round_L.Row[0] - pin;
        if (row <= 15)break;
        colmin = Round_L.Col[pin - 1] - 10;
        colmax = Round_L.Col[pin - 1] + 10;
        for (col = colmax; col >= colmin; col--)
        {
            if (col > COL - 5) col = COL - 5;
            if (col > 5)
            {
                if (IMG_DATA[row][col] == BLACK && IMG_DATA[row][col - 1] == BLACK)
                {
                    if (IMG_DATA[row][col - 2] == WHITE && IMG_DATA[row][col - 3] == WHITE)
                    {
                        Round_L.Row[pin] = row;
                        Round_L.Col[pin] = col - 1;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    row = Round_L.Row[0];
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (flag > Round_L.Row[0])
        {
            if (::right.Col[pinn] != 254)
            {
                middleline[pin] = Round_L.Col[0] - TrackWild[row] / 2;
                col = middleline[pin];
                mid_row[pin] = ::right.Row[pinn];
                IMG_DATA[flag][col] = GREEN;
                pinn++;
            }
        }
        else
        {
            if (Round_L.Row[i] == 254) break;
            row = Round_L.Row[i];
            col = Round_L.Col[i] - TrackWild[row] / 2;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }
    return;
}

//**************************************************************
//* �������ƣ� Raceing_line_RoundIn_R
//* ����˵���� �һ������벹��
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_RoundIn_R()
{
    int pin, pinn;//, i, j;
    int row, col, find;
    int colmin, colmax;
    int flag, i;
    i = 0;
    pinn = 0;
    if (Round_R.Row[0] == 254)
    {
        return;
    }
    pin = 1;
    for (row = Round_R.Row[0] - pin; pin < 240; pin++)
    {
        find = 0;
        row = Round_R.Row[0] - pin;
        if (row <= 15)break;
        colmin = Round_R.Col[pin - 1] - 10;
        colmax = Round_R.Col[pin - 1] + 10;
        for (col = colmin; col <= colmax; col++)
        {
            if (col < 5) col = 5;
            if (col < COL - 5)
            {
                if (IMG_DATA[row][col] == BLACK && IMG_DATA[row][col + 1] == BLACK)
                {
                    if (IMG_DATA[row][col + 2] == WHITE && IMG_DATA[row][col + 3] == WHITE)
                    {
                        Round_R.Row[pin] = row;
                        Round_R.Col[pin] = col + 1;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if (find == 0)
        {
            break;
        }
    }
    row = Round_R.Row[0];
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (flag > Round_R.Row[0])
        {
            if (::left.Col[pinn] != 254)
            {
                middleline[pin] = Round_R.Col[0] + TrackWild[row];
                col = middleline[pin];
                mid_row[pin] = ::left.Row[pinn];
                IMG_DATA[flag][col] = GREEN;
                pinn++;
            }
        }
        else
        {
            if (Round_R.Row[i] == 254) break;
            row = Round_R.Row[i];
            col = Round_R.Col[i] + TrackWild[row];
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }
    return;
}

//**************************************************************
//* �������ƣ� Raceing_line_RoundIn_L
//* ����˵���� �󻷵��뿪����
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_RoundOUT_L()
{
    int i, pin;
    if (right_flag == 1)
    {
        RacingLine_R(2);
        return;
    }
    for (i = 120, pin = 0; i > 60; i--)
    {
        middleline[pin] = 0;
        mid_row[pin] = i;
        pin++;
    }
    return;
}

//**************************************************************
//* �������ƣ� Raceing_line_RoundIn_L
//* ����˵���� �һ����뿪����
//* �������أ�
//* �� ע��
//**************************************************************
void Raceing_line_RoundOUT_R()
{
    int i, pin;
    if (left_flag == 1)
    {
        RacingLine_L(2);
        return;
    }
    for (i = 120, pin = 0; i > 60; i--)
    {
        middleline[pin] = 188;
        mid_row[pin] = i;
        pin++;
    }
    return;
}

void Raceing_line_RoundIn_L_Blan_B()
{
    int row, col, pin, find, i;
    int col_min, col_max, flag;
    int star_row, star_col, star_flag;
    int max_row, max_col;
    float k;
    find = 0; max_row = 0; max_col = 0;
    i = 1;
    col_min = left_apex.Apex_Col + 1;
    col_max = right_apex.Apex_Col;
    for (col = col_max; col > col_min; col--)
    {
        if (Roundabout_flag == IN_ROUND)
        {
            max_row = ::right.Row[0];
            max_col = ::right.Col[0];
            break;
        }
        if (col < 5) break;
        if (left_apex.Apex_Row < right_apex.Apex_Row + 20 && ::left.Row[0] < 100)
        {
            max_row = ::left.Row[0];
            max_col = ::left.Col[0];
            break;
        }
        for (row = left_end_row - 5; row > 5; row--)
        {
            if (IMG_DATA[row][col] == WHITE && IMG_DATA[row - 1][col] == WHITE)
            {
                if (IMG_DATA[row - 2][col] == BLACK && IMG_DATA[row - 3][col] == BLACK)
                {
                    if (row > max_row)
                    {
                        max_row = row;
                        max_col = col;
                    }
                    break;
                }
            }
        }
    }//�յ�
    Round_L.Row[0] = max_row;
    Round_L.Col[0] = max_col;
    star_row = 115;
    star_col = 180;
    for (pin = 0; pin < 240; pin++)
    {
        if (Roundabout_flag == IN_ROUND)
        {
            star_row = 115;
            star_col = 180;
            break;
        }
        if (::right.Row[pin] == 254) break;
        if (left_apex.Apex_Row < right_apex.Apex_Row + 20 && ::left.Row[0] < 100)
        {
            star_row = 115;
            star_col = 180;
            break;
        }
        if (::right.Row[pin] <= left_apex.Apex_Row)
        {
            star_row = ::right.Row[pin];
            star_col = ::right.Col[pin];
            star_flag = pin;
            break;
        }
    }//���
    IMG_DATA[star_row][star_col] = GREEN;
    k = (star_col * 1.0 - Round_L.Col[0]) / (star_row * 1.0 - Round_L.Row[0]);
    row = star_row;
    col = star_col;
    for (pin = 1; pin < 240; pin++)
    {
        if (star_row - pin >= Round_L.Row[0])
        {
            Round_L.Row[pin] = star_row - pin;
            Round_L.Col[pin] = star_col - (int)pin * k;
            IMG_DATA[Round_L.Row[pin]][Round_L.Col[pin]] = BLUE;
        }
        else if (Roundabout_flag == IN_ROUND)
        {
            if (::right.Row[pin] == 254) break;
            Round_L.Row[pin] = ::right.Row[pin];
            Round_L.Col[pin] = ::right.Col[pin];
        }
        else
        {
            find = 0;
            row = star_row - pin;
            if (row <= 15)break;
            col_min = Round_L.Col[pin - 1] - 10;
            col_max = Round_L.Col[pin - 1] + 10;
            for (col = col_max; col >= col_min; col--)
            {
                if (col > COL - 5) col = COL - 5;
                if (col > 5)
                {
                    if (IMG_DATA[row][col] == BLACK && IMG_DATA[row][col - 1] == BLACK)
                    {
                        if (IMG_DATA[row][col - 2] == WHITE && IMG_DATA[row][col - 3] == WHITE)
                        {
                            Round_L.Row[pin] = row;
                            Round_L.Col[pin] = col - 1;
                            if (pin > 1) IMG_DATA[Round_L.Row[pin]][Round_L.Col[pin]] = BLUE;
                            find = 1;
                            break;
                        }
                    }
                }
            }
            if (find == 0)
            {
                break;
            }
        }
    }//����
    row = Round_L.Row[1];
    i = 1;
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (Round_L.Row[i] == 254) break;
        if (flag > Round_L.Row[1])
        {
            middleline[pin] = Round_L.Col[1] - TrackWild[row] / 2;
            col = middleline[pin];
            mid_row[pin] = flag;
            IMG_DATA[flag][col] = GREEN;
        }
        else
        {
            row = Round_L.Row[i];
            col = Round_L.Col[i] - TrackWild[row] / 2;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }//Ѳ��
}

void Raceing_line_RoundIn_R_Blan_B()
{
    int row, col, pin, find, i;
    int col_min, col_max, flag;
    int star_row, star_col, star_flag;
    int max_row, max_col;
    float k;
    find = 0; max_row = 0; max_col = 188;
    i = 1;
    col_min = left_apex.Apex_Col;
    col_max = right_apex.Apex_Col - 1;
    for (col = col_min; col < col_max; col++)
    {
        if (col > COL - 5) break;
        if (left_apex.Apex_Row + 20 > right_apex.Apex_Row && ::right.Row[0] < 100)
        {
            max_row = ::right.Row[0];
            max_col = ::right.Col[0];
            break;
        }
        for (row = right_end_row - 5; row > 5; row--)
        {
            if (IMG_DATA[row][col] == WHITE && IMG_DATA[row - 1][col] == WHITE)
            {
                if (IMG_DATA[row - 2][col] == BLACK && IMG_DATA[row - 3][col] == BLACK)
                {
                    if (row > max_row)
                    {
                        max_row = row;
                        max_col = col;
                    }
                    break;
                }
            }
        }
    }//�յ�
    Round_R.Row[0] = max_row;
    Round_R.Col[0] = max_col;
    round_row_flag = max_row;
    star_row = 115;
    star_col = 8;
    for (pin = 0; pin < 240; pin++)
    {
        if (::left.Row[pin] == 254) break;
        if (left_apex.Apex_Row + 20 > right_apex.Apex_Row && ::right.Row[0] < 100)
        {
            star_row = 115;
            star_col = 8;
            break;
        }
        if (::left.Row[pin] == right_apex.Apex_Row)
        {
            star_row = ::left.Row[pin];
            star_col = ::left.Col[pin];
            star_flag = pin;
            break;
        }
    }//���
    IMG_DATA[star_row][star_col] = GREEN;
    k = (star_col * 1.0 - Round_R.Col[0]) / (star_row * 1.0 - Round_R.Row[0]);
    row = star_row;
    col = star_col;
    for (pin = 1; pin < 240; pin++)
    {
        if (star_row - pin >= Round_R.Row[0])
        {
            Round_R.Row[pin] = star_row - pin;
            Round_R.Col[pin] = star_col - (int)pin * k;
            IMG_DATA[Round_R.Row[pin]][Round_R.Col[pin]] = BLUE;
        }
        else
        {
            find = 0;
            row = star_row - pin;
            if (row <= 15)break;
            col_min = Round_R.Col[pin - 1] - 10;
            col_max = Round_R.Col[pin - 1] + 10;
            for (col = col_min; col <= col_max; col++)
            {
                if (col < 5) col = 5;
                if (col < COL - 5)
                {
                    if (IMG_DATA[row][col] == BLACK && IMG_DATA[row][col + 1] == BLACK)
                    {
                        if (IMG_DATA[row][col + 2] == WHITE && IMG_DATA[row][col + 3] == WHITE)
                        {
                            Round_R.Row[pin] = row;
                            Round_R.Col[pin] = col + 1;
                            if (pin > 1) IMG_DATA[Round_R.Row[pin]][Round_R.Col[pin]] = BLUE;
                            find = 1;
                            break;
                        }
                    }
                }
            }
            if (find == 0)
            {
                break;
            }
        }
    }//����
    row = Round_R.Row[1];
    i = 1;
    for (pin = 0; pin < 240; pin++)
    {
        flag = 119 - pin;
        if (Round_R.Row[i] == 254) break;
        if (flag > Round_R.Row[1])
        {
            middleline[pin] = Round_R.Col[1] + TrackWild[row] / 2;
            col = middleline[pin];
            mid_row[pin] = flag;
            IMG_DATA[flag][col] = GREEN;
        }
        else
        {
            row = Round_R.Row[i];
            col = Round_R.Col[i] + TrackWild[row] / 2;
            middleline[pin] = col;
            mid_row[pin] = row;
            i++;
            if (row > 5 && row < ROW - 5)
            {
                if (col > 5 && col < COL - 5)
                {
                    IMG_DATA[row][col] = GREEN;
                }
            }
        }
    }//Ѳ��
}

void RaceingLine_ramp_Plan_B()
{
    int pin, row, col, i;
    int mark_flag = 0;;
    if (left_flag == 0 || right_flag == 0)
    {
        Racing_Line();
        middleline[60] = 254;
        mid_row[60] = 254;
    }
    else
    {
        if (::left.Row[0] < ::right.Row[0])
        {
            for (pin = 0; pin < 240; pin++)
                if (::right.Row[pin] == ::left.Row[0])
                    mark_flag = pin;
            row = 119; i = 0;
            for (pin = 0; pin < 240; pin++)
            {
                if (row <= 60) break;
                if (row > ::left.Row[0])
                {
                    middleline[pin] = (::left.Col[0] + ::right.Col[mark_flag]) / 2;
                    col = middleline[pin];
                    mid_row[pin] = row;
                    row--;
                    if (row > 5 && row < ROW - 5)
                    {
                        if (col > 5 && col < COL - 5)
                        {
                            IMG_DATA[row][col] = GREEN;
                        }
                    }
                }
                else
                {
                    if (::left.Col[i] == 254 || ::right.Col[mark_flag + i] == 254) break;
                    middleline[pin] = (::left.Col[i] + ::right.Col[mark_flag + i]) / 2;
                    col = middleline[pin];
                    mid_row[pin] = row;
                    row--;
                    i++;
                    if (row > 5 && row < ROW - 5)
                    {
                        if (col > 5 && col < COL - 5)
                        {
                            IMG_DATA[row][col] = GREEN;
                        }
                    }
                }
            }
        }
        else
        {
            for (pin = 0; pin < 240; pin++)
                if (::left.Row[pin] == ::right.Row[0])
                    mark_flag = pin;
            row = 119; i = 0;
            for (pin = 0; pin < 240; pin++)
            {
                if (row <= 60) break;
                if (row > ::right.Row[0])
                {
                    middleline[pin] = (::right.Col[0] + ::left.Col[mark_flag]) / 2;
                    col = middleline[pin];
                    mid_row[pin] = row;
                    row--;
                    if (row > 5 && row < ROW - 5)
                    {
                        if (col > 5 && col < COL - 5)
                        {
                            IMG_DATA[row][col] = GREEN;
                        }
                    }
                }
                else
                {
                    if (::right.Col[i] == 254 || ::left.Col[mark_flag + i] == 254) break;
                    middleline[pin] = (::right.Col[i] + ::left.Col[mark_flag + i]) / 2;
                    col = middleline[pin];
                    mid_row[pin] = row;
                    row--;
                    i++;
                    if (row > 5 && row < ROW - 5)
                    {
                        if (col > 5 && col < COL - 5)
                        {
                            IMG_DATA[row][col] = GREEN;
                        }
                    }
                }
            }
        }
    }
}

void RaceLine()
{
    if (Fork_Flag == IN_FORK)
    {
        if (fork_turn == 1)
            Racing_Line_Fork_L();
        else if (fork_turn == 2)
            Racing_Line_Fork_R();
    }
    else if (T_flag == IN_T)
    {
        if (T_go_flag[T_go_flag_pin] == 0)
        {
            fork_times = 2;
            Raceing_line_T_R();//������Ҫ���Ӷ����жϣ������������
        }
        else if (T_go_flag[T_go_flag_pin])
            Raceing_line_T_L();
    }
    else if (Garage_flag == GET_OUT) Raceing_line_G_R();
    else if (Garage_flag == GET_IN) Raceing_line_G_R();
    else if (Roundabout_flag == GET_IN_ROUND)
    {
        if (Roundabout_flag_position == ROUND_L)
            Raceing_line_RoundIn_L_Blan_B();
        else if (Roundabout_flag_position == ROUND_R)
            Raceing_line_RoundIn_R_Blan_B();
    }
    else if (Roundabout_flag == IN_ROUND)
    {
        if (Roundabout_flag_position == ROUND_L)
        {
            if (::right.Col[0] < 140)
                RacingLine_R(2);
            else
                Racing_Line();
        }
        else if (Roundabout_flag_position == ROUND_R)
        {
            if (::left.Col[0] > 48)
                RacingLine_L(2);
            else
                Racing_Line();
        }
    }
    else if (Roundabout_flag == GET_OUT_ROUND)
    {
        if (Roundabout_flag_position == ROUND_L)
            Raceing_line_G_L();
        else if (Roundabout_flag_position == ROUND_R)
            Raceing_line_G_R();
    }
    else if (Roundabout_flag == OUTTING)
    {
        if (Roundabout_flag_position == ROUND_L)
            Raceing_line_G_L();
        else if (Roundabout_flag_position == ROUND_R)
            Raceing_line_G_R();
    }
    else if (Roundabout_flag == OUT_ROUND)
    {
        if (Roundabout_flag_position == ROUND_L)
            RacingLine_R(2);
        else if (Roundabout_flag_position == ROUND_R)
            RacingLine_L(2);
    }
    else if (ramp_flag == IN_RAMP) RaceingLine_ramp_Plan_B();
    else Racing_Line();
}

void ImageDeal_main()
{
    Deal_Init();
    InitData();
    CorssCol = Cross_col();
    if (CorssCol < 0.1) CorssCol = 0.1;
    protect();
    LeftStartFind();
    RightStartFind();
    fixpoint();
    left_jump();
    right_jump();
    Left_Apex();
    Right_Apex();
    Garage_Deal();
    Fork_Deal();
    Roundabout_Deal();
    RaceLine();
}