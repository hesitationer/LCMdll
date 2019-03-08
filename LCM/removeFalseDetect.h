#pragma once
#include "LCM.h"
#include "removeFalseDetect.h"


//12.18 lyl update 接口
//void removePointFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeMuraFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeLouguangFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeSubTmpFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse);

//1.4 lyl update 添加flag 如果flag = true则显示瑕疵信息，false则隐藏瑕疵的具体信息
void removePointFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse,int cameraFlag,int imgFlag, bool flag  , vector<defectInformation> *defect);
void removeMuraFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);
void removeLouguangFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag , int imgFlag, bool flag, vector<defectInformation> *defect);
void removeSubTmpFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);

//0118 add 只利用梯度统计值去筛选瑕疵的函数
void removeGradientFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect ,  int DefectType);


void removeGrayFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut,  vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);