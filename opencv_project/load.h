#pragma once
#ifndef LOAD_H_
#define LOAD_H_

#include <opencv2/opencv.hpp>
#include "Img_deal.h"

#define IMG_ROW 120
#define IMG_COL 188
extern uint8_t Imgdata[IMG_ROW][IMG_COL];
extern uint8_t img_error;
extern cv::String folder;

void Imgread_Mat2Buffer(cv::String addr);
void Imshow_Buffer2Mat();
void Read_folder(cv::String folder_addr, int FramTime);

typedef enum
{
	PLAYING,
	PAUSE,
	NEXT,
	LAST,
}play_enum;

#endif // !LOAD_H_

