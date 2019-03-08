#pragma once
#include "onedll.h"

/*****************ãÐÖµ·Ö¸î*********************/
void DrawRectangle(Mat& img, Rect box);
void onMouse(int event, int x, int y, int flags, void *param);
void onMouse1(int event, int x, int y, int flags, void *param);
void changeIOUthreshod(Rect rect);
void getBorderThreshold(float f1, Mat &maskChangeGradietRCorner);
void getShrinkThreshold(float f3, Mat &maskChangeGradientDrawIn);
void initChangePartOfTheRegionalGradient(Mat ImgIn, Mat bordertft, Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, CanShuMask canshumask);
// Mat &imgIN ,Mat &imgOut
void ChangePartOfTheRegionalGradient(Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, Mat &imgIn, Mat &imgOut, float f1, float f2);