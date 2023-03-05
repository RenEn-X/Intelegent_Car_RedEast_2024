#pragma once
#ifndef IMG_DEAL_
#define IMG_DEAL_

#include <stdint.h>
#include "load.h"
using namespace std;

typedef enum
{
    NO_JUGED,
    IN_JUGED,
}ALL_enum;//所有元素标志位

typedef enum
{
    NO_FORK,
    IN_FORK,
}FORK_enum;//三叉

typedef enum
{
    NO_T,
    IN_T,
    T_READY,
}T_CONNER_enum;//T弯

typedef enum
{
    GET_OUT,
    FIRST,
    READY,
    GET_IN,
    NO_GARAGE,
}Garage_enmum;//车库

typedef enum
{
    ROUND_READY,
    GET_IN_READY,
    GET_IN_ROUND,
    IN_ROUND,
    GET_OUT_ROUND,
    OUTTING,
    OUT_WAY,
    OUT_ROUND,
    NO_ROUND,
}Roundabout_enmum;//环岛

typedef enum
{
    ROUND_L,
    ROUND_R,
    NOROUND,
}Roundabout_position_enmum;//环岛

typedef enum
{
    NO_RAMP,
    RAMP_READY,
    IN_RAMP,
}Ramp_enmum;//环岛


#define WHITE    255      
#define BLACK    0        
#define RED      128
#define GREEN    100
#define BLUE     254

void ImageDeal_main();
void reset_staus();

#endif // !IMG_DEAL_
