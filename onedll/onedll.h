#pragma once
// dustGetYu.cpp: 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <string>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件
#include <windows.h>
#include "opencv\cv.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include<cmath>
#include <thread>
#include <typeinfo>
#include <time.h>
#include <math.h>

using namespace cv;
using namespace std;


struct defectInformation {

	int  s_cx = 0;
	int s_cy = 0;
	int w = 0;
	int h = 0;
	int S = 0;
	int numGray = 0;
	int numGradient = 0;
	double dst_m_Gradient = 0.0;
	double dst_m_Gray = 0.0;
	double dst_s_Gray = 0.0;
	double dst_s_Gradient = 0.0;
	bool TrueOrFalse = false;
	int bright_num = 0;
	int dark_num = 0;
	Point2f P[4];

	bool transYorN = false;

	int typenum;
	string type;
};

//struct defectInformations {
//	vector<defectInformation> xxx(9);
//};
/***************************************************参数-类****************************************************************/
class sendImg

{

private: //成员访问限制符



public: //public设置之前的成员都是上面所设置的私有的

	Mat imgTFT[10];             //最多10台相机同时工作
	Mat imgTFT_gray;
	Mat imgTFTpoint_gray[10];
	Mat imgTFTmura_gray[10];
	Mat imgTFTlouguang_gray;
	Mat TFTb;
	Mat TFTg;
	Mat TFTr;
	Mat imgDust;
	Mat BorderTFT;
	Mat gradientTFT;
	Mat gradientTFT_point;
	Mat gradientTFT_pointChange;    //point的经过区域调整后的梯度图
	Mat gradientTFT_muras[10][20];
	Mat gradientTFTb_muras[20];
	Mat gradientTFTg_muras[20];
	Mat gradientTFTr_muras[20];
	Mat gradientTFTbgr_muras[10][20];
	Mat gradientTFTb_point[20];
	Mat gradientTFTg_point[20];
	Mat gradientTFTr_point[20];
	Mat gradientTFTbgr_point[10][20];

	Mat gradientTFT_muraChange;     //mura的经过区域调整后的梯度图
	Mat gradientTFT_louguang;
	Mat gradientTFT_louguangChange;  //漏光的经过区域调整后的梯度图
	Mat subtractTemplateTftImg[10];


	Mat gradientTFTb;
	Mat gradientTFTg;
	Mat gradientTFTr;
	Mat gradientTFT_or;
	Mat gradientTtfMaskValue;
	Mat gradientDUST;
	Mat imgDustColorCut;
	Mat imgTftColorCut[10];
	Mat imgTftColorCuts[10][20];
	Mat CutTemplate_fill[10][20];  //add
	Mat GradientTemplate_fill[10][20];  //add


	Mat imgTftColorCutFill;
	Mat imgTftColorCutFills[10][20];
	Mat imgTftColor_point[10];
	Mat imgTftColor_mura[10];
	Mat imgTftColor_louguang[10];
	Mat img_border;
	Mat img_center;
	Mat img_border_gradient;
	Mat img_border_dust;
	Mat img_center_gradient;
	Mat img_center_dust;
	bool useCameras = false;   //False单相机，true多相机
	Mat dust_points;
	Mat  imgTEMP;
	Mat gradientTftThreshold;
	Mat gradientTftThreshold_point;
	Mat gradientTftThreshold_mura;
	Mat gradientTftThreshold_louguang;
	Mat gradientDustThreshold;
	Mat showDustImg;
	Mat deteceResult;
	Mat gradientDustThresholDilate;
	Mat gradientResult;
	Mat gradientResult_point;
	Mat gradientResult_mura;
	Mat gradientResult_louguang;
	Mat gradientResultThreshold;
	Mat gradientResultThreshold_points[10][20];
	Mat gradientResultThreshold_muras[10][20];
	Mat gradientResultThreshold_louguangs[10][20];
	Mat gradientResultThreshold_subTeps[10][20];
	Mat gradientResultThreshold_all[10][20];


	Mat tftResultShow;
	Mat tftResultShow_point;
	Mat tftResultShow_mura;
	Mat tftResultShow_louguang;
	Mat tftResultShow_subTmp;
	Mat tftResultShow_gradient[10][20][4];
	Mat tftResultShow_gray[20];
	Mat tftResultShow_all[20];

	Mat tftResultShow_final;
	string projectDir;
	string sNowTimeFileDir;
	Mat gradient_dst;
	//RotatedRect rect[10];

	Mat mask1;
	Mat mask2;

