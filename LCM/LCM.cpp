// dustGetYu.cpp: 定义控制台应用程序的入口点。
//0119 新添判断阈值 极亮点、极暗点
//0123 修改了subdust_subTemp函数中的二值化阈值
//修改了LCM.H中的sub的内缩系数
//0124 lyl add 增加三张图合并膨胀之后去原始二值化图中寻找对应区域，统计这些区域的异常点个数（之前只有灰度图信息统计）
//0124 修改了count_gray_only函数，新添加了统计量
//0124 修改了 determine函数的 判定条件
//num_yichang 要加入参数里面，作为第一套参数开放
//0124 修改了群点判定的函数
//修改了getpointResult函数
//修改determine函数 
//0125 修改了drawpic函数 增加了旋转后的坐标对应关系
//0125 增加群点判定类型 （待修改）
//0125 修改了R角函数
//0126 修改了drawpic函数
//0219 增加了sub的changiou过程
//0219 增加了保存中间结果的过程
//0219 增加了一个霍夫变换的参数  以及四个边界点的参数 （共计五个参数）
//0220 第一套参数的判定条件依然有待商榷
//0221 增加了一套参数和二套参数的滑窗窗体（调参使用）
//修改了count_gradient_only函数里面的判定条件
//0222 lyl与wzx核对了代码，黑屏相关的判定代码未更新增加（之后需要add）
//0222 增加了一组sub的参数，现在三种检测方法的参数完全独立分开管控
//0225 调整sub函数的结构
//0225 新添加了一种检测方法，修改了漏光检测方法
//0227 调整了debug函数的结构
//0227 修改滑杆函数，增加threshold等二值化参数  修复了滑竿函数的bug问题
//0228 增加了灰尘相关的滑杆参数
//0228 修改了二套参数调节的滑杆函数onSecondParameter  
//0228 onSecondParameter目前只是融合了单相机的单张图片的多种检测方法的结果，后续有待修改成为多相机多屏幕的版本
//0301 增加了霍夫变换获取ROI区域相关的参数调节
//0301 第五种sub方法的二值化阈值白屏20，红屏160，变化很大，有待查明原因，很奇怪？？？？
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
#include "LCM.h"
#include "creatDir.h"
#include "writeAndReadIni.h"
#include "Rotating.h"
#include "fill.h"
#include "padding.h"
#include "ComputingTheGradient.h"
#include "CalculatingDefectInformation.h"
#include "subDustBorder.h"
#include "removeFalseDetect.h"
#include "divideIOU.h"
#include "transform.h"
#include "debugAdd.h"
using namespace cv;
using namespace std;

sendImg sendImgExtern;

int houghthr;   //霍夫变换二值化参数
int DUSTHOLD;  //104
int DUSTSHAPE;   //5
int fill1;  //灰度异常总数判断阈值

int TFTHOLD_points = 180;   //130     //TFTHOLD是图片的灰度图二值化的阈值，在SubDustAndBorder函数里面调用
int TFTHOLD_muras= 180;   //130     //TFTHOLD是图片的灰度图二值化的阈值，在SubDustAndBorder函数里面调用

int sort_dxy1;    //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy2;    //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy3;     //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy4;    //灰尘相机的侧拍参数（用于划分四个边界点）
int subThresh =20;
int subBGRThresh =70;
int bgrThresh =20;
int hsvThresh=20;
int Rarea =50;
int i =0;
int j=0;

//0301新增
int houghP;

CanShu canshuExtern(subThresh, subBGRThresh, bgrThresh, hsvThresh, sort_dxy1, sort_dxy2, sort_dxy3, sort_dxy4, houghthr, DUSTHOLD, DUSTSHAPE, fill1, Rarea, TFTHOLD_muras, TFTHOLD_points, houghP, i, j);

int point_num = 3;
int line_num = 3;
int mura_num = 5;
int tuan_num = 5;
double point_m_gray = 3;
double line_m_gray = 3;
double mura_m_gray = 3;
double tuan_m_gray = 3;
int maxDistance = 100;
int maxGParea = 50;
int point_bk_num = 0;
int point_bk_m_gray = 0;

CanShuFinal SecondCanshu(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);
CanShuFinal SecondCanshu2(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);
CanShuFinal SecondCanshu3(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);
CanShuFinal SecondCanshu4(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);
CanShuFinal SecondCanshu5(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);
CanShuFinal SecondCanshu6(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);

//对象数组
CanShuFinal CanshuForSecond[6] = { SecondCanshu, SecondCanshu2 ,SecondCanshu3  ,SecondCanshu4 ,SecondCanshu5 ,SecondCanshu6 };

int keneralsize;   //水滴屏幕参数 100
int size_x_left;   //水滴屏幕参数 100
int size_x_right;  //水滴屏幕参数 100
int size_y_up;      //水滴屏幕参数 100
int size_y_down;    //水滴屏幕参数 100

CanShuMask PointCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);
CanShuMask MuraCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);   //25   之前的刘海屏幕参数设置
CanShuMask lightCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);
CanShuMask SubCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);   //25   之前的刘海屏幕参数设置
CanShuMask SubBGRCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);   //25   之前的刘海屏幕参数设置
//CanShuMask SubCanShuMaskx(30, 70, 0, 20, 0);   //25   之前的刘海屏幕参数设置

//
//初始化Mask参数

//(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int meanValueGradient, int dst_s_gradient, int  dst_s_grayScale)//移除假缺陷参数

////移除假缺陷参数
//int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, 
int num_gradient;    //梯度异常总数判断阈值
int num_grayScale;  //灰度异常总数判断阈值
int dst_m_gradient; //梯度异常判断阈值
int dst_m_grayScale; //灰度异常判断阈值
int dst_s_gradient;//梯度标准差 20 before
int dst_s_grayScale; //灰度异常判断阈值
int meanValueGradient;

int bright_num;  //灰度异常总数判断阈值
int dark_num;  //灰度异常总数判断阈值


CanShureMoveFalse    PointCanShureMoveFalsex(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);

//CanShureMoveFalse(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, int bright_num, int dark_num)
CanShureMoveFalse  muraCanShureMoveFalsex(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);

CanShureMoveFalse  LightCanShureMoveFalsex(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);

CanShureMoveFalse  SubCanShureMoveFalsex(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);

CanShureMoveFalse  SubBGRCanShureMoveFalsex(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);


//vector <defectInformationss> defectInformations(30);

//defectInformation detectInfor[30][4][200];
//设定为全局变量
sendImg *sendImg1 = &sendImgExtern;


CanShu * canshuX = &canshuExtern;


//CanShu * canshu_num = &BrightDark_Num;

CanShuMask *PointCanShuMask = &PointCanShuMaskx;

CanShuMask *MuraCanShuMask = &MuraCanShuMaskx;

CanShuMask *lightCanShuMask = &lightCanShuMaskx;

CanShuMask *SubCanShuMask = &SubCanShuMaskx;

CanShuMask *SubBGRCanShuMask = &SubBGRCanShuMaskx;



CanShureMoveFalse  *muraCanShureMoveFalse = &muraCanShureMoveFalsex;

CanShureMoveFalse    *PointCanShureMoveFalse = &PointCanShureMoveFalsex;

CanShureMoveFalse    *LightCanShureMoveFalse = &LightCanShureMoveFalsex;

CanShureMoveFalse    *SubCanShureMoveFalse = &SubCanShureMoveFalsex;

CanShureMoveFalse    *SubBGRCanShureMoveFalse = &SubBGRCanShureMoveFalsex;





//相机畸变校准用到的参数
float neican_data[] = { 124046.170922198990,0,1672.8744383824846000,0, 123576.1775990077400,1436.80076020515030,0,0,1 };
float jibian_data[] = { -115.529366790098550, -13.3238477842862260, -0.0822106292981709260,0.00036432147392857344,-0.00288576837799785390 };

float neican_data_A[] = { 74783.7435244331720,0,1678.03692091216480,0, 78656.741412721021000,1372.67656859171460000,0,0,1 };
float jibian_data_A[] = { -8.86938558282631110, 2.26982865811383980, -0.121296204933671560,0.151996295500854470,0.0010363396229630410 };

float neican_data_E[] = { 14057.49668362299100,0,1914.823952172616400,0, 13696.913440609065000,792.15791617674211000,0,0,1 };
float jibian_data_E[] = { -0.54469973088577128000, -38.23095327363589700, 0.03861500246177145700,-0.01644584532002359500,1743.74914915230310000000 };
//缺陷信息存储

vector<defectInformation> defectsInforGray[40][20][5];


vector<defectInformation> defectsInfor[40][20][5];

//缺陷信息存储
//vector<defectInformation> defectsInfor[10][20][4];

//labview传图片数据初始化为图片
void InitMat(Mat& m, float* num)
{
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			m.at<float>(i, j) = *(num + i * m.rows + j);
}


