#pragma once
#pragma once
#include "LCMdll.h"
//梯度异常点计数
//===================hyw & lyl=================
bool count_gradient(RotatedRect rect, CanShureMoveFalse *canshuMoveFalse);

//bool border_count_gradient(RotatedRect rect);
void count_information_img(Mat &imgInTFT, Mat &imgInGradient, int cameraFlag, int flag);
//1.4 lyl update 添加flag 如果flag = true则显示瑕疵信息，false则隐藏瑕疵的具体信息
bool count_gradient1(Mat &gradientIn, Mat &TftIn, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);
//bool count_gradient1(Mat &gradientIn, Mat &TftIn, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int imgFlag, bool flag ,  defectInformation  *Info);

//保存各个屏幕图像统计信息
/*
double imgInTFTMeans[20];
double imgInTFTStddevs[20];
double 	imgInGradientMeans[20];
double imgInGradientStddevs[20];
*/

//0118 add lyl
bool count_gradient_only(Mat &gradientIn, Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);

//只统计灰度信息，利用灰度信息进行筛选
bool count_gray_only(Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);