	int  shrink_size = 4;   //上下左右边缘忽略尺寸
	int  shrink_size_mura = 15;   //上下左右边缘忽略尺寸  针对团的size比较大取8  //8 before
	int  shrink_size_point = 4;   //上下左右边缘忽略尺寸  针对点和线的size比较小取4
	int  shrink_size_louguang = 4;   //上下左右边缘忽略尺寸  针对漏光的size比较小取4
	int  shrink_size_subTmp = 12;   //上下左右边缘忽略尺寸  针对漏光的size比较小取4
	Mat BorderTftDilate;

	int Max_x;
	int Max_y;

	//ROI
	//get_img_rotate_info计算手机屏幕区域最小旋转外接矩形信息，包括角度，坐标    
	RotatedRect get_rotate_rect[10];   // [10]存储最多十个相机的数据
	Point2f box[10][4];

	vector<Point2f> pts1[10];  //由白屏计算出的屏幕梯形四个角点坐标, 增加[10]最多10个相机
	vector<Point2f>pts1_tmp;  //模板图的四个角点坐标
	string pts1_tmp_file; // = projectDir + "R_tmmp//pts1_tmp.txt";  //模板图的四个角点坐标

	//给定的统一矩形四个角点坐标，即透视变换中的pts2
	vector<Point2f> WhiteScreenQuadrilateral;

	//透视变换的 长和宽 Size(2315, 1115));
	int PersPectiveTransformation_weight = 2315;
	int PersPectiveTransformation_hight = 1115;


	// R角
	vector<string> res;
	string res_tmp_dir; //= projectDir + "R_tmmp//";
	Mat tmp;
	Mat R_tmp;
	int R_compare_res[4] = { 0,0,0,0 }; //R角检测结果


	vector<vector<Point>>  contours3;
	vector<vector<Point>>  contours;
	vector<vector<Point>>  contours_points[10][20];
	vector<vector<Point>>  contours_muras[10][20];
	vector<vector<Point>>  contours_louguangs[10][20];
	vector<vector<Point>>  contours_subTmps[10][20];
	vector<vector<Point>>  contours_all[10][20];

	Mat GradientResult[20];
	Mat GradientResult_point[10][20];
	Mat GradientResult_mura[20];
	Mat GradientResult_louguang[10][20];
	Mat subtractTemplateTftImgs[10][20];
	Mat GradientResult_or[20];
	Mat GradientResult_final;
	Mat Template[10][20];    //最多10个相机，每个相机20张图片
	Mat CutTemplate[10][20];

	int h; //roi区域的高
	int w;  //roi区域的宽

	int new_x;
	int new_y;

	Mat img_detect[20];


	int  p_result;
	int  l_result;


	int MaxFlag[10] = { 0,0,0,0,0,0,0,0,0,0 };    //每个相机检测了多少张图片
	int Flag_White = 0;
	int Flag_Green = 0;
	int Flag_Yellow = 0;
	int Flag_Blue = 0;



	//检测结果相关
	vector<vector<Point>> pointResults[10][20];
	vector<vector<Point>> muraResults[10][20];
	vector<vector<Point>> louguangResults[10][20];
	vector<vector<Point>> subTmpResults[10][20];




	//**************分区域阈值中间图片*****************
	Mat plusResult;    //边界阈值缩小后的结果图片
	Mat ResultBorderinvgradTFT; //保留了中间部分梯度的图片
	Mat BorderTFTthreshold;
	Mat ResultBorderthreshgradTFT; //保留了边缘梯度的图片

								   //左右内缩
	int  shrink_x = 3;
	int  shrink_y = 6;
	Point2f box_TFT[4];
	Mat  OuterBorderTFTImg;

	//鼠标标定
	Mat getMouseChooseimg;
	Rect rect_getIOU;
	Mat ResultMouse;

	Mat onMouseResultShow;

	//保存的区域梯度调整mask图片

	//R角Mask
	Mat PointMaskChangeGradietRCorner;      //调整梯度的R角区域模板  
	Mat MuraMaskChangeGradietRCorner;      //调整梯度的R角区域模板  
	Mat LightMaskChangeGradietRCorner;      //调整梯度的R角区域模板  


	//缩进Mask
	Mat PointMaskChangeGradientDrawIn;		//调整梯度的缩进区域模板
	Mat MuraMaskChangeGradientDrawIn;		//调整梯度的缩进区域模板
	Mat LightMaskChangeGradientDrawIn;


	//自定义Mask
	Mat  PointMaskChangeGradientMouseArea;	//调整梯度的鼠标选定区域模板
	Mat  MuraMaskChangeGradientMouseArea;
	Mat  LightMaskChangeGradientMouseArea;


	//用于处理不同图片时的检测值



	RotatedRect rect_res;   ////矫正后的坐标点

	//图片灰度，梯度均值数组
	double imgInTFTMeans[10][20];
	double imgInTFTStddevs[10][20];
	double 	imgInGradientMeans[10][20];
	double imgInGradientStddevs[10][20];