void Initcanshu()
{
	//白屏参数
	PointCanShureMoveFalsex._num_gradient[0][0] = 3;
	PointCanShureMoveFalsex._num_grayScale[0][0] = 5;
	PointCanShureMoveFalsex._dst_m_gradient[0][0] = 4;
	PointCanShureMoveFalsex._dst_m_grayScale[0][0] = 8;
	PointCanShureMoveFalsex._dst_s_gradient[0][0] = 10;
	PointCanShureMoveFalsex._dst_s_grayScale[0][0] = 8;
	PointCanShureMoveFalsex._meanValueGradient[0][0] = 32;
	PointCanShureMoveFalsex._bright_num[0][0] = 20;
	PointCanShureMoveFalsex._dark_num[0][0] = 20;

	muraCanShureMoveFalsex._num_gradient[0][0] = 8;
	muraCanShureMoveFalsex._num_grayScale[0][0] = 5;
	muraCanShureMoveFalsex._dst_m_gradient[0][0] = 5;
	muraCanShureMoveFalsex._dst_m_grayScale[0][0] = 8;
	muraCanShureMoveFalsex._dst_s_gradient[0][0] = 10;
	muraCanShureMoveFalsex._dst_s_grayScale[0][0] = 8;
	muraCanShureMoveFalsex._meanValueGradient[0][0] = 32;
	muraCanShureMoveFalsex._bright_num[0][0] = 20;
	muraCanShureMoveFalsex._dark_num[0][0] = 20;

	LightCanShureMoveFalsex._num_gradient[0][0] = 20;
	LightCanShureMoveFalsex._num_grayScale[0][0] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[0][0] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[0][0] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[0][0] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[0][0] = 8;
	LightCanShureMoveFalsex._meanValueGradient[0][0] = 40;
	LightCanShureMoveFalsex._bright_num[0][0] = 20;
	LightCanShureMoveFalsex._dark_num[0][0] = 20;

	SubCanShureMoveFalsex._num_gradient[0][0] = 20;
	SubCanShureMoveFalsex._num_grayScale[0][0] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[0][0] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[0][0] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[0][0] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[0][0] = 8;
	SubCanShureMoveFalsex._meanValueGradient[0][0] = 40;
	SubCanShureMoveFalsex._bright_num[0][0] = 20;
	SubCanShureMoveFalsex._dark_num[0][0] = 20;

	SubBGRCanShureMoveFalsex._num_gradient[0][0] = 3;
	SubBGRCanShureMoveFalsex._num_grayScale[0][0] = 5;
	SubBGRCanShureMoveFalsex._dst_m_gradient[0][0] =4;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[0][0] = 8;
	SubBGRCanShureMoveFalsex._dst_s_gradient[0][0] = 3;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[0][0] = 8;
	SubBGRCanShureMoveFalsex._meanValueGradient[0][0] = 25;
	SubBGRCanShureMoveFalsex._bright_num[0][0] = 20;
	SubBGRCanShureMoveFalsex._dark_num[0][0] = 20;


	PointCanShuMaskx._keneralsize[0][0] = 15;
	PointCanShuMaskx._size_x_left[0][0] = 10;
	PointCanShuMaskx._size_x_right[0][0] = 10;
	PointCanShuMaskx._size_y_down[0][0] = 10;
	PointCanShuMaskx._size_y_up[0][0] = 10;



	MuraCanShuMaskx._keneralsize[0][0] = 15;
	MuraCanShuMaskx._size_x_left[0][0] = 20;
	MuraCanShuMaskx._size_x_right[0][0] = 20;
	MuraCanShuMaskx._size_y_down[0][0] = 20;
	MuraCanShuMaskx._size_y_up[0][0] = 20;

	lightCanShuMaskx._keneralsize[0][0] = 30;
	lightCanShuMaskx._size_x_left[0][0] = 70;
	lightCanShuMaskx._size_x_right[0][0] = 20;
	lightCanShuMaskx._size_y_down[0][0] = 20;
	lightCanShuMaskx._size_y_up[0][0] = 20;

	SubCanShuMaskx._keneralsize[0][0] = 30;
	SubCanShuMaskx._size_x_left[0][0] = 70;
	SubCanShuMaskx._size_x_right[0][0] = 20;
	SubCanShuMaskx._size_y_down[0][0] = 20;
	SubCanShuMaskx._size_y_up[0][0] = 20;

	SubBGRCanShuMaskx._keneralsize[0][0] = 15;
	SubBGRCanShuMaskx._size_x_left[0][0] = 10;
	SubBGRCanShuMaskx._size_x_right[0][0] = 10;
	SubBGRCanShuMaskx._size_y_down[0][0] = 10;
	SubBGRCanShuMaskx._size_y_up[0][0] = 10;


	canshuExtern._subThresh[0][0] = 50;
	canshuExtern._subBGRThresh[0][0] = 45;
	canshuExtern._bgrThresh[0][0] = 255;
	canshuExtern._hsvThresh[0][0] = 50;
	canshuExtern._sort_dxy1[0][0] = 15;
	canshuExtern._sort_dxy2[0][0] = 12;
	canshuExtern._sort_dxy3[0][0] = 20;
	canshuExtern._sort_dxy4[0][0] = 20;
	canshuExtern._houghthr[0][0] = 70;
	canshuExtern._DUSTHOLD[0][0] = 104;
	canshuExtern._DUSTSHAPE[0][0] = 5;
	canshuExtern._fill[0][0] = 20;
	canshuExtern._Rarea[0][0] = 50;
	canshuExtern._TFTHOLD_points[0][0] = 180;
	canshuExtern._TFTHOLD_muras[0][0] = 170;
	canshuExtern._houghP[0] = 70;






	//0号1号图片参数///  绿屏参数
	PointCanShureMoveFalsex._num_gradient[0][1] =4;
	PointCanShureMoveFalsex._num_grayScale[0][1] = 5;
	PointCanShureMoveFalsex._dst_m_gradient[0][1] =3;
	PointCanShureMoveFalsex._dst_m_grayScale[0][1] = 8;
	PointCanShureMoveFalsex._meanValueGradient[0][1] = 63;
	PointCanShureMoveFalsex._dst_s_gradient[0][1] = 2;
	PointCanShureMoveFalsex._dst_s_grayScale[0][1] = 20;
	PointCanShureMoveFalsex._bright_num[0][1] = 20;
	PointCanShureMoveFalsex._dark_num[0][1] = 20;

	muraCanShureMoveFalsex._num_gradient[0][1] = 4;
	muraCanShureMoveFalsex._num_grayScale[0][1] = 10;
	muraCanShureMoveFalsex._dst_m_gradient[0][1] =6;
	muraCanShureMoveFalsex._dst_m_grayScale[0][1] = 6;
	muraCanShureMoveFalsex._meanValueGradient[0][1] =63;
	muraCanShureMoveFalsex._dst_s_gradient[0][1] = 2;
	muraCanShureMoveFalsex._dst_s_grayScale[0][1] =10;
	muraCanShureMoveFalsex._bright_num[0][1] =20;
	muraCanShureMoveFalsex._dark_num[0][1] =20;



	LightCanShureMoveFalsex._num_gradient[0][1] = 20;
	LightCanShureMoveFalsex._num_grayScale[0][1] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[0][1] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[0][1] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[0][1] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[0][1] = 8;
	LightCanShureMoveFalsex._meanValueGradient[0][1] = 40;
	LightCanShureMoveFalsex._bright_num[0][1] = 20;
	LightCanShureMoveFalsex._dark_num[0][1] = 20;

	SubCanShureMoveFalsex._num_gradient[0][1] = 20;
	SubCanShureMoveFalsex._num_grayScale[0][1] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[0][1] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[0][1] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[0][1] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[0][1] = 8;
	SubCanShureMoveFalsex._meanValueGradient[0][1] = 40;
	SubCanShureMoveFalsex._bright_num[0][1] = 20;
	SubCanShureMoveFalsex._dark_num[0][1] = 20;

	SubBGRCanShureMoveFalsex._num_gradient[0][1] = 10;
	SubBGRCanShureMoveFalsex._num_grayScale[0][1] = 10;
	SubBGRCanShureMoveFalsex._dst_m_gradient[0][1] = 20;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[0][1] = 20;
	SubBGRCanShureMoveFalsex._meanValueGradient[0][1] = 63;
	SubBGRCanShureMoveFalsex._dst_s_gradient[0][1] = 2;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[0][1] = 10;
	SubBGRCanShureMoveFalsex._bright_num[0][1] = 20;
	SubBGRCanShureMoveFalsex._dark_num[0][1] = 20;






	lightCanShuMaskx._keneralsize[0][1] = 30;
	lightCanShuMaskx._size_x_left[0][1] = 70;
	lightCanShuMaskx._size_x_right[0][1] = 20;
	lightCanShuMaskx._size_y_down[0][1] = 20;
	lightCanShuMaskx._size_y_up[0][1] = 20;

	SubCanShuMaskx._keneralsize[0][1] = 30;
	SubCanShuMaskx._size_x_left[0][1] = 70;
	SubCanShuMaskx._size_x_right[0][1] = 20;
	SubCanShuMaskx._size_y_down[0][1] = 20;
	SubCanShuMaskx._size_y_up[0][1] = 20;

	
	PointCanShuMaskx._keneralsize[0][1] = 15;
	PointCanShuMaskx._size_x_left[0][1] = 10;
	PointCanShuMaskx._size_x_right[0][1] = 10;
	PointCanShuMaskx._size_y_down[0][1] = 10;
	PointCanShuMaskx._size_y_up[0][1] = 10;



	MuraCanShuMaskx._keneralsize[0][1] = 15;
	MuraCanShuMaskx._size_x_left[0][1] = 20;
	MuraCanShuMaskx._size_x_right[0][1] = 20;
	MuraCanShuMaskx._size_y_down[0][1] = 20;
	MuraCanShuMaskx._size_y_up[0][1] = 20;

	lightCanShuMaskx._keneralsize[0][1] = 30;
	lightCanShuMaskx._size_x_left[0][1] = 70;
	lightCanShuMaskx._size_x_right[0][1] = 20;
	lightCanShuMaskx._size_y_down[0][1] = 20;
	lightCanShuMaskx._size_y_up[0][1] = 20;

	SubCanShuMaskx._keneralsize[0][1] = 30;
	SubCanShuMaskx._size_x_left[0][1] = 70;
	SubCanShuMaskx._size_x_right[0][1] = 20;
	SubCanShuMaskx._size_y_down[0][1] = 20;
	SubCanShuMaskx._size_y_up[0][1] = 20;

	SubBGRCanShuMaskx._keneralsize[0][1] = 15;
	SubBGRCanShuMaskx._size_x_left[0][1] = 10;
	SubBGRCanShuMaskx._size_x_right[0][1] = 10;
	SubBGRCanShuMaskx._size_y_down[0][1] = 10;
	SubBGRCanShuMaskx._size_y_up[0][1] = 10;

	canshuExtern._subThresh[0][1] = 50;
	canshuExtern._subBGRThresh[0][1] = 80;
	canshuExtern._bgrThresh[0][1] = 255;
	canshuExtern._hsvThresh[0][1] = 50;
	canshuExtern._sort_dxy1[0][1] = 15;
	canshuExtern._sort_dxy2[0][1] = 12;
	canshuExtern._sort_dxy3[0][1] = 20;
	canshuExtern._sort_dxy4[0][1] = 20;
	canshuExtern._houghthr[0][1] = 70;
	canshuExtern._DUSTHOLD[0][1] = 104;
	canshuExtern._DUSTSHAPE[0][1] = 5;
	canshuExtern._fill[0][1] = 20;
	canshuExtern._Rarea[0][1] = 50;
	canshuExtern._TFTHOLD_points[0][1] = 110;
	canshuExtern._TFTHOLD_muras[0][1] = 110;
	canshuExtern._houghP[0] = 70;



	//0号相机2号图片
	//第三个颜色  蓝色

	PointCanShureMoveFalsex._num_gradient[0][2] =10 ;
	PointCanShureMoveFalsex._num_grayScale[0][2] =10;
	PointCanShureMoveFalsex._dst_m_gradient[0][2] = 3;
	PointCanShureMoveFalsex._dst_m_grayScale[0][2] =3;
	PointCanShureMoveFalsex._meanValueGradient[0][2] = 40;
	PointCanShureMoveFalsex._dst_s_gradient[0][2] =30;
	PointCanShureMoveFalsex._dst_s_grayScale[0][2] =20;
	PointCanShureMoveFalsex._bright_num[0][2] =20;
	PointCanShureMoveFalsex._dark_num[0][2] = 20;

	muraCanShureMoveFalsex._num_gradient[0][2] =25;
	muraCanShureMoveFalsex._num_grayScale[0][2] = 25;
	muraCanShureMoveFalsex._dst_m_gradient[0][2] =4;
	muraCanShureMoveFalsex._dst_m_grayScale[0][2] = 4;
	muraCanShureMoveFalsex._meanValueGradient[0][2] = 40;
	muraCanShureMoveFalsex._dst_s_gradient[0][2] =33;
	muraCanShureMoveFalsex._dst_s_grayScale[0][2] =20;
	muraCanShureMoveFalsex._bright_num[0][2] =20;
	muraCanShureMoveFalsex._dark_num[0][2] = 20;

	LightCanShureMoveFalsex._num_gradient[0][2] = 20;
	LightCanShureMoveFalsex._num_grayScale[0][2] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[0][2] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[0][2] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[0][2] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[0][2] = 8;
	LightCanShureMoveFalsex._meanValueGradient[0][2] = 40;
	LightCanShureMoveFalsex._bright_num[0][2] = 20;
	LightCanShureMoveFalsex._dark_num[0][2] = 20;

	SubCanShureMoveFalsex._num_gradient[0][2] = 20;
	SubCanShureMoveFalsex._num_grayScale[0][2] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[0][2] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[0][2] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[0][2] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[0][2] = 8;
	SubCanShureMoveFalsex._meanValueGradient[0][2] = 40;
	SubCanShureMoveFalsex._bright_num[0][2] = 20;
	SubCanShureMoveFalsex._dark_num[0][2] = 20;

	SubBGRCanShureMoveFalsex._num_gradient[0][2] = 10;
	SubBGRCanShureMoveFalsex._num_grayScale[0][2] = 10;
	SubBGRCanShureMoveFalsex._dst_m_gradient[0][2] = 20;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[0][2] = 20;
	SubBGRCanShureMoveFalsex._meanValueGradient[0][2] = 40;
	SubBGRCanShureMoveFalsex._dst_s_gradient[0][2] = 4;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[0][2] = 4;
	SubBGRCanShureMoveFalsex._bright_num[0][2] = 20;
	SubBGRCanShureMoveFalsex._dark_num[0][2] = 20;

	PointCanShuMaskx._keneralsize[0][2] = 15;
	PointCanShuMaskx._size_x_left[0][2] = 10;
	PointCanShuMaskx._size_x_right[0][2] = 10;
	PointCanShuMaskx._size_y_down[0][2] = 10;
	PointCanShuMaskx._size_y_up[0][2] = 10;



	MuraCanShuMaskx._keneralsize[0][2] = 15;
	MuraCanShuMaskx._size_x_left[0][2] = 20;
	MuraCanShuMaskx._size_x_right[0][2] = 20;
	MuraCanShuMaskx._size_y_down[0][2] = 20;
	MuraCanShuMaskx._size_y_up[0][2] = 20;

	lightCanShuMaskx._keneralsize[0][2] = 30;
	lightCanShuMaskx._size_x_left[0][2] = 70;
	lightCanShuMaskx._size_x_right[0][2] = 20;
	lightCanShuMaskx._size_y_down[0][2] = 20;
	lightCanShuMaskx._size_y_up[0][2] = 20;

	SubCanShuMaskx._keneralsize[0][2] = 30;
	SubCanShuMaskx._size_x_left[0][2] = 70;
	SubCanShuMaskx._size_x_right[0][2] = 20;
	SubCanShuMaskx._size_y_down[0][2] = 20;
	SubCanShuMaskx._size_y_up[0][2] = 20;

	SubBGRCanShuMaskx._keneralsize[0][2] = 15;
	SubBGRCanShuMaskx._size_x_left[0][2] = 10;
	SubBGRCanShuMaskx._size_x_right[0][2] = 10;
	SubBGRCanShuMaskx._size_y_down[0][2] = 10;
	SubBGRCanShuMaskx._size_y_up[0][2] = 10;

	canshuExtern._subThresh[0][2] = 50;
	canshuExtern._subBGRThresh[0][2] = 70;
	canshuExtern._bgrThresh[0][2] = 255;
	canshuExtern._hsvThresh[0][2] = 50;
	canshuExtern._sort_dxy1[0][2] = 15;
	canshuExtern._sort_dxy2[0][2] = 12;
	canshuExtern._sort_dxy3[0][2] = 20;
	canshuExtern._sort_dxy4[0][2] = 20;
	canshuExtern._houghthr[0][2] = 70;
	canshuExtern._DUSTHOLD[0][2] = 104;
	canshuExtern._DUSTSHAPE[0][2] = 5;
	canshuExtern._fill[0][2] = 20;
	canshuExtern._Rarea[0][2] = 50;
	canshuExtern._TFTHOLD_points[0][2] = 180;
	canshuExtern._TFTHOLD_muras[0][2] = 180;
	canshuExtern._houghP[0] = 70;


	//第四个颜色  黑屏

	PointCanShureMoveFalsex._num_gradient[0][3] =20;
	PointCanShureMoveFalsex._num_grayScale[0][3] = 20;
	PointCanShureMoveFalsex._dst_m_gradient[0][3] = 10;
	PointCanShureMoveFalsex._dst_m_grayScale[0][3] =10;
	PointCanShureMoveFalsex._meanValueGradient[0][3] = 10;
	PointCanShureMoveFalsex._dst_s_gradient[0][3] =1;
	PointCanShureMoveFalsex._dst_s_grayScale[0][3] =1;
	PointCanShureMoveFalsex._bright_num[0][3] = 20;
	PointCanShureMoveFalsex._dark_num[0][3] = 20;

	muraCanShureMoveFalsex._num_gradient[0][3] = 25;
	muraCanShureMoveFalsex._num_grayScale[0][3] = 25;
	muraCanShureMoveFalsex._dst_m_gradient[0][3] = 4;
	muraCanShureMoveFalsex._dst_m_grayScale[0][3] = 4;
	muraCanShureMoveFalsex._meanValueGradient[0][3] = 40;
	muraCanShureMoveFalsex._dst_s_gradient[0][3] = 33;
	muraCanShureMoveFalsex._dst_s_grayScale[0][3] = 20;
	muraCanShureMoveFalsex._bright_num[0][3] = 20;
	muraCanShureMoveFalsex._dark_num[0][3] = 20;

	LightCanShureMoveFalsex._num_gradient[0][3] = 20;
	LightCanShureMoveFalsex._num_grayScale[0][3] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[0][3] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[0][3] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[0][3] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[0][3] = 8;
	LightCanShureMoveFalsex._meanValueGradient[0][3] = 40;
	LightCanShureMoveFalsex._bright_num[0][3] = 20;
	LightCanShureMoveFalsex._dark_num[0][3] = 20;

	SubCanShureMoveFalsex._num_gradient[0][3] = 20;
	SubCanShureMoveFalsex._num_grayScale[0][3] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[0][3] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[0][3] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[0][3] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[0][3] = 8;
	SubCanShureMoveFalsex._meanValueGradient[0][3] = 40;
	SubCanShureMoveFalsex._bright_num[0][3] = 20;
	SubCanShureMoveFalsex._dark_num[0][3] = 20;

	SubBGRCanShureMoveFalsex._num_gradient[0][3] = 20;
	SubBGRCanShureMoveFalsex._num_grayScale[0][3] = 20;
	SubBGRCanShureMoveFalsex._dst_m_gradient[0][3] =10;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[0][3] =10;
	SubBGRCanShureMoveFalsex._meanValueGradient[0][3] = 10;
	SubBGRCanShureMoveFalsex._dst_s_gradient[0][3] = 1;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[0][3] = 1;
	SubBGRCanShureMoveFalsex._bright_num[0][3] = 20;
	SubBGRCanShureMoveFalsex._dark_num[0][3] = 20;

	PointCanShuMaskx._keneralsize[0][3] = 15;
	PointCanShuMaskx._size_x_left[0][3] = 10;
	PointCanShuMaskx._size_x_right[0][3] = 10;
	PointCanShuMaskx._size_y_down[0][3] = 10;
	PointCanShuMaskx._size_y_up[0][3] = 10;



	MuraCanShuMaskx._keneralsize[0][3] = 15;
	MuraCanShuMaskx._size_x_left[0][3] = 20;
	MuraCanShuMaskx._size_x_right[0][3] = 20;
	MuraCanShuMaskx._size_y_down[0][3] = 20;
	MuraCanShuMaskx._size_y_up[0][3] = 20;

	lightCanShuMaskx._keneralsize[0][3] = 30;
	lightCanShuMaskx._size_x_left[0][3] = 70;
	lightCanShuMaskx._size_x_right[0][3] = 20;
	lightCanShuMaskx._size_y_down[0][3] = 20;
	lightCanShuMaskx._size_y_up[0][3] = 20;

	SubCanShuMaskx._keneralsize[0][3] = 30;
	SubCanShuMaskx._size_x_left[0][3] = 70;
	SubCanShuMaskx._size_x_right[0][3] = 20;
	SubCanShuMaskx._size_y_down[0][3] = 20;
	SubCanShuMaskx._size_y_up[0][3] = 20;

	SubBGRCanShuMaskx._keneralsize[0][3] = 15;
	SubBGRCanShuMaskx._size_x_left[0][3] = 10;
	SubBGRCanShuMaskx._size_x_right[0][3] = 10;
	SubBGRCanShuMaskx._size_y_down[0][3] = 10;
	SubBGRCanShuMaskx._size_y_up[0][3] = 10;

	canshuExtern._subThresh[0][3] = 50;
	canshuExtern._subBGRThresh[0][3] = 19;
	canshuExtern._bgrThresh[0][3] = 255;
	canshuExtern._hsvThresh[0][3] = 50;
	canshuExtern._sort_dxy1[0][3] = 15;
	canshuExtern._sort_dxy2[0][3] = 12;
	canshuExtern._sort_dxy3[0][3] = 20;
	canshuExtern._sort_dxy4[0][3] = 20;
	canshuExtern._houghthr[0][3] = 70;
	canshuExtern._DUSTHOLD[0][3] = 104;
	canshuExtern._DUSTSHAPE[0][3] = 5;
	canshuExtern._fill[0][3] = 20;
	canshuExtern._Rarea[0][3] = 50;
	canshuExtern._TFTHOLD_points[0][3] = 180;
	canshuExtern._TFTHOLD_muras[0][3] = 180;
	canshuExtern._houghP[0] = 70;


	//第五个颜色

	PointCanShureMoveFalsex._num_gradient[0][4] = 4;
	PointCanShureMoveFalsex._num_grayScale[0][4] = 4;
	PointCanShureMoveFalsex._dst_m_gradient[0][4] =2;
	PointCanShureMoveFalsex._dst_m_grayScale[0][4] = 4;
	PointCanShureMoveFalsex._meanValueGradient[0][4] = 20;
	PointCanShureMoveFalsex._dst_s_gradient[0][4] = 25;
	PointCanShureMoveFalsex._dst_s_grayScale[0][4] = 25;
	PointCanShureMoveFalsex._bright_num[0][4] =20;
	PointCanShureMoveFalsex._dark_num[0][4] = 20;

	muraCanShureMoveFalsex._num_gradient[0][4] = 26;
	muraCanShureMoveFalsex._num_grayScale[0][4] = 26;
	muraCanShureMoveFalsex._dst_m_gradient[0][4] = 3;
	muraCanShureMoveFalsex._dst_m_grayScale[0][4] = 3;
	muraCanShureMoveFalsex._meanValueGradient[0][4] = 30;
	muraCanShureMoveFalsex._dst_s_gradient[0][4] = 13;
	muraCanShureMoveFalsex._dst_s_grayScale[0][4] = 13;
	muraCanShureMoveFalsex._bright_num[0][4] = 20;
	muraCanShureMoveFalsex._dark_num[0][4] = 20;

	LightCanShureMoveFalsex._num_gradient[0][4] = 20;
	LightCanShureMoveFalsex._num_grayScale[0][4] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[0][4] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[0][4] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[0][4] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[0][4] = 8;
	LightCanShureMoveFalsex._meanValueGradient[0][4] = 40;
	LightCanShureMoveFalsex._bright_num[0][4] = 20;
	LightCanShureMoveFalsex._dark_num[0][4] = 20;

	SubCanShureMoveFalsex._num_gradient[0][4] = 20;
	SubCanShureMoveFalsex._num_grayScale[0][4] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[0][4] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[0][4] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[0][4] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[0][4] = 8;
	SubCanShureMoveFalsex._meanValueGradient[0][4] = 40;
	SubCanShureMoveFalsex._bright_num[0][4] = 20;
	SubCanShureMoveFalsex._dark_num[0][4] = 20;

	

	SubBGRCanShureMoveFalsex._num_gradient[0][4] = 18;
	SubBGRCanShureMoveFalsex._num_grayScale[0][4] = 18;
	SubBGRCanShureMoveFalsex._dst_m_gradient[0][4] = 10;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[0][4] = 10;
	SubBGRCanShureMoveFalsex._meanValueGradient[0][4] =150;
	SubBGRCanShureMoveFalsex._dst_s_gradient[0][4] = 8;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[0][4] =8;
	SubBGRCanShureMoveFalsex._bright_num[0][4] = 20;
	SubBGRCanShureMoveFalsex._dark_num[0][4] = 20;

	PointCanShuMaskx._keneralsize[0][4] = 15;
	PointCanShuMaskx._size_x_left[0][4] = 10;
	PointCanShuMaskx._size_x_right[0][4] = 10;
	PointCanShuMaskx._size_y_down[0][4] = 10;
	PointCanShuMaskx._size_y_up[0][4] = 10;



	MuraCanShuMaskx._keneralsize[0][4] = 15;
	MuraCanShuMaskx._size_x_left[0][4] = 20;
	MuraCanShuMaskx._size_x_right[0][4] = 20;
	MuraCanShuMaskx._size_y_down[0][4] = 20;
	MuraCanShuMaskx._size_y_up[0][4] = 20;

	lightCanShuMaskx._keneralsize[0][4] = 30;
	lightCanShuMaskx._size_x_left[0][4] = 70;
	lightCanShuMaskx._size_x_right[0][4] = 20;
	lightCanShuMaskx._size_y_down[0][4] = 20;
	lightCanShuMaskx._size_y_up[0][4] = 20;

	SubCanShuMaskx._keneralsize[0][4] = 30;
	SubCanShuMaskx._size_x_left[0][4] = 70;
	SubCanShuMaskx._size_x_right[0][4] = 20;
	SubCanShuMaskx._size_y_down[0][4] = 20;
	SubCanShuMaskx._size_y_up[0][4] = 20;

	SubBGRCanShuMaskx._keneralsize[0][4] = 15;
	SubBGRCanShuMaskx._size_x_left[0][4] = 10;
	SubBGRCanShuMaskx._size_x_right[0][4] = 10;
	SubBGRCanShuMaskx._size_y_down[0][4] = 10;
	SubBGRCanShuMaskx._size_y_up[0][4] = 10;

	canshuExtern._subThresh[0][4] = 50;
	canshuExtern._subBGRThresh[0][4] = 19;
	canshuExtern._bgrThresh[0][4] = 255;
	canshuExtern._hsvThresh[0][4] = 50;
	canshuExtern._sort_dxy1[0][4] = 15;
	canshuExtern._sort_dxy2[0][4] = 12;
	canshuExtern._sort_dxy3[0][4] = 20;
	canshuExtern._sort_dxy4[0][4] = 20;
	canshuExtern._houghthr[0][4] = 70;
	canshuExtern._DUSTHOLD[0][4] = 104;
	canshuExtern._DUSTSHAPE[0][4] = 5;
	canshuExtern._fill[0][4] = 20;
	canshuExtern._Rarea[0][4] = 50;
	canshuExtern._TFTHOLD_points[0][4] = 180;
	canshuExtern._TFTHOLD_muras[0][4] = 180;
	canshuExtern._houghP[0] = 70;

	




	/*1号相机白屏参数*/

	PointCanShureMoveFalsex._num_gradient[1][0] = 20;
	PointCanShureMoveFalsex._num_grayScale[1][0] = 5;
	PointCanShureMoveFalsex._dst_m_gradient[1][0] = 20;
	PointCanShureMoveFalsex._dst_m_grayScale[1][0] = 8;
	PointCanShureMoveFalsex._dst_s_gradient[1][0] = 20;
	PointCanShureMoveFalsex._dst_s_grayScale[1][0] = 8;
	PointCanShureMoveFalsex._meanValueGradient[1][0] = 40;
	PointCanShureMoveFalsex._bright_num[1][0] = 20;
	PointCanShureMoveFalsex._dark_num[1][0] = 20;

	muraCanShureMoveFalsex._num_gradient[1][0] = 20;
	muraCanShureMoveFalsex._num_grayScale[1][0] = 5;
	muraCanShureMoveFalsex._dst_m_gradient[1][0] = 30;
	muraCanShureMoveFalsex._dst_m_grayScale[1][0] = 8;
	muraCanShureMoveFalsex._dst_s_gradient[1][0] = 30;
	muraCanShureMoveFalsex._dst_s_grayScale[1][0] = 8;
	muraCanShureMoveFalsex._meanValueGradient[1][0] = 200;
	muraCanShureMoveFalsex._bright_num[1][0] = 20;
	muraCanShureMoveFalsex._dark_num[1][0] = 20;

	LightCanShureMoveFalsex._num_gradient[1][0] = 20;
	LightCanShureMoveFalsex._num_grayScale[1][0] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[1][0] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[1][0] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[1][0] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[1][0] = 8;
	LightCanShureMoveFalsex._meanValueGradient[1][0] = 40;
	LightCanShureMoveFalsex._bright_num[1][0] = 20;
	LightCanShureMoveFalsex._dark_num[1][0] = 20;

	SubCanShureMoveFalsex._num_gradient[1][0] = 20;
	SubCanShureMoveFalsex._num_grayScale[1][0] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[1][0] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[1][0] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[1][0] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[1][0] = 8;
	SubCanShureMoveFalsex._meanValueGradient[1][0] = 40;
	SubCanShureMoveFalsex._bright_num[1][0] = 20;
	SubCanShureMoveFalsex._dark_num[1][0] = 20;

	SubBGRCanShureMoveFalsex._num_gradient[1][0] = 20;
	SubBGRCanShureMoveFalsex._num_grayScale[1][0] = 5;
	SubBGRCanShureMoveFalsex._dst_m_gradient[1][0] = 1;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[1][0] = 8;
	SubBGRCanShureMoveFalsex._dst_s_gradient[1][0] = 1;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[1][0] = 8;
	SubBGRCanShureMoveFalsex._meanValueGradient[1][0] = 40;
	SubBGRCanShureMoveFalsex._bright_num[1][0] = 20;
	SubBGRCanShureMoveFalsex._dark_num[1][0] = 20;

	PointCanShuMaskx._keneralsize[1][0] = 25;
	PointCanShuMaskx._size_x_left[1][0] = 30;
	PointCanShuMaskx._size_x_right[1][0] = 10;
	PointCanShuMaskx._size_y_down[1][0] = 10;
	PointCanShuMaskx._size_y_up[1][0] = 20;

	PointCanShuMaskx._keneralsize[1][0] = 60;
	PointCanShuMaskx._size_x_left[1][0] = 40;
	PointCanShuMaskx._size_x_right[1][0] = 20;
	PointCanShuMaskx._size_y_down[1][0] = 20;
	PointCanShuMaskx._size_y_up[1][0] = 30;

	MuraCanShuMaskx._keneralsize[1][0] = 30;
	MuraCanShuMaskx._size_x_left[1][0] = 70;
	MuraCanShuMaskx._size_x_right[1][0] = 20;
	MuraCanShuMaskx._size_y_down[1][0] = 20;
	MuraCanShuMaskx._size_y_up[1][0] = 20;

	lightCanShuMaskx._keneralsize[1][0] = 30;
	lightCanShuMaskx._size_x_left[1][0] = 70;
	lightCanShuMaskx._size_x_right[1][0] = 20;
	lightCanShuMaskx._size_y_down[1][0] = 20;
	lightCanShuMaskx._size_y_up[1][0] = 20;

	SubCanShuMaskx._keneralsize[1][0] = 30;
	SubCanShuMaskx._size_x_left[1][0] = 70;
	SubCanShuMaskx._size_x_right[1][0] = 20;
	SubCanShuMaskx._size_y_down[1][0] = 20;
	SubCanShuMaskx._size_y_up[1][0] = 20;

	SubBGRCanShuMaskx._keneralsize[1][0] = 30;
	SubBGRCanShuMaskx._size_x_left[1][0] = 70;
	SubBGRCanShuMaskx._size_x_right[1][0] = 20;
	SubBGRCanShuMaskx._size_y_down[1][0] = 20;
	SubBGRCanShuMaskx._size_y_up[1][0] = 20;


	canshuExtern._subThresh[1][0] = 20;
	canshuExtern._subBGRThresh[1][0] = 20;
	canshuExtern._bgrThresh[1][0] = 20;
	canshuExtern._hsvThresh[1][0] = 20;
	canshuExtern._sort_dxy1[1][0] = 15;
	canshuExtern._sort_dxy2[1][0] = 12;
	canshuExtern._sort_dxy3[1][0] = 20;
	canshuExtern._sort_dxy4[1][0] = 20;
	canshuExtern._houghthr[1][0] = 45;  //不同相机位白屏二值化阈值不同（用于hough变换）
	canshuExtern._DUSTHOLD[1][0] = 104;
	canshuExtern._DUSTSHAPE[1][0] = 5;
	canshuExtern._fill[1][0] = 20;
	canshuExtern._Rarea[1][0] = 50;
	canshuExtern._TFTHOLD_points[1][0] = 180;
	canshuExtern._TFTHOLD_muras[1][0] = 180;
	canshuExtern._houghP[1] = 40;

	/*2号相机白屏参数*/

	PointCanShureMoveFalsex._num_gradient[2][0] = 20;
	PointCanShureMoveFalsex._num_grayScale[2][0] = 5;
	PointCanShureMoveFalsex._dst_m_gradient[2][0] = 20;
	PointCanShureMoveFalsex._dst_m_grayScale[2][0] = 8;
	PointCanShureMoveFalsex._dst_s_gradient[2][0] = 20;
	PointCanShureMoveFalsex._dst_s_grayScale[2][0] = 8;
	PointCanShureMoveFalsex._meanValueGradient[2][0] = 40;
	PointCanShureMoveFalsex._bright_num[2][0] = 20;
	PointCanShureMoveFalsex._dark_num[2][0] = 20;


	muraCanShureMoveFalsex._num_gradient[2][0] = 20;
	muraCanShureMoveFalsex._num_grayScale[2][0] = 5;
	muraCanShureMoveFalsex._dst_m_gradient[2][0] = 30;
	muraCanShureMoveFalsex._dst_m_grayScale[2][0] = 8;
	muraCanShureMoveFalsex._dst_s_gradient[2][0] = 30;
	muraCanShureMoveFalsex._dst_s_grayScale[2][0] = 8;
	muraCanShureMoveFalsex._meanValueGradient[2][0] = 200;
	muraCanShureMoveFalsex._bright_num[2][0] = 20;
	muraCanShureMoveFalsex._dark_num[2][0] = 20;

	LightCanShureMoveFalsex._num_gradient[2][0] = 20;
	LightCanShureMoveFalsex._num_grayScale[2][0] = 5;
	LightCanShureMoveFalsex._dst_m_gradient[2][0] = 20;
	LightCanShureMoveFalsex._dst_m_grayScale[2][0] = 8;
	LightCanShureMoveFalsex._dst_s_gradient[2][0] = 20;
	LightCanShureMoveFalsex._dst_s_grayScale[2][0] = 8;
	LightCanShureMoveFalsex._meanValueGradient[2][0] = 40;
	LightCanShureMoveFalsex._bright_num[2][0] = 20;
	LightCanShureMoveFalsex._dark_num[2][0] = 20;


	SubCanShureMoveFalsex._num_gradient[2][0] = 20;
	SubCanShureMoveFalsex._num_grayScale[2][0] = 5;
	SubCanShureMoveFalsex._dst_m_gradient[2][0] = 20;
	SubCanShureMoveFalsex._dst_m_grayScale[2][0] = 8;
	SubCanShureMoveFalsex._dst_s_gradient[2][0] = 20;
	SubCanShureMoveFalsex._dst_s_grayScale[2][0] = 8;
	SubCanShureMoveFalsex._meanValueGradient[2][0] = 40;
	SubCanShureMoveFalsex._bright_num[2][0] = 20;
	SubCanShureMoveFalsex._dark_num[2][0] = 20;


	SubBGRCanShureMoveFalsex._num_gradient[2][0] = 20;
	SubBGRCanShureMoveFalsex._num_grayScale[2][0] = 5;
	SubBGRCanShureMoveFalsex._dst_m_gradient[2][0] = 1;
	SubBGRCanShureMoveFalsex._dst_m_grayScale[2][0] = 8;
	SubBGRCanShureMoveFalsex._dst_s_gradient[2][0] = 1;
	SubBGRCanShureMoveFalsex._dst_s_grayScale[2][0] = 8;
	SubBGRCanShureMoveFalsex._meanValueGradient[2][0] = 40;
	SubBGRCanShureMoveFalsex._bright_num[2][0] = 20;
	SubBGRCanShureMoveFalsex._dark_num[2][0] = 20;


	PointCanShuMaskx._keneralsize[2][0] = 25;
	PointCanShuMaskx._size_x_left[2][0] = 30;
	PointCanShuMaskx._size_x_right[2][0] = 10;
	PointCanShuMaskx._size_y_down[2][0] = 10;
	PointCanShuMaskx._size_y_up[2][0] = 20;

	PointCanShuMaskx._keneralsize[2][0] = 60;
	PointCanShuMaskx._size_x_left[2][0] = 40;
	PointCanShuMaskx._size_x_right[2][0] = 20;
	PointCanShuMaskx._size_y_down[2][0] = 20;
	PointCanShuMaskx._size_y_up[2][0] = 30;

	MuraCanShuMaskx._keneralsize[2][0] = 30;
	MuraCanShuMaskx._size_x_left[2][0] = 70;
	MuraCanShuMaskx._size_x_right[2][0] = 20;
	MuraCanShuMaskx._size_y_down[2][0] = 20;
	MuraCanShuMaskx._size_y_up[2][0] = 20;

	lightCanShuMaskx._keneralsize[2][0] = 30;
	lightCanShuMaskx._size_x_left[2][0] = 70;
	lightCanShuMaskx._size_x_right[2][0] = 20;
	lightCanShuMaskx._size_y_down[2][0] = 20;
	lightCanShuMaskx._size_y_up[2][0] = 20;

	SubCanShuMaskx._keneralsize[2][0] = 30;
	SubCanShuMaskx._size_x_left[2][0] = 70;
	SubCanShuMaskx._size_x_right[2][0] = 20;
	SubCanShuMaskx._size_y_down[2][0] = 20;
	SubCanShuMaskx._size_y_up[2][0] = 20;

	SubBGRCanShuMaskx._keneralsize[2][0] = 30;
	SubBGRCanShuMaskx._size_x_left[2][0] = 70;
	SubBGRCanShuMaskx._size_x_right[2][0] = 20;
	SubBGRCanShuMaskx._size_y_down[2][0] = 20;
	SubBGRCanShuMaskx._size_y_up[2][0] = 20;


	canshuExtern._subThresh[2][0] = 20;
	canshuExtern._subBGRThresh[2][0] = 200;
	canshuExtern._bgrThresh[2][0] = 50;
	canshuExtern._hsvThresh[2][0] = 30;
	canshuExtern._sort_dxy1[2][0] = 15;
	canshuExtern._sort_dxy2[2][0] = 12;
	canshuExtern._sort_dxy3[2][0] = 20;
	canshuExtern._sort_dxy4[2][0] = 20;
	canshuExtern._houghthr[2][0] = 50;//不同相机位白屏二值化阈值不同（用于hough变换）
	canshuExtern._DUSTHOLD[2][0] = 104;
	canshuExtern._DUSTSHAPE[2][0] = 5;
	canshuExtern._fill[2][0] = 20;
	canshuExtern._Rarea[2][0] = 50;
	canshuExtern._TFTHOLD_points[2][0] = 180;
	canshuExtern._TFTHOLD_muras[2][0] = 180;
	canshuExtern._houghP[2] = 40;
}

//CanShuMask

//相机畸变标准
void camera_cali_fisrt(Mat imageSource, float neican_data[], float jibian_data[], Mat newimage)
{
	Size image_size;  /* 图像的尺寸 */
					  //获取图像大小
	image_size.width = imageSource.cols;
	image_size.height = imageSource.rows;
	cout << "image_size.width:" << image_size.width << endl;
	cout << "image_size.height:" << image_size.height << endl;

	/****************** 参数矩阵赋值 *************************/
	//cameraMatrix为测得的相机内参
	Mat cameraMatrix(3, 3, CV_32FC1);
	InitMat(cameraMatrix, neican_data);

	//distCoeffs为测得的畸变系数
	Mat distCoeffs(1, 5, CV_32FC1); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	InitMat(distCoeffs, jibian_data);

	/************************ 相机矫正 ************************/
	Mat R = Mat::eye(3, 3, CV_32F);  //第三个参数R，可选的输入，是第一和第二相机坐标之间的旋转矩阵
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);

	//initUndistortRectifyMap用来计算畸变映射，remap把求得的映射应用到图像上
	initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_16SC2, mapx, mapy);  //CV_32FC1
	remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);

}

/*************************************************   labview   检测输出代码      ****************************************************************/



//!!!
//设定检测图片位置
void setTestData()
{

	//单相机方案需要的数据
	char   imgNameTFT[128];
	char   imgNameDust[128];
	char   tmpNameDust[128];

	//strcpy_s(imgNameTFT, "D:\\project\\vs2017\\huituan.png");
	//strcpy_s(imgNameDust, "D:\\project\\color\\dust.png");
	//strcpy_s(tmpNameDust, "D:\\project\\vs2017\\5620.png");

	strcpy_s(imgNameTFT, "..\\heituan2.png");
	strcpy_s(imgNameDust, "..\\dust.png");
	strcpy_s(tmpNameDust, "..\\13.png");

	//是否使用多相机方案
	//sendImg1.useCameras = false;

	/*********************多相机方案*******************/
	//多相机方案需要增加的数据
	char   name_dust_points[128];
	char   name_tmp[128];
	//strcpy_s(name_dust_points, "\\tuan_img\\3.bmp");
	//strcpy_s(name_tmp, "\\tuan_img\\2.bmp");
	strcpy_s(name_dust_points, "..\\heituan2.png");
	strcpy_s(name_tmp, "..\\13.png");

	/*********************多相机方案*******************/

	//读取相关图片
	Mat imgTFT = imread(imgNameTFT);
	Mat imgDust = imread(imgNameDust);
	Mat dust_points = imread(name_dust_points);
	Mat tmp = imread(name_tmp);
	Mat R_tmp = imread(tmpNameDust);

	//参数初始化
	int TFTHOLD = 130;
	int DUSTHOLD = 50;
	int DUSTSHAPE = 7;
	int dst_m_gradient = 100; //梯度异常判断阈值
	int dst_m_grayScale = 8; //灰度异常判断阈值
	int dst_s_grayScale = 3; //灰度异常判断阈值
	int num_gradient = 4;    //梯度异常总数判断阈值
	int num_grayScale = 5;  //灰度异常总数判断阈值
	int fill = 20;  //灰度异常总数判断阈值
	int meanValueGradient = 20;

	//
	camera_cali_fisrt(imgTFT, neican_data, jibian_data, imgTFT);
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	camera_cali_fisrt(R_tmp, neican_data, jibian_data, R_tmp);

	//
	(*sendImg1).imgTFT[0] = imgTFT;
	(*sendImg1).imgDust = imgDust;
	(*sendImg1).R_tmp = R_tmp;
}


