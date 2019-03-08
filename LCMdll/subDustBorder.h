#pragma once
#pragma once
#include "LCMdll.h"
#include "subDustBorder.h"

void subDustBorder(Mat &imgIn, Mat &imgOut, vector<vector<Point>>&contours, int cameraFlag, int imgFlag);

void subDustBorder_mura(Mat &imgIn, Mat &imgOut, vector<vector<Point>>&contours, int cameraFlag, int imgFlag);

void subDustBorder_louguang(Mat &imgIn, Mat &imgOut, vector<vector<Point>>&contours);

void subDustBorder_subTmp(Mat &imgIn, Mat &imgOut, vector<vector<Point>>&contours1, int imgFlag);

void subDustBorder_BGRsub(Mat &imgIn, Mat &imgOut, vector<vector<Point>> &contours, int imgFlag);