	Mat imgOut_g;   //测试用


	// 汇总图
	vector<Point2i> points; //画汇总图红绿点用，统计缺陷类型
	Mat back;  // 画汇总九宫图用，在缺陷位置标红框和表示缺陷类型的数字


	vector<Point2d> group_points[30];

	int matrix[10][7][8] = { 0 };

	Point2f P[4][2];

protected:


};


//1.全局变量
//Canshu lougunagCanshu =() 
//Canshu pointDetect = ()
//Canshu muraDetect = ()


class CanShu

{

private: //成员访问限制符

public: //public设置之前的成员都是上面所设置的私有的
	int _TFTHOLD;
	int _DUSTHOLD;
	int _DUSTSHAPE;

	//缺陷异常检测参数
	int _num_gradient, _num_grayScale; //梯度和灰度异常像素点个数
	double _dst_m_gradient, _dst_s_gradient; //梯度均值
	double _dst_m_grayScale, _dst_s_grayScale; //灰度均值和方差;
	double _meanValueGradient;
	int _fill;

	int _dis_u; //上边缘距离
	int _dis_d; //下边缘距离
	int _dis_l; //左边缘距离
	int _dis_r; //右边缘距离


	int _bright_num; //极亮点阈值
	int _dark_num;  //极暗点阈值

	CanShu(int TFTHOLD, int DUSTHOLD, int DUSTSHAPE)
	{

		_TFTHOLD = TFTHOLD;
		_DUSTHOLD = DUSTHOLD;
		_DUSTSHAPE = DUSTSHAPE;

	}

	CanShu(int bright_num, int dark_num)
	{
		bright_num = _bright_num;
		dark_num = _dark_num;
	}


	CanShu(int TFTHOLD, int DUSTHOLD, int DUSTSHAPE, int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int fill, int meanValueGradient, int dst_s_gradient, int  dst_s_grayScale, int bright_num, int dark_num)
	{

		_TFTHOLD = TFTHOLD;
		_DUSTHOLD = DUSTHOLD;
		_DUSTSHAPE = DUSTSHAPE;

		//缺陷异常检测
		_num_gradient = num_gradient;
		_num_grayScale = num_grayScale;
		_dst_m_gradient = dst_m_gradient;
		_dst_m_grayScale = dst_m_grayScale;
		_fill = fill;
		_dst_s_grayScale = dst_s_grayScale;
		_dst_s_gradient = dst_s_gradient;
		_meanValueGradient = float(meanValueGradient) / 10;
		_bright_num = bright_num;
		_dark_num = dark_num;



	}

	CanShu(int  a)
	{


	}
	void initCanshu()
	{
		_TFTHOLD = 187;
		_DUSTHOLD = 134;
		_DUSTSHAPE = 5;

		_num_gradient = 300;
		_num_grayScale = 200;
		_dst_m_gradient = 20;
		_dst_m_grayScale = 44;
		_dst_s_grayScale = 40;
		_fill = 20;
		_meanValueGradient = 10;
		_dis_u = 60;
		_dis_d = 60;
		_dis_l = 60;
		_dis_r = 60;
		_bright_num = 10;
		_dark_num = 10;


	}




protected:
};



//分区域设置阈值的参数
/*
int _keneralsize;            手机轮廓的膨胀系数，越大，检测的区域越小
int _size_x_left;			 手机左边界内缩像素行数
int _size_x_right;			 手机右边界内缩像素行数
int _size_y_up;				 手机上边界内缩像素行数
int _size_y_down;			 手机下边界内缩像素行数



*/

class CanShuMask

{

private: //成员访问限制符

public: //public设置之前的成员都是上面所设置的私有的


	int _keneralsize;
	int _size_x_left;
	int _size_x_right;
	int _size_y_up;
	int _size_y_down;

	int _point_num;
	int _line_num;
	int _mura_num;
	int _tuan_num;
	double _point_m_gray;
	double _line_m_gray;
	double _mura_m_gray;
	double _tuan_m_gray;


	//构造函数，用于默认赋值
	CanShuMask(int keneralsize, int size_x_left, int size_x_right, int size_y_up, int size_y_down)
	{
		initCanShuMask(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down);

	}

	//初始化函数，用于改变默认赋值
	void initCanShuMask(int keneralsize, int size_x_left, int size_x_right, int size_y_up, int size_y_down)
	{
		_keneralsize = keneralsize;
		_size_x_left = size_x_left;
		_size_x_right = size_x_right;
		_size_y_up = size_y_up;
		_size_y_down = size_y_down;

	}

protected:
};


class CanShuFinal

{

private: //成员访问限制符

public: //public设置之前的成员都是上面所设置的私有的