////0108 lyl add
//将模板进行填充 CutTemplate_fill
//计算模板图片的梯度图片并进行保存  GradientTemplate_fill
//修改成多相机模式--ysq0123
//0301新增getROI函数是否显示截取过程
void SetTemplate(Mat image, int cameraFlag, int pictureFlag, bool if_showimg)
{
	/*
	输入：Template[flag]  模板图
	输出：CutTemplate[flag] 模板ROI
	*/
	(*sendImg1).Template[cameraFlag][pictureFlag] = image;
	(*sendImg1).imgTFT[cameraFlag] = image;

	if (pictureFlag == 0)  // 白屏模板
	{
		string sProjectDir = "D://lcmProject//";
		(*sendImg1).projectDir = sProjectDir;
		(*sendImg1).res_tmp_dir = sProjectDir + "R_tmmp//";
		(*sendImg1).pts1_tmp_file = sProjectDir + "R_tmmp//pts1_tmp.txt";

		//【获取ROI图】imgTftColorCut，true表示此时为模板图，会【保存四个角点信息】写在文件中
 		getRoi(cameraFlag, true, if_showimg);

		// 检测模板R角，并裁剪四个R角保存【只在0相机的白屏模板保存R角模板--2019/01/23】
		if (cameraFlag == 0)
			detectRCornorInit(cameraFlag);
	}
	else  // 非白屏模板直接用白屏的旋转信息进行ROI获取
	{
		int rows = (*sendImg1).imgTFT[cameraFlag].rows;
		int cols = (*sendImg1).imgTFT[cameraFlag].cols;

		Point2f center = (*sendImg1).get_rotate_rect[cameraFlag].center;
		Size2f size = (*sendImg1).get_rotate_rect[cameraFlag].size;
		float angle = (*sendImg1).get_rotate_rect[cameraFlag].angle;
		Mat res;
		if (angle <= -45)
		{
			angle += 90;
		}
		//cout << "angle:" << angle << endl;

		//根据计算的角度进行角度旋转
		Mat M = getRotationMatrix2D(center, angle, 1);
		warpAffine((*sendImg1).imgTFT[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size(cols, rows));

		Mat M1 = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);
		//进行透视变换
		warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M1, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
	}

	// 写入模板ROI
	(*sendImg1).CutTemplate[cameraFlag][pictureFlag] = (*sendImg1).imgTftColorCut[cameraFlag].clone();
	//0108 lyl add
	//将模板进行填充
	//计算模板图片的梯度图片并进行保存
	(*sendImg1).CutTemplate_fill[cameraFlag][pictureFlag] = fill((*sendImg1).CutTemplate[cameraFlag][pictureFlag], 20);  // fill =20
	pointDetect((*sendImg1).CutTemplate[cameraFlag][pictureFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][pictureFlag]);
	
	
	//如果是0号相机白屏，创建手机轮廓图片  227
	if ((cameraFlag == 0) && (pictureFlag == 0))
	{
		getBorder(cameraFlag, sendImg1);
		setEachMethodMask();
	}


}

bool judgment(Mat image, int cameraFlag, int flag)
{
	Mat img = image.clone();
	//imwrite("D:\\project\\img.png", img);
	Mat img1 = (*sendImg1).Template[cameraFlag][flag].clone();
	//imwrite("D:\\project\\img1.png", img1);
	cvtColor(img1, img1, COLOR_BGR2GRAY);
	double ret1 = cv::threshold(img1, img1, 10, 255, THRESH_BINARY);
	//imwrite("D:\\project\\img1thre.png", img1);
	cvtColor(img, img, COLOR_BGR2GRAY);
	double ret2 = cv::threshold(img, img, 10, 255, THRESH_BINARY);
	//imwrite("D:\\project\\imgthre.png", img);
	Mat diffImg;
	absdiff(img, img1, diffImg);
	//imwrite("D:\\project\\diffImg.png", diffImg);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(diffImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	double size = 0;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		//cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
		size = size + fabs(g_dConArea);
	}
	cout << "用judgment轮廓面积计算函数计算出来的总面积为：" << size << endl;
	if (size < 1000000000)   // 确定阈值
	{
		return true;
	}
	else
	{
		int x = MessageBox(GetForegroundWindow(), "图片颜色 or 角度异常", "", MB_OKCANCEL);
		if (x == 2)
		{
			exit(0);
		}
		return false;
	}
}

bool judgment1(Mat image, int cameraFlag, int flag)
{
	Mat img = image.clone();
	//imwrite("D:\\project\\tmp.png", img);
	Mat img1 = (*sendImg1).CutTemplate[cameraFlag][flag].clone();
	//imwrite("D:\\project\\tmp1.png", img1);
	cvtColor(img1, img1, COLOR_BGR2GRAY);
	double ret1 = cv::threshold(img1, img1, 10, 255, THRESH_BINARY);
	//imwrite("D:\\project\\tmp1thre.png", img1);
	cvtColor(img, img, COLOR_BGR2GRAY);
	double ret2 = cv::threshold(img, img, 10, 255, THRESH_BINARY);
	//imwrite("D:\\project\\tmpthre.png", img);
	Mat diffImg;
	absdiff(img, img1, diffImg);
	//imwrite("D:\\project\\diffImgtmp.png", diffImg);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(diffImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	double size = 0;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		//cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
		size = size + fabs(g_dConArea);
	}
	cout << "用judgment1轮廓面积计算函数计算出来的总面积为：" << size << endl;
	if (size < 1000000)   // 确定阈值
	{
		return true;
	}
	else
	{
		int x = MessageBox(GetForegroundWindow(), "图片ROI异常", "", MB_OKCANCEL);
		if (x == 2)
		{
			exit(0);
		}
		return false;
	}
}



//修改成多相机模式--ysq0123
void processDust(Mat imgDust,  CanShu * canshuX, int cameraFlag)
{
	(*sendImg1).imgDust = imgDust.clone();
	// 灰尘图直接由cameraFlag相应的相机白屏来确定灰尘角点--0123
	Point center = (*sendImg1).get_rotate_rect[cameraFlag].center;
	double angle = (*sendImg1).get_rotate_rect[cameraFlag].angle;
	vector<Point2f>pts_dust = (*sendImg1).pts1[cameraFlag];

	// 多相机场景，灰尘图有单独的白屏来确定灰尘角点
	//此时不再使用tft角点
	/*
	if (if_multi_camera)
	{
	Mat imgdust_tmpColorCut;

	RotatedRect rect;  // 灰尘图用到rect的angle和center
	Mat dst;
	Point2f box[4];
	//1.对图片进行角度矫正
	img_rotate(imgdust_tmp, box, rect, imgdust_tmpColorCut, false, false);
	vector<int>sort_dxy_dust;  //12.29修改sort_point带参数
	sort_dxy_dust.push_back(210);
	sort_dxy_dust.push_back(20);
	sort_dxy_dust.push_back(80);
	sort_dxy_dust.push_back(100);
	hough_detect(imgdust_tmpColorCut, pts_dust, sort_dxy_dust, 80,false);

	//获取旋转结果
	center = rect.center;
	angle = rect.angle;

	}
	*/


	//*********************  计算灰尘   ******************************************//
	//旋转截取灰尘屏区域
	int rows = imgDust.rows;
	int cols = imgDust.cols;
	if (angle <= -45)
	{
		angle += 90;
	}
	Mat M(rows, cols, imgDust.type());
	M = getRotationMatrix2D(center, angle, 1);
	/*灰尘图旋转*/
	warpAffine(imgDust, imgDust, M, imgDust.size());

	//旋转->仿射变换->截取
	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/
	////img_rotate(imgTFT, res_path, box, rect, imgTFT_out, false, false);
	////变换后分别在左上、右上、右下、左下四个点
	////pts2 = np.float32([[0, 0], [1130, 0], [1130, 2280], [0, 2280]])
	//vector<Point2f> pts1_dust;
	//hough_detect(imgDust, pts1_dust, true);   // 灰尘图杂物太多 不能直接霍夫检测角点
	//而是要直接用tft的角点信息




	//生成透视变换矩阵
	Mat M1 = getPerspectiveTransform(pts_dust, (*sendImg1).WhiteScreenQuadrilateral);
	//进行透视变换
	warpPerspective(imgDust, imgDust, M1, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
	//resize(imgDust, imgDust, Size((*sendImg1).PersPectiveTransformation_hight, (*sendImg1).PersPectiveTransformation_weight));
	(*sendImg1).imgDustColorCut = imgDust.clone();



	//roi区域边界各自内缩10个像素
	//int shrink_size = (*sendImg1).shrink_size;
	//int w_TFT, h_TFT;
	//w_TFT = imgDust.rows;
	//h_TFT = imgDust.cols;
	//imgDust = imgDust(Range(shrink_size, w_TFT - shrink_size), Range(shrink_size, h_TFT - shrink_size));

	////12.17 不进行内缩了
	//只內缩左、右和上
	//imgDust = imgDust(Range(shrink_size, w_TFT ), Range(shrink_size, h_TFT - shrink_size));
	//cout << "imgDust.rows" << imgDust.rows << "imgDust.cols" << imgDust.cols << endl;
	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/


	//roi区域边界各内缩10个像素
	/*vector<Point2f>shrink_box;
	shrink_poly((*sendImg2).box, shrink_box, (*sendImg2).shrink_size);
	////imgDUST = imgDUST(Rect(box[0].x, box[0].y, box[1].y - box[0].y, box[2].x - box[0].x));//要注意点的顺序
	imgDust = imgDust(Rect(shrink_box[1].x, shrink_box[1].y, shrink_box[2].x - shrink_box[1].x, shrink_box[0].y - shrink_box[1].y));
	*/


	/*计算图像梯度 -----哪个梯度算子是更好的*/
	Mat kernel2 = (Mat_<char>(9, 9) <<
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, 8, 8, 8, -1, -1, -1,
		-1, -1, -1, 8, 8, 8, -1, -1, -1,
		-1, -1, -1, 8, 8, 8, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1);
	//处理灰尘屏区域
	cvtColor(imgDust, imgDust, COLOR_BGR2GRAY);
	/*图像膨胀，膨胀灰尘*/
	Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	dilate(imgDust, imgDust, kernel3);
	//获取灰尘梯度图
	cv::filter2D(imgDust, (*sendImg1).gradientDUST, -1, kernel2);

	double ret3 = cv::threshold((*sendImg1).gradientDUST, (*sendImg1).gradientDustThreshold, (*canshuX)._DUSTHOLD[cameraFlag][0], 255, THRESH_BINARY);

	//膨胀二值化的灰尘梯度图，方便查看灰尘
	Mat showDUST = (*sendImg1).gradientDustThreshold.clone();
	Mat kernel4 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));   
	dilate(showDUST, showDUST, kernel4);
	vector<vector<Point>>contours2;
	Mat hierarchy2;
	cv::findContours(showDUST, contours2, hierarchy2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	//在彩图上显示灰尘缺陷
	cv::drawContours((*sendImg1).imgDustColorCut, contours2, -1, Scalar(0, 0, 255), 2);
	int DUSTshape = (*canshuX)._DUSTSHAPE[cameraFlag][0];
	Mat kernel5 = cv::getStructuringElement(cv::MORPH_RECT, Size(DUSTshape, DUSTshape));
	dilate((*sendImg1).gradientDustThreshold, (*sendImg1).gradientDustThresholDilate, kernel5);

	//imwrite("D://project//Dustthreshold.png", (*sendImg1).gradientDustThreshold);
	imwrite((*sendImg1).sNowTimeFileDir + "gradientDustThreshold.png", (*sendImg1).gradientDustThreshold);
	imwrite((*sendImg1).sNowTimeFileDir + "gradientDustThresholDilate.png", (*sendImg1).gradientDustThresholDilate);
}



//多种检测方法的多线程代码
/*
0:检测点缺陷
1.检测mura缺陷
2.检测漏光
3 检测差分图
*/

//1.9 add sub函数的 缩进

