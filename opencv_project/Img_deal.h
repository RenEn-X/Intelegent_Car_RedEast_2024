#pragma once
#ifndef IMG_DEAL_
#define IMG_DEAL_

#include <stdint.h>
#include "load.h"
using namespace std;


#define WHITE    255      
#define BLACK    0        
#define RED      128
#define GREEN    100
#define BLUE     254

#define RESULT_ROW 120 // 结果图行列
#define RESULT_COL 188
#define USED_ROW 120 // 用于透视图的行列
#define USED_COL 188
extern uint8_t PerImg[RESULT_ROW][RESULT_COL];
void ImagePerspective_Init(void);
void ImageDeal_main(void);
void reset_staus(void);

#endif // !IMG_DEAL_
