#pragma once
#include "LCM.h"
#include "removeFalseDetect.h"


//12.18 lyl update �ӿ�
//void removePointFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeMuraFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeLouguangFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *CanShureMoveFalse);
//void removeSubTmpFalse1(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse);

//1.4 lyl update ���flag ���flag = true����ʾ覴���Ϣ��false������覴õľ�����Ϣ
void removePointFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse,int cameraFlag,int imgFlag, bool flag  , vector<defectInformation> *defect);
void removeMuraFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);
void removeLouguangFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag , int imgFlag, bool flag, vector<defectInformation> *defect);
void removeSubTmpFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);

//0118 add ֻ�����ݶ�ͳ��ֵȥɸѡ覴õĺ���
void removeGradientFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect ,  int DefectType);


void removeGrayFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut,  vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag,int imgFlag, bool flag, vector<defectInformation> *defect);