//0112 lyl 修改了mura检测的过程
//增加了三通道检测
//新注册了全局变量 (*sendImg1).gradientTFTg_muras G通道的梯度图
//目前采取的方式是融合 三通道+gray通道的梯度图，后续进行二值化的操作
//目前点和mura的检测都采用了三通道，效果上有了提升，以后应该使用RGB三通道检测
//mura  RGB 屏幕统一使用 130 作为阈值，白屏的 要高一些   点 150 mura 160
void detectDifferentTypeDefect(int DefectType, int cameraFlag, int pictureFlag)
{

	//cout << "进入多线程函数" << endl;
	switch (DefectType)
	{

		// 检测点缺陷
	case(0): {

		// 灰度图检测
		drawIn((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColor_point[cameraFlag], (*sendImg1).shrink_size_point);
		// fill 【填充】（在内缩之后）
		(*sendImg1).imgTftColor_point[cameraFlag] = fill((*sendImg1).imgTftColor_point[cameraFlag], (*canshuX)._fill[cameraFlag][pictureFlag]);


		Mat imgOut_b;
		Mat imgOut_g;
		Mat imgOut_r;
		Mat channels[3];
		int RGB_Threshold;

		if (pictureFlag == 0)
		{
			RGB_Threshold = (*canshuX)._TFTHOLD_points[cameraFlag][pictureFlag];
		}
		else
		{
			RGB_Threshold = (*canshuX)._TFTHOLD_muras[cameraFlag][pictureFlag];
		}

		//必须分通道
		split((*sendImg1).imgTftColor_point[cameraFlag], channels);//分割imgTftColor_mura的通道
																   //gradientTFT = muraDetect(imgTFT_gray);
		pointDetect(channels[0], imgOut_b);
		pointDetect(channels[1], imgOut_g);
		pointDetect(channels[2], imgOut_r);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_b_point" + ".jpg", imgOut_b);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_g_point" + ".jpg", imgOut_g);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_r_point" + ".jpg", imgOut_r);


		cv::threshold(imgOut_b, imgOut_b, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_g, imgOut_g, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_r, imgOut_r, RGB_Threshold, 255, THRESH_BINARY);  //100
																			   //增加了全局变量 (*sendImg1).gradientTFTg_point 与原始的灰度梯度图二值化后的结果或在一起
		bitwise_or(imgOut_b, imgOut_g, (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag]);
		bitwise_or(imgOut_r, (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag]);




		cvtColor((*sendImg1).imgTftColor_point[cameraFlag], (*sendImg1).imgTFTpoint_gray[cameraFlag], CV_BGR2GRAY);
		pointDetect((*sendImg1).imgTFTpoint_gray[cameraFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag]);
		cv::threshold((*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag], RGB_Threshold, 255, THRESH_BINARY);
		bitwise_or((*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag]);
		//bitwise_or((*sendImg1).gradientTFTbgr_point[flag], (*sendImg1).GradientResult_point[flag], (*sendImg1).gradientTFTbgr_point[flag]);
		//repadding((*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).shrink_size_point);
		repadding((*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).shrink_size_point);
		//imwrite((*sendImg1).sNowTimeFileDir + "GradientResult_point" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).GradientResult_point[cameraFlag][pictureFlag]);

		//GaussianBlur(imgOut_g, imgOut_g, Size(9, 9), 0, 0, BORDER_DEFAULT);
		//cv::medianBlur(imgOut_g, imgOut_g, 3);
		//GaussianBlur(imgOut_g, imgOut_g, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_g, imgOut_g, (*sendImg1).shrink_size_point);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOutpoint_g" + to_string(flag) + ".jpg", imgOut_g);
		//cv::threshold(imgOut_g, (*sendImg1).gradientTFTg_point[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		//GaussianBlur(imgOut_b, imgOut_b, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_b, imgOut_b, (*sendImg1).shrink_size_point);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOutpoint_b" + to_string(flag) + ".jpg", imgOut_b);
		//cv::threshold(imgOut_b, (*sendImg1).gradientTFTb_point[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		//GaussianBlur(imgOut_r, imgOut_r, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_r, imgOut_r, (*sendImg1).shrink_size_point);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOutpoint_r" + to_string(flag) + ".jpg", imgOut_r);
		//cv::threshold(imgOut_r, (*sendImg1).gradientTFTr_point[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		//写入 test
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTg_point" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTg_point[flag]);
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTb_point" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTb_point[flag]);
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTr_point" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTr_point[flag]);

		//cvtColor((*sendImg1).imgTftColor_point, (*sendImg1).imgTFTpoint_gray, CV_BGR2GRAY);
		//pointDetect((*sendImg1).imgTFTpoint_gray, (*sendImg1).GradientResult_point[flag]);
		//repadding((*sendImg1).GradientResult_point[flag], (*sendImg1).GradientResult_point[flag], (*sendImg1).shrink_size_point);
		//cout << "2222222222" << endl;
		//cout << "repadding1.row:" << (*sendImg1).gradientTFT_point.rows << endl;
		//cout << "repadding1.col:" << (*sendImg1).gradientTFT_point.cols << endl;
		//imwrite((*sendImg1).sNowTimeFileDir + "GradientResult_point" + to_string(flag) + ".jpg", (*sendImg1).GradientResult_point[flag]);

		// 灰度图检测
		//cvtColor((*sendImg1).imgTftColor_mura, (*sendImg1).imgTFTmura_gray, CV_BGR2GRAY);
		//muraDetect((*sendImg1).imgTFTmura_gray, (*sendImg1).gradientTFT_muras[flag]);
		//添加梯度图的写 test 0109
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFT_muras" + to_string(flag) + ".jpg", (*sendImg1).gradientTFT_muras[flag]);

		//repadding((*sendImg1).gradientTFT_muras[flag], (*sendImg1).gradientTFT_muras[flag], (*sendImg1).shrink_size_mura);

		break; // 可选的
	};

			 // 检测mura缺陷
	case(1): {

		// 内缩+填充
		Mat imgMura = (*sendImg1).imgTftColorCut[cameraFlag].clone();
		drawIn(imgMura, imgMura, (*sendImg1).shrink_size_mura);
		(*sendImg1).imgTftColor_mura[cameraFlag] = fill(imgMura, (*canshuX)._fill[cameraFlag][pictureFlag]);
		Mat imgOut_b;
		Mat imgOut_g;
		Mat imgOut_r;
		Mat channels[3];


		int RGB_Threshold_b, RGB_Threshold_g, RGB_Threshold_r;
		//白屏阈值与其他不一样
		if (pictureFlag == 0)
		{
			RGB_Threshold_b = RGB_Threshold_g = RGB_Threshold_r = (*canshuX)._TFTHOLD_muras[cameraFlag][pictureFlag];
		}
		else
		{
			RGB_Threshold_b = (*canshuX)._TFTHOLD_muras[cameraFlag][pictureFlag];
			RGB_Threshold_g = (*canshuX)._TFTHOLD_muras[cameraFlag][pictureFlag];
			RGB_Threshold_r = (*canshuX)._TFTHOLD_muras[cameraFlag][pictureFlag];
		}

		//必须分通道
		split((*sendImg1).imgTftColor_mura[cameraFlag], channels);//分割imgTftColor_mura的通道
																  //gradientTFT = muraDetect(imgTFT_gray);
		muraDetect(channels[0], imgOut_b);
		muraDetect(channels[1], imgOut_g);
		muraDetect(channels[2], imgOut_r);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_b_mura"  + ".jpg", imgOut_b);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_g_mura"  + ".jpg", imgOut_g);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_r_mura"  + ".jpg", imgOut_r);


		cv::threshold(imgOut_b, imgOut_b, RGB_Threshold_b, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_g, imgOut_g, RGB_Threshold_g, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_r, imgOut_r, RGB_Threshold_r, 255, THRESH_BINARY);  //100
																			   //增加了全局变量 (*sendImg1).gradientTFTg_muras 与原始的灰度梯度图二值化后的结果或在一起
		bitwise_or(imgOut_b, imgOut_g, (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);
		bitwise_or(imgOut_r, (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);

		// 灰度图检测
		cvtColor((*sendImg1).imgTftColor_mura[cameraFlag], (*sendImg1).imgTFTmura_gray[cameraFlag], CV_BGR2GRAY);
		muraDetect((*sendImg1).imgTFTmura_gray[cameraFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag]);
		cv::threshold((*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], RGB_Threshold_b, 255, THRESH_BINARY);
		bitwise_or((*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag]);

		//!!!
		//repadding((*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).shrink_size_mura);
		repadding((*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).shrink_size_mura);
		imwrite((*sendImg1).sNowTimeFileDir + "gradientTFT_muras" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag]);


		//cv::threshold((*sendImg1).gradientTFTbgr_muras[flag], (*sendImg1).gradientTFTbgr_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		//GaussianBlur(imgOut_g, imgOut_g, Size(9, 9), 0, 0, BORDER_DEFAULT);
		//cv::medianBlur(imgOut_g, imgOut_g, 3);
		//GaussianBlur(imgOut_g, imgOut_g, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_g, imgOut_g, (*sendImg1).shrink_size_mura);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOut_g" + to_string(flag) + ".jpg", imgOut_g);
		//cv::threshold(imgOut_g, (*sendImg1).gradientTFTg_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		////GaussianBlur(imgOut_b, imgOut_b, Size(5, 5), 0, 0, BORDER_DEFAULT);m
		//repadding(imgOut_b, imgOut_b, (*sendImg1).shrink_size_mura);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOut_b" + to_string(flag) + ".jpg", imgOut_b);
		//cv::threshold(imgOut_b, (*sendImg1).gradientTFTb_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		////GaussianBlur(imgOut_r, imgOut_r, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_r, imgOut_r, (*sendImg1).shrink_size_mura);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOut_r" + to_string(flag) + ".jpg", imgOut_r);
		//cv::threshold(imgOut_r, (*sendImg1).gradientTFTr_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		////写入 test
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTg_muras" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTg_muras[flag]);
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTb_muras" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTb_muras[flag]);
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTr_muras" + to_string(flag) + ".jpg", (*sendImg1).gradientTFTr_muras[flag]);

		//repadding((*sendImg1).gradientTFT_muras[flag], (*sendImg1).gradientTFT_muras[flag], (*sendImg1).shrink_size_mura);
		//cout << "1111111111" << endl;
		////添加梯度图的写 test 0109
		//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras before" + to_string(cameraFlag) + "_" + to_string(pictureFlag) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);
		//cout << "repadding2.row:" << (*sendImg1).GradientResult_mura[flag].rows << endl;
		//cout << "repadding2.col:" << (*sendImg1).GradientResult_mura[flag].cols << endl;

		break; // 可选的

	};


			 // 检测漏光
	case(2): {

		Mat imgLouguang = (*sendImg1).imgTftColorCut[cameraFlag].clone();
		drawIn(imgLouguang, imgLouguang, (*sendImg1).shrink_size_louguang);
		(*sendImg1).imgTftColor_louguang[cameraFlag] = fill(imgLouguang, (*canshuX)._fill[cameraFlag][pictureFlag]);
		// 彩图检测
		getLightLeakArea((*sendImg1).imgTftColor_louguang[cameraFlag], (*sendImg1).GradientResult_louguang[cameraFlag][pictureFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][pictureFlag], cameraFlag, pictureFlag);
		repadding((*sendImg1).gradientResultThreshold_louguangs[cameraFlag][pictureFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][pictureFlag], (*sendImg1).shrink_size_louguang);
		repadding((*sendImg1).GradientResult_louguang[cameraFlag][pictureFlag], (*sendImg1).GradientResult_louguang[cameraFlag][pictureFlag], (*sendImg1).shrink_size_louguang);




		break; // 可选的


	}

			 // 检测差分图
	case(3): {

		//subTemplate((*sendImg1).subtractTemplateTftImg, (*sendImg1).subtractTemplateTftImgs[flag]);
		//namedWindow("SSSS", 0);
		//imshow("SSSS", (*sendImg1).subtractTemplateTftImgs[flag]);
		//waitKey(0);


		absdiff((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).CutTemplate[cameraFlag][pictureFlag], (*sendImg1).subtractTemplateTftImg[cameraFlag]);
		drawIn((*sendImg1).subtractTemplateTftImg[cameraFlag], (*sendImg1).subtractTemplateTftImg[cameraFlag], (*sendImg1).shrink_size_subTmp);
		subTemplate((*sendImg1).subtractTemplateTftImg[cameraFlag], (*sendImg1).subtractTemplateTftImgs[cameraFlag][pictureFlag]);
		repadding((*sendImg1).subtractTemplateTftImgs[cameraFlag][pictureFlag], (*sendImg1).subtractTemplateTftImgs[cameraFlag][pictureFlag], (*sendImg1).shrink_size_subTmp);


		//namedWindow("SSSS", 0);
		//imshow("SSSS", (*sendImg1).subtractTemplateTftImgs[flag]);
		//waitKey(0);


		break; // 可选的


	}
	// 新添加一种检测方法，使用bgr-bgr50
	case(4): {



		Mat channels[3];
		//必须分通道
		drawIn((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColor_bgr_sub[cameraFlag], (*sendImg1).shrink_size_subBGR);

		//imwrite((*sendImg1).sNowTimeFileDir + "imgTftColorCut" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).imgTftColorCut[cameraFlag]);

		split((*sendImg1).imgTftColor_bgr_sub[cameraFlag], channels);//分割imgTftColor_mura的通道

		Mat b500;
		Mat b30;
		Mat b50;
		Mat b0_50;
		Mat b0_50threshold;

		Mat bsub;
		blur(channels[0], b500, Size(500, 500));//openCV库自带的均值滤波函数													   
		blur(channels[0], b30, Size(30, 30));//openCV库自带的均值滤波函数	
		blur(channels[0], b50, Size(50, 50));//openCV库自带的均值滤波函数
		//absdiff(b500, b30, bsub);
		absdiff(channels[0], b50, b0_50);
		//absdiff(channels[0], b30, b0_50);



		Mat g500;
		Mat g30;
		Mat g50;
		Mat g0_50;
		Mat g0_50threshold;
		Mat gsub;
		blur(channels[1], g500, Size(500, 500));//openCV库自带的均值滤波函数													   
		blur(channels[1], g30, Size(30, 30));//openCV库自带的均值滤波函数
		blur(channels[1], g50, Size(50, 50));//openCV库自带的均值滤波函数
		//absdiff(g500, g30, gsub);
		absdiff(channels[1], g50, g0_50);
		//absdiff(channels[1], g30, g0_50);


		Mat r500;
		Mat r30;
		Mat r50;
		Mat r0_50;
		Mat r0_50threshold;
		Mat rsub;
		blur(channels[2], r500, Size(500, 500));//openCV库自带的均值滤波函数													   
		blur(channels[2], r30, Size(30, 30));//openCV库自带的均值滤波函数	
		blur(channels[1], r50, Size(50, 50));//openCV库自带的均值滤波函数
		//absdiff(r500, r30, rsub);
		absdiff(channels[2], r50, r0_50);
		//absdiff(channels[2], r30, r0_50);

		imwrite((*sendImg1).sNowTimeFileDir + "b0_50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b0_50);
		imwrite((*sendImg1).sNowTimeFileDir + "g0_50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g0_50);
		imwrite((*sendImg1).sNowTimeFileDir + "r0_50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r0_50);



		//cv::threshold(bsub, bsub, 10, 255, THRESH_BINARY);  //100         这个二值化阈值有待商榷
		//cv::threshold(gsub, gsub, 10, 255, THRESH_BINARY);  //100
		//cv::threshold(rsub, rsub, 10, 255, THRESH_BINARY);  //100


		Mat result;
		Mat resultthreshold;
		add(b0_50, g0_50, result);
		add(result, r0_50, result);

		//namedWindow("sub_result",0);
		//imshow("sub_result", result);
		//waitKey(0);

		(*sendImg1).imgTftColor_sub_result[cameraFlag][pictureFlag] = result.clone();
		(*sendImg1).bgrsubTemplateImgs[cameraFlag][pictureFlag] = result.clone();

		cout << "(*canshuX)._subBGRThresh:" << (*canshuX)._subBGRThresh<< endl;
		cv::threshold(b0_50, b0_50threshold, (*canshuX)._subBGRThresh[cameraFlag][pictureFlag], 255, THRESH_BINARY);  //100   20
		cv::threshold(g0_50, g0_50threshold, (*canshuX)._subBGRThresh[cameraFlag][pictureFlag], 255, THRESH_BINARY);  //100
		cv::threshold(r0_50, r0_50threshold, (*canshuX)._subBGRThresh[cameraFlag][pictureFlag], 255, THRESH_BINARY);  //100





		cv::threshold(result, (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][pictureFlag], (*canshuX)._subBGRThresh[cameraFlag][pictureFlag], 255, THRESH_BINARY);  //100
		imwrite((*sendImg1).sNowTimeFileDir + "threshresult" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][pictureFlag]);
		imwrite((*sendImg1).sNowTimeFileDir + "bgrsubTemplateImgs" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).bgrsubTemplateImgs[cameraFlag][pictureFlag]);
		//namedWindow("sub_threshresult", 0);
		//imshow("sub_threshresult", result);
		//waitKey(0);


	   //cv::threshold(result, resultthreshold, 0, 255, THRESH_BINARY);  //100


		imwrite((*sendImg1).sNowTimeFileDir + "b50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b50);
		imwrite((*sendImg1).sNowTimeFileDir + "g50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g50);
		imwrite((*sendImg1).sNowTimeFileDir + "r50" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r50);

		imwrite((*sendImg1).sNowTimeFileDir + "b" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", channels[0]);
		imwrite((*sendImg1).sNowTimeFileDir + "g" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", channels[1]);
		imwrite((*sendImg1).sNowTimeFileDir + "r" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", channels[2]);

		//imwrite((*sendImg1).sNowTimeFileDir + "b500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b500);
		//imwrite((*sendImg1).sNowTimeFileDir + "g500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g500);
		//imwrite((*sendImg1).sNowTimeFileDir + "r500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r500);


		//imwrite((*sendImg1).sNowTimeFileDir + "bsub" + ".jpg", bsub);
		//imwrite((*sendImg1).sNowTimeFileDir + "gsub" + ".jpg", gsub);
		//imwrite((*sendImg1).sNowTimeFileDir + "rsub"  + ".jpg", rsub);



		imwrite((*sendImg1).sNowTimeFileDir + "b0_50threshold" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b0_50threshold);
		imwrite((*sendImg1).sNowTimeFileDir + "g0_50threshold" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g0_50threshold);
		imwrite((*sendImg1).sNowTimeFileDir + "r0_50threshold" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r0_50threshold);



		imwrite((*sendImg1).sNowTimeFileDir + "imgTftColor_sub_result" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", (*sendImg1).imgTftColor_sub_result[cameraFlag][pictureFlag]);

		repadding((*sendImg1).bgrsubTemplateImgs[cameraFlag][pictureFlag], (*sendImg1).bgrsubTemplateImgs[cameraFlag][pictureFlag], (*sendImg1).shrink_size_subBGR);
		repadding((*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][pictureFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][pictureFlag], (*sendImg1).shrink_size_subBGR);






		break; // 可选的


	}


	default:
		break;
	}





}




////12.17 改成三种方式检测
// 四种检测方法
//修改成多相机模式--ysq0123
//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
void detect(Mat imgTFT, int cameraFlag, int pictureFlag, bool if_showimg)
{
	(*sendImg1).imgTFT[cameraFlag] = imgTFT;   //放入待检测图片

	/****************** result为第一次判断检测图与模板图差异是否太大的结果  ****************************/
	long t11 = GetTickCount();
	bool result = judgment((*sendImg1).imgTFT[cameraFlag], cameraFlag, pictureFlag);
	long t12 = GetTickCount();
	cout << "judgment-1函数运行的时间为：" << (t12 - t11) << endl;  //0.2S-0.25S


	long t1 = GetTickCount();


	if (result == true)  // 这次judgment在于看旋转偏差，偏差不大则进行检测图的【ROI获取+R角检测】
	{
		if (pictureFlag == 0)  //如果是白屏创建工程文件夹，计算iou
		{
			if (cameraFlag == 0)//【只0相机时创建工程文件目录，避免重复创建--0123】
			{
				//创建存放中间检测过程以及最终检测结果图片的文件夹
				string sProjectDir = "D://lcmProject//";
				(*sendImg1).projectDir = sProjectDir;
				(*sendImg1).sNowTimeFileDir = mkdirUseTime(sProjectDir);  //使用时间创建一个文件夹
				//0号相机白屏增加获取手机border
				getBorder(cameraFlag, sendImg1);
				
			}


			//告诉detect检测到第几张图的标准位清零
			(*sendImg1).MaxFlag[cameraFlag] = 0;

			//白屏时候进行漏光检测
			//getLightLeakArea();


			//检测图【ROI获取】（同时对检测到的四个角点坐标进行与模板的角点匹配检测【只对0相机进行角点匹配--0123】）
			getRoi(cameraFlag, false, if_showimg);
			//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
			if (cameraFlag == 1)
				return;


			//只白屏检测图做【R角检测】（与模板R角相减）
			// 【只0相机做R角检测--0123】
			if (cameraFlag == 0)
				getRCornorResult(cameraFlag);


		}

		else  // 非白屏直接用白屏的旋转数据进行【ROI获取】，不进行R角检测
		{
			int rows = (*sendImg1).imgTFT[cameraFlag].rows;
			int cols = (*sendImg1).imgTFT[cameraFlag].cols;

			Point2f center = (*sendImg1).get_rotate_rect[cameraFlag].center;
			Size2f size = (*sendImg1).get_rotate_rect[cameraFlag].size;
			float angle = (*sendImg1).get_rotate_rect[cameraFlag].angle;
			Mat res;
			if (angle <= -45)
			{
				angle += 90;
			}
			//根据计算的角度进行角度旋转
			Mat M = getRotationMatrix2D(center, angle, 1);
			warpAffine((*sendImg1).imgTFT[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size(cols, rows));

			Mat M1 = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);
			//进行透视变换
			warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M1, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		}

		//long t2 = GetTickCount();
		//cout << "Roi—time：" << (t2 - t1) << endl;
		//imwrite("D:\\project\\imgTftColorCut111111.png", (*sendImg1).imgTftColorCut);
		//roi区域比对



		/************************ result1为第二次进一步判断检测图与模板图差异是否太大的结果 （在第一次为true前提下） ***************/

		long t55 = GetTickCount();
		////4.！！！需要修改成多相机模式
		bool result1 = judgment1((*sendImg1).imgTftColorCut[cameraFlag], cameraFlag, pictureFlag);
		long t56 = GetTickCount();
		//cout << "judgment-2函数运行的时间为：" << (t56 - t55) << endl;  //0.2S-0.25S


		int back_row = (*sendImg1).imgTftColorCut[cameraFlag].rows;
		int back_col = (*sendImg1).imgTftColorCut[cameraFlag].cols;


		(*sendImg1).P[0][0].x = 0;
		(*sendImg1).P[0][0].y = 0;
		(*sendImg1).P[0][1].x = 100;
		(*sendImg1).P[0][1].y = 100;
		(*sendImg1).P[1][0].x = back_col - 101;
		(*sendImg1).P[1][0].y = 0;
		(*sendImg1).P[1][1].x = back_col - 1;
		(*sendImg1).P[1][1].y = 100;
		(*sendImg1).P[2][0].x = back_col - 101;
		(*sendImg1).P[2][0].y = back_row - 101;
		(*sendImg1).P[2][1].x = back_col - 1;
		(*sendImg1).P[2][1].y = back_row - 1;
		(*sendImg1).P[3][0].x = 0;
		(*sendImg1).P[3][0].y = back_row - 101;
		(*sendImg1).P[3][1].x = 100;
		(*sendImg1).P[3][1].y = back_row - 1;


		Mat img_back(back_row, back_col, CV_8UC3, Scalar(255, 255, 255));
		(*sendImg1).back = img_back.clone();
		if (result1 == true)  // 这次judgment1在于看是否存在大面积缺陷
		{
			//ROI图像预处理  【获取边界轮廓】
			long t57 = GetTickCount();
			getBorder(cameraFlag, sendImg1);
			long t58 = GetTickCount();
			cout << "process函数获取边界运行的时间为：" << (t58 - t57) << endl;

			//无内缩直接fill填充图片
			//long t66 = GetTickCount();
			(*sendImg1).imgTftColorCutFills[cameraFlag][pictureFlag] = fill((*sendImg1).imgTftColorCut[cameraFlag], (*canshuX)._fill[cameraFlag][pictureFlag]);
			// 检测图ROI 与 模板图ROI 相减，得到差分图用于后续辅助缺陷检测
			//absdiff((*sendImg1).imgTftColorCut, (*sendImg1).CutTemplate[flag], (*sendImg1).subtractTemplateTftImg);
			(*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag] = (*sendImg1).imgTftColorCut[cameraFlag].clone();
			//(*sendImg1).imgTftColorCutFills[flag] = (*sendImg1).imgTftColor_point.clone();



			//【多线程缺陷检测】  点，Mura,漏光
			/*
			0:检测点缺陷
			1.检测mura缺陷
			2.检测漏光
			3 检测差分图
			*/




			long t8 = GetTickCount();
			//for (int numthread = 0; numthread < 5; numthread++)
			//{
			//	thread t(detectDifferentTypeDefect, numthread, cameraFlag, pictureFlag);
			//	t.detach();

			//}
			for (int defecttype = 0; defecttype < 5; defecttype++) 
			{
				detectDifferentTypeDefect(defecttype, cameraFlag, pictureFlag);
			}
			
			long t9 = GetTickCount();
			cout << "点线梯度计算函数运行的时间为：" << (t9 - t8) << endl;


			//计算梯度 
			//gradient_sobel_channel3_or(sendImg1);
			//(*sendImg1).GradientResult[flag] = (*sendImg1).gradientTFT.clone();
			/***********************/

			(*sendImg1).MaxFlag[cameraFlag] = (*sendImg1).MaxFlag[cameraFlag] + 1;
		}
	}
}



////12.17 lyl test//
//点的结果显示
//1.4  lyl update 添加flag 如果flag = true则显示瑕疵信息，false则隐藏瑕疵的具体信息

//点的用红框标记，mura用黄框标记，subTmp的用品红色的标记
//待解决，结果与0107版本的不一样，少了划痕和mura团的检测结果
//划痕是点线和mura检测检出来的
//0114 lyl (*sendImg1).gradientTFTg_point，gradientTFTb_point，gradientTFTr_point 将这三张图和灰度图的二值化图或在一起后，进行后续减边除尘的操作
//该函数流程为：先进行mask掩模操作，再减边除尘，最后除去假缺陷
//0115 lyl add 
//0118 lyl test
void getOneColorPointResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).GradientResult_point[cameraFlag][imgFlag], 0.0, 0.0);

	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], 0.0, 0.0);

	//subDustBorder_mura((*sendImg1).gradientTFT_muras[imgFlag], (*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).contours_muras[imgFlag], imgFlag);
	subDustBorder((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_points" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_points[imgFlag]);

	//removePointFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_points[imgFlag], (*sendImg1).pointResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect);  //true

	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_points" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag]);
	removeGradientFalse((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], (*sendImg1).pointResults[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 0);  //true


	ofstream location_out;
	location_out.open((*sendImg1).sNowTimeFileDir + "log.txt", std::ios::out | std::ios::app);
	location_out << "第" << to_string(cameraFlag) << "个相机" << "第" << to_string(imgFlag) << "张图片" << endl;
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = CV_FONT_HERSHEY_SIMPLEX;

	if (sizeof(defect[0]) != NULL)
	{
		for (int j = 0; j < (defect[0]).size(); j++)
		{
			defectInformation defectInforSigle = defect[0][j];

			for (int j = 0; j < 4; j++)
			{


				if (defectInforSigle.TrueOrFalse == true) {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
				else {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}


			}
			cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
			location_out << "第0种方法第" << (j + 1) << "个缺陷信息" << endl;
			location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
			location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
			location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
		}
	}
	imwrite((*sendImg1).sNowTimeFileDir + "ZeroMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
	location_out.close();

	cout << "point end" << endl;

}

//排查问题：mura的梯度图计算结果相同，但是二值化后的结果不同？？相同的二值化阈值呀？？？
//不是阈值的问题，阈值可以正常传入，降低阈值依然没有那个mura缺陷，在flag = 1的时候
//但是在白屏上可以检测出，但是绿屏上就是没有那个mura点！！！
//发现二值化过程没有问题，就是计算的梯度图不同，他们只是看起来一样= =
//有待解决gradientTFT_muras 存在问题
//真相是：之前的代码把输入的白屏图的检测结果也或到其他flag的梯度图片上了，导致其他颜色屏幕的mura呈现可以检测出的情况，其实不是。

//0112 lyl 修改了其中的子函数 subDustBorder_mura
//0114 lyl(*sendImg1).gradientTFTg_muras，gradientTFTb_muras，gradientTFTr_muras 将这三张图和灰度图的二值化图或在一起后，进行后续减边除尘的操作
//该函数流程为：先进行mask掩模操作，再减边除尘，最后除去假缺陷
//0115 存在的问题：图片2的瑕疵会投影到图片1上（已经解决）
//0118 lyl add test
void getOneColorMuraResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect) {

	//0114 add test lyl
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTg_muras[imgFlag], (*sendImg1).gradientTFTg_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTb_muras[imgFlag], (*sendImg1).gradientTFTb_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTr_muras[imgFlag], (*sendImg1).gradientTFTr_muras[imgFlag], 0.0, 0.0);


	//0115 修改 lyl 简化流程
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], 0.0, 0.0);
	ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], 0.0, 0.0);

	//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras after" + to_string(cameraFlag)+to_string(imgFlag) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag]);
	subDustBorder((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写 test 0109
	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_muras" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag]);
	//removeMuraFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_muras[imgFlag], (*sendImg1).muraResults[imgFlag], muraCanShureMoveFalse, imgFlag, flag, defect);   //flag

	//0118
	removeGradientFalse((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], (*sendImg1).muraResults[cameraFlag][imgFlag], muraCanShureMoveFalse, cameraFlag, imgFlag, true, defect, 1);   //flag
	

	ofstream location_out;
	location_out.open((*sendImg1).sNowTimeFileDir + "log.txt", std::ios::out | std::ios::app);
	location_out << "第" << to_string(cameraFlag) << "个相机" << "第" << to_string(imgFlag) << "张图片" << endl;
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = CV_FONT_HERSHEY_SIMPLEX;

	if (sizeof(defect[0]) != NULL)
	{
		for (int j = 0; j < (defect[0]).size(); j++)
		{
			defectInformation defectInforSigle = defect[0][j];

			for (int j = 0; j < 4; j++)
			{

				if (defectInforSigle.TrueOrFalse == true) {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
				else {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}

				//line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
			}
			cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
			location_out << "第一种方法第" << (j + 1) << "个缺陷信息" << endl;
			location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
			location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
			location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
		}
	}
	imwrite((*sendImg1).sNowTimeFileDir + "FirstMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
	location_out.close();



}




//该函数流程：先减边除尘，然后去除假缺陷
void getOneColorLouguangResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	ChangePartOfTheRegionalGradient((*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).LightMaskChangeGradientDrawIn, (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder((*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).contours_louguangs[cameraFlag][imgFlag], cameraFlag, imgFlag);
	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_louguangs" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag]);

	
	removeGradientFalse((*sendImg1).GradientResult_louguang[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).contours_louguangs[cameraFlag][imgFlag], (*sendImg1).louguangResults[cameraFlag][imgFlag], SubCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 2);

	ofstream location_out;
	location_out.open((*sendImg1).sNowTimeFileDir + "log.txt", std::ios::out | std::ios::app);
	location_out << "第" << to_string(cameraFlag) << "个相机" << "第" << to_string(imgFlag) << "张图片" << endl;
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = CV_FONT_HERSHEY_SIMPLEX;

	if (sizeof(defect[0]) != NULL)
	{
		for (int j = 0; j < (defect[0]).size(); j++)
		{
			defectInformation defectInforSigle = defect[0][j];

			for (int j = 0; j < 4; j++)
			{

				if (defectInforSigle.TrueOrFalse == true) {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
				else {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255 , 0, 0), 2);
				}


			}
			cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
			location_out << "第二种方法第" << (j + 1) << "个缺陷信息" << endl;
			location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
			location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
			location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
		}
	}
	imwrite((*sendImg1).sNowTimeFileDir + "SecondMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
	location_out.close();



}

//该函数流程：先减边除尘，然后去除假缺陷
//0219  增加了change函数
//修改成sub的参数
void getOneColorSubTemplateResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	//imwrite((*sendImg1).sNowTimeFileDir + "subtractTemplateTftImgs" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag]);
	ChangePartOfTheRegionalGradient((*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder_subTmp((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag] , imgFlag);
	//添加梯度图的写
	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_subTeps" +to_string(cameraFlag)+ to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag]);
	//removeSubTmpFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_subTmps[imgFlag], (*sendImg1).subTmpResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect);

	//0118
	removeGradientFalse((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag], (*sendImg1).subTmpResults[cameraFlag][imgFlag], SubCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 3);

	ofstream location_out;
	location_out.open((*sendImg1).sNowTimeFileDir + "log.txt", std::ios::out | std::ios::app);
	location_out << "第" << to_string(cameraFlag) << "个相机" << "第" << to_string(imgFlag) << "张图片" << endl;
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = CV_FONT_HERSHEY_SIMPLEX;

	if (sizeof(defect[0]) != NULL)
	{
		for (int j = 0; j < (defect[0]).size(); j++)
		{
			defectInformation defectInforSigle = defect[0][j];

			for (int j = 0; j < 4; j++)
			{

				if (defectInforSigle.TrueOrFalse == true) {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
				else {
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}
			}
			cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
			location_out << "第三种方法第" << (j + 1) << "个缺陷信息" << endl;
			location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
			location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
			location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
		}
	}
	imwrite((*sendImg1).sNowTimeFileDir + "ThirdMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
	location_out.close();





}


//new method
void getOneColorbgrSubTemplateResult(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	imwrite((*sendImg1).sNowTimeFileDir + " beforesub" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder((*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).contours_BGRsub[cameraFlag][imgFlag], cameraFlag,imgFlag);
	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_bgr_sub before" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
	imwrite((*sendImg1).sNowTimeFileDir + " aftersub" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);

	//0118
	removeGradientFalse((*sendImg1).bgrsubTemplateImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).contours_BGRsub[cameraFlag][imgFlag], (*sendImg1).BGRsubResults[cameraFlag][imgFlag], SubBGRCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 4);

	
	ofstream location_out;
	location_out.open((*sendImg1).sNowTimeFileDir + "log.txt", std::ios::out | std::ios::app);
	location_out << "第" << to_string(cameraFlag) << "个相机" << "第" << to_string(imgFlag) << "张图片" << endl;
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = CV_FONT_HERSHEY_SIMPLEX;

	if (sizeof(defect[0]) != NULL)
	{
		for (int j = 0; j < (defect[0]).size(); j++)
		{
			defectInformation defectInforSigle = defect[0][j];

			for (int j = 0; j < 4; j++)
			{

				if (defectInforSigle.TrueOrFalse == true) 
				{
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}
				else
				{
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}
			}
			cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
			location_out << "第四种方法第" << (j + 1) << "个缺陷信息" << endl;
			location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
			location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
			location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
		}
	}


	imwrite((*sendImg1).sNowTimeFileDir + "after_remove" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
	imwrite((*sendImg1).sNowTimeFileDir + "ForthMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
	location_out.close();

}

//0118 add test lyl
//将进行梯度筛选后的二值化后的图片合在一起，然后进行灰度筛选
//CombAndRemoveGrayFalse((*sendImg1).gradientResultThreshold_points[imgFlag], (*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).gradientResultThreshold_subTeps[imgFlag], (*sendImg1).gradientResultThreshold_all[imgFlag], imgFlag, flag, defectsInfor);
//0225 add test
void CombAndRemoveGrayFalse(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{

	(*defect).clear();
	(*sendImg1).contours_all[cameraFlag][imgFlag].clear();
	bitwise_or((*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
	bitwise_or((*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
	bitwise_or((*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
	bitwise_or((*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);

	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_all_first" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
	Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	Mat dilateResultTFT;
	dilate((*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], dilateResultTFT, kernel);
	//计算出可能存在缺陷的区域
	//vector<vector<Point>>contours3;
	Mat hierarchy;
	cv::findContours(dilateResultTFT, (*sendImg1).contours_all[cameraFlag][imgFlag], hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


	//将筛选后的瑕疵信息合并到一起
	//(*sendImg1).CombineResults[imgFlag].insert((*sendImg1).CombineResults[imgFlag].end(), (*sendImg1).pointResults[imgFlag].begin(), (*sendImg1).pointResults[imgFlag].end());
	//(*sendImg1).CombineResults[imgFlag].insert((*sendImg1).CombineResults[imgFlag].end(), (*sendImg1).muraResults[imgFlag].begin(), (*sendImg1).muraResults[imgFlag].end());
	//(*sendImg1).CombineResults[imgFlag].insert((*sendImg1).CombineResults[imgFlag].end(), (*sendImg1).subTmpResults[imgFlag].begin(), (*sendImg1).subTmpResults[imgFlag].end());

	//或在一起之后进行灰度阈值筛选

	//removeGradientFalse((*sendImg1).gradientResultThreshold_subTeps[imgFlag], (*sendImg1).contours_subTmps[imgFlag], (*sendImg1).subTmpResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect, 3);
	//cout << "PointCanShureMoveFalse->_bright_num:" << (*PointCanShureMoveFalse)._bright_num << endl;
	//cout << "PointCanShureMoveFalse->_dark_num:" << (*PointCanShureMoveFalse)._dark_num << endl;
	removeGrayFalse((*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).contours_all[cameraFlag][imgFlag], (*sendImg1).contours_all[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, true, defect);  //flag

	cout << "lallllalaal" << endl;


}

//多种检测方法获取瑕疵的检测结果
/*
0:检测点缺陷
1.检测mura缺陷
2.检测漏光

*/
void getDifferentTypeDefectResult(int DefectType, int  *ifdone, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defectsInfor)
{





	(*defectsInfor).clear();




	switch (DefectType)
	{

	case(0): {
		//getOneColorPointResult(imgFlag, flag, defectsInfor);
		getOneColorPointResult1(cameraFlag, imgFlag, flag, defectsInfor);
		
		*ifdone = 1;
		break; // 可选的
	};

	case(1): {

		//getOneColorMuraResult(imgFlag, flag, defectsInfor);
		getOneColorMuraResult1(cameraFlag, imgFlag, flag, defectsInfor);
		*ifdone = 1;

		break; // 可选的

	};

	case(2): {



		getOneColorLouguangResult1(cameraFlag, imgFlag, flag, defectsInfor);

		*ifdone = 1;
		break; // 可选的


	}

	case(3): {


		//getOneColorSubTemplateResult(imgFlag, flag, defectsInfor);
		getOneColorSubTemplateResult1(cameraFlag, imgFlag, flag, defectsInfor);

		*ifdone = 1;
		break; // 可选的


	}
	case(4): {


		//getOneColorSubTemplateResult(imgFlag, flag, defectsInfor);
		getOneColorbgrSubTemplateResult(cameraFlag, imgFlag, flag, defectsInfor);

		*ifdone = 1;
		break; // 可选的


	}




	default:
		break;
	}





}
//融合画框函数
//此函数待添加完善
//0115 lyl 将四种检测方式得到的梯度二值化结果合并在一起，然后去数瑕疵、去除瑕疵和画框
//void drawResult(int imgFlag, bool flag)
//{
//	bitwise_or((*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).gradientResultThreshold_points[imgFlag], (*sendImg1).gradientResultThreshold_all[imgFlag]);
//	//bitwise_or((*sendImg1).gradientResultThreshold_all[imgFlag], (*sendImg1).GradientResult_louguang[imgFlag], (*sendImg1).gradientResultThreshold_all[imgFlag]);
//	bitwise_or((*sendImg1).gradientResultThreshold_all[imgFlag], (*sendImg1).gradientResultThreshold_subTeps[imgFlag], (*sendImg1).gradientResultThreshold_all[imgFlag]);
//
//	//test
//	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_all" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_all[imgFlag]);
//
//	//或在一起后，进行膨胀数瑕疵轮廓
//	Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
//	Mat dilateResultTFT_all;
//	dilate((*sendImg1).gradientResultThreshold_all[imgFlag], dilateResultTFT_all, kernel);
//
//	Mat hierarchy;
//	cv::findContours(dilateResultTFT_all, (*sendImg1).contours_all[imgFlag], hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//
//	//去除假缺陷 假的瑕疵
//	//removePointFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_all[imgFlag], (*sendImg1).contours_all[imgFlag], PointCanShureMoveFalse, imgFlag, true  );  //true
//
//
//
//
//
//
//}

//0119 add lyl
//把经过两次筛选后的瑕疵全部一起画出来
void drawFinalResult(Mat &imgIn, Mat &imgOut, vector<vector<Point>>contoursIn, int imgFlag, bool flag)
{
	//绘制缺陷的最小外接矩形,蓝色的矩形框
	Point2f P[4];
	RotatedRect min_rect;
	///*设置书写的字体*/
	Mat tftResultShow = imgIn.clone();
	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
	Point2f rect_points[4];


	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{

			min_rect = minAreaRect(contoursIn[i]);
			min_rect.points(rect_points);
			//min_rect.points(P);
			if (flag)
			{
				cout << "第" << i + 1 << "个合并后的缺陷" << endl;
			}
			//在图上标出各个缺陷的序号
			Moments s_N = moments(contoursIn[i]);//计算第一条轮廓的矩
			int s_cx = int(s_N.m10 / s_N.m00);   //计算缺陷中心点
			int s_cy = int(s_N.m01 / s_N.m00);
			//写上序号
			//cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//画最小外接框  红色的
			//rectangle(tftResultShow, P[1], P[3], cvScalar(0, 0, 250), 3, 4, 0);
			for (int j = 0; j < 4; j++)
			{
				line(tftResultShow, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 250), 2);
			}



			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);

		}
	}
	imgOut = tftResultShow.clone();
	//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + to_string(imgFlag) + ".jpg", imgOut);




}

//**********多线程运行四种检测方法，得到检测结果*************//
void getOneColorResult(int cameraFlag, int imgFlag, bool flag, int *ifthreadDonn, vector<defectInformation> defectsInfor[5])
{



	//count_information_img((*sendImg1).CutTemplate_fill[cameraFlag][imgFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][flag], cameraFlag, imgFlag);
	//使用bgr-bgr50 的结果作为灰度统计数据的模板图片
	count_information_img((*sendImg1).imgTftColor_sub_result[cameraFlag][imgFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][flag], cameraFlag, imgFlag);
	int threadDone[5] = { 0,0,0,0,0 };
	for (int DefectType = 0; DefectType < 5; DefectType++) {

		getDifferentTypeDefectResult(DefectType, &threadDone[DefectType], cameraFlag, imgFlag, flag, &defectsInfor[DefectType]);

		//thread t(getDifferentTypeDefectResult, DefectType, &threadDone[DefectType], cameraFlag,imgFlag, flag, &defectsInfor[DefectType]);
		//t.detach();
	}
	while (threadDone[0] != 1 || threadDone[1] != 1 || threadDone[2] != 1 || threadDone[3] != 1 || threadDone[4] != 1)
		Sleep(10);


	//getOneColorPointResult( imgFlag, flag);

	//getOneColorMuraResult(imgFlag, flag);

	//getOneColorLouguangResult1(imgFlag, flag);

	//getOneColorSubTemplateResult(imgFlag, flag);
	*ifthreadDonn = 1;
}

//12.18 update将真实瑕疵分别统计保存
// flag =  true 显示瑕疵具体信息
void CombineXiaci(bool flag)
{
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_16S);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_16S);
	// int cameraFlag, int pictureFlag
	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++) {

		//(*sendImg1).MaxFlag
		int threadDone[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		int threadDoneSum = 0;
		for (int imgFlag = 0; imgFlag < (*sendImg1).MaxFlag[cameraFlag]; imgFlag++) {
			//QQQQQQQ,统计模板的会好一点   统计整张图片灰度，梯度均值方差
			//应该统计模板图片fill后的灰度的各种统计量，以及模板梯度图的梯度统计量
			//count_information_img((*sendImg1).imgTftColorCutFills[imgFlag],(*sendImg1).GradientResult_point[flag], imgFlag);
			//count_information_img((*sendImg1).CutTemplate_fill[imgFlag],(*sendImg1).GradientTemplate_fill[flag], imgFlag);
			//获取检测结果

			getOneColorResult(cameraFlag, imgFlag, flag, &threadDone[imgFlag], defectsInforGray[cameraFlag][imgFlag]);
			//0118 add test
			CombAndRemoveGrayFalse(cameraFlag, imgFlag, flag, defectsInfor[cameraFlag][imgFlag]);

			//thread t1(getOneColorResult, imgFlag, flag ,&threadDone[imgFlag]);
			//t1.detach();

			//getOneColorResult(imgFlag, flag);	

		}

		//waitKey(0);

		//等待所有线程完成
		while (threadDoneSum != (*sendImg1).MaxFlag[cameraFlag])
		{
			Sleep(10);
			threadDoneSum = 0;
			for (int threadDoneNum = 0; threadDoneNum < (*sendImg1).MaxFlag[cameraFlag]; threadDoneNum++)
				threadDoneSum = threadDoneSum + threadDone[threadDoneNum];
		}

	}

}

/*
maxDistance = 100  群点最大距离
maxNunPoint = 3    群点最少点数
需要输入，记录群点位置的向量
(*sendImg1).group_points
查找到的群点添加入
defectsInfor[imgFlag][0]中
*/
void getGrounpPointResult(int maxDistance = 100, int maxNunPoint = 2)
{
	vector<Point2f> pointGroundTrue[10][20];

	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)
	{
		for (int imgFlag = 0; imgFlag < (*sendImg1).MaxFlag[cameraFlag]; imgFlag++)    //遍历不同颜色的图片

		{
			vector<int>  NumberOfApproachesPoints;


			for (int pointNumCenter = 0; pointNumCenter < (*sendImg1).group_points[cameraFlag][imgFlag].size(); pointNumCenter++)
			{

				int  NumberOfApproachesPoint = 0;
				//计算与该点相近的点数，最少为一，也就是本身
				for (int pointNum = 0; pointNum < (*sendImg1).group_points[cameraFlag][imgFlag].size(); pointNum++)
				{
					double  xDistance = ((*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].x - (*sendImg1).group_points[cameraFlag][imgFlag][pointNum].x);
					double  yDistance = ((*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].y - (*sendImg1).group_points[cameraFlag][imgFlag][pointNum].y);
					double Distance = xDistance * xDistance + yDistance * yDistance;
					if ((Distance) < maxDistance *maxDistance)
					{
						NumberOfApproachesPoint++;

					}

				}
				NumberOfApproachesPoints.push_back(NumberOfApproachesPoint);

				//如果存在很多相近的点，则判定为群点
				if (NumberOfApproachesPoint >= maxNunPoint)
				{
					bool ifExistPoint = true;

					//查看是否重复判定群点
					for (int checkDuplicate = 0; checkDuplicate < pointGroundTrue[cameraFlag][imgFlag].size(); checkDuplicate++)
					{

						int centerDistanceX = abs((*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].x - pointGroundTrue[cameraFlag][imgFlag][checkDuplicate].x);
						int centerDistanceY = abs((*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].y - pointGroundTrue[cameraFlag][imgFlag][checkDuplicate].y);

						if ((centerDistanceX < maxDistance) && (centerDistanceY < maxDistance)) {

							ifExistPoint = false;
						}
					}
					//记录该群点位置
					if (ifExistPoint) {
						pointGroundTrue[cameraFlag][imgFlag].push_back((*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter]);
						defectInformation Xiaci;
						Xiaci.s_cx = (*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].x;
						Xiaci.s_cy = (*sendImg1).group_points[cameraFlag][imgFlag][pointNumCenter].y;



						//Xiaci.h = maxDistance;
						//Xiaci.w = maxDistance;


						int maxX = Xiaci.s_cx + maxDistance;
						int minX = Xiaci.s_cx - maxDistance;
						int maxY = Xiaci.s_cy + maxDistance;
						int minY = Xiaci.s_cy - maxDistance;

						//限制缺陷区域不能大于图片区域
						if (maxX > (*sendImg1).Max_x - 1)
						{
							maxX = (*sendImg1).Max_x - 1;
							//cout << "maxX:" << maxX;
						}
						if (maxY > (*sendImg1).Max_y - 1)
						{
							maxY = (*sendImg1).Max_y - 1;
							//cout << "maxY:" << maxY;
						}
						if (minX < 0)
						{
							minX = 0;
						}
						if (minY < 0)
						{
							minY = 0;
							//cout << "minY:" << minY;
						}
						Xiaci.w = maxX - minX;
						Xiaci.h = maxY - minY;
						Xiaci.type = "g_p";
						Xiaci.TrueOrFalse = true;
						Xiaci.transYorN = true;

						//Point2f p2[4];
						Xiaci.P[0].x = Xiaci.s_cx - int(Xiaci.w / 2 - 2);
						Xiaci.P[0].y = Xiaci.s_cy - int(Xiaci.h / 2 - 2);
						Xiaci.P[1].x = Xiaci.s_cx + int(Xiaci.w / 2 - 2);
						Xiaci.P[1].y = Xiaci.s_cy - int(Xiaci.h / 2 - 2);
						Xiaci.P[2].x = Xiaci.s_cx + int(Xiaci.w / 2 + 2);
						Xiaci.P[2].y = Xiaci.s_cy + int(Xiaci.h / 2 + 2);
						Xiaci.P[3].x = Xiaci.s_cx - int(Xiaci.w / 2 - 2);
						Xiaci.P[3].y = Xiaci.s_cy + int(Xiaci.h / 2 + 2);
						Xiaci.typenum = 7;
						defectsInfor[cameraFlag][imgFlag][0].push_back(Xiaci);
						(*sendImg1).matrix[cameraFlag][imgFlag][7] = 1;
					}
				}
			}
		}
	}
}

void determine()
{
	//
	Point2i temp;
	//群点坐标
	Point2d group_temp;
	int y;

	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)

	{
		for (int x = 0; x < (*sendImg1).MaxFlag[cameraFlag]; x++)    //遍历不同颜色的图片
		{
			(*sendImg1).group_points[cameraFlag][x].clear();
			int num_group_point = 0;
			if (sizeof(defectsInfor[cameraFlag][x]) != NULL)    //结构体不为空
			{
				for (int type = 0; type < 1; type++)    //遍历不同检测方法结果
				{
					if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
					{
						for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
						{
							defectInformation defectInforSigle = defectsInfor[cameraFlag][x][type][i];
							
							bool point_add = (30 <=defectsInfor[cameraFlag][x][type][i].num_yichang < 500 );
							bool line_add = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 500);

							
							if (defectInforSigle.TrueOrFalse == true)
							{
								//point的判定条件
								bool point_h = (defectInforSigle.h < 10 + 19);
								bool point_w = (defectInforSigle.w < 10 + 19);
								bool point_w_h = point_w && point_w;
								bool point_num = (defectInforSigle.numGray < 500);
								bool xianzhi_size_point = point_w_h  && point_num;    //点的大小限制
								cout << "颜色：" << x << "类型：" << type << "顺序：" << i << endl;
								bool xianzhi_if_group_point1 = defectInforSigle.numGray > 50;     //判定单点、群点定义
								bool xianzhi_if_group_point2 = defectInforSigle.num_yichang >=  30;     //判定单点、群点定义
								bool xianzhi_if_group_point = (xianzhi_if_group_point1 || xianzhi_if_group_point2);
								bool xianzhi_point_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);  //亮点
								bool xianzhi_point_dark = (defectInforSigle.dark_num >=  defectInforSigle.bright_num);    //暗点
																																								//point的第二套参数判定条件
								bool point_2 = (defectInforSigle.numGray >= SecondCanshu._point_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._point_m_gray);
								//bool point_3 = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 30);
								//point_2 = (point_2 || point_3);
								//cout << i << endl;


								cout << "cameraFlag:" << cameraFlag << endl;
								cout << "No." << x << "个图片信息：" << endl;
								cout << "第" << i << "个瑕疵：" << endl;
								cout << "point_h:" <<point_h<< endl;
								cout << "point_w:" << point_w << endl;
								cout << "point_w_h:" << point_w_h << endl;
								cout << "point_num:" << point_num << endl;
								cout << "xianzhi_size_point:" << xianzhi_size_point << endl;
								cout << "xianzhi_if_group_point:" << xianzhi_if_group_point << endl;
								cout << "xianzhi_point_bright:" << xianzhi_point_bright << endl;
								cout << "xianzhi_point_dark:" << xianzhi_point_dark<<endl;
								cout << "point_2: "<<point_2 << endl;
								//cout << "point_3: "<< point_3 << endl;
							
							


								//line的判定条件
								bool xianzhi_line_1 = (defectInforSigle.h - 18) >= 2 * (defectInforSigle.w - 18);
								bool xianzhi_line_2 = (defectInforSigle.w - 18) >= 2 * (defectInforSigle.h - 18);       //满足长宽比关系
																																							   //bool xianzhi_line_3 = abs(defectsInfor[x][type][i].dst_m_Gray) > 1;    //满足对比度关系
								bool xianzhi_line = (xianzhi_line_1 || xianzhi_line_2) || (defectInforSigle.num_yichang >= 500);
								bool xianzhi_line_bright = defectInforSigle.dst_m_Gray > 0;
								bool xianzhi_line_dark = defectInforSigle.dst_m_Gray <= 0;     //判定亮暗
																												  //line的第二套参数判定条件
								bool line_2 = (defectInforSigle.numGray >= SecondCanshu._line_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._line_m_gray);
								// 对line新加一种判定条件
								bool line_add = defectInforSigle.num_yichang >= 500;
								line_2 = line_2 || line_add;
						
								cout << "xianzhi_line:" << xianzhi_line << endl;
								cout << "xianzhi_line_bright:" << xianzhi_line_bright << endl;
								cout << "xianzhi_line_dark" << xianzhi_line_dark << endl;
								cout << "line_add:" << line_add << endl;
								cout << "line_2:" <<line_2<< endl;


								//mura的判定条件
								bool xianzhi_mura_1 = defectInforSigle.numGray > 500;     //mura满足面积关系
								bool xianzhi_mura_2 = abs(defectInforSigle.dst_m_Gray) < 1;   //满足对比度关系
								bool xianzhi_mura = xianzhi_mura_1 && xianzhi_mura_2;
								//mura的第二套参数判定条件
								bool mura_2 = (defectInforSigle.numGray >= SecondCanshu._mura_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._mura_m_gray);
								bool mura_add = defectInforSigle.num_yichang >= 500;
								mura_2 = mura_2 || mura_add;
								//tuan的判定条件
								bool xianzhi_tuan_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);
								bool xianzhi_tuan_dark = (defectInforSigle.dark_num >=  defectInforSigle.bright_num);
								//tuan的第二套参数判定条件
								bool tuan_2 = (defectInforSigle.numGray >= SecondCanshu._tuan_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._tuan_m_gray);
								bool tuan_add = defectInforSigle.num_yichang >= 500;
								tuan_2 = tuan_2 || tuan_add;

								cout << "xianzhi_tuan_bright:" << xianzhi_tuan_bright << endl;
								cout << "xianzhi_tuan_dark" << xianzhi_tuan_dark << endl;
								cout << "tuan_2:" << tuan_2 << endl;


								if (xianzhi_size_point)
								{
									if (xianzhi_if_group_point)  //判定是否为群点
									{
										//判定亮暗
										if (xianzhi_point_bright)
										{
											y = 0;
											defectInforSigle.type = "b_p";
											if (point_2)
											{
												defectInforSigle.transYorN = true;  //满足第二套参数输出
											}
										}
										else if (xianzhi_point_dark)
										{
											y = 1;
											defectInforSigle.type = "d_p";
											if (point_2)
											{
												defectInforSigle.transYorN = true;  //满足第二套参数输出
											}
										}

									}
									else
									{
										defectInforSigle.type = "unknown";
										defectInforSigle.TrueOrFalse = false;
										//将满足群点定义的坐标写入group_points中，并返回group_points
										group_temp.x = defectInforSigle.s_cx;
										group_temp.y = defectInforSigle.s_cy;
										//x表示颜色，num_group_point表示该图中的第几个
										(*sendImg1).group_points[cameraFlag][x].push_back(group_temp);

									}
								}

								else if (xianzhi_line == 1)
								{
									if (xianzhi_line_bright == 1)
									{
										y = 2;
										defectInforSigle.type = "b_l";
										if (line_2 == 1)
										{
											cout << "66666666666" << endl;
											defectInforSigle.transYorN = true;
										}
									}
									else if (xianzhi_line_dark == 1)
									{
										y = 3;
										defectInforSigle.type = "d_l";
										if (line_2 == 1)
										{
											defectInforSigle.transYorN = true;
										}
									}

								}

								else if (xianzhi_mura)
								{
									y = 4;
									defectInforSigle.type = "mura";

									if (mura_2)
									{
										defectInforSigle.transYorN = true;
									}
								}

								else
								{
									if (xianzhi_tuan_bright)
									{
										y = 5;
										defectInforSigle.type = "b_t";
										if (tuan_2)
										{
											defectInforSigle.transYorN = true;
										}
									}
									else if (xianzhi_tuan_dark)
									{
										y = 6;
										defectInforSigle.type = "d_t";
										if (tuan_2)
										{
											defectInforSigle.transYorN = true;
										}
									}


								}

								if ((*sendImg1).matrix[cameraFlag][x][y] == 0)
								{
									(*sendImg1).matrix[cameraFlag][x][y] = 1;
								}
								else
									continue;



								//int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
								//Point2f p1[2];
								//p1[0].x = defectsInfor[x][type][i].s_cx - int(defectsInfor[x][type][i].w/2);
								//p1[0].y = defectsInfor[x][type][i].s_cy - int(defectsInfor[x][type][i].h/2);
								//p1[1].x = defectsInfor[x][type][i].s_cx + int(defectsInfor[x][type][i].w/2);
								//p1[1].y = defectsInfor[x][type][i].s_cy + int(defectsInfor[x][type][i].h/2);
								//rectangle((*sendImg1).imgTftColorCuts[x], p1[0], p1[1], (255, 0, 0), 2);
								//cv::putText((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].type, Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 0, 0), 2);
								//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
							}
						}
					}
				}
			}
			//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[x]);
		}

	}


	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)

	{
		for (int i = 0; i < (*sendImg1).MaxFlag[cameraFlag]; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if ((*sendImg1).matrix[cameraFlag][i][j] == 1)
				{
					temp.x = i;
					temp.y = j;
					(*sendImg1).points.push_back(temp);

				}
			}
		}


	}

}


//0124 test
//目前使用！！！！！！！  0222
void determine1()
{
	//
	Point2i temp;
	//群点坐标
	Point2d group_temp;
	int y;
	int determine_Type;

	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)

	{
		for (int x = 0; x < (*sendImg1).MaxFlag[cameraFlag]; x++)    //遍历不同颜色的图片
		{
			(*sendImg1).group_points[cameraFlag][x].clear();
			//0127 add test
			(*sendImg1).pointResults[cameraFlag][x].clear();
			(*sendImg1).muraResults[cameraFlag][x].clear();
			(*sendImg1).subTmpResults[cameraFlag][x].clear();

			(*sendImg1).contours_points[cameraFlag][x].clear();
			(*sendImg1).contours_muras[cameraFlag][x].clear();
			(*sendImg1).contours_subTmps[cameraFlag][x].clear();
			(*sendImg1).contours_all[cameraFlag][x].clear();

			(*sendImg1).group_points[cameraFlag][x].clear();





			int num_group_point = 0;
			if (sizeof(defectsInfor[cameraFlag][x]) != NULL)    //结构体不为空
			{
				for (int type = 0; type < 1; type++)    //遍历不同检测方法结果
				{
					if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
					{
						for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
						{
							defectInformation &defectInforSigle = defectsInfor[cameraFlag][x][type][i];

							bool point_add = (30 <= defectsInfor[cameraFlag][x][type][i].num_yichang < 500);
							bool line_add = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 500);


							//0125 add test
							if (defectInforSigle.TrueOrFalse == false)
							{
								defectInforSigle.transYorN = false;
							}




							if (defectInforSigle.TrueOrFalse == true)
							{
								//point的判定条件
								bool point_h = (defectInforSigle.h <= 29);
								bool point_w = (defectInforSigle.w <= 29);
								bool point_w_h = point_w && point_w;
								bool point_num = (defectInforSigle.numGray < 500);   //异常点个数
								//0125 修改test
								bool xianzhi_size_point = point_w_h || point_num;    //点的大小限制   之前是&& 现在改成||
								cout << "颜色：" << x << "类型：" << type << "顺序：" << i << endl;
								
								
								
								bool xianzhi_if_single_point1 = defectInforSigle.numGray > SecondCanshu._maxGParea;     //判定单点、群点定义
								bool xianzhi_if_single_point2 = defectInforSigle.num_yichang >= 30;     //判定单点、群点定义梯度异常点
								bool xianzhi_if_single_point = (xianzhi_if_single_point1 || xianzhi_if_single_point2);
								bool xianzhi_if_group_point = defectInforSigle.numGray > 20;

								//亮暗点的判定条件
								bool xianzhi_point_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);  //亮点
								bool xianzhi_point_dark = (defectInforSigle.dark_num >= defectInforSigle.bright_num);    //暗点
																														 //point的第二套参数判定条件
								
								
								//个数、灰度值同时满足要求方能卡出这个瑕疵
								bool point_2 = (defectInforSigle.numGray >= SecondCanshu._point_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._point_m_gray);
							
								

								cout << "cameraFlag:" << cameraFlag << endl;
								cout << "No." << x << "个图片信息：" << endl;
								cout << "第" << i << "个瑕疵：" << endl;
								cout << "point_h:" << point_h << endl;
								cout << "point_w:" << point_w << endl;
								cout << "point_w_h:" << point_w_h << endl;
								cout << "point_num:" << point_num << endl;
								cout << "xianzhi_size_point:" << xianzhi_size_point << endl;
								cout << "xianzhi_if_single_point:" << xianzhi_if_single_point << endl;
								cout << "xianzhi_point_bright:" << xianzhi_point_bright << endl;
								cout << "xianzhi_point_dark:" << xianzhi_point_dark << endl;
								cout << "point_2: " << point_2 << endl;
								//cout << "point_3: "<< point_3 << endl;




								//line的判定条件
								bool xianzhi_line_1 = (defectInforSigle.h - 18) >= 1.5 * (defectInforSigle.w - 18);
								bool xianzhi_line_2 = (defectInforSigle.w - 18) >= 1.5 * (defectInforSigle.h - 18);       //满足长宽比关系
																														//bool xianzhi_line_3 = abs(defectsInfor[x][type][i].dst_m_Gray) > 1;    //满足对比度关系
								bool xianzhi_line = (xianzhi_line_1 || xianzhi_line_2) && (defectInforSigle.num_yichang >= 500);    
								bool xianzhi_line_bright = defectInforSigle.dst_m_Gray > 0;
								bool xianzhi_line_dark = defectInforSigle.dst_m_Gray <= 0;     //判定亮暗
																							   //line的第二套参数判定条件
								bool line_2 = (defectInforSigle.numGray >= SecondCanshu._line_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._line_m_gray);
								// 对line新加一种判定条件
								bool line_add = defectInforSigle.num_yichang >= 500;
								line_2 = line_2 || line_add;

								
								cout << "xianzhi_line:" << xianzhi_line << endl;
								cout << "xianzhi_line_bright:" << xianzhi_line_bright << endl;
								cout << "xianzhi_line_dark" << xianzhi_line_dark << endl;
								cout << "line_add:" << line_add << endl;
								cout << "line_2:" << line_2 << endl;


								//mura的判定条件
								bool xianzhi_mura_1 = defectInforSigle.numGray > 500;     //mura满足面积关系
								bool xianzhi_mura_2 = abs(defectInforSigle.dst_m_Gray) < 1;   //满足对比度关系
								bool xianzhi_mura = xianzhi_mura_1 && xianzhi_mura_2;
								//mura的第二套参数判定条件
								bool mura_2 = (defectInforSigle.numGray >= SecondCanshu._mura_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._mura_m_gray);
								bool mura_add = defectInforSigle.num_yichang >= 500;
								mura_2 = mura_2 || mura_add;
								//tuan的判定条件
								bool xianzhi_tuan_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);
								bool xianzhi_tuan_dark = (defectInforSigle.dark_num >= defectInforSigle.bright_num);
								//tuan的第二套参数判定条件
								bool tuan_2 = (defectInforSigle.numGray >= SecondCanshu._tuan_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._tuan_m_gray);
								bool tuan_add = defectInforSigle.num_yichang >= 500;
								tuan_2 = tuan_2 || tuan_add;


								

								//cout << "xianzhi_tuan_bright:" << xianzhi_tuan_bright << endl;
								//cout << "xianzhi_tuan_dark" << xianzhi_tuan_dark << endl;
								//cout << "tuan_2:" << tuan_2 << endl;
								

								if (xianzhi_size_point == 1) {
								
									determine_Type = 0;
								
								}
								else if(xianzhi_line == 1) {
								
									determine_Type = 1;
								
								}
								
								else if (xianzhi_line == 1) {

									determine_Type = 1;

								}
								//else 
								//{
								//
								//	if (xianzhi_mura == 1) {
								//		determine_Type = 2;
								//	
								//	}
								//	else {
								//	
								//		determine_Type = 3;
								//	
								//	}
								//
								//}
								else if (xianzhi_mura_1 == 1)
								{
									determine_Type = 3;

									if (xianzhi_mura_2 == 1)
									{
										determine_Type = 2;
									}
								}
								else
								{
									defectInforSigle.transYorN = false;
								}

								
								
								
								
									
				
								cout << "determine_Type:" << determine_Type << endl;

								switch (determine_Type)
								{
								case(0): {
									if (xianzhi_if_single_point)  //判定是否为群点
									{
										//判定是否符合二套参数
										if (point_2 == 0)
										{
											defectInforSigle.transYorN = false;  //不满足第二套参数，不输出
										}
										//判定亮暗
										if (xianzhi_point_bright)
										{
											y = 0;
											defectInforSigle.type = "b_p";
											defectInforSigle.typenum = 0;

										}
										else if (xianzhi_point_dark)
										{
											y = 1;
											defectInforSigle.type = "d_p";
											defectInforSigle.typenum = 1;
										}}
										else
										{
											defectInforSigle.type = "unknown";
											defectInforSigle.TrueOrFalse = false;
											defectInforSigle.transYorN = false;
											if (xianzhi_if_group_point == 1)    //满足可以进入筛选群点的要求，就加入
											{
												
												//将满足群点定义的坐标写入group_points中，并返回group_points
												group_temp.x = defectInforSigle.s_cx;
												group_temp.y = defectInforSigle.s_cy;
												//x表示颜色，num_group_point表示该图中的第几个
												(*sendImg1).group_points[cameraFlag][x].push_back(group_temp);
											}
											

										}
									

									break;
								};

								//line判定
								case(1): {

									if (line_2 == 0)
									{
										defectInforSigle.transYorN = false;
									}
									if (xianzhi_line_bright == 1)
									{
										y = 2;
										defectInforSigle.type = "b_l";
										defectInforSigle.typenum = 2;
									
									}
									else if (xianzhi_line_dark == 1)
									{
										y = 3;
										defectInforSigle.type = "d_l";
										defectInforSigle.typenum = 3;
									}


									break;
								};
								case(2): {
									y = 4;
									defectInforSigle.type = "mura";
									defectInforSigle.typenum = 4;

									if (mura_2 == 0)
									{
										defectInforSigle.transYorN = false;
									}
								
									break;
								};
								case(3): {
									if (tuan_2 == 0)
									{
										defectInforSigle.transYorN = false;
									}
									if (xianzhi_tuan_bright)
									{
										y = 5;
										defectInforSigle.type = "b_t";
										defectInforSigle.typenum = 5;
										
									}
									else if (xianzhi_tuan_dark)
									{
										y = 6;
										defectInforSigle.type = "d_t";
										defectInforSigle.typenum = 6;

									}

									break;
								};
					

								}





								//if (xianzhi_size_point)
								//{
								//	if (xianzhi_if_group_point)  //判定是否为群点
								//	{
								//		//判定亮暗
								//		if (xianzhi_point_bright)
								//		{
								//			y = 0;
								//			defectInforSigle.type = "b_p";
								//			if (point_2)
								//			{
								//				defectInforSigle.transYorN = true;  //满足第二套参数输出
								//			}
								//		}
								//		else if (xianzhi_point_dark)
								//		{
								//			y = 1;
								//			defectInforSigle.type = "d_p";
								//			if (point_2)
								//			{
								//				defectInforSigle.transYorN = true;  //满足第二套参数输出
								//			}
								//		}

								//	}
								//	else
								//	{
								//		defectInforSigle.type = "unknown";
								//		defectInforSigle.TrueOrFalse = false;
								//		//将满足群点定义的坐标写入group_points中，并返回group_points
								//		group_temp.x = defectInforSigle.s_cx;
								//		group_temp.y = defectInforSigle.s_cy;
								//		//x表示颜色，num_group_point表示该图中的第几个
								//		(*sendImg1).group_points[cameraFlag][x].push_back(group_temp);

								//	}
								//}

								//else if (xianzhi_line == 1)
								//{
								//	if (xianzhi_line_bright == 1)
								//	{
								//		y = 2;
								//		defectInforSigle.type = "b_l";
								//		if (line_2 == 1)
								//		{
								//			cout << "66666666666" << endl;
								//			defectInforSigle.transYorN = true;
								//		}
								//	}
									//else if (xianzhi_line_dark == 1)
									//{
									//	y = 3;
									//	defectInforSigle.type = "d_l";
									//	if (line_2 == 1)
									//	{
									//		defectInforSigle.transYorN = true;
									//	}
									//}

								//}

								//else if (xianzhi_mura)
								//{
								//	y = 4;
								//	defectInforSigle.type = "mura";

								//	if (mura_2)
								//	{
								//		defectInforSigle.transYorN = true;
								//	}
								//}

								//else
								//{
								//	if (xianzhi_tuan_bright)
								//	{
								//		y = 5;
								//		defectInforSigle.type = "b_t";
								//		if (tuan_2)
								//		{
								//			defectInforSigle.transYorN = true;
								//		}
								//	}
								//	else if (xianzhi_tuan_dark)
								//	{
								//		y = 6;
								//		defectInforSigle.type = "d_t";
								//		if (tuan_2)
								//		{
								//			defectInforSigle.transYorN = true;
								//		}
								//	}


								//}

								if ((*sendImg1).matrix[cameraFlag][x][y] == 0)
								{
									(*sendImg1).matrix[cameraFlag][x][y] = 1;
								}
								else
									continue;



								//int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
								//Point2f p1[2];
								//p1[0].x = defectsInfor[x][type][i].s_cx - int(defectsInfor[x][type][i].w/2);
								//p1[0].y = defectsInfor[x][type][i].s_cy - int(defectsInfor[x][type][i].h/2);
								//p1[1].x = defectsInfor[x][type][i].s_cx + int(defectsInfor[x][type][i].w/2);
								//p1[1].y = defectsInfor[x][type][i].s_cy + int(defectsInfor[x][type][i].h/2);
								//rectangle((*sendImg1).imgTftColorCuts[x], p1[0], p1[1], (255, 0, 0), 2);
								//cv::putText((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].type, Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 0, 0), 2);
								//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
							}
						}
					}
				}
			}
			//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[x]);
		}

	}


	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)

	{
		for (int i = 0; i < (*sendImg1).MaxFlag[cameraFlag]; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if ((*sendImg1).matrix[cameraFlag][i][j] == 1)
				{
					temp.x = i;
					temp.y = j;
					(*sendImg1).points.push_back(temp);

				}
			}
		}


	}

}


//0228 test
//单相机的单张图片的determine函数
void determine_Single(int cameraFlag, int imgFlag)
{
	//
	Point2i temp;
	//群点坐标
	Point2d group_temp;
	int y;
	int determine_Type;
	int x = imgFlag;
	//for (int x = 0; x < (*sendImg1).MaxFlag[cameraFlag]; x++)    //遍历不同颜色的图片
	//{

	(*sendImg1).group_points[cameraFlag][x].clear();
	//0127 add test
	(*sendImg1).pointResults[cameraFlag][x].clear();
	(*sendImg1).muraResults[cameraFlag][x].clear();
	(*sendImg1).subTmpResults[cameraFlag][x].clear();

	(*sendImg1).contours_points[cameraFlag][x].clear();
	(*sendImg1).contours_muras[cameraFlag][x].clear();
	(*sendImg1).contours_subTmps[cameraFlag][x].clear();
	(*sendImg1).contours_all[cameraFlag][x].clear();

	(*sendImg1).group_points[cameraFlag][x].clear();





	int num_group_point = 0;
	if (sizeof(defectsInfor[cameraFlag][x]) != NULL)    //结构体不为空
	{
		for (int type = 0; type < 1; type++)    //遍历不同检测方法结果
		{
			if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
			{
				for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
				{
					defectInformation &defectInforSigle = defectsInfor[cameraFlag][x][type][i];

					bool point_add = (30 <= defectsInfor[cameraFlag][x][type][i].num_yichang < 500);
					bool line_add = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 500);


					//0125 add test
					if (defectInforSigle.TrueOrFalse == false)
					{
						defectInforSigle.transYorN = false;
					}




					if (defectInforSigle.TrueOrFalse == true)
					{
						//point的判定条件
						bool point_h = (defectInforSigle.h <= 29);
						bool point_w = (defectInforSigle.w <= 29);
						bool point_w_h = point_w && point_w;
						bool point_num = (defectInforSigle.numGray < 500);   //异常点个数
																			 //0125 修改test
						bool xianzhi_size_point = point_w_h || point_num;    //点的大小限制   之前是&& 现在改成||
						cout << "颜色：" << x << "类型：" << type << "顺序：" << i << endl;



						bool xianzhi_if_single_point1 = defectInforSigle.numGray > SecondCanshu._maxGParea;     //判定单点、群点定义
						bool xianzhi_if_single_point2 = defectInforSigle.num_yichang >= 30;     //判定单点、群点定义梯度异常点
						bool xianzhi_if_single_point = (xianzhi_if_single_point1 || xianzhi_if_single_point2);
						bool xianzhi_if_group_point = defectInforSigle.numGray > 20;

						//亮暗点的判定条件
						bool xianzhi_point_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);  //亮点
						bool xianzhi_point_dark = (defectInforSigle.dark_num >= defectInforSigle.bright_num);    //暗点
																												 //point的第二套参数判定条件


																												 //个数、灰度值同时满足要求方能卡出这个瑕疵
						bool point_2 = (defectInforSigle.numGray >= SecondCanshu._point_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._point_m_gray);



						cout << "cameraFlag:" << cameraFlag << endl;
						cout << "No." << x << "个图片信息：" << endl;
						cout << "第" << i << "个瑕疵：" << endl;
						cout << "point_h:" << point_h << endl;
						cout << "point_w:" << point_w << endl;
						cout << "point_w_h:" << point_w_h << endl;
						cout << "point_num:" << point_num << endl;
						cout << "xianzhi_size_point:" << xianzhi_size_point << endl;
						cout << "xianzhi_if_single_point:" << xianzhi_if_single_point << endl;
						cout << "xianzhi_point_bright:" << xianzhi_point_bright << endl;
						cout << "xianzhi_point_dark:" << xianzhi_point_dark << endl;
						cout << "point_2: " << point_2 << endl;
						//cout << "point_3: "<< point_3 << endl;




						//line的判定条件
						bool xianzhi_line_1 = (defectInforSigle.h - 18) >= 1.5 * (defectInforSigle.w - 18);
						bool xianzhi_line_2 = (defectInforSigle.w - 18) >= 1.5 * (defectInforSigle.h - 18);       //满足长宽比关系
																												  //bool xianzhi_line_3 = abs(defectsInfor[x][type][i].dst_m_Gray) > 1;    //满足对比度关系
						bool xianzhi_line = (xianzhi_line_1 || xianzhi_line_2) && (defectInforSigle.num_yichang >= 500);
						bool xianzhi_line_bright = defectInforSigle.dst_m_Gray > 0;
						bool xianzhi_line_dark = defectInforSigle.dst_m_Gray <= 0;     //判定亮暗
																					   //line的第二套参数判定条件
						bool line_2 = (defectInforSigle.numGray >= SecondCanshu._line_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._line_m_gray);
						// 对line新加一种判定条件
						bool line_add = defectInforSigle.num_yichang >= 500;
						line_2 = line_2 || line_add;


						cout << "xianzhi_line:" << xianzhi_line << endl;
						cout << "xianzhi_line_bright:" << xianzhi_line_bright << endl;
						cout << "xianzhi_line_dark" << xianzhi_line_dark << endl;
						cout << "line_add:" << line_add << endl;
						cout << "line_2:" << line_2 << endl;


						//mura的判定条件
						bool xianzhi_mura_1 = defectInforSigle.numGray > 500;     //mura满足面积关系
						bool xianzhi_mura_2 = abs(defectInforSigle.dst_m_Gray) < 1;   //满足对比度关系
						bool xianzhi_mura = xianzhi_mura_1 && xianzhi_mura_2;
						//mura的第二套参数判定条件
						bool mura_2 = (defectInforSigle.numGray >= SecondCanshu._mura_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._mura_m_gray);
						bool mura_add = defectInforSigle.num_yichang >= 500;
						mura_2 = mura_2 || mura_add;
						//tuan的判定条件
						bool xianzhi_tuan_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);
						bool xianzhi_tuan_dark = (defectInforSigle.dark_num >= defectInforSigle.bright_num);
						//tuan的第二套参数判定条件
						bool tuan_2 = (defectInforSigle.numGray >= SecondCanshu._tuan_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._tuan_m_gray);
						bool tuan_add = defectInforSigle.num_yichang >= 500;
						tuan_2 = tuan_2 || tuan_add;




						//cout << "xianzhi_tuan_bright:" << xianzhi_tuan_bright << endl;
						//cout << "xianzhi_tuan_dark" << xianzhi_tuan_dark << endl;
						//cout << "tuan_2:" << tuan_2 << endl;


						if (xianzhi_size_point == 1) {

							determine_Type = 0;

						}
						else if (xianzhi_line == 1) {

							determine_Type = 1;

						}

						else if (xianzhi_line == 1) {

							determine_Type = 1;

						}
						//else 
						//{
						//
						//	if (xianzhi_mura == 1) {
						//		determine_Type = 2;
						//	
						//	}
						//	else {
						//	
						//		determine_Type = 3;
						//	
						//	}
						//
						//}
						else if (xianzhi_mura_1 == 1)
						{
							determine_Type = 3;

							if (xianzhi_mura_2 == 1)
							{
								determine_Type = 2;
							}
						}
						else
						{
							defectInforSigle.transYorN = false;
						}







						cout << "determine_Type:" << determine_Type << endl;

						switch (determine_Type)
						{
						case(0): {
							if (xianzhi_if_single_point)  //判定是否为群点
							{
								//判定是否符合二套参数
								if (point_2 == 0)
								{
									defectInforSigle.transYorN = false;  //不满足第二套参数，不输出
								}
								//判定亮暗
								if (xianzhi_point_bright)
								{
									y = 0;
									defectInforSigle.type = "b_p";
									defectInforSigle.typenum = 0;

								}
								else if (xianzhi_point_dark)
								{
									y = 1;
									defectInforSigle.type = "d_p";
									defectInforSigle.typenum = 1;
								}
							}
							else
							{
								defectInforSigle.type = "unknown";
								defectInforSigle.TrueOrFalse = false;
								defectInforSigle.transYorN = false;
								if (xianzhi_if_group_point == 1)    //满足可以进入筛选群点的要求，就加入
								{

									//将满足群点定义的坐标写入group_points中，并返回group_points
									group_temp.x = defectInforSigle.s_cx;
									group_temp.y = defectInforSigle.s_cy;
									//x表示颜色，num_group_point表示该图中的第几个
									(*sendImg1).group_points[cameraFlag][x].push_back(group_temp);
								}


							}


							break;
						};

								 //line判定
						case(1): {

							if (line_2 == 0)
							{
								defectInforSigle.transYorN = false;
							}
							if (xianzhi_line_bright == 1)
							{
								y = 2;
								defectInforSigle.type = "b_l";
								defectInforSigle.typenum = 2;

							}
							else if (xianzhi_line_dark == 1)
							{
								y = 3;
								defectInforSigle.type = "d_l";
								defectInforSigle.typenum = 3;
							}


							break;
						};
						case(2): {
							y = 4;
							defectInforSigle.type = "mura";
							defectInforSigle.typenum = 4;

							if (mura_2 == 0)
							{
								defectInforSigle.transYorN = false;
							}

							break;
						};
						case(3): {
							if (tuan_2 == 0)
							{
								defectInforSigle.transYorN = false;
							}
							if (xianzhi_tuan_bright)
							{
								y = 5;
								defectInforSigle.type = "b_t";
								defectInforSigle.typenum = 5;

							}
							else if (xianzhi_tuan_dark)
							{
								y = 6;
								defectInforSigle.type = "d_t";
								defectInforSigle.typenum = 6;

							}

							break;
						};


						}





						//if (xianzhi_size_point)
						//{
						//	if (xianzhi_if_group_point)  //判定是否为群点
						//	{
						//		//判定亮暗
						//		if (xianzhi_point_bright)
						//		{
						//			y = 0;
						//			defectInforSigle.type = "b_p";
						//			if (point_2)
						//			{
						//				defectInforSigle.transYorN = true;  //满足第二套参数输出
						//			}
						//		}
						//		else if (xianzhi_point_dark)
						//		{
						//			y = 1;
						//			defectInforSigle.type = "d_p";
						//			if (point_2)
						//			{
						//				defectInforSigle.transYorN = true;  //满足第二套参数输出
						//			}
						//		}

						//	}
						//	else
						//	{
						//		defectInforSigle.type = "unknown";
						//		defectInforSigle.TrueOrFalse = false;
						//		//将满足群点定义的坐标写入group_points中，并返回group_points
						//		group_temp.x = defectInforSigle.s_cx;
						//		group_temp.y = defectInforSigle.s_cy;
						//		//x表示颜色，num_group_point表示该图中的第几个
						//		(*sendImg1).group_points[cameraFlag][x].push_back(group_temp);

						//	}
						//}

						//else if (xianzhi_line == 1)
						//{
						//	if (xianzhi_line_bright == 1)
						//	{
						//		y = 2;
						//		defectInforSigle.type = "b_l";
						//		if (line_2 == 1)
						//		{
						//			cout << "66666666666" << endl;
						//			defectInforSigle.transYorN = true;
						//		}
						//	}
						//else if (xianzhi_line_dark == 1)
						//{
						//	y = 3;
						//	defectInforSigle.type = "d_l";
						//	if (line_2 == 1)
						//	{
						//		defectInforSigle.transYorN = true;
						//	}
						//}

						//}

						//else if (xianzhi_mura)
						//{
						//	y = 4;
						//	defectInforSigle.type = "mura";

						//	if (mura_2)
						//	{
						//		defectInforSigle.transYorN = true;
						//	}
						//}

						//else
						//{
						//	if (xianzhi_tuan_bright)
						//	{
						//		y = 5;
						//		defectInforSigle.type = "b_t";
						//		if (tuan_2)
						//		{
						//			defectInforSigle.transYorN = true;
						//		}
						//	}
						//	else if (xianzhi_tuan_dark)
						//	{
						//		y = 6;
						//		defectInforSigle.type = "d_t";
						//		if (tuan_2)
						//		{
						//			defectInforSigle.transYorN = true;
						//		}
						//	}


						//}

						if ((*sendImg1).matrix[cameraFlag][x][y] == 0)
						{
							(*sendImg1).matrix[cameraFlag][x][y] = 1;
						}
						else
							continue;



						//int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
						//Point2f p1[2];
						//p1[0].x = defectsInfor[x][type][i].s_cx - int(defectsInfor[x][type][i].w/2);
						//p1[0].y = defectsInfor[x][type][i].s_cy - int(defectsInfor[x][type][i].h/2);
						//p1[1].x = defectsInfor[x][type][i].s_cx + int(defectsInfor[x][type][i].w/2);
						//p1[1].y = defectsInfor[x][type][i].s_cy + int(defectsInfor[x][type][i].h/2);
						//rectangle((*sendImg1).imgTftColorCuts[x], p1[0], p1[1], (255, 0, 0), 2);
						//cv::putText((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].type, Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 0, 0), 2);
						//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
					}
				}
			}
		}
	}
	//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[x]);


}

//0125  test
//增加了 坐标旋转对应关系
void drawPic()
{

	int font = CV_FONT_HERSHEY_SIMPLEX;
	int h = (*sendImg1).PersPectiveTransformation_hight;
	int w = (*sendImg1).PersPectiveTransformation_weight;
	int judge_h = int(h / 2);
	int judge_w = int(w / 2);

	int row = (*sendImg1).back.rows;
	int	col = (*sendImg1).back.cols;


	//(*sendImg1).back.at<Vec3b>(100,150) = 0;
	//imwrite("d://project//11.png", (*sendImg1).back);
	//cout << "row"<<(*sendImg1).back.rows << endl;
	//顺时针旋转90°
	Mat dst;
	transpose((*sendImg1).back, dst);
	flip(dst, dst, 1);
	(*sendImg1).back = dst.clone();

	//imwrite("d://project//12.png", (*sendImg1).back);


	
	Point2i  p_1, p_2, p_c;
	Point p2[2];
	int x1, x2,x3, x4, y1, y2, y3, y4,x_c, y_c;
	int x00, y00, x11,y11;
	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)
	{
		for (int x = 0; x < (*sendImg1).MaxFlag[cameraFlag]; x++)    //遍历不同颜色的图片
		{
			if (sizeof(defectsInfor[cameraFlag][x]) != NULL)    //结构体不为空
			{
				for (int type = 0; type < 1; type++)    //遍历不同检测方法结果

				{
					if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
					{
						for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
						{

							defectInformation  &defectInforSigle = defectsInfor[cameraFlag][x][type][i];

							//0126 取代之前的rectangle函数，用画线连接的方式
							for (int j = 0; j < 4; j++)
							{
								line((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 0), 1);
							}
							//before
							//rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.P[0], defectInforSigle.P[2], Scalar(255, 0, 0), 2);

							if (defectInforSigle.transYorN == false)
							{
								Point p3[2];
								p3[0].x = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
								p3[0].y = defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
								p3[1].x = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
								p3[1].y = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

								float m1 = defectInforSigle.dst_m_Gray;

								//只保留小数点后两位
								m1 = round(m1 * 100) / 100.0;
								ostringstream oss1;
								oss1 << m1;
								string dst_m_gray1;
								dst_m_gray1 = oss1.str();

								if (defectInforSigle.s_cx > judge_w)
								{
									// 在第四象限
									if (defectInforSigle.s_cy > judge_h)
									{
										//cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p3[1].x - 30, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray1, Point(p3[0].x - 30, p3[0].y - 15), font, 0.6, cvScalar(0, 255, 0, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p3[0].x - 20, p3[0].y), font, 0.6, cvScalar(0, 255, 255, 1), 2);
									}
									//在第二象限
									else
									{
										//cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x - 20, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray1, Point(p3[0].x, p3[1].y + 10), font, 0.6, cvScalar(0, 255, 0, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p3[0].x, p3[1].y + 25), font, 0.6, cvScalar(0, 255, 255, 1), 2);
									}
								}
								else
								{
									//在第三象限
									if (defectInforSigle.s_cy > judge_h)
									{
										//cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray1, Point(p3[0].x, p3[0].y - 15), font, 0.6, cvScalar(0, 255, 0, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p3[0].x - 20, p3[0].y), font, 0.6, cvScalar(0, 255, 255, 1), 2);
									}
									//在第一象限
									else
									{
										//cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray1, Point(p3[1].x, p3[1].y), font, 0.6, cvScalar(0, 255, 0, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p3[0].x, p3[1].y + 10), font, 0.6, cvScalar(0, 255, 255, 1), 2);
									}
								}
							}

							//cv::putText((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].type, Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 0, 0), 2);
							//cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(i + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 1.2, (255, 255, 255), 2);



							//for (int j = 0; j < 4; j++)
							//{
							//	line((*sendImg1).back, defectsInfor[x][type][i].P[j], defectsInfor[x][type][i].P[(j + 1) % 4], Scalar(0, 0, 255), 2);
							//}

							//rectangle((*sendImg1).back, defectInforSigle.P[0], defectInforSigle.P[2], (0, 0, 255), 2);

							if (defectInforSigle.transYorN == true)
							{
									

								//第一个点
								x1 = -defectInforSigle.P[0].y + row;
								y1 = defectInforSigle.P[0].x;
								//第三个点
								x2 = -defectInforSigle.P[2].y + row;
								y2 = defectInforSigle.P[2].x;
								//第二个点
								x3 = -defectInforSigle.P[1].y + row;
								y3 = defectInforSigle.P[1].x;
								//第四个点
								x4 = -defectInforSigle.P[3].y + row;
								y4 = defectInforSigle.P[3].x;

								x_c = -defectInforSigle.s_cy + row;
								y_c = defectInforSigle.s_cx;



								//cout << "x1:" << x1 << endl;
								//cout << "y1:" << y1 << endl;
								//cout << "x2:" << x2 << endl;
								//cout << "y2:" << y2 << endl;
								//cout << "x_c:" << x_c << endl;
								//cout << "y_c:" << y_c << endl;

								Point p_1(x1, y1);
								Point p_2(x2, y2);
								Point p_c(x_c, y_c);

								Point p[4];
								p[0].x = x1;
								p[0].y = y1;
								p[1].x = x3;
								p[1].y = y3;
								p[2].x = x2;
								p[2].y = y2;
								p[3].x = x4;
								p[3].y = y4;


								//0125 test
								//transpose();
							

								for (int k = 0; k < 4; k++)
								{
									line((*sendImg1).back, p[k], p[(k+1)%4], Scalar(0, 0, 255), 3);
								}

								//rectangle((*sendImg1).back, p_1, p_2, Scalar(0, 0, 255), 3);
								//rectangle((*sendImg1).back, defectsInfor[cameraFlag][x][type][i].P[0], defectsInfor[cameraFlag][x][type][i].P[2], Scalar(0, 0, 255), 3);
								//cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), Point(defectsInfor[cameraFlag][x][type][i].s_cx, defectsInfor[cameraFlag][x][type][i].s_cy), font, 1.2, Scalar(0, 0, 0), 2);
								cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), p_c, font, 1.2, Scalar(0, 0, 0), 2);


							

								
								p2[0].x = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
								p2[0].y = defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
								p2[1].x = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
								p2[1].y = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

								//x00 = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
								//y00= defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
								//x11 = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
								//y11 = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

								//Point p22(x00, y00);
								//Point p33(x11, y11);
							

								rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p2[0], p2[1], cvScalar(255 , 255 , 255), 1);
								//rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p22, p33, (255, 255, 0), 1);
								float m = defectInforSigle.dst_m_Gray;

								//只保留小数点后两位
								m = round(m * 100) / 100.0;
								ostringstream oss;
								oss << m;
								string dst_m_gray;
								dst_m_gray = oss.str();
								//cout << "dst_m_gray:"<<dst_m_gray << endl;
								//cout << 99999 << endl;


								// 判定瑕疵书写的部位
								if (defectInforSigle.s_cx > judge_w)
								{
									// 在第四象限
									if (defectInforSigle.s_cy > judge_h)
									{


										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x - 30, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x - 30, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
									//在第二象限
									else
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x - 20, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[1].y + 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x, p2[1].y + 25), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
								}
								else
								{
									//在第三象限
									if (defectInforSigle.s_cy > judge_h)
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
									//在第一象限
									else
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[1].x, p2[1].y), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x, p2[1].y + 10), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
								}
							}
						}
					}
				}
			}
			imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[cameraFlag][x]);
		}

		for (int i = 0; i < 4; i++)
		{
			if ((*sendImg1).R_compare_res[i] == 1)
			{
				Point p_R_1, p_R_2, p_R_3;
				p_R_1.x = -(*sendImg1).P[i][0].y + row;
				p_R_1.y = (*sendImg1).P[i][0].x;
				p_R_2.x = -(*sendImg1).P[i][1].y + row;
				p_R_2.y = (*sendImg1).P[i][1].x;
		
				rectangle((*sendImg1).back, p_R_1, p_R_2, Scalar(0, 0, 255), 4);

				Point point;
				point.x = ((*sendImg1).P[i][0].x + (*sendImg1).P[i][1].x) / 2;
				point.y = ((*sendImg1).P[i][0].y + (*sendImg1).P[i][1].y) / 2;
				p_R_3.x = -point.y + row;
				p_R_3.y = point.x;

				cv::putText((*sendImg1).back, to_string(8), p_R_3, font, 1.2, Scalar(0, 0, 0), 4);
			}
		}

		//imwrite((*sendImg1).sNowTimeFileDir + "final_result.png", (*sendImg1).back);
	}
}




//0220 update drawpic函数
//增加了画框和写信息（针对没有通过二套参数的瑕疵）
//画出二套参数筛选后的图以及信息
void drawSiglePic(int cameraFlag, int imgFlag) {

	int font = CV_FONT_HERSHEY_SIMPLEX;
	int h = (*sendImg1).PersPectiveTransformation_hight;
	int w = (*sendImg1).PersPectiveTransformation_weight;
	int judge_h = int(h / 2);
	int judge_w = int(w / 2);

	int row = (*sendImg1).back.rows;
	int	col = (*sendImg1).back.cols;



	Mat dst;
	transpose((*sendImg1).back, dst);
	flip(dst, dst, 1);
	(*sendImg1).back = dst.clone();




	Point2i  p_1, p_2, p_c;
	Point p2[2];
	int x1, x2, x3, x4, y1, y2, y3, y4, x_c, y_c;
	int x00, y00, x11, y11;

	int x = imgFlag;
	int type = 0;
	if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
	{
		for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
		{

			defectInformation  &defectInforSigle = defectsInfor[cameraFlag][x][type][i];

			//0126 取代之前的rectangle函数，用画线连接的方式
			for (int j = 0; j < 4; j++)
			{
				line((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 1);
			}

			



			if (defectInforSigle.transYorN == true)
			{


				//第一个点
				x1 = -defectInforSigle.P[0].y + row;
				y1 = defectInforSigle.P[0].x;
				//第三个点
				x2 = -defectInforSigle.P[2].y + row;
				y2 = defectInforSigle.P[2].x;
				//第二个点
				x3 = -defectInforSigle.P[1].y + row;
				y3 = defectInforSigle.P[1].x;
				//第四个点
				x4 = -defectInforSigle.P[3].y + row;
				y4 = defectInforSigle.P[3].x;

				x_c = -defectInforSigle.s_cy + row;
				y_c = defectInforSigle.s_cx;



				//cout << "x1:" << x1 << endl;
				//cout << "y1:" << y1 << endl;
				//cout << "x2:" << x2 << endl;
				//cout << "y2:" << y2 << endl;
				//cout << "x_c:" << x_c << endl;
				//cout << "y_c:" << y_c << endl;

				Point p_1(x1, y1);
				Point p_2(x2, y2);
				Point p_c(x_c, y_c);

				Point p[4];
				p[0].x = x1;
				p[0].y = y1;
				p[1].x = x3;
				p[1].y = y3;
				p[2].x = x2;
				p[2].y = y2;
				p[3].x = x4;
				p[3].y = y4;


				//0125 test
				//transpose();


				for (int k = 0; k < 4; k++)
				{
					line((*sendImg1).back, p[k], p[(k + 1) % 4], Scalar(0, 0, 255), 3);
				}

				//rectangle((*sendImg1).back, p_1, p_2, Scalar(0, 0, 255), 3);
				//rectangle((*sendImg1).back, defectsInfor[cameraFlag][x][type][i].P[0], defectsInfor[cameraFlag][x][type][i].P[2], Scalar(0, 0, 255), 3);
				//cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), Point(defectsInfor[cameraFlag][x][type][i].s_cx, defectsInfor[cameraFlag][x][type][i].s_cy), font, 1.2, Scalar(0, 0, 0), 2);
				cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), p_c, font, 1.2, Scalar(0, 0, 255), 2);
				





				p2[0].x = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
				p2[0].y = defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
				p2[1].x = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
				p2[1].y = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

				//x00 = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
				//y00= defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
				//x11 = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
				//y11 = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

				//Point p22(x00, y00);
				//Point p33(x11, y11);


				rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p2[0], p2[1], (255, 255, 0), 1);
				//rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p22, p33, (255, 255, 0), 1);
				float m = defectInforSigle.dst_m_Gray;

				//只保留小数点后两位
				m = round(m * 100) / 100.0;
				ostringstream oss;
				oss << m;
				string dst_m_gray;
				dst_m_gray = oss.str();
				//cout << "dst_m_gray:"<<dst_m_gray << endl;
				//cout << 99999 << endl;


				// 判定瑕疵书写的部位
				if (defectInforSigle.s_cx > judge_w)
				{
					// 在第四象限
					if (defectInforSigle.s_cy > judge_h)
					{


						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x - 30, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x - 30, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
					//在第二象限
					else
					{
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x - 20, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[1].y + 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x, p2[1].y + 25), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
				}
				else
				{
					//在第三象限
					if (defectInforSigle.s_cy > judge_h)
					{
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
					//在第一象限
					else
					{
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectInforSigle.type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[1].x, p2[1].y), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectInforSigle.numGray), Point(p2[0].x, p2[1].y + 10), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
				}
			}
		}
	}
	(*sendImg1).debug_img_secondCanshu = (*sendImg1).imgTftColorCuts[cameraFlag][x].clone();
	imwrite((*sendImg1).sNowTimeFileDir + "debug_img_secondCanshu" + ".jpg", (*sendImg1).debug_img_secondCanshu);

}


