#pragma once
#pragma once
#include "LCMdll.h"
//�ݶ��쳣�����
//===================hyw & lyl=================
bool count_gradient(RotatedRect rect, CanShureMoveFalse *canshuMoveFalse);

//bool border_count_gradient(RotatedRect rect);
void count_information_img(Mat &imgInTFT, Mat &imgInGradient, int cameraFlag, int flag);
//1.4 lyl update ���flag ���flag = true����ʾ覴���Ϣ��false������覴õľ�����Ϣ
bool count_gradient1(Mat &gradientIn, Mat &TftIn, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);
//bool count_gradient1(Mat &gradientIn, Mat &TftIn, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int imgFlag, bool flag ,  defectInformation  *Info);

//���������Ļͼ��ͳ����Ϣ
/*
double imgInTFTMeans[20];
double imgInTFTStddevs[20];
double 	imgInGradientMeans[20];
double imgInGradientStddevs[20];
*/

//0118 add lyl
bool count_gradient_only(Mat &gradientIn, Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);

//ֻͳ�ƻҶ���Ϣ�����ûҶ���Ϣ����ɸѡ
bool count_gray_only(Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);