	//int _keneralsize;
	//int _size_x_left;
	//int _size_x_right;
	//int _size_y_up;
	//int _size_y_down;

	int _point_num;
	int _line_num;
	int _mura_num;
	int _tuan_num;
	double _point_m_gray;
	double _line_m_gray;
	double _mura_m_gray;
	double _tuan_m_gray;


	//构造函数，用于默认赋值
	CanShuFinal(int point_num, int line_num, int mura_num, int tuan_num, double point_m_gray, double line_m_gray, double mura_m_gray, double tuan_m_gray)
	{
		initCanShuMask(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray);

	}

	//初始化函数，用于改变默认赋值
	void initCanShuMask(int point_num, int line_num, int mura_num, int tuan_num, double point_m_gray, double line_m_gray, double mura_m_gray, double tuan_m_gray)
	{
		_point_num = point_num;
		_line_num = line_num;
		_mura_num = mura_num;
		_tuan_num = tuan_num;
		_point_m_gray = point_m_gray;
		_line_m_gray = line_m_gray;
		_mura_m_gray = mura_m_gray;
		_tuan_m_gray = tuan_m_gray;

	}

protected:
};


//去除假缺陷的参数
/*
int _num_gradient, _num_grayScale; //梯度和灰度异常像素点个数
double _dst_m_gradient, _dst_s_gradient; //梯度均值和方差
double _dst_m_grayScale, _dst_s_grayScale; //灰度均值和方差;
double _meanValueGradient;
*/
class CanShureMoveFalse

{

private: //成员访问限制符

public: //public设置之前的成员都是上面所设置的私有的
		//缺陷异常检测参数
	int _num_gradient, _num_grayScale; //梯度和灰度异常像素点个数
	double _dst_m_gradient, _dst_s_gradient; //梯度均值
	double _dst_m_grayScale, _dst_s_grayScale; //灰度均值和方差;
	double _meanValueGradient;
	int _bright_num;
	int _dark_num;


	CanShureMoveFalse(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, int bright_num, int dark_num)
	{
		//缺陷异常检测
		initCanShureMoveFalse(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num);

	}



	void initCanShureMoveFalse(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, int bright_num, int dark_num)
	{
		//缺陷异常检测
		_num_gradient = num_gradient;
		_num_grayScale = num_grayScale;
		_dst_m_gradient = dst_m_gradient;
		_dst_m_grayScale = dst_m_grayScale;
		_dst_s_gradient = dst_s_gradient;
		_dst_s_grayScale = dst_s_grayScale;
		_meanValueGradient = float(meanValueGradient) / 10;

	}

protected:
};



//



//class BorderCanShu
//{
//
//private: //成员访问限制符
//
//public: //public设置之前的成员都是上面所设置的私有的
//
//	//缺陷异常检测参数
//	int _b_num_gradient, _b_num_grayScale; //梯度和灰度异常像素点个数
//	double _b_dst_m_gradient, _b_dst_s_gradient; //梯度均值
//	double _b_dst_m_grayScale, _b_dst_s_grayScale; //灰度均值和方差;
//
//	BorderCanShu(int b_num_gradient, int b_num_grayScale, int b_dst_m_gradient, int  b_dst_m_grayScale,  int b_dst_s_gradient, int  b_dst_s_grayScale)
//	{
//
//	
//		//缺陷异常检测
//		_b_num_gradient = b_num_gradient;
//		_b_num_grayScale = b_num_grayScale;
//		_b_dst_m_gradient = b_dst_m_gradient;
//		_b_dst_m_grayScale = b_dst_m_grayScale;
//		_b_dst_s_grayScale = b_dst_s_grayScale;
//		_b_dst_s_gradient = b_dst_s_gradient;
//
//	}
//
//protected:
//};



//全局变量声明
extern sendImg *sendImg1;
extern  CanShu *canshuX;
extern  CanShu *canshuMura;
extern  CanShu *canshuLouguang;
extern  CanShuMask *PointCanShuMask;
extern  CanShuMask *MuraCanShuMask;
extern CanShureMoveFalse  *muraCanShureMoveFalse;
extern CanShureMoveFalse *PointCanShureMoveFalse;

//extern  BorderCanShu *canshuY;

//extern sendImg *sendImg1 = &sendImgExtern;
//extern  CanShu * canshuX = &canshuExtern;


//滑动窗调用函数声明

//不再使用
//void on_trackbar_1(int, void* names);
//不再使用
//void on_trackbar(int, void* names);
//用于重新调整区域检测区域参数 ，调整mask的大小 
//void on_trackbar3(int, void *names);
//曲线参数赋值 ， 用于重新移除假缺陷
//void on_trackbar4(int, void *names);