//画一套参数的信息与框
//红色表示true瑕疵，蓝色表示false瑕疵
void drawSiglePic_first(int cameraFlag, int imgFlag , int method) {

	int font = CV_FONT_HERSHEY_SIMPLEX;
	int h = (*sendImg1).PersPectiveTransformation_hight;
	int w = (*sendImg1).PersPectiveTransformation_weight;
	int judge_h = int(h / 2);
	int judge_w = int(w / 2);

	int row = (*sendImg1).back.rows;
	int	col = (*sendImg1).back.cols;



	//Mat dst;
	//transpose((*sendImg1).back, dst);
	//flip(dst, dst, 1);
	//(*sendImg1).back = dst.clone();
	Mat img = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();



	Point2i  p_1, p_2, p_c;
	Point p2[2];
	int x1, x2, x3, x4, y1, y2, y3, y4, x_c, y_c;
	int x00, y00, x11, y11;

	int x = imgFlag;
	int type = method;


	if (sizeof(defectsInforGray[cameraFlag][x][type]) != 0)   //不为空
	{
		for (int i = 0; i < (defectsInforGray[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
		{

			defectInformation  &defectInforSigle = defectsInforGray[cameraFlag][x][type][i];

			//0126 取代之前的rectangle函数，用画线连接的方式
			for (int j = 0; j < 4; j++)
			{

				if (defectInforSigle.TrueOrFalse == true)
				{
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
				}

				else
				{
					line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
				}

			}
			
				//第一个点
				x1 = -defectInforSigle.P[0].y + row;
				y1 = defectInforSigle.P[0].x;
				//第三个点
				x2 = -defectInforSigle.P[2].y + row;
				y2 = defectInforSigle.P[2].x;
				//第二个点
				x3 = -defectInforSigle.P[1].y + row;
				y3 = defectInforSigle.P[1].x;
				//第四个点
				x4 = -defectInforSigle.P[3].y + row;
				y4 = defectInforSigle.P[3].x;

				x_c = -defectInforSigle.s_cy + row;
				y_c = defectInforSigle.s_cx;



				//cout << "x1:" << x1 << endl;
				//cout << "y1:" << y1 << endl;
				//cout << "x2:" << x2 << endl;
				//cout << "y2:" << y2 << endl;
				//cout << "x_c:" << x_c << endl;
				//cout << "y_c:" << y_c << endl;

				Point p_1(x1, y1);
				Point p_2(x2, y2);
				Point p_c(x_c, y_c);

				Point p[4];
				p[0].x = x1;
				p[0].y = y1;
				p[1].x = x3;
				p[1].y = y3;
				p[2].x = x2;
				p[2].y = y2;
				p[3].x = x4;
				p[3].y = y4;


				//0125 test
				//transpose();


				//rectangle((*sendImg1).back, p_1, p_2, Scalar(0, 0, 255), 3);
				//rectangle((*sendImg1).back, defectsInfor[cameraFlag][x][type][i].P[0], defectsInfor[cameraFlag][x][type][i].P[2], Scalar(0, 0, 255), 3);
				//cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), Point(defectsInfor[cameraFlag][x][type][i].s_cx, defectsInfor[cameraFlag][x][type][i].s_cy), font, 1.2, Scalar(0, 0, 0), 2);
				//cv::putText((*sendImg1).back, to_string(defectsInfor[cameraFlag][x][type][i].typenum), p_c, font, 1.2, Scalar(0, 0, 255), 2);


				p2[0].x = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
				p2[0].y = defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
				p2[1].x = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
				p2[1].y = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

				//x00 = defectInforSigle.s_cx - int(defectInforSigle.w / 2 - 2);
				//y00= defectInforSigle.s_cy - int(defectInforSigle.h / 2 - 2);
				//x11 = defectInforSigle.s_cx + int(defectInforSigle.w / 2 + 2);
				//y11 = defectInforSigle.s_cy + int(defectInforSigle.h / 2 + 2);

				//Point p22(x00, y00);
				//Point p33(x11, y11);


				//rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p2[0], p2[1], (255, 255, 0), 1);
				//rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p22, p33, (255, 255, 0), 1);
				float m = defectInforSigle.dst_m_Gradient;
				float s = defectInforSigle.dst_s_Gradient;
				//只保留小数点后两位
				m = round(m * 100) / 100.0;
				s = round(s * 100) / 100.0;
				ostringstream oss,oss1;
				oss << m;
				string dst_m_Gradient, dst_s_Gradient;
				dst_m_Gradient = oss.str();
				oss1 << s;
				dst_s_Gradient = oss1.str();
				//cout << "dst_m_gray:"<<dst_m_gray << endl;
				//cout << 99999 << endl;
				//cout << "debug_m_gradient:" << (*sendImg1).debug_m_gradient << endl;
				//cout << "debug_s_gradient:" << (*sendImg1).debug_s_gradient << endl;
				//cout << "debug_mValuegradient:" << (*sendImg1).debug_mValuegradient << endl;
				//cout << "debug_num_gradient:" << (*sendImg1).debug_num_gradient << endl;
				//cout << "numGradient:" << defectInforSigle.numGradient << endl;
				
				//defectInforSigle.numGradient = (*sendImg1).debug_num_gradient;

				// 判定瑕疵书写的部位
				if (defectInforSigle.s_cx > judge_w)
				{
					// 在第四象限
					if (defectInforSigle.s_cy > judge_h)
					{


						cv::putText(img, dst_s_Gradient, Point(p2[1].x - 30, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText(img, dst_m_Gradient, Point(p2[0].x - 30, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText(img, to_string(defectInforSigle.numGradient), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
					//在第二象限
					else
					{
						cv::putText(img, dst_s_Gradient, Point(p2[1].x - 20, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText(img, dst_m_Gradient, Point(p2[0].x, p2[1].y + 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText(img, to_string(defectInforSigle.numGradient), Point(p2[0].x, p2[1].y + 25), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
				}
				else
				{
					//在第三象限
					if (defectInforSigle.s_cy > judge_h)
					{
						cv::putText(img, dst_s_Gradient, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						cv::putText(img, dst_m_Gradient, Point(p2[0].x, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText(img, to_string(defectInforSigle.numGradient), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
					//在第一象限
					else
					{
						cv::putText(img, dst_s_Gradient, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
						//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
						cv::putText(img, dst_m_Gradient, Point(p2[1].x, p2[1].y), font, 0.6, cvScalar(255, 255, 255, 1), 2);
						cv::putText(img, to_string(defectInforSigle.numGradient), Point(p2[0].x, p2[1].y + 10), font, 0.6, cvScalar(255, 255, 0, 1), 2);

					}
				}
			
		}
	}


	namedWindow("showFirstmethod", 0);
	imshow("showFirstmethod", img);

}
//0125
// 用于九宫格和红绿点输出总的瑕疵统计结果--ysq0123
void get_table_result(vector<Point2i>coordinate, int R_res[4], Mat final_img, Mat& res_table)
{
	Point2i p;
	/*
	for (int i = 0; i < 7; i++)
	{
	for (int j = 0; j < 8; j++)
	{
	p.x = 835 + i * 60;
	p.y = 125 + j * 60;
	circle(res_table, p, 16, Scalar(0, 255, 0), -1);
	}
	}
	Point RR;
	RR.x = 3 * 60 + 835;
	RR.y = 8 * 60 + 125;
	circle(res_table, RR, 16, Scalar(0, 255,0), -1);
	rectangle(res_table, Point(984, 1118), Point(1341, 1174), Scalar(0, 255, 0), -1);
	imwrite(".\\result_table\\LCM_res_table.png", res_table);
	*/

	//在相应位置画红点
	for (int i = 0; i < coordinate.size(); i++)
	{
		p.x = coordinate[i].x * 60 + 835;
		p.y = coordinate[i].y * 60 + 125;
		circle(res_table, p, 16, Scalar(0, 0, 255), -1);
	}
	//判断R角缺陷是否有，有则标红点
	int R_size = 0;
	Point R;
	R.x = 3 * 60 + 835;
	R.y = 8 * 60 + 125;
	for (int j = 0; j < 4; j++)
	{
		if (R_res[j] == 1)
		{
			circle(res_table, R, 16, Scalar(0, 0, 255), -1);
			R_size = 1;
			break;
		}
	}

	// 有缺陷则总结果处为红色
	if (coordinate.size()>0 || R_size >0)
		rectangle(res_table, Point(984, 1118), Point(1341, 1174), Scalar(0, 0, 255), -1);

	// 左边贴上结果红框图
	//Mat dst;
	//transpose(final_img, dst);
	//flip(dst, dst, 1);
	resize(final_img, final_img, Size(605, 1130));
	Mat imgROI = res_table(Rect(35, 40, final_img.cols, final_img.rows));
	final_img.copyTo(imgROI);

	//画上九宫格
	line(res_table, Point(238, 38), Point(238, 1174), 0, 3);
	line(res_table, Point(442, 38), Point(442, 1174), 0, 3);
	line(res_table, Point(36, 456), Point(642, 456), 0, 3);
	line(res_table, Point(36, 818), Point(642, 818), 0, 3);

	imwrite((*sendImg1).sNowTimeFileDir + "res_table"  +".jpg", res_table);

}




//模拟labview输入所有数据进行测试
/*************************************************   debug  模式     ****************************************************************/



//debug模式主函数
//0219 add sub的缩进mask操作

//void debug()
//{
//


//
//	/**************************   灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123）   ***********************************/
//	long t51 = GetTickCount();
//
//	//Mat imgDust = imread("..\\E4.png");  //灰灰尘图路径
//	Mat imgDust = imread("..\\0223\\dust3.png");  //灰灰尘图路径
//										   //Mat imgDust = imread("..\\dust0109-2.png");  //灰灰尘图路径
//	camera_flag = 1;
//	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
//	// 灰尘检测多相机场景修改--ysq0123
//	processDust(imgDust, canshuX, 0);  //camera_flag
//
//
//												  //Mat imgDust_tmp = imgDust.clone();  //单相机中没有这张图，故直接用imgdust复制，不影响						
//												  //bool multi_camera = false;// true表示多相机场景，灰尘图有单独的白屏图imgDust_tmp来确定灰尘角点		
//												  //if (multi_camera)
//												  //{
//												  //	imgDust_tmp = imread(".\\2camera-grid\\grid40_left.png");  // 多相机场景中灰层白屏图的路径   //多相机方案目前不被使用
//												  //	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
//												  //}
//												  //processDust(imgDust, configDir, canshuX, imgDust_tmp,0, multi_camera);
//
//												  //计算灰尘梯度，膨胀显示梯度
//												  //中间得到：灰尘图ROI， 灰尘梯度图
//
//	long t52 = GetTickCount();
//	cout << "processDust函数运行的时间为：" << (t52 - t51) << endl;
//
//
//
//
//
//
//
//	/**************************   分区域设定梯度系数（初始化,只针对point和mura）    ***********************************/
//	long t1 = GetTickCount();
//
//	////初始化用于区域调整的mask区域
//	////输入梯度图和边界图，得到边界mask和内缩mask
//
//
//
//	//!!!!!!!!不同的相机需要不同的缩进吗
//	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_point[0][0], (*sendImg1).BorderTFT,
//		(*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);   //20 10 20 50 60
//																												   //创建团的Mask模板
//
//																												   //************  图片1和图片4的大小一样，图片2和图片3的大小一样
//	initChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_muras[0][0], (*sendImg1).BorderTFT,
//		(*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);
//
//
//	//0219 add sub的缩进mask操作
//	initChangePartOfTheRegionalGradient((*sendImg1).subtractTemplateTftImgs[0][0], (*sendImg1).BorderTFT,
//		(*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, *SubCanShuMask);
//
//
//
//	//repadding((*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).shrink_size_mura);
//	//initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_louguang[0], (*sendImg1).BorderTFT,
//	//	(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);
//	//imwrite((*sendImg1).sNowTimeFileDir + "MuraMaskChangeGradietRCorner.png", (*sendImg1).MuraMaskChangeGradietRCorner);
//	//imwrite((*sendImg1).sNowTimeFileDir + "MuraMaskChangeGradientDrawIn.png", (*sendImg1).MuraMaskChangeGradientDrawIn);
//	//imwrite((*sendImg1).sNowTimeFileDir + "BorderTFT.png", (*sendImg1).BorderTFT);
//	//imwrite((*sendImg1).sNowTimeFileDir + "BorderTftDilate.png", (*sendImg1).BorderTftDilate);
//	//initChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);     //20 10 20 50 60																																				   //创建团的Mask模板
//	//initChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);     //20 10 20 50 60
//	//////改变调整区域的调整参数，目前默认为0.0
//	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).gradientTFT_point, (*sendImg1).gradientTFT_point,0.0 , 0.0);
//	//////针对mura设置参数
//	//initChangePartOfTheRegionalGradient(3, 0, 0, 0, 0);     //20 10 20 50 60
//	//ChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_mura, (*sendImg1).gradientTFT_mura,0.0 , 0.0);
//	////针对漏光设置参数
//	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn,(*sendImg1).gradientTFT_louguang, (*sendImg1).gradientTFT_louguang,0.0 , 0.0);
//
//	long t2 = GetTickCount();
//	cout << "区域调整函数运行的时间为：" << (t2 - t1) << endl;
//
//	////test 确保图片大小尺寸是相同的//
//	//cout << "gradientTFT_pointChange.rows:" << (*sendImg1).gradientTFT_pointChange.rows << endl;
//	//cout << "gradientTFT_pointChange.cols:" << (*sendImg1).gradientTFT_pointChange.cols << endl;
//
//
//
//
//	/**************************   综合三种缺陷（减去灰尘图与边界&筛选伪缺陷）    ***********************************/
//	long t31 = GetTickCount();
//	//获取该屏幕的多颜色综合梯度 + 分区域设定系数 + 减去边界和灰尘 + 统计缺陷信息 + 去除假缺陷 + 绘制缺陷
//	bool flag = false;
//	CombineXiaci(flag);
//	long t32 = GetTickCount();
//	cout << "CombineXiaci函数运行的时间为：" << (t32 - t31) << endl;
//
//
//	long t313 = GetTickCount();
//	//determine();
//	determine1();
//	getGrounpPointResult();
//	long t324 = GetTickCount();
//	cout << "determine函数运行的时间为：" << (t324 - t313) << endl;
//	drawPic();
//	long t323 = GetTickCount();
//	cout << "draw函数运行的时间为：" << (t323 - t324) << endl;
//	/**************************   画出经过两步筛选（梯度和灰度筛选后的）瑕疵   ***********************************/
//	long t41 = GetTickCount();
//
//	//for (int imgFlag = 0; imgFlag < 7 ; imgFlag++)
//	//{
//	//	drawFinalResult((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).tftResultShow_all[imgFlag], (*sendImg1).contours_all[imgFlag], imgFlag, flag);
//	//}
//
//
//	long t42 = GetTickCount();
//	cout << "drawFinalResult函数运行的时间为：" << (t42 - t41) << endl;
//
//
//	cout << "整体函数运行的时间为：" << (t32 - t11) << endl;
//
//	//重新获取参数
//	//removeFalseDefects();
//
//
//	// 用于九宫格和红绿点输出总的瑕疵统计结果
//	long ttt1 = GetTickCount();
//	Mat result_table = imread(".\\result_table\\LCM_res_table.png");
//	//get_table_result((*sendImg1).points, (*sendImg1).back, result_table );
//	get_table_result((*sendImg1).points, (*sendImg1).R_compare_res, (*sendImg1).back, result_table);
//	long ttt2 = GetTickCount();
//	cout << "result_table函数运行的时间为：" << (ttt2 - ttt1) << endl;
//
//
//	//imwrite((*sendImg1).sNowTimeFileDir + "result_table.png", result_table);
//	//namedWindow("result_final_show", 0);
//	//imshow("result_final_show", result_table);
//
//
//	//on_trackbar(0, sendImg1);
//	//waitKey(0);
//	destroyAllWindows();
//	//saveImg();
//}


vector <string> GetAllFiles(string path)
{
	vector<string> files;
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	LARGE_INTEGER size;
	hFind = FindFirstFile(path.c_str(), &findData); //搜索第一个文件，创建并返回搜索句柄，有则返回TRUE
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to find first file!\n";
		return files;
	}
	do
	{
		// 忽略"."和".."两个结果 
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)   //strcmp 比较字符，相同则返回0
			continue;
		files.push_back(findData.cFileName);
	} while (FindNextFile(hFind, &findData));
	return files;
}

string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value)
{
	while (true) {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}





//0229  测试图片用的

void setSetTemplate_debug1(bool if_showroi)
{

	/**************************   设置输入图路径/轨迹条     ***********************************/
	/******************  增加camera_flag相机标志位 ---  2019/1/22  ****************************/
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）


	//模拟labview读取图片进入dll
	Mat imgtmp = imread("..\\0223\\wtest2.png");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正

	Mat imgtmp1 = imread("..\\0223\\g1.png");
	camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);
	
	
	//设置相机一模板
	camera_flag = 0;
	SetTemplate(imgtmp, camera_flag, 0, if_showroi);
	//SetTemplate(imgtmp1, camera_flag, 1);

}


//0301 单相机测试使用
void setSetTemplate_debug(bool if_showroi)
{

	/**************************   设置输入图路径/轨迹条     ***********************************/
	/******************  增加camera_flag相机标志位 ---  2019/1/22  ****************************/
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）


	//模拟labview读取图片进入dll
	//Mat imgtmp = imread("..\\4\\color_900_up_white.bmp");  //white
	Mat imgtmp = imread("..\\0223\\wtest2.png");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正


	//Mat imgtmp1 = imread("..\\4\\gray_900_up_white.bmp");
	Mat imgtmp1 = imread("..\\0223\\r2.png");
	camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);

	//Mat imgtmp2 = imread("..\\4\\color_900_right_white.bmp");
	//camera_cali_fisrt(imgtmp2, neican_data, jibian_data, imgtmp2);


	//设置相机一模板
	//if_showroi是否显示roi获取过程
	SetTemplate(imgtmp, 0, 0, if_showroi);  //正拍相机0号白屏
	SetTemplate(imgtmp, 0, 1, if_showroi);  //正拍相机1号其他颜色屏幕
	//SetTemplate(imgtmp1, 1, 0, if_showroi);  //灰尘相机1号白屏
	//SetTemplate(imgtmp2, 2, 0, if_showroi);  // 侧拍相机2号白屏

}



//0301 多相机测试
void setSetTemplate_debug_forMulti(bool if_showroi)
{

	/**************************   设置输入图路径/轨迹条     ***********************************/
	/******************  增加camera_flag相机标志位 ---  2019/1/22  ****************************/
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）


						  //模拟labview读取图片进入dll
	Mat imgtmp = imread("..\\4\\color_900_up_white.bmp");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正


	Mat imgtmp1 = imread("..\\4\\gray_900_up_white.bmp");
	camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);

	Mat imgtmp2 = imread("..\\4\\color_900_right_white.bmp");
	camera_cali_fisrt(imgtmp2, neican_data, jibian_data, imgtmp2);


	//设置相机一模板
	//if_showroi是否显示roi获取过程
	SetTemplate(imgtmp, 0, 0, if_showroi);  //正拍相机0号白屏
	SetTemplate(imgtmp1, 1, 0, if_showroi);  //灰尘相机1号白屏
	SetTemplate(imgtmp2, 2, 0, if_showroi);  // 侧拍相机2号白屏

}


//void debug1()
//{
//
//	setSetTemplate_debug();
//	int camera_flag = 0;	
//	Mat imgtest = imread("..\\0223\\wtest2.png");
//	camera_cali_fisrt(imgtest, neican_data, jibian_data, imgtest);
//
//	//输入其他颜色屏幕图片
//	long t21 = GetTickCount();
//
//
//	Mat imgtest1 = imread("..\\0223\\g1.png");  //其他颜色屏幕
//	long timebegin = GetTickCount();
//	camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);
//
//
//
//	camera_flag = 0;
//	detect(imgtest, camera_flag, 0);
//	detect(imgtest1, camera_flag, 1);
//
//
//
//	//读入灰尘屏幕图片
//	Mat imgDust_tmp = imread("..\\0223\\dusttest1.png");  //1相机的0颜色即为灰尘模板图
//												  //imgDust_tmp = imread("..\\dust0109-2.png");  //1相机的0颜色即为灰尘模板图
//	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
//	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
//
//
//
//	/**************************   灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123）   ***********************************/
//	long t51 = GetTickCount();
//
//	//Mat imgDust = imread("..\\E4.png");  //灰灰尘图路径
//	Mat imgDust = imread("..\\0223\\dusttest1.png");  //灰灰尘图路径
//												  //Mat imgDust = imread("..\\dust0109-2.png");  //灰灰尘图路径
//	camera_flag = 1;
//	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
//	// 灰尘检测多相机场景修改--ysq0123
//	processDust(imgDust, canshuX, 0);  //camera_flag
//
//
//
//
//
//	setEachMethodMask();
//
//
//	/**************************   综合三种缺陷（减去灰尘图与边界&筛选伪缺陷）    ***********************************/
//	long t31 = GetTickCount();
//	//获取该屏幕的多颜色综合梯度 + 分区域设定系数 + 减去边界和灰尘 + 统计缺陷信息 + 去除假缺陷 + 绘制缺陷
//	bool flag = false;
//	CombineXiaci(flag);
//	long t32 = GetTickCount();
//	cout << "CombineXiaci函数运行的时间为：" << (t32 - t31) << endl;
//
//
//	long t313 = GetTickCount();
//	//determine();
//	determine1();
//	getGrounpPointResult();
//	long t324 = GetTickCount();
//	cout << "determine函数运行的时间为：" << (t324 - t313) << endl;
//	drawPic();
//	long t323 = GetTickCount();
//	cout << "draw函数运行的时间为：" << (t323 - t324) << endl;
//	/**************************   画出经过两步筛选（梯度和灰度筛选后的）瑕疵   ***********************************/
//	long t41 = GetTickCount();
//
//	// 用于九宫格和红绿点输出总的瑕疵统计结果
//	Mat result_table = imread(".\\result_table\\LCM_res_table.png");
//	//get_table_result((*sendImg1).points, (*sendImg1).back, result_table );
//	get_table_result((*sendImg1).points, (*sendImg1).R_compare_res, (*sendImg1).back, result_table);
//	namedWindow("result_final_show", 0);
//	imshow("result_final_show", result_table);
//	waitKey(0);
//	destroyAllWindows();
//
//}

//0301新增  多相机版本
void debug1()
{
	// 设置模板
	bool if_show_tmp_roi = false; //是否显示roi获取过程
	setSetTemplate_debug(if_show_tmp_roi);

	// 读入待检测图
	int camera_flag = 0;
	Mat imgtest = imread("..\\4\\color_900_up_white.bmp");
	camera_cali_fisrt(imgtest, neican_data, jibian_data, imgtest);

	//1相机的0颜色即为灰尘模板图
	Mat imgDust_tmp = imread("..\\4\\gray_900_up_white.bmp");
	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);

	Mat imgtest2 = imread("..\\4\\color_900_right_white.bmp");
	camera_cali_fisrt(imgtest2, neican_data, jibian_data, imgtest2);

	//detect
	bool if_show_img_roi = true;  //是否显示roi获取过程
	detect(imgtest, 0, 0, if_show_img_roi);

	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
	detect(imgDust_tmp, 1, 0, if_show_img_roi);

	detect(imgtest2, 2, 0, true);

	//灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123
	//读入灰尘屏幕图片
	Mat imgDust = imread("..\\4\\gray_900_up_dust.bmp");   //gray_900_up_dust.bmp
	camera_flag = 1;
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	// 灰尘检测多相机场景修改--ysq0123
	processDust(imgDust, canshuX, 1);  //camera_flag

	setEachMethodMask();

	/**************************   综合三种缺陷（减去灰尘图与边界&筛选伪缺陷）    ***********************************/
	long t31 = GetTickCount();
	//获取该屏幕的多颜色综合梯度 + 分区域设定系数 + 减去边界和灰尘 + 统计缺陷信息 + 去除假缺陷 + 绘制缺陷
	bool flag = false;
	CombineXiaci(flag);
	long t32 = GetTickCount();
	cout << "CombineXiaci函数运行的时间为：" << (t32 - t31) << endl;

	long t313 = GetTickCount();
	//determine();
	determine1();
	getGrounpPointResult();
	long t324 = GetTickCount();
	cout << "determine函数运行的时间为：" << (t324 - t313) << endl;
	drawPic();
	long t323 = GetTickCount();
	cout << "draw函数运行的时间为：" << (t323 - t324) << endl;
	/**************************   画出经过两步筛选（梯度和灰度筛选后的）瑕疵   ***********************************/
	long t41 = GetTickCount();

	// 用于九宫格和红绿点输出总的瑕疵统计结果
	Mat result_table = imread(".\\result_table\\LCM_res_table.png");
	//get_table_result((*sendImg1).points, (*sendImg1).back, result_table );
	get_table_result((*sendImg1).points, (*sendImg1).R_compare_res, (*sendImg1).back, result_table);
	namedWindow("result_final_show", 0);
	imshow("result_final_show", result_table);
	waitKey(0);
	destroyAllWindows();

}



char* str1_str2(char ch[])
{
	char *tmp = (char *)malloc(sizeof(1024)); //记得释放内存
	char *p = ch;
	int i = 0;
	while (*p != NULL)
	{


		if (*p == '\\')  //注解：反斜杠\字符用'\\'表示
		{
			tmp[i] = '/';
			i++;
			tmp[i] = '/';
			i++;
			p++;
		}
		else {

			tmp[i] = *p;
			i++;
			p++;

		}

	}
	tmp[i] = '\0';
	return tmp;
}

//设置滑竿方便调试
void onWhiteSlicer(int, void * names)
{

	Mat binary;
	int whiteSlicerHold = getTrackbarPos("whiteSlicerHold", "whiteSlicerWindow");
	threshold((*sendImg1).imgTFT[0], binary, whiteSlicerHold, 255, CV_THRESH_BINARY);


	Mat gray;
	cvtColor((*sendImg1).imgTFT[0], gray, CV_BGR2GRAY);
	//高斯平滑
	Mat blurred;
	GaussianBlur(gray, blurred, Size(5, 5), 0, 0);




	imshow("whiteSlicerShow", binary);



}


void onMethod(int, void * names)
{

	int beginDebug = getTrackbarPos("启动调试", "method");
	if (beginDebug == (*sendImg1).beginDebug)
	{
		return;
	}
	else
	{
		(*sendImg1).beginDebug = beginDebug;


	}



	int methondnow = getTrackbarPos("methondnow", "method");



	int DUSTHOLD = (*canshuX)._DUSTHOLD[0][0] = getTrackbarPos("DUSTHOLD", "method");
	int DUSTSHAPE = (*canshuX)._DUSTSHAPE[0][0] = getTrackbarPos("DUSTSHAPE", "method");

	int drawIn = getTrackbarPos("drawIn", "method");
	if (drawIn < 3)
		drawIn = 3;

	int up = getTrackbarPos("up", "method");
	int down = getTrackbarPos("down", "method");
	int left = getTrackbarPos("left", "method");
	int right = getTrackbarPos("right", "method");

	int camera_flag = (*sendImg1).debug_camera_flag;
	int imgFlag = (*sendImg1).debug_imgFlag;
	int method = 0;
	Mat imgtest = (*sendImg1).debug_img.clone();
	(*sendImg1).gradientResultThreshold_bgr_sub[camera_flag][imgFlag] = (*sendImg1).debug_img1.clone();
	(*sendImg1).gradientResultThreshold_muras[camera_flag][imgFlag] = (*sendImg1).debug_img_mura.clone();
	(*sendImg1).gradientResultThreshold_points[camera_flag][imgFlag] = (*sendImg1).debug_img_point.clone();
	(*sendImg1).gradientResultThreshold_subTeps[camera_flag][imgFlag] = (*sendImg1).debug_img_sub.clone();
	(*sendImg1).gradientResultThreshold_louguangs[camera_flag][imgFlag] = (*sendImg1).debug_img_hsv.clone();




	switch (methondnow)
	{
	case 0:
		PointCanShuMaskx.initCanShuMask(drawIn, left, right, up, down, 0, 0);
		break;

	case 1:
		MuraCanShuMaskx.initCanShuMask(drawIn, left, right, up, down, 0, 0);
		break;
	case 2:
		lightCanShuMaskx.initCanShuMask(drawIn, left, right, up, down, 0, 0);
		break;
	case 3:
		SubCanShuMaskx.initCanShuMask(drawIn, left, right, up, down, 0, 0);
		break;
	case 4:
		SubBGRCanShuMaskx.initCanShuMask(drawIn, left, right, up, down, 0, 0);
		break;


	}



	// 灰尘检测函数
	Mat imgDust = (*sendImg1).debug_img_dust.clone();
	processDust(imgDust, canshuX, 0);  //camera_flag

	setEachMethodMask();


	//imwrite((*sendImg1).sNowTimeFileDir + " debug_img_hsv" + ".jpg", (*sendImg1).debug_img_hsv);

	int TFTthreshold_point = 180;
	int TFTthreshold_mura = 180;
	int subThresh = 20;
	int subBGRThresh = 75;
	int hsvThresh = 20;
	//"mean", "method",




	int m[5];
	m[0] = PointCanShureMoveFalsex._dst_m_gradient[camera_flag][imgFlag] = getTrackbarPos("mean", "method");
	m[1] = muraCanShureMoveFalsex._dst_m_gradient[camera_flag][imgFlag] = getTrackbarPos("mean", "method");
	m[2] = LightCanShureMoveFalsex._dst_m_gradient[camera_flag][imgFlag] = getTrackbarPos("mean", "method");
	m[3] = SubCanShureMoveFalsex._dst_m_gradient[camera_flag][imgFlag] = getTrackbarPos("mean", "method");
	m[4] = SubBGRCanShureMoveFalsex._dst_m_gradient[camera_flag][imgFlag] = getTrackbarPos("mean", "method");

	int s[5];
	s[0] = PointCanShureMoveFalsex._dst_s_gradient[camera_flag][imgFlag] = getTrackbarPos("Smean", "method");
	s[1] = muraCanShureMoveFalsex._dst_s_gradient[camera_flag][imgFlag] = getTrackbarPos("Smean", "method");
	s[2] = LightCanShureMoveFalsex._dst_s_gradient[camera_flag][imgFlag] = getTrackbarPos("Smean", "method");
	s[3] = SubCanShureMoveFalsex._dst_s_gradient[camera_flag][imgFlag] = getTrackbarPos("Smean", "method");
	s[4] = SubBGRCanShureMoveFalsex._dst_s_gradient[camera_flag][imgFlag] = getTrackbarPos("Smean", "method");

	int num[5];
	num[0] = PointCanShureMoveFalsex._num_gradient[camera_flag][imgFlag] = getTrackbarPos("numPoint", "method");
	num[1] = muraCanShureMoveFalsex._num_gradient[camera_flag][imgFlag] = getTrackbarPos("numPoint", "method");
	num[2] = LightCanShureMoveFalsex._num_gradient[camera_flag][imgFlag] = getTrackbarPos("numPoint", "method");
	num[3] = SubCanShureMoveFalsex._num_gradient[camera_flag][imgFlag] = getTrackbarPos("numPoint", "method");
	num[4] = SubBGRCanShureMoveFalsex._num_gradient[camera_flag][imgFlag] = getTrackbarPos("numPoint", "method");

	int mValueGradient[5];
	mValueGradient[0] = PointCanShureMoveFalsex._meanValueGradient[camera_flag][imgFlag] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[1] = muraCanShureMoveFalsex._meanValueGradient[camera_flag][imgFlag] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[2] = LightCanShureMoveFalsex._meanValueGradient[camera_flag][imgFlag] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[3] = SubCanShureMoveFalsex._meanValueGradient[camera_flag][imgFlag] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[4] = SubBGRCanShureMoveFalsex._meanValueGradient[camera_flag][imgFlag] = getTrackbarPos("mValueGradient", "method");








	TFTthreshold_point = (*canshuX)._TFTHOLD_points[camera_flag][imgFlag] = getTrackbarPos("方法0", "method");
	TFTthreshold_mura = (*canshuX)._TFTHOLD_muras[camera_flag][imgFlag] = getTrackbarPos("方法1", "method");
	hsvThresh = (*canshuX)._hsvThresh[camera_flag][imgFlag] = getTrackbarPos("方法2", "method");
	subThresh = (*canshuX)._subThresh[camera_flag][imgFlag] = getTrackbarPos("方法3", "method");
	subBGRThresh = (*canshuX)._subBGRThresh[camera_flag][imgFlag] = getTrackbarPos("方法4", "method");


	//if (TFTthreshold_point != (*sendImg1).debug_thre_point) 
	//{
	//	method = 0;
	//}
	//if(TFTthreshold_mura != (*sendImg1).debug_thre_mura) {
	//	method = 1;
	//}
	//if (hsvThresh != (*sendImg1).debug_hsvThre) {
	//	method = 2;
	//}
	//if (subThresh != (*sendImg1).debug_subThre) {
	//	method = 3;
	//}
	//if (subBGRThresh != (*sendImg1).debug_subBGR) {
	//	method = 4;
	//}

	method = methondnow;



	m[method] = getTrackbarPos("mean", "method");
	s[method] = getTrackbarPos("Smean", "method");
	num[method] = getTrackbarPos("numPoint", "method");
	mValueGradient[method] = getTrackbarPos("mValueGradient", "method");


	(*sendImg1).debug_m_gradient = m[method];
	(*sendImg1).debug_s_gradient = s[method];
	(*sendImg1).debug_num_gradient = num[method];
	(*sendImg1).debug_mValuegradient = mValueGradient[method];
	(*sendImg1).debug_thre_point = TFTthreshold_point;
	(*sendImg1).debug_thre_mura = TFTthreshold_mura;
	(*sendImg1).debug_subThre = subThresh;
	(*sendImg1).debug_subBGR = subBGRThresh;
	(*sendImg1).debug_hsvThre = hsvThresh;

	//for (int DefectType = 0; DefectType < 5; DefectType++) 
	//{
	detectDifferentTypeDefect(methondnow, camera_flag, imgFlag);
	//}



	int threadDone;
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_16S);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_16S);
	getOneColorResult(camera_flag, imgFlag, 0, &threadDone, defectsInforGray[camera_flag][imgFlag]);
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_8U);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_8U);
	//画图,show 
	int font = CV_FONT_HERSHEY_SIMPLEX;
	imwrite((*sendImg1).sNowTimeFileDir + " gradientResultThreshold_bgr_sub" +to_string(camera_flag)+ to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[camera_flag][imgFlag]);

	drawSiglePic_first(camera_flag, imgFlag, method);


}


void onSecondParameter(int, void * names)
{


	int beginDebug = getTrackbarPos("启动调试", "secondParameter");
	if (beginDebug == (*sendImg1).beginDebug1)
	{
		return;
	}
	else
	{
		(*sendImg1).beginDebug1 = beginDebug;


	}


	int camera_flag = (*sendImg1).debug_camera_flag;
	int imgFlag = (*sendImg1).debug_imgFlag;
	int method = (*sendImg1).debug_method;
	(*sendImg1).imgTftColorCuts[camera_flag][imgFlag] = (*sendImg1).debug_cutImg.clone();
	//"mean", "method",

	int p_m = 130;   //这个值应该来源于参数
	int p_n = 30;
	int l_m = 130;
	int l_n = 30;
	int t_m = 130;
	int t_n = 30;
	int m_m = 130;
	int m_n = 30;
	int maxDistance = 130;
	int maxGParea = 30;
	int point_bk_num = 130;
	int point_bk_m_gray = 30;








	SecondCanshu._point_m_gray = p_m = getTrackbarPos("p_m", "secondParameter");
	SecondCanshu._point_num = p_n = getTrackbarPos("p_n", "secondParameter");
	SecondCanshu._line_m_gray = l_m = getTrackbarPos("l_m", "secondParameter");
	SecondCanshu._line_num = l_n = getTrackbarPos("l_n", "secondParameter");
	SecondCanshu._mura_m_gray = m_m = getTrackbarPos("m_m", "secondParameter");
	SecondCanshu._mura_num = m_n = getTrackbarPos("m_n", "secondParameter");
	SecondCanshu._tuan_m_gray = t_m = getTrackbarPos("t_m", "secondParameter");
	SecondCanshu._tuan_num = t_n = getTrackbarPos("t_n", "secondParameter");
	SecondCanshu._maxDistance = maxDistance = getTrackbarPos("maxDistance", "secondParameter");
	SecondCanshu._maxGParea = maxGParea = getTrackbarPos("maxGParea", "secondParameter");
	SecondCanshu._point_bk_m_gray = point_bk_m_gray = getTrackbarPos("point_bk_m_gray", "secondParameter");
	SecondCanshu._point_bk_num = point_bk_num = getTrackbarPos("point_bk_num", "secondParameter");





	//对三合一图灰度筛选
	CombAndRemoveGrayFalse(camera_flag, imgFlag, 0, defectsInfor[camera_flag][imgFlag]);


	//分类筛选
	//determine1();
	determine_Single(camera_flag, imgFlag);


	getGrounpPointResult();

	//画出单相机的所有方法的检测结果
	drawSiglePic(camera_flag, imgFlag);


	//画图,show 
	namedWindow("showFinalResult", 0);
	imshow("showFinalResult", (*sendImg1).imgTftColorCuts[camera_flag][imgFlag]);


}

//设置滑竿
void setWhiteSlicerTrackbar()
{
	int whiteSlicerHold = 130;
	namedWindow("whiteSlicerWindow", 0);
	namedWindow("whiteSlicerShow", 0);
	createTrackbar("whiteSlicerHold", "whiteSlicerWindow", &whiteSlicerHold, 255, onWhiteSlicer, &sendImg1);
}


void onDustSlicer(int, void * names)
{

	Mat binary;

	int DUSTHOLD = getTrackbarPos("DUSTHOLD", "DustSlicerWindow");
	threshold((*sendImg1).imgTFT[0], binary, DUSTHOLD, 255, CV_THRESH_BINARY);


	Mat gray;
	cvtColor((*sendImg1).imgTFT[0], gray, CV_BGR2GRAY);
	//高斯平滑
	Mat blurred;
	GaussianBlur(gray, blurred, Size(5, 5), 0, 0);




	imshow("whiteSlicerShow", binary);



}

//hough变换滑杆

void hough_debug(Mat& image, vector<int>houghP, int binary_thres, bool show_img)
{
	int rows = image.rows;
	int cols = image.cols;
	Mat img_draw = image.clone();
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	//高斯平滑
	Mat blurred;
	GaussianBlur(gray, blurred, Size(5, 5), 0, 0);
	//二值化
	Mat binary;
	threshold(blurred, binary, binary_thres, 255, CV_THRESH_BINARY);
	//检测物体轮廓
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//在黑背景上画出轮廓
	Mat tmp_contour(rows, cols, CV_8UC1, Scalar(0));  //8位无符号char型单通道
	drawContours(tmp_contour, contours, -1, Scalar(255), 2);


	//取canny边界或者轮廓边界
	Mat edge = tmp_contour.clone();
	//Canny(blurred, edge, 2, 50, 3);

	/* 累积概率霍夫直线变换*/
	vector<Vec4i>lines;
	//////////11.26改了霍夫变换的参数/////////
	HoughLinesP(edge, lines, 1, CV_PI / 180, houghP[0], houghP[1], houghP[2]);  //  0.68   120   100 00
	//将检测到的线画出来
	vector<Point>cnt;
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//将4列[x1,y1,x2,y2]转化为2列[x1,y1],[x2,y2]
		Point p1(l[0], l[1]);
		Point p2(l[2], l[3]);
		cnt.push_back(p1);
		cnt.push_back(p2);
		line(img_draw, Point(l[0], l[1]), Point(l[2], l[3]),
			Scalar(0, 0, 255), 1, CV_AA);
	}
	;

	//将检测的线段的端点画出来
	Mat tmp_points(rows, cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < cnt.size(); i++)
		circle(tmp_points, cnt[i], 5, Scalar(255, 255, 255), -1);


	long th3 = GetTickCount();

	/*============ 筛选分类这这些点为四类 ============*/
	map<string, vector<Point>>classfied_point;  //对应python中的字典
	map<string, vector<Point>>::iterator iter;

	//sort_points(cnt, classfied_point, sort_dxy);
	vector<int>sort_dxy{ 12,15,202,20 };
	sort_points1(cnt, classfied_point, sort_dxy);  //20190227

	//四类的点分别求拟合直线
	vector<Vec4i>final_points;
	for (iter = classfied_point.begin(); iter != classfied_point.end(); iter++)
	{
		//cout << iter->first << ' ' << iter->second << endl;
		vector<Point>pxy = iter->second;
		Vec4f line_res;
		//直线拟合
		fitLine(pxy, line_res, DIST_L1, 0, 1e-2, 1e-2);
		//cout << "line_res" << line_res << endl;
		//获取点斜式的点和斜率
		Point point0;
		point0.x = line_res[2];
		point0.y = line_res[3];
		double k = line_res[1] / line_res[0];

		//计算直线的端点(y = k(x - x0) + y0)
		Point point1, point2;
		point1.x = 0;
		point1.y = k * (0 - point0.x) + point0.y;
		point2.x = cols;
		point2.y = k * (cols - point0.x) + point0.y;
		Vec4i p;
		p[0] = (int)point2.x;
		p[1] = (int)point2.y;
		p[2] = (int)point1.x;
		p[3] = (int)point1.y;


		// 避免出现斜率无限大的情况 ==== 2018/11/22
		if (line_res[0] < 0.0001)
		{
			//cout << "bad k" << line_res[0] << endl;
			p[0] = pxy[0].x;
			p[1] = pxy[0].y;
			p[2] = pxy[1].x;
			p[3] = pxy[1].y;
		}
		//cout << "p" << p << endl;
		// 避免出现斜率无限大的情况 ==== 2018/11/22


		final_points.push_back(p);
	}
	Point2f p1, p2, p3, p4;
	cross_point(final_points[3], final_points[1], p1);
	//cout << "p1" << ' ' << p1 << endl;
	circle(img_draw, p1, 13, Scalar(0, 255, 0), -1);
	cross_point(final_points[3], final_points[0], p2);
	circle(img_draw, p2, 13, Scalar(0, 255, 0), -1);
	cross_point(final_points[2], final_points[0], p3);
	circle(img_draw, p3, 13, Scalar(0, 255, 0), -1);
	cross_point(final_points[2], final_points[1], p4);
	circle(img_draw, p4, 13, Scalar(0, 255, 0), -1);


	line(img_draw, p1, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p4, Scalar(255, 255, 0), 1);
	line(img_draw, p1, p4, Scalar(255, 255, 0), 1);



	if (show_img)
	{
		namedWindow("tmp_points", 0);
		imshow("tmp_points", tmp_points);
		namedWindow("img_res_draw", 0);
		imshow("img_res_draw", img_draw);
	}

}

//霍夫ROI滑杆回调函数--ysq0228
void SetHoughROI_Func1(int, void * names)
{
	int cameraFlag;
	// 获取各个相机的二值化阈值
	cameraFlag = 0;
	Mat gray;
	cvtColor((*sendImg1).img_roi_debug[cameraFlag], gray, CV_BGR2GRAY);
	//高斯平滑
	//Mat blurred;
	//GaussianBlur(gray, blurred, Size(5, 5), 0, 0);
	int binary_thres0 = getTrackbarPos("Binary0", "SetHoughROIWindow");
	Mat binary0;
	threshold(gray, binary0, binary_thres0, 255, CV_THRESH_BINARY);
	(*sendImg1).hough_binary_debug[cameraFlag] = binary_thres0;
	(*canshuX)._houghthr[cameraFlag][0] = binary_thres0;

	cameraFlag = 1;
	cvtColor((*sendImg1).img_roi_debug[cameraFlag], gray, CV_BGR2GRAY);
	int binary_thres1 = getTrackbarPos("Binary1", "SetHoughROIWindow");
	Mat binary1;
	threshold(gray, binary1, binary_thres1, 255, CV_THRESH_BINARY);
	(*sendImg1).hough_binary_debug[cameraFlag] = binary_thres1;
	(*canshuX)._houghthr[cameraFlag][0] = binary_thres1;

	cameraFlag = 2;
	cvtColor((*sendImg1).img_roi_debug[cameraFlag], gray, CV_BGR2GRAY);
	int binary_thres2 = getTrackbarPos("Binary2", "SetHoughROIWindow");
	Mat binary2;
	threshold(gray, binary2, binary_thres2, 255, CV_THRESH_BINARY);
	(*sendImg1).hough_binary_debug[cameraFlag] = binary_thres2;
	(*canshuX)._houghthr[cameraFlag][0] = binary_thres2;

	namedWindow("BinaryShow_0", 0);
	imshow("BinaryShow_0", binary0);
	namedWindow("BinaryShow_1", 0);
	imshow("BinaryShow_1", binary1);
	namedWindow("BinaryShow_2", 0);
	imshow("BinaryShow_2", binary2);

}

void SetHoughROI_Func2(int, void * names)
{
	vector<int>houghP{ 0,0,0 };
	houghP[0] = getTrackbarPos("HoughP1", "SetHoughROIWindow");
	houghP[1] = getTrackbarPos("HoughP2", "SetHoughROIWindow");
	houghP[2] = getTrackbarPos("HoughP3", "SetHoughROIWindow");

	int cameraFlag = getTrackbarPos("cameraFlag", "SetHoughROIWindow");
	hough_debug((*sendImg1).img_roi_debug[cameraFlag], houghP, (*canshuX)._houghthr[cameraFlag][0], true);

	for (int i = 0; i < 10; i++)
	{
		(*canshuX)._houghP[i] = houghP[i];
	}
	
}

//霍夫ROI设置滑竿--ysq0228
void SetHoughROITrackbar()
{
	//多相机白屏图，用于二值化阈值的滑杆调试
	//按照cameraFlag存放
	(*sendImg1).img_roi_debug[0] = imread("..\\4\\color_900_up_white.bmp"); //color_900_up_white.bmp
	(*sendImg1).img_roi_debug[1] = imread("..\\4\\gray_900_up_white.bmp");  // gray_900_up_white.bmp
	(*sendImg1).img_roi_debug[2] = imread("..\\4\\color_900_right_white.bmp");  //color_900_right_white.bmp

	// 设置滑杆窗口
	namedWindow("SetHoughROIWindow", 0);
	// 针对不同相机位创建二值化阈值滑杆
	// 012数字为cameraFlag
	int Binary0 = 70;
	createTrackbar("Binary0", "SetHoughROIWindow", &Binary0, 200, SetHoughROI_Func1, &sendImg1);
	int Binary1 = 50;
	createTrackbar("Binary1", "SetHoughROIWindow", &Binary1, 200, SetHoughROI_Func1, &sendImg1);
	int Binary2 = 50;
	createTrackbar("Binary2", "SetHoughROIWindow", &Binary2, 200, SetHoughROI_Func1, &sendImg1);

	waitKey(0);
	destroyAllWindows();


	namedWindow("SetHoughROIWindow", 0);
	// 创建HoughP参数滑杆
	int cameraFlag = 2;
	createTrackbar("cameraFlag", "SetHoughROIWindow", &cameraFlag, 5, SetHoughROI_Func2, &sendImg1);
	int HoughP1 = 140;
	createTrackbar("HoughP1", "SetHoughROIWindow", &HoughP1, 200, SetHoughROI_Func2, &sendImg1);
	int HoughP2 = 60;
	createTrackbar("HoughP2", "SetHoughROIWindow", &HoughP2, 200, SetHoughROI_Func2, &sendImg1);
	int HoughP3 = 100;
	createTrackbar("HoughP3", "SetHoughROIWindow", &HoughP3, 200, SetHoughROI_Func2, &sendImg1);


	// 创建sort_point参数滑杆
	//createTrackbar("SetHoughROI_Binary", "SetHoughROIWindow", &SetHoughROI_Binary, 200, SetHoughROI_Func, &sendImg1);

	waitKey(0);
	destroyAllWindows();
}


//int main()
//{
//
//	Initcanshu();
//
//	setSetTemplate_debug(false);
//	//debug1();
//	Mat imgtmp = imread("..\\0223\\wtest2.png");  //white
//	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);
//	(*sendImg1).imgTFT[0] = imgtmp;
//	(*sendImg1).debug_img = imgtmp.clone();
//	Mat imgDust = imread("..\\0223\\dusttest1.png");
//	(*sendImg1).debug_img_dust = imgDust.clone();
//	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
//	string configDir;
//
//	processDust(imgDust,  canshuX, 0);  //camera_flag
//	setmethodTrackbar(imgtmp, 0, 1);  //其他颜色屏幕
//	//setmethodTrackbar(imgtmp, 0, 0);
//
//	//setsecondParameterTrackbar(imgtmp, 0, 1, 0);
//	waitKey(0);
//
//
//	return 0;
//
//
//
//
//}


//0301新增 main函数
//添加了针对霍夫ROI调节的滑杆
//int main()
//{
//	Initcanshu();
//
//	SetHoughROITrackbar();
//	debug1();
//	return 0;
//
//}

//12.21 wzx 创建模板





int main()
{


	Mat imgtmp1 = imread("D:\\lcmProject\\20190228-141244\\FirstMethod_0_1.png");
	camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);
	setMouseChoseArea(imgtmp1);
	Initcanshu();


	//Mat imgtmp2 = imread("..\\4\\color_900_right_white.bmp");
	//camera_cali_fisrt(imgtmp2, neican_data, jibian_data, imgtmp2);



	
	
	
	
	
	vector<string> colorDir;
	vector<vector<string>> filenames;
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\白屏\\");
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\绿屏\\");
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\红屏\\");
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\黑屏\\");
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\蓝屏\\");
	colorDir.push_back("F:\\LCMData\\图片\\goodPic\\灰尘图\\");
	vector<string>replaceName;
	replaceName.push_back("白屏");
	replaceName.push_back("绿屏");
	replaceName.push_back("红屏");
	replaceName.push_back("黑屏");
	replaceName.push_back("蓝屏");
	replaceName.push_back("灰尘图");
	vector<vector<string>> existFile;

	bool ifexit[6] = { true,true,true,true,true };



	 

	int FileNum = colorDir.size();
	for (int filenum = 0; filenum < FileNum; filenum++)
	{
		vector<string> filename;
		string path = colorDir[filenum];
		path = path + "*.*";
		filename = GetAllFiles(path);
		filenames.push_back(filename);

	}

	int whitePicNum = filenames[0].size();
	//cout<<whitePicNum << endl;
	for (int filenum = 0; filenum < whitePicNum; filenum++)
	{
		cout << filenum << endl;
		vector<string> sixFilename;
		sixFilename.push_back(colorDir[0] + filenames[0][filenum]);
		
		
		
		for (int replaceNum = 1; replaceNum <6; replaceNum++)
		{
			string whiteTmpName = filenames[0][filenum];
			string tmpName = replace_all(whiteTmpName, "白屏", replaceName[replaceNum]);

			{
				vector<string>::iterator it;
				it = find(filenames[replaceNum].begin(), filenames[replaceNum].end(), tmpName);

				if (it != filenames[replaceNum].end())
				{
					ifexit[replaceNum] = true;
				}
				else
				{
					ifexit[replaceNum] = false;
					//vec中不存在value值
				}




			}


			tmpName = colorDir[replaceNum] + tmpName;
			sixFilename.push_back(tmpName);
			//cout << tmpName << endl;
		}



		 if(ifexit[0] == true && ifexit[1] == true && ifexit[2] == true && ifexit[3] == true && ifexit[4] == true && ifexit[5] == true)
		{
			existFile.push_back(sixFilename);

		}
	
	}






	for (int i = 0; i < 5; i++)
	{
				
		
																		  //	flag1 = 1;
		cout << existFile[0][i] << endl;
		Mat imgtmp1 = imread(existFile[0][i]);
		camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);
		//	camera_flag = 0;
		SetTemplate(imgtmp1, 0, i,true);
	}

	for (int i = 0; i < existFile.size(); i++)
	{

		for (int j = 0; i < 5; j++)

		{
			Mat imgtmp1 = imread(existFile[i][j]);
			camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);
			detect(imgtmp1, 0, j, false);

		
		}

		Mat imgDust = imread((existFile[i][5])); 

		camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
		processDust(imgDust, canshuX, 0);  //camera_flag
		CombineXiaci(false);

		determine1();
		getGrounpPointResult();
		drawPic();
		Mat result_table = imread(".\\result_table\\LCM_res_table.png");
		//get_table_result((*sendImg1).points, (*sendImg1).back, result_table );
		get_table_result((*sendImg1).points, (*sendImg1).R_compare_res, (*sendImg1).back, result_table);








	}












	return 0;




}