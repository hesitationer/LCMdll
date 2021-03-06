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
#include "LCMdll.h"
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

//参数初始化
int houghthr;   //霍夫变换二值化参数
int DUSTHOLD;  //104
int DUSTSHAPE;   //5
int fill1;  //灰度异常总数判断阈值

int TFTHOLD_points;   //130     //TFTHOLD是图片的灰度图二值化的阈值，在SubDustAndBorder函数里面调用
int TFTHOLD_muras;   //130     //TFTHOLD是图片的灰度图二值化的阈值，在SubDustAndBorder函数里面调用

int sort_dxy1;    //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy2;    //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy3;     //灰尘相机的侧拍参数（用于划分四个边界点）
int sort_dxy4;    //灰尘相机的侧拍参数（用于划分四个边界点）
int subThresh;
int subBGRThresh;
int bgrThresh;
int hsvThresh;
int Rarea;
int i, j;

CanShu canshuExtern(subThresh, subBGRThresh, bgrThresh, hsvThresh, sort_dxy1, sort_dxy2, sort_dxy3, sort_dxy4, houghthr, DUSTHOLD, DUSTSHAPE, fill1, Rarea, TFTHOLD_muras, TFTHOLD_points, i, j);

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
void SetTemplate(Mat image, int cameraFlag, int pictureFlag)
{
	/*
	输入：Template[flag]  模板图
	输出：CutTemplate[flag] 模板ROI
	*/
	(*sendImg1).Template[cameraFlag][pictureFlag] = image.clone();
	(*sendImg1).imgTFT[cameraFlag] = image;

	if (pictureFlag == 0)  // 白屏模板
	{

		//【获取ROI图】imgTftColorCut，true表示此时为模板图，会【保存四个角点信息】写在文件中
		getRoi(cameraFlag, true);

		imwrite("D://project//0getRoi_after.png", image);

		// 检测模板R角，并裁剪四个R角保存【只在0相机的白屏模板保存R角模板--2019/01/23】
		if (cameraFlag == 0)
			detectRCornorInit(cameraFlag);
		//    imwrite("D://project//0detect_R.png", image);
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

	imwrite("D://project//0getRoi_after0.png", image);

	// 写入模板ROI
	(*sendImg1).CutTemplate[cameraFlag][pictureFlag] = (*sendImg1).imgTftColorCut[cameraFlag].clone();
	//0108 lyl add
	//将模板进行填充
	//计算模板图片的梯度图片并进行保存
	(*sendImg1).CutTemplate_fill[cameraFlag][pictureFlag] = fill((*sendImg1).CutTemplate[cameraFlag][pictureFlag], 20);  // fill =20
	pointDetect((*sendImg1).CutTemplate[cameraFlag][pictureFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][pictureFlag]);

	imwrite("D://project//0getRoi_after1.png", image);

	//如果是0号相机白屏，创建手机轮廓图片  227
	if ((cameraFlag == 0) && (pictureFlag == 0))
	{
		getBorder(cameraFlag, sendImg1);
		setEachMethodMask();
	}

	imwrite("D://project//0getRoi_after2.png", image);

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
void processDust(Mat imgDust, CanShu * canshuX, int cameraFlag)
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
			RGB_Threshold = (*canshuX)._TFTHOLD_points[cameraFlag][pictureFlag];
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
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_b_mura" + ".jpg", imgOut_b);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_g_mura" + ".jpg", imgOut_g);
		imwrite((*sendImg1).sNowTimeFileDir + "imgOut_r_mura" + ".jpg", imgOut_r);


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
			 // 新添加一种检测方法，使用bgr-bgr30
	case(4): {



		Mat channels[3];
		//必须分通道
		drawIn((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColor_bgr_sub[cameraFlag], (*sendImg1).shrink_size_subBGR);
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

		cout << "(*canshuX)._subBGRThresh:" << (*canshuX)._subBGRThresh << endl;
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


		imwrite((*sendImg1).sNowTimeFileDir + "b30" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b30);
		imwrite((*sendImg1).sNowTimeFileDir + "g30" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g30);
		imwrite((*sendImg1).sNowTimeFileDir + "r30" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r30);

		imwrite((*sendImg1).sNowTimeFileDir + "b500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", b500);
		imwrite((*sendImg1).sNowTimeFileDir + "g500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", g500);
		imwrite((*sendImg1).sNowTimeFileDir + "r500" + to_string(cameraFlag) + to_string(pictureFlag) + ".jpg", r500);


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
void detect(Mat imgTFT, int cameraFlag, int pictureFlag)
{
	(*sendImg1).imgTFT[cameraFlag] = imgTFT;   //放入待检测图片

	/****************** result为第一次判断检测图与模板图差异是否太大的结果  ****************************/

	bool result = judgment((*sendImg1).imgTFT[cameraFlag], cameraFlag, pictureFlag);

	if (result == true)  // 这次judgment在于看旋转偏差，偏差不大则进行检测图的【ROI获取+R角检测】
	{
		if (pictureFlag == 0)  //如果是白屏创建工程文件夹，计算iou
		{
			if (cameraFlag == 0)//【只0相机时创建工程文件目录，避免重复创建--0123】
			{
				//创建存放中间检测过程以及最终检测结果图片的文件夹

				

			}


			//告诉detect检测到第几张图的标准位清零
			(*sendImg1).MaxFlag[cameraFlag] = 0;

			//白屏时候进行漏光检测
			//getLightLeakArea();


			//检测图【ROI获取】（同时对检测到的四个角点坐标进行与模板的角点匹配检测【只对0相机进行角点匹配--0123】）
			getRoi(cameraFlag, false);

			imwrite("D://project//0img_aftergetroi.png", imgTFT);

			imwrite("D://project//0img_beforegetborder.png", imgTFT);
			getBorder(cameraFlag, sendImg1);
			imwrite("D://project//0img_aftergetborder.png", imgTFT);

			

			//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
			if (cameraFlag == 1)
				return;

			//只白屏检测图做【R角检测】（与模板R角相减）
			// 【只0相机做R角检测--0123】

			if (cameraFlag == 0)
				getRCornorResult(cameraFlag);
			//	imwrite("D://project//0img_aftergetRcorner.png", imgTFT);


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

		////4.！！！需要修改成多相机模式
		bool result1 = judgment1((*sendImg1).imgTftColorCut[cameraFlag], cameraFlag, pictureFlag);



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
			getBorder(cameraFlag, sendImg1);

			imwrite("D://project//0img_aftergetborder2.png", imgTFT);


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

	imwrite((*sendImg1).saveFileDir + "gradientResultThreshold" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_point" + ".png", (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag]);
	removeGradientFalse((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], (*sendImg1).pointResults[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 0);  //true


	if ((*sendImg1).if_save_midlle_result == 1)
	{
		ofstream location_out;
		location_out.open((*sendImg1).fir_result_file, std::ios::out | std::ios::app);
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
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
					}
					else
					{
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
					}
				}
				cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
				location_out << "第零种方法第" << (j + 1) << "个缺陷信息" << endl;
				location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
				location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
				location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
			}
		}
		imwrite((*sendImg1).saveFileDir + "zeroMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
		location_out.close();
	}

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
	imwrite((*sendImg1).saveFileDir + "gradientResultThreshold" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_mura" + ".png", (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag]);
	//removeMuraFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_muras[imgFlag], (*sendImg1).muraResults[imgFlag], muraCanShureMoveFalse, imgFlag, flag, defect);   //flag

	//0118
	removeGradientFalse((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], (*sendImg1).muraResults[cameraFlag][imgFlag], muraCanShureMoveFalse, cameraFlag, imgFlag, true, defect, 1);   //flag


	if ((*sendImg1).if_save_midlle_result == 1)
	{
		ofstream location_out;
		location_out.open((*sendImg1).fir_result_file, std::ios::out | std::ios::app);
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
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
					}
					else
					{
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
					}
				}
				cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
				location_out << "第一种方法第" << (j + 1) << "个缺陷信息" << endl;
				location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
				location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
				location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
			}
		}
		imwrite((*sendImg1).saveFileDir + "FirstMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
		location_out.close();
	}

}


//该函数流程：先减边除尘，然后去除假缺陷
void getOneColorLouguangResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	ChangePartOfTheRegionalGradient((*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).LightMaskChangeGradientDrawIn, (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder((*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).contours_louguangs[cameraFlag][imgFlag], cameraFlag, imgFlag);
	imwrite((*sendImg1).saveFileDir + "gradientResultThreshold_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_louguangs.jpg", (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag]);


	removeGradientFalse((*sendImg1).GradientResult_louguang[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_louguangs[cameraFlag][imgFlag], (*sendImg1).contours_louguangs[cameraFlag][imgFlag], (*sendImg1).louguangResults[cameraFlag][imgFlag], SubCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 2);

	if ((*sendImg1).if_save_midlle_result == 1)
	{
		ofstream location_out;
		location_out.open((*sendImg1).fir_result_file, std::ios::out | std::ios::app);
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
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
					}
					else
					{
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
					}

				}
				cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
				location_out << "第二种方法第" << (j + 1) << "个缺陷信息" << endl;
				location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
				location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
				location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
			}
		}
		imwrite((*sendImg1).saveFileDir + "SecondMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
		location_out.close();
	}

}

//该函数流程：先减边除尘，然后去除假缺陷
//0219  增加了change函数
//修改成sub的参数
void getOneColorSubTemplateResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	//imwrite((*sendImg1).sNowTimeFileDir + "subtractTemplateTftImgs" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag]);
	ChangePartOfTheRegionalGradient((*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder_subTmp((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag], imgFlag);
	//添加梯度图的写
	imwrite((*sendImg1).saveFileDir + "gradientResultThreshold_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_subTeps.png", (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag]);
	//removeSubTmpFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_subTmps[imgFlag], (*sendImg1).subTmpResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect);

	//0118
	removeGradientFalse((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag], (*sendImg1).subTmpResults[cameraFlag][imgFlag], SubCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 3);

	if ((*sendImg1).if_save_midlle_result == 1)
	{
		ofstream location_out;
		location_out.open((*sendImg1).fir_result_file, std::ios::out | std::ios::app);
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
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
					}
					else
					{
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
					}
				}
				cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
				location_out << "第三种方法第" << (j + 1) << "个缺陷信息" << endl;
				location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
				location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
				location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
			}
		}
		imwrite((*sendImg1).saveFileDir + "ThirdMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
		location_out.close();
	}

}


//new method
void getOneColorbgrSubTemplateResult(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	imwrite((*sendImg1).sNowTimeFileDir + " beforesub" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder((*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).contours_BGRsub[cameraFlag][imgFlag], cameraFlag, imgFlag);
	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_bgr_sub before" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
	imwrite((*sendImg1).saveFileDir + " aftersub_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_.png", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);

	//0118
	removeGradientFalse((*sendImg1).bgrsubTemplateImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag], (*sendImg1).contours_BGRsub[cameraFlag][imgFlag], (*sendImg1).BGRsubResults[cameraFlag][imgFlag], SubBGRCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 4);

	if ((*sendImg1).if_save_midlle_result == 1)
	{
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
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(255, 0, 0), 2);
					}
					else
					{
						line(img, defectInforSigle.P[j], defectInforSigle.P[(j + 1) % 4], Scalar(0, 0, 255), 2);
					}
				}
				cv::putText(img, to_string(j + 1), Point(defectInforSigle.s_cx, defectInforSigle.s_cy), font, 0.6, cvScalar(0, 255, 0, 1), 2);
				location_out << "第四种方法第" << (j + 1) << "个缺陷信息" << endl;
				location_out << "梯度均值：" << defectInforSigle.dst_m_Gradient << endl;
				location_out << "梯度标准差：" << defectInforSigle.dst_s_Gradient << endl;
				location_out << "梯度异常点个数：" << defectInforSigle.numGradient << endl;
			}
		}

		imwrite((*sendImg1).saveFileDir + "after_remove_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + "_.png", (*sendImg1).gradientResultThreshold_bgr_sub[cameraFlag][imgFlag]);
		imwrite((*sendImg1).saveFileDir + "ForthMethod" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", img);
		location_out.close();
	}

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

							bool point_add = (30 <= defectsInfor[cameraFlag][x][type][i].num_yichang < 500);
							bool line_add = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 500);


							if (defectInforSigle.TrueOrFalse == true)
							{
								//point的判定条件
								bool point_h = (defectInforSigle.h < 10 + 19);
								bool point_w = (defectInforSigle.w < 10 + 19);
								bool point_w_h = point_w && point_w;
								bool point_num = (defectInforSigle.numGray < 500);
								bool xianzhi_size_point = point_w_h && point_num;    //点的大小限制
								cout << "颜色：" << x << "类型：" << type << "顺序：" << i << endl;
								bool xianzhi_if_group_point1 = defectInforSigle.numGray > 50;     //判定单点、群点定义
								bool xianzhi_if_group_point2 = defectInforSigle.num_yichang >= 30;     //判定单点、群点定义
								bool xianzhi_if_group_point = (xianzhi_if_group_point1 || xianzhi_if_group_point2);
								bool xianzhi_point_bright = (defectInforSigle.bright_num > defectInforSigle.dark_num);  //亮点
								bool xianzhi_point_dark = (defectInforSigle.dark_num >= defectInforSigle.bright_num);    //暗点
																																								//point的第二套参数判定条件
								bool point_2 = (defectInforSigle.numGray >= SecondCanshu._point_num) && (abs(defectInforSigle.dst_m_Gray) > SecondCanshu._point_m_gray);
								//bool point_3 = (defectsInfor[cameraFlag][x][type][i].num_yichang >= 30);
								//point_2 = (point_2 || point_3);
								//cout << i << endl;


								cout << "cameraFlag:" << cameraFlag << endl;
								cout << "No." << x << "个图片信息：" << endl;
								cout << "第" << i << "个瑕疵：" << endl;
								cout << "point_h:" << point_h << endl;
								cout << "point_w:" << point_w << endl;
								cout << "point_w_h:" << point_w_h << endl;
								cout << "point_num:" << point_num << endl;
								cout << "xianzhi_size_point:" << xianzhi_size_point << endl;
								cout << "xianzhi_if_group_point:" << xianzhi_if_group_point << endl;
								cout << "xianzhi_point_bright:" << xianzhi_point_bright << endl;
								cout << "xianzhi_point_dark:" << xianzhi_point_dark << endl;
								cout << "point_2: " << point_2 << endl;
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
	int x1, x2, x3, x4, y1, y2, y3, y4, x_c, y_c;
	int x00, y00, x11, y11;
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
									line((*sendImg1).back, p[k], p[(k + 1) % 4], Scalar(0, 0, 255), 3);
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


								rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p2[0], p2[1], cvScalar(255, 255, 255), 1);
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

}

//画一套参数的信息与框
//红色表示true瑕疵，蓝色表示false瑕疵
void drawSiglePic_first(int cameraFlag, int imgFlag, int method) {

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
			ostringstream oss, oss1;
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
	if (coordinate.size() > 0 || R_size > 0)
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

	imwrite((*sendImg1).sNowTimeFileDir + "res_table" + ".jpg", res_table);

}

/*************************************************   debug  模式     ****************************************************************/

//debug模式主函数
//0219 add sub的缩进mask操作

void debug()
{

	/**************************   设置输入图路径/轨迹条     ***********************************/
	string configDir;
	//setTestData();  //现已不在这设置图片路径01/05
	//BuildingTemplate();

	//setremoveFalseTrackbar();



	/******************  增加camera_flag相机标志位 ---  2019/1/22  ****************************/
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）



						  /**************************   构建模板  （模板颜色flag1）   ***********************************/

						  // ++++++++++++++++++++++++++  cameraFlag =0 （正拍主相机）


						  //构建白屏模板，不同flag表示不同颜色的模板屏幕输入，flag1 = 0表示白屏，至少需要构建白屏模板
	int flag1 = 0;   //模板颜色flag1
					 //Mat imgtmp = imread("..\\A4.png");  //white
					 //Mat imgtmp = imread("..\\white.png");  //white
	Mat imgtmp = imread("..\\0223\\wtest.png");  //white
										//Mat imgtmp = imread("..\\E4.png");  //white
										//Mat imgtmp = imread("..\\white0109.png");  //white
										//Mat imgtmp = imread("..\\y_1.png");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正

																  /*输入：Template[flag]  模板图
																  输出：CutTemplate[flag] 模板ROI*/
																  //SetTemplate(imgtmp, camera_flag, flag1);  // 增加camera_flag


																  //构建flag= 1的颜色屏幕模板
	flag1 = 1;
	//Mat imgtmp1 = imread("..\\A4.png");
	Mat imgtmp1 = imread("..\\0223\\wtest.png");
	//Mat imgtmp1 = imread("..\\green0109.png");
	//Mat imgtmp1 = imread("..\\blue0109.png");
	//Mat imgtmp1 = imread("..\\red0109.png");
	//Mat imgtmp1 = imread("..\\blue.png");
	//Mat imgtmp1 = imread("..\\white.png");

	camera_cali_fisrt(imgtmp1, neican_data, jibian_data, imgtmp1);
	camera_flag = 0;
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag,4);
	//SetTemplate(imgtmp1, camera_flag,5);
	//SetTemplate(imgtmp1, camera_flag, 6);
	//SetTemplate(imgtmp1, 2);
	//SetTemplate(imgtmp1, 3);
	//SetTemplate(imgtmp1, 4);
	//SetTemplate(imgtmp1, 5);
	//SetTemplate(imgtmp1, 6);
	//SetTemplate(imgtmp1, 7);
	//SetTemplate(imgtmp1, 8);
	//SetTemplate(imgtmp1, 9);
	//SetTemplate(imgtmp1, 10);
	//SetTemplate(imgtmp1, 11);
	//SetTemplate(imgtmp1, 12);


	// ++++++++++++++++++++++++++  cameraFlag =1（灰尘相机）

	//Mat Dust_tmp = imread("..\\E4.png");  //1相机的0颜色即为灰尘模板图
	//Mat Dust_tmp = imread("..\\dust.png");  //1相机的0颜色即为灰尘模板图
	Mat Dust_tmp = imread("..\\0223\\dust3.png");  //1相机的0颜色即为灰尘模板图
												 //Mat Dust_tmp = imread("..\\dust0109-2.png");  //1相机的0颜色即为灰尘模板图
												 //Mat Dust_tmp = imread("..\\y_dust_1.png");  //1相机的0颜色即为灰尘模板图
	camera_cali_fisrt(Dust_tmp, neican_data, jibian_data, Dust_tmp);


	camera_flag = 1;
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);






	//Dust_tmp = imread("..\\E4.png");  //1相机的0颜色即为灰尘模板图
	//Dust_tmp = imread("..\\dust.png");  //1相机的0颜色即为灰尘模板图
	//camera_cali_fisrt(Dust_tmp, neican_data, jibian_data, Dust_tmp);



	//camera_flag = 2;
	//SetTemplate(imgtmp, camera_flag, 0);
	//SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);


	//camera_flag = 3;
	//SetTemplate(imgtmp, camera_flag, 0);
	//SetTemplate(imgtmp1, camera_flag, 1);
	////SetTemplate(imgtmp1, camera_flag, 2);
	////SetTemplate(imgtmp1, camera_flag, 3);
	////SetTemplate(imgtmp1, camera_flag, 4);
	////SetTemplate(imgtmp1, camera_flag, 5);
	////SetTemplate(imgtmp1, camera_flag, 6);



	//camera_flag = 4;
	//SetTemplate(imgtmp, camera_flag, 0);
	//SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);




	//TestPart();


	/**************************   进行缺陷（梯度）检测   （检测图的颜色flag2）  ***********************************/
	// ++++++++++++++++++++++++++  cameraFlag =0 （正拍主相机）
	long t11 = GetTickCount();
	int flag2 = 0;   // 检测图的颜色flag2
	camera_flag = 0;
	//Mat imgtest = imread("..\\A4.png");
	//Mat imgtest = imread("..\\w_h_18.png");
	Mat imgtest = imread("..\\0223\\wtest.png");
	//Mat imgtest = imread("..\\E4.png");
	//Mat imgtest = imread("..\\w_test.png");
	//Mat imgtest = imread("..\\w_test_1.png");
	//Mat imgtest = imread("..\\R2.png");
	//Mat imgtest = imread("..\\y_1.png");
	//Mat imgtest = imread("..\\0109test\\w_l_5.png");
	//Mat imgtest = imread("..\\0109test\\w_xl_36.png");

	//Mat imgtest = imread("..\\w_h_18.png");
	// 待检测图		
	//Mat imgtest = imread("..\\white.png");  // 待检测图		
	camera_cali_fisrt(imgtest, neican_data, jibian_data, imgtest);

	// 进行【缺陷（梯度）检测】 + 【R角检测】，且检测之前先judgement判断检测图与模板差异大不大
	// 得到的中间结果有：【边界轮廓】，  【点、mura、漏光三种梯度图】，  与模板的ROI【差分灰度图】

	long t12 = GetTickCount();
	cout << "detect-0函数运行的时间为：" << (t12 - t11) << endl;


	//输入其他颜色屏幕图片
	long t21 = GetTickCount();
	flag2 = 1;
	//0108 test pics lyl
	//Mat imgtest1 = imread("D:\\LCM12.20\\LCM\\PictureData\\blueTest\\m\\b_m_1.png");  //0
	//Mat imgtest2 = imread("D:\\LCM12.20\\LCM\\PictureData\\blueTest\\m\\b_m_11.png");  //1
	//Mat imgtest3 = imread("D:\\LCM12.20\\LCM\\PictureData\\blueTest\\m\\b_m_15.png");  //2
	//Mat imgtest4 = imread("D:\\LCM12.20\\LCM\\PictureData\\blue_detected\\b_Group_point_7.png");  //3
	//Mat imgtest5 = imread("D:\\LCM12.20\\LCM\\PictureData\\blue_detected\\b_h_1.png");  //4

	Mat imgtest1 = imread("..\\0223\\wtest.png");  //其他颜色屏幕
	//Mat imgtest1 = imread("..\\0109test\\g_l_6.png");
	//Mat imgtest1 = imread("..\\0109test\\b_xm_20.png");
	//Mat imgtest1 = imread("..\\0109test\\r_m_51.png");
	//Mat imgtest1 = imread("..\\test1.png");
	//Mat imgtest1 = imread("..\\A4.png");
	//Mat imgtest1 = imread("..\\test.png");
	//Mat imgtest1 = imread("..\\w_h_39.png");
	//0
	//Mat imgtest1 = imread("..\\test.png");  //0
	//Mat imgtest2 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\g_m_47.png");  //1
	//Mat imgtest3 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\g_h_2.png");  //2
	//Mat imgtest1 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\g_m_40.png");  //3
	//Mat imgtest1 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\b_m_11.png");  //4
	//Mat imgtest1 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\r_m_12.png");  //4
	//Mat imgtest6 = imread("D:\\LCM12.20\\LCM\\PictureData\\forHC\\g_m_47.png");  //4

	long timebegin = GetTickCount();
	camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);



	camera_flag = 0;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);
	//detect(imgtest1, camera_flag,2);
	//detect(imgtest1, camera_flag,3);
	//detect(imgtest1, camera_flag, 4);
	//detect(imgtest1, camera_flag,5);
	//detect(imgtest1, camera_flag, 6);


	long timeend = GetTickCount();
	cout << "detect-0函数运行的时间为：" << (timeend - timebegin) << endl;
	//camera_cali_fisrt(imgtest2, neican_data, jibian_data, imgtest2);
	//detect(imgtest2, 2);

	//camera_cali_fisrt(imgtest3, neican_data, jibian_data, imgtest3);
	//detect(imgtest3, 3);

	//camera_cali_fisrt(imgtest4, neican_data, jibian_data, imgtest4);
	//detect(imgtest4, 4);


	//camera_cali_fisrt(imgtest5, neican_data, jibian_data, imgtest5);
	//detect(imgtest5, 5);

	//camera_cali_fisrt(imgtest6, neican_data, jibian_data, imgtest6);
	//detect(imgtest6, 6);

	//detect(imgtest, 7);
	//detect(imgtest, 8);
	//detect(imgtest, 9);
	//detect(imgtest, 10);
	//detect(imgtest, 11);
	//detect(imgtest, 12);
	//long t22 = GetTickCount();
	//cout << "detect-1函数运行的时间为：" << (t22 - t21) << endl;




	// ++++++++++++++++++++++++++  cameraFlag =1（灰尘相机）

	//Mat imgDust_tmp = imread("..\\E4.png");  //1相机的0颜色即为灰尘模板图
	Mat imgDust_tmp = imread("..\\0223\\dust3.png");  //1相机的0颜色即为灰尘模板图
											   //Mat imgDust_tmp = imread("..\\dust0109-2.png");  //1相机的0颜色即为灰尘模板图
	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123




	camera_flag = 1;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);
	//detect(imgtest1, camera_flag, 2);
	//detect(imgtest1, camera_flag, 3);
	//detect(imgtest1, camera_flag, 4);
	//detect(imgtest1, camera_flag, 5);
	//detect(imgtest1, camera_flag, 6);





	//detect(imgDust_tmp, camera_flag, 5);


	//imgDust_tmp = imread("..\\E4.png");  //1相机的0颜色即为灰尘模板图
	imgDust_tmp = imread("..\\0223\\dust3.png");  //1相机的0颜色即为灰尘模板图
										   //imgDust_tmp = imread("..\\dust0109-2.png");  //1相机的0颜色即为灰尘模板图
	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123




	//camera_flag = 2;
	//detect(imgtest, camera_flag, 0);
	//detect(imgtest1, camera_flag, 1);
	//detect(imgtest1, camera_flag, 2);
	//detect(imgtest1, camera_flag, 3);
	//detect(imgtest1, camera_flag, 4);
	//detect(imgtest1, camera_flag, 5);
	//detect(imgtest1, camera_flag, 6);




	//detect(imgDust_tmp, camera_flag, 5);



	//camera_flag = 3;
	//detect(imgtest, camera_flag, 0);
	//detect(imgtest1, camera_flag, 1);

	//camera_flag = 4;
	//detect(imgtest, camera_flag, 0);
	//detect(imgtest1, camera_flag, 1);


	/**************************   灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123）   ***********************************/
	long t51 = GetTickCount();

	//Mat imgDust = imread("..\\E4.png");  //灰灰尘图路径
	Mat imgDust = imread("..\\0223\\dust3.png");  //灰灰尘图路径
										   //Mat imgDust = imread("..\\dust0109-2.png");  //灰灰尘图路径
	camera_flag = 1;
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	// 灰尘检测多相机场景修改--ysq0123
	processDust(imgDust, canshuX, 0);  //camera_flag


												  //Mat imgDust_tmp = imgDust.clone();  //单相机中没有这张图，故直接用imgdust复制，不影响						
												  //bool multi_camera = false;// true表示多相机场景，灰尘图有单独的白屏图imgDust_tmp来确定灰尘角点		
												  //if (multi_camera)
												  //{
												  //	imgDust_tmp = imread(".\\2camera-grid\\grid40_left.png");  // 多相机场景中灰层白屏图的路径   //多相机方案目前不被使用
												  //	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
												  //}
												  //processDust(imgDust, configDir, canshuX, imgDust_tmp,0, multi_camera);

												  //计算灰尘梯度，膨胀显示梯度
												  //中间得到：灰尘图ROI， 灰尘梯度图

	long t52 = GetTickCount();
	cout << "processDust函数运行的时间为：" << (t52 - t51) << endl;







	/**************************   分区域设定梯度系数（初始化,只针对point和mura）    ***********************************/
	long t1 = GetTickCount();

	////初始化用于区域调整的mask区域
	////输入梯度图和边界图，得到边界mask和内缩mask



	//!!!!!!!!不同的相机需要不同的缩进吗
	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_point[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);   //20 10 20 50 60
																												   //创建团的Mask模板

																												   //************  图片1和图片4的大小一样，图片2和图片3的大小一样
	initChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_muras[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);


	//0219 add sub的缩进mask操作
	initChangePartOfTheRegionalGradient((*sendImg1).subtractTemplateTftImgs[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, *SubCanShuMask);



	//repadding((*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).shrink_size_mura);
	//initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_louguang[0], (*sendImg1).BorderTFT,
	//	(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);
	//imwrite((*sendImg1).sNowTimeFileDir + "MuraMaskChangeGradietRCorner.png", (*sendImg1).MuraMaskChangeGradietRCorner);
	//imwrite((*sendImg1).sNowTimeFileDir + "MuraMaskChangeGradientDrawIn.png", (*sendImg1).MuraMaskChangeGradientDrawIn);
	//imwrite((*sendImg1).sNowTimeFileDir + "BorderTFT.png", (*sendImg1).BorderTFT);
	//imwrite((*sendImg1).sNowTimeFileDir + "BorderTftDilate.png", (*sendImg1).BorderTftDilate);
	//initChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);     //20 10 20 50 60																																				   //创建团的Mask模板
	//initChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);     //20 10 20 50 60
	//////改变调整区域的调整参数，目前默认为0.0
	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).gradientTFT_point, (*sendImg1).gradientTFT_point,0.0 , 0.0);
	//////针对mura设置参数
	//initChangePartOfTheRegionalGradient(3, 0, 0, 0, 0);     //20 10 20 50 60
	//ChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_mura, (*sendImg1).gradientTFT_mura,0.0 , 0.0);
	////针对漏光设置参数
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn,(*sendImg1).gradientTFT_louguang, (*sendImg1).gradientTFT_louguang,0.0 , 0.0);

	long t2 = GetTickCount();
	cout << "区域调整函数运行的时间为：" << (t2 - t1) << endl;

	////test 确保图片大小尺寸是相同的//
	//cout << "gradientTFT_pointChange.rows:" << (*sendImg1).gradientTFT_pointChange.rows << endl;
	//cout << "gradientTFT_pointChange.cols:" << (*sendImg1).gradientTFT_pointChange.cols << endl;




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

	//for (int imgFlag = 0; imgFlag < 7 ; imgFlag++)
	//{
	//	drawFinalResult((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).tftResultShow_all[imgFlag], (*sendImg1).contours_all[imgFlag], imgFlag, flag);
	//}


	long t42 = GetTickCount();
	cout << "drawFinalResult函数运行的时间为：" << (t42 - t41) << endl;


	cout << "整体函数运行的时间为：" << (t32 - t11) << endl;

	//重新获取参数
	//removeFalseDefects();


	// 用于九宫格和红绿点输出总的瑕疵统计结果
	long ttt1 = GetTickCount();
	Mat result_table = imread(".\\result_table\\LCM_res_table.png");
	//get_table_result((*sendImg1).points, (*sendImg1).back, result_table );
	get_table_result((*sendImg1).points, (*sendImg1).R_compare_res, (*sendImg1).back, result_table);
	long ttt2 = GetTickCount();
	cout << "result_table函数运行的时间为：" << (ttt2 - ttt1) << endl;


	//imwrite((*sendImg1).sNowTimeFileDir + "result_table.png", result_table);
	//namedWindow("result_final_show", 0);
	//imshow("result_final_show", result_table);


	//on_trackbar(0, sendImg1);
	//waitKey(0);
	destroyAllWindows();
	//saveImg();
}


void setSetTemplate_debug()
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
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);

}

//测试图片用的
void debug1()
{

	setSetTemplate_debug();
	int camera_flag = 0;
	Mat imgtest = imread("..\\0223\\wtest2.png");
	camera_cali_fisrt(imgtest, neican_data, jibian_data, imgtest);

	//输入其他颜色屏幕图片
	long t21 = GetTickCount();


	Mat imgtest1 = imread("..\\0223\\g1.png");  //其他颜色屏幕
	long timebegin = GetTickCount();
	camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);



	camera_flag = 0;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);



	//读入灰尘屏幕图片
	Mat imgDust_tmp = imread("..\\0223\\dusttest1.png");  //1相机的0颜色即为灰尘模板图
												  //imgDust_tmp = imread("..\\dust0109-2.png");  //1相机的0颜色即为灰尘模板图
	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123



	/**************************   灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123）   ***********************************/
	long t51 = GetTickCount();

	//Mat imgDust = imread("..\\E4.png");  //灰灰尘图路径
	Mat imgDust = imread("..\\0223\\dusttest1.png");  //灰灰尘图路径
												  //Mat imgDust = imread("..\\dust0109-2.png");  //灰灰尘图路径
	camera_flag = 1;
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	// 灰尘检测多相机场景修改--ysq0123
	processDust(imgDust, canshuX, 0);  //camera_flag





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

//0228 add 灰尘参数调节滑杆
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
	m[0] = PointCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "method");
	m[1] = muraCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "method");
	m[2] = LightCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "method");
	m[3] = SubCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "method");
	m[4] = SubBGRCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "method");

	int s[5];
	s[0] = PointCanShureMoveFalsex._dst_s_gradient[0][0] = getTrackbarPos("Smean", "method");
	s[1] = muraCanShureMoveFalsex._dst_s_gradient[0][0] = getTrackbarPos("Smean", "method");
	s[2] = LightCanShureMoveFalsex._dst_s_gradient[0][0] = getTrackbarPos("Smean", "method");
	s[3] = SubCanShureMoveFalsex._dst_s_gradient[0][0] = getTrackbarPos("Smean", "method");
	s[4] = SubBGRCanShureMoveFalsex._dst_s_gradient[0][0] = getTrackbarPos("Smean", "method");

	int num[5];
	num[0] = PointCanShureMoveFalsex._num_gradient[0][0] = getTrackbarPos("numPoint", "method");
	num[1] = muraCanShureMoveFalsex._num_gradient[0][0] = getTrackbarPos("numPoint", "method");
	num[2] = LightCanShureMoveFalsex._num_gradient[0][0] = getTrackbarPos("numPoint", "method");
	num[3] = SubCanShureMoveFalsex._num_gradient[0][0] = getTrackbarPos("numPoint", "method");
	num[4] = SubBGRCanShureMoveFalsex._num_gradient[0][0] = getTrackbarPos("numPoint", "method");

	int mValueGradient[5];
	mValueGradient[0] = PointCanShureMoveFalsex._meanValueGradient[0][0] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[1] = muraCanShureMoveFalsex._meanValueGradient[0][0] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[2] = LightCanShureMoveFalsex._meanValueGradient[0][0] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[3] = SubCanShureMoveFalsex._meanValueGradient[0][0] = getTrackbarPos("mValueGradient", "method");
	mValueGradient[4] = SubBGRCanShureMoveFalsex._meanValueGradient[0][0] = getTrackbarPos("mValueGradient", "method");








	TFTthreshold_point = (*canshuX)._TFTHOLD_points[0][0] = getTrackbarPos("方法0", "method");
	TFTthreshold_mura = (*canshuX)._TFTHOLD_muras[0][0] = getTrackbarPos("方法1", "method");
	hsvThresh = (*canshuX)._hsvThresh[0][0] = getTrackbarPos("方法2", "method");
	subThresh = (*canshuX)._subThresh[0][0] = getTrackbarPos("方法3", "method");
	subBGRThresh = (*canshuX)._subBGRThresh[0][0] = getTrackbarPos("方法4", "method");


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
	imwrite((*sendImg1).sNowTimeFileDir + " debug_img1" + ".jpg", (*sendImg1).gradientResultThreshold_bgr_sub[0][0]);

	drawSiglePic_first(camera_flag, imgFlag, method);


}

void onSecondParameter(int, void * names)
{
	int camera_flag = (*sendImg1).debug_camera_flag;
	int imgFlag = (*sendImg1).debug_imgFlag;
	int method = (*sendImg1).debug_method;
	(*sendImg1).imgTftColorCuts[camera_flag][method] = (*sendImg1).debug_cutImg.clone();
	//"mean", "method",

	SecondCanshu._point_m_gray = PointCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("mean", "secondParameter");
	SecondCanshu._point_num = PointCanShureMoveFalsex._dst_m_gradient[0][0] = getTrackbarPos("numPoint", "secondParameter");

	//对三合一图灰度筛选
	CombAndRemoveGrayFalse(camera_flag, imgFlag, 0, defectsInfor[camera_flag][imgFlag]);

	//分类筛选
	determine1();

	getGrounpPointResult();

	drawSiglePic(camera_flag, imgFlag);

	//画图,show 
	namedWindow("showmethod", 0);
	imshow("showmethod", (*sendImg1).imgTftColorCuts[camera_flag][imgFlag]);

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


int main()
{
	setSetTemplate_debug();
	//debug1();
	Mat imgtmp = imread("..\\0223\\g1.png");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);
	(*sendImg1).imgTFT[0] = imgtmp;
	(*sendImg1).debug_img = imgtmp.clone();
	Mat imgDust = imread("..\\0223\\dusttest1.png");
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	string configDir;

	processDust(imgDust, canshuX, 0);  //camera_flag
	//setmethodTrackbar(imgtmp, 0, 1, 0);  //其他颜色屏幕
	setmethodTrackbar(imgtmp, 0, 1);

	//setsecondParameterTrackbar(imgtmp, 0, 0, 0);
	waitKey(0);


	return 0;



}

//12.21 wzx 创建模板

/*************************************************   labview   检测输出代码      ****************************************************************/

Mat BuildImgFromLabview(int rows, int cols, void *data)
{
	Mat image_src(rows, cols, CV_8UC4, data);
	cvtColor(image_src, image_src, CV_RGBA2RGB);
	//imshow("test1", image_src);
	return image_src;
}

//读写图片存储路径
void writefile(const char *filename, int(&arr)[10][20])
{
	//open
	FILE *fpw;
	errno_t err;
	if ((err = fopen_s(&fpw, filename, "wb+")) != 0)
	{
		cout << "Attention, 已经发现待读取目标文件" << endl;
	}

	//imwrite("D://project//112.png",(*sendImg1).GradientTemplate_fill[0][0]);

	fwrite(&arr, sizeof(int), 200, fpw);

	//close
	fclose(fpw);
}

void readfile(const char *filename, int(&B)[10][20])
{
	//open
	FILE *fpr;
	errno_t errt;
	if ((errt = fopen_s(&fpr, filename, "rb")) != 0)
	{
		cout << "找到需要的文件" << endl;
	}
	if (fpr == NULL)
	{
		cout << "Open error!" << endl;
		fclose(fpr);
		return;
	}


	//char* pData = (char*)image2.data;
	fread((int*)B, sizeof(int), 200, fpr);    //正确
	fclose(fpr);

	cout << B;

	//close
	fclose(fpr);
}

extern "C" __declspec(dllexport) int setDir(char *address)
{
	//char buffer[MAX_PATH];
	//_getcwd(buffer, MAX_PATH);

	if (address == NULL)
	{
		return 1;
	}
	else
	{
		_mkdir(address);
		string s(address);
		(*sendImg1).Initpath = s;
		//log日志文件
		(*sendImg1).logtxt = (*sendImg1).Initpath + "//log.txt";


		//R角文件夹
		(*sendImg1).res_tmp_dir = s + "//R_tmmp//";
		char *data1;
		int len1 = (*sendImg1).res_tmp_dir.length();
		data1 = (char *)malloc((len1 + 1) * sizeof(char));
		(*sendImg1).res_tmp_dir.copy(data1, len1, 0);
		_mkdir(data1);
		//模板图片文件夹
		string tmpImg_path = s + "//tmpImg//";
		(*sendImg1).tmpImg_path = tmpImg_path;
		char *data2;
		int len2 = tmpImg_path.length();
		data2 = (char *)malloc((len2 + 1) * sizeof(char));
		tmpImg_path.copy(data2, len2, 0);
		_mkdir(data2);

		(*sendImg1).pts1_tmp_file = s + "//R_tmmp//pts1_tmp.txt";
		(*sendImg1).h_w_tmp_file = s + "//R_tmmp//h_w_tmp.txt";

		(*sendImg1).R_difftxt = s + "//R_dif//R_diff_area.txt";
		//二进制文件
		string tmptxt = s + "//tmpImg//" + "//patch.bin";
		(*sendImg1).tmptxt = tmptxt;

		ofstream location_out;
		location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间
		char cNowTime[64];
		strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
		location_out << cNowTime << endl;
		string ss = "int setDir ";
		string ss1 = "0 初始化路径成功！！";
		location_out << ss << endl;
		location_out << ss1 << "路径：" << s << endl << endl;
		location_out << "****************SUCCESS******************" << endl;
		location_out.close();

		return 0;
	}
}

extern "C" __declspec(dllexport) int setCanshu_single(const float* canshu1, const float* canshu2, const float* canshu3, const float* canshu4, const float* canshu5,
	const float* canshu6, const float* canshu7, const float* canshu8, const float* canshu9, const float* canshu10, const float* canshu11, const float* canshu12, int flag, int saveflag)
{

	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	location_out << cNowTime << endl;


	(*sendImg1).if_show_result = flag;
	(*sendImg1).if_save_midlle_result = saveflag;

	//判断传入参数是否为空
	if (canshu1 == NULL || canshu2 == NULL || canshu3 == NULL || canshu3 == NULL || canshu4 == NULL || canshu5 == NULL || canshu6 == NULL || canshu7 == NULL || canshu8 == NULL || canshu9 == NULL || canshu10 == NULL || canshu11 == NULL || canshu12 == NULL)
	{
		string ss1 = "有参数未传入";
		location_out << ss1 << endl;
		return 1;
	}
	else
	{
		//传入参数

		//第一个颜色
		PointCanShureMoveFalsex._num_gradient[0][0] = canshu1[0];
		PointCanShureMoveFalsex._num_grayScale[0][0] = canshu1[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][0] = canshu1[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu1[3];
		PointCanShureMoveFalsex._meanValueGradient[0][0] = canshu1[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][0] = canshu1[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu1[6];
		PointCanShureMoveFalsex._bright_num[0][0] = canshu1[7];
		PointCanShureMoveFalsex._dark_num[0][0] = canshu1[8];

		muraCanShureMoveFalsex._num_gradient[0][0] = canshu2[0];
		muraCanShureMoveFalsex._num_grayScale[0][0] = canshu2[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][0] = canshu2[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu2[3];
		muraCanShureMoveFalsex._meanValueGradient[0][0] = canshu2[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][0] = canshu2[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu2[6];
		muraCanShureMoveFalsex._bright_num[0][0] = canshu2[7];
		muraCanShureMoveFalsex._dark_num[0][0] = canshu2[8];

		LightCanShureMoveFalsex._num_gradient[0][0] = canshu3[0];
		LightCanShureMoveFalsex._num_grayScale[0][0] = canshu3[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][0] = canshu3[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu3[3];
		LightCanShureMoveFalsex._meanValueGradient[0][0] = canshu3[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][0] = canshu3[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu3[6];
		LightCanShureMoveFalsex._bright_num[0][0] = canshu3[7];
		LightCanShureMoveFalsex._dark_num[0][0] = canshu3[8];

		SubCanShureMoveFalsex._num_gradient[0][0] = canshu4[0];
		SubCanShureMoveFalsex._num_grayScale[0][0] = canshu4[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][0] = canshu4[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu4[3];
		SubCanShureMoveFalsex._meanValueGradient[0][0] = canshu4[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][0] = canshu4[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu4[6];
		SubCanShureMoveFalsex._bright_num[0][0] = canshu4[7];
		SubCanShureMoveFalsex._dark_num[0][0] = canshu4[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][0] = canshu5[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][0] = canshu5[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][0] = canshu5[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu5[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][0] = canshu5[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][0] = canshu5[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu5[6];
		SubBGRCanShureMoveFalsex._bright_num[0][0] = canshu4[7];
		SubBGRCanShureMoveFalsex._dark_num[0][0] = canshu4[8];

		PointCanShuMaskx._keneralsize[0][0] = canshu6[0];
		PointCanShuMaskx._size_x_left[0][0] = canshu6[1];
		PointCanShuMaskx._size_x_right[0][0] = canshu6[2];
		PointCanShuMaskx._size_y_up[0][0] = canshu6[3];
		PointCanShuMaskx._size_y_down[0][0] = canshu6[4];

		MuraCanShuMaskx._keneralsize[0][0] = canshu7[0];
		MuraCanShuMaskx._size_x_left[0][0] = canshu7[1];
		MuraCanShuMaskx._size_x_right[0][0] = canshu7[2];
		MuraCanShuMaskx._size_y_up[0][0] = canshu7[3];
		MuraCanShuMaskx._size_y_down[0][0] = canshu7[4];

		lightCanShuMaskx._keneralsize[0][0] = canshu8[0];
		lightCanShuMaskx._size_x_left[0][0] = canshu8[1];
		lightCanShuMaskx._size_x_right[0][0] = canshu8[2];
		lightCanShuMaskx._size_y_up[0][0] = canshu8[3];
		lightCanShuMaskx._size_y_down[0][0] = canshu8[4];

		SubCanShuMaskx._keneralsize[0][0] = canshu9[0];
		SubCanShuMaskx._size_x_left[0][0] = canshu9[1];
		SubCanShuMaskx._size_x_right[0][0] = canshu9[2];
		SubCanShuMaskx._size_y_up[0][0] = canshu9[3];
		SubCanShuMaskx._size_y_down[0][0] = canshu9[4];

		SubBGRCanShuMaskx._keneralsize[0][0] = canshu10[0];
		SubBGRCanShuMaskx._size_x_left[0][0] = canshu10[1];
		SubBGRCanShuMaskx._size_x_right[0][0] = canshu10[2];
		SubBGRCanShuMaskx._size_y_up[0][0] = canshu10[3];
		SubBGRCanShuMaskx._size_y_down[0][0] = canshu10[4];

		canshuExtern._sort_dxy1[0][0] = canshu11[0];
		canshuExtern._sort_dxy2[0][0] = canshu11[1];
		canshuExtern._sort_dxy3[0][0] = canshu11[2];
		canshuExtern._sort_dxy4[0][0] = canshu11[3];
		canshuExtern._houghthr[0][0] = canshu11[4];
		canshuExtern._DUSTHOLD[0][0] = canshu11[5];
		canshuExtern._DUSTSHAPE[0][0] = canshu11[6];
		canshuExtern._fill[0][0] = canshu11[7];
		canshuExtern._TFTHOLD_points[0][0] = canshu11[8];
		canshuExtern._TFTHOLD_muras[0][0] = canshu11[9];
		canshuExtern._subThresh[0][0] = canshu11[10];
		canshuExtern._bgrThresh[0][0] = canshu11[11];
		canshuExtern._subBGRThresh[0][0] = canshu11[12];
		canshuExtern._hsvThresh[0][0] = canshu11[13];

		//第二个颜色


		SecondCanshu._point_num = canshu12[0];
		SecondCanshu._line_num = canshu12[1];
		SecondCanshu._mura_num = canshu12[2];
		SecondCanshu._tuan_num = canshu12[3];
		SecondCanshu._point_m_gray = canshu12[4];
		SecondCanshu._line_m_gray = canshu12[5];
		SecondCanshu._mura_m_gray = canshu12[6];
		SecondCanshu._tuan_m_gray = canshu12[7];
		SecondCanshu._maxDistance = canshu12[8];
		SecondCanshu._maxGParea = canshu12[9];
		SecondCanshu._point_bk_num = canshu12[10];
		SecondCanshu._point_bk_m_gray = canshu12[11];

		if (saveflag == 1)
		{
			string sProjectDir = (*sendImg1).Initpath + "//midlle_result//";
			(*sendImg1).projectDir = sProjectDir;
			(*sendImg1).saveFileDir = mkdirUseTime(sProjectDir);  //使用时间创建一个文件夹
			(*sendImg1).fir_result_file = (*sendImg1).saveFileDir + "fir_result_file.txt";
		}

		string ss = "int setCanshu ";
		string ss1 = "0 初始化参数成功！！";
		location_out << ss << endl;
		location_out << ss1 << endl;
		location_out << "****************SUCCESS******************" << endl;
		location_out.close();

		return 0;
	}
}


extern "C" __declspec(dllexport) int setCanshu(const float* canshu1, const float* canshu2, const float* canshu3, const float* canshu4, const float* canshu5,const float* canshu6, const float* canshu7, const float* canshu8, const float* canshu9, const float* canshu10, const float* canshu11, 
	const float* canshu12, const float* canshu13, const float* canshu14, const float* canshu15, const float* canshu16, const float* canshu17, const float* canshu18, const float* canshu19, const float* canshu20, const float* canshu21, const float* canshu22, 
	const float* canshu23, const float* canshu24, const float* canshu25, const float* canshu26, const float* canshu27, const float* canshu28, const float* canshu29, const float* canshu30, const float* canshu31, const float* canshu32, const float* canshu33, 
	const float* canshu34, const float* canshu35, const float* canshu36, const float* canshu37, const float* canshu38, const float* canshu39, const float* canshu40, const float* canshu41, const float* canshu42, const float* canshu43, const float* canshu44, 
	const float* canshu45, const float* canshu46, const float* canshu47, const float* canshu48, const float* canshu49, const float* canshu50, const float* canshu51, const float* canshu52, const float* canshu53, const float* canshu54, const float* canshu55, 
	const float* canshu56, const float* canshu57, const float* canshu58, const float* canshu59, const float* canshu60, const float* canshu61, const float* canshu62, const float* canshu63, const float* canshu64, const float* canshu65, const float* canshu66, 
	const float* canshu67, const float* canshu68, const float* canshu69, const float* canshu70, const float* canshu71, const float* canshu72, const float* canshu73, const float* canshu74, const float* canshu75, const float* canshu76, const float* canshu77, 
	const float* canshu78, int flag, int saveflag)
{

	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	location_out << cNowTime << endl;


	(*sendImg1).if_show_result = flag;
	(*sendImg1).if_save_midlle_result = saveflag;

	//判断传入参数是否为空
	if (canshu1 == NULL || canshu2 == NULL || canshu3 == NULL || canshu3 == NULL || canshu4 == NULL || canshu5 == NULL || canshu6 == NULL || canshu7 == NULL || canshu8 == NULL || canshu9 == NULL || canshu10 == NULL || canshu11 == NULL || canshu12 == NULL)
	{
		string ss1 = "有参数未传入";
		location_out << ss1 << endl;
		return 1;
	}
	else
	{
		//传入参数

		//第一个颜色
		PointCanShureMoveFalsex._num_gradient[0][0] = canshu1[0];
		PointCanShureMoveFalsex._num_grayScale[0][0] = canshu1[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][0] = canshu1[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu1[3];
		PointCanShureMoveFalsex._meanValueGradient[0][0] = canshu1[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][0] = canshu1[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu1[6];
		PointCanShureMoveFalsex._bright_num[0][0] = canshu1[7];
		PointCanShureMoveFalsex._dark_num[0][0] = canshu1[8];

		muraCanShureMoveFalsex._num_gradient[0][0] = canshu2[0];
		muraCanShureMoveFalsex._num_grayScale[0][0] = canshu2[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][0] = canshu2[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu2[3];
		muraCanShureMoveFalsex._meanValueGradient[0][0] = canshu2[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][0] = canshu2[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu2[6];
		muraCanShureMoveFalsex._bright_num[0][0] = canshu2[7];
		muraCanShureMoveFalsex._dark_num[0][0] = canshu2[8];

		LightCanShureMoveFalsex._num_gradient[0][0] = canshu3[0];
		LightCanShureMoveFalsex._num_grayScale[0][0] = canshu3[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][0] = canshu3[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu3[3];
		LightCanShureMoveFalsex._meanValueGradient[0][0] = canshu3[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][0] = canshu3[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu3[6];
		LightCanShureMoveFalsex._bright_num[0][0] = canshu3[7];
		LightCanShureMoveFalsex._dark_num[0][0] = canshu3[8];

		SubCanShureMoveFalsex._num_gradient[0][0] = canshu4[0];
		SubCanShureMoveFalsex._num_grayScale[0][0] = canshu4[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][0] = canshu4[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu4[3];
		SubCanShureMoveFalsex._meanValueGradient[0][0] = canshu4[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][0] = canshu4[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu4[6];
		SubCanShureMoveFalsex._bright_num[0][0] = canshu4[7];
		SubCanShureMoveFalsex._dark_num[0][0] = canshu4[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][0] = canshu5[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][0] = canshu5[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][0] = canshu5[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][0] = canshu5[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][0] = canshu5[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][0] = canshu5[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][0] = canshu5[6];
		SubBGRCanShureMoveFalsex._bright_num[0][0] = canshu4[7];
		SubBGRCanShureMoveFalsex._dark_num[0][0] = canshu4[8];

		PointCanShuMaskx._keneralsize[0][0] = canshu6[0];
		PointCanShuMaskx._size_x_left[0][0] = canshu6[1];
		PointCanShuMaskx._size_x_right[0][0] = canshu6[2];
		PointCanShuMaskx._size_y_up[0][0] = canshu6[3];
		PointCanShuMaskx._size_y_down[0][0] = canshu6[4];

		MuraCanShuMaskx._keneralsize[0][0] = canshu7[0];
		MuraCanShuMaskx._size_x_left[0][0] = canshu7[1];
		MuraCanShuMaskx._size_x_right[0][0] = canshu7[2];
		MuraCanShuMaskx._size_y_up[0][0] = canshu7[3];
		MuraCanShuMaskx._size_y_down[0][0] = canshu7[4];

		lightCanShuMaskx._keneralsize[0][0] = canshu8[0];
		lightCanShuMaskx._size_x_left[0][0] = canshu8[1];
		lightCanShuMaskx._size_x_right[0][0] = canshu8[2];
		lightCanShuMaskx._size_y_up[0][0] = canshu8[3];
		lightCanShuMaskx._size_y_down[0][0] = canshu8[4];

		SubCanShuMaskx._keneralsize[0][0] = canshu9[0];
		SubCanShuMaskx._size_x_left[0][0] = canshu9[1];
		SubCanShuMaskx._size_x_right[0][0] = canshu9[2];
		SubCanShuMaskx._size_y_up[0][0] = canshu9[3];
		SubCanShuMaskx._size_y_down[0][0] = canshu9[4];

		SubBGRCanShuMaskx._keneralsize[0][0] = canshu10[0];
		SubBGRCanShuMaskx._size_x_left[0][0] = canshu10[1];
		SubBGRCanShuMaskx._size_x_right[0][0] = canshu10[2];
		SubBGRCanShuMaskx._size_y_up[0][0] = canshu10[3];
		SubBGRCanShuMaskx._size_y_down[0][0] = canshu10[4];

		canshuExtern._sort_dxy1[0][0] = canshu11[0];
		canshuExtern._sort_dxy2[0][0] = canshu11[1];
		canshuExtern._sort_dxy3[0][0] = canshu11[2];
		canshuExtern._sort_dxy4[0][0] = canshu11[3];
		canshuExtern._houghthr[0][0] = canshu11[4];
		canshuExtern._DUSTHOLD[0][0] = canshu11[5];
		canshuExtern._DUSTSHAPE[0][0] = canshu11[6];
		canshuExtern._fill[0][0] = canshu11[7];
		canshuExtern._TFTHOLD_points[0][0] = canshu11[8];
		canshuExtern._TFTHOLD_muras[0][0] = canshu11[9];
		canshuExtern._subThresh[0][0] = canshu11[10];
		canshuExtern._bgrThresh[0][0] = canshu11[11];
		canshuExtern._subBGRThresh[0][0] = canshu11[12];
		canshuExtern._hsvThresh[0][0] = canshu11[13];

		//第二个颜色

		PointCanShureMoveFalsex._num_gradient[0][1] = canshu12[0];
		PointCanShureMoveFalsex._num_grayScale[0][1] = canshu12[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][1] = canshu12[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][1] = canshu12[3];
		PointCanShureMoveFalsex._meanValueGradient[0][1] = canshu12[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][1] = canshu12[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][1] = canshu12[6];
		PointCanShureMoveFalsex._bright_num[0][1] = canshu12[7];
		PointCanShureMoveFalsex._dark_num[0][1] = canshu12[8];

		muraCanShureMoveFalsex._num_gradient[0][1] = canshu13[0];
		muraCanShureMoveFalsex._num_grayScale[0][1] = canshu13[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][1] = canshu13[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][1] = canshu13[3];
		muraCanShureMoveFalsex._meanValueGradient[0][1] = canshu13[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][1] = canshu13[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][1] = canshu13[6];
		muraCanShureMoveFalsex._bright_num[0][1] = canshu13[7];
		muraCanShureMoveFalsex._dark_num[0][1] = canshu13[8];

		LightCanShureMoveFalsex._num_gradient[0][1] = canshu14[0];
		LightCanShureMoveFalsex._num_grayScale[0][1] = canshu14[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][1] = canshu14[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][1] = canshu14[3];
		LightCanShureMoveFalsex._meanValueGradient[0][1] = canshu14[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][1] = canshu14[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][1] = canshu14[6];
		LightCanShureMoveFalsex._bright_num[0][1] = canshu14[7];
		LightCanShureMoveFalsex._dark_num[0][1] = canshu14[8];

		SubCanShureMoveFalsex._num_gradient[0][1] = canshu15[0];
		SubCanShureMoveFalsex._num_grayScale[0][1] = canshu15[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][1] = canshu15[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][1] = canshu15[3];
		SubCanShureMoveFalsex._meanValueGradient[0][1] = canshu15[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][1] = canshu15[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][1] = canshu15[6];
		SubCanShureMoveFalsex._bright_num[0][1] = canshu15[7];
		SubCanShureMoveFalsex._dark_num[0][1] = canshu15[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][1] = canshu16[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][1] = canshu16[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][1] = canshu16[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][1] = canshu16[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][1] = canshu16[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][1] = canshu16[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][1] = canshu16[6];
		SubBGRCanShureMoveFalsex._bright_num[0][1] = canshu16[7];
		SubBGRCanShureMoveFalsex._dark_num[0][1] = canshu16[8];

		PointCanShuMaskx._keneralsize[0][1] = canshu17[0];
		PointCanShuMaskx._size_x_left[0][1] = canshu17[1];
		PointCanShuMaskx._size_x_right[0][1] = canshu17[2];
		PointCanShuMaskx._size_y_up[0][1] = canshu17[3];
		PointCanShuMaskx._size_y_down[0][1] = canshu17[4];

		MuraCanShuMaskx._keneralsize[0][1] = canshu18[0];
		MuraCanShuMaskx._size_x_left[0][1] = canshu18[1];
		MuraCanShuMaskx._size_x_right[0][1] = canshu18[2];
		MuraCanShuMaskx._size_y_up[0][1] = canshu18[3];
		MuraCanShuMaskx._size_y_down[0][1] = canshu18[4];

		lightCanShuMaskx._keneralsize[0][1] = canshu19[0];
		lightCanShuMaskx._size_x_left[0][1] = canshu19[1];
		lightCanShuMaskx._size_x_right[0][1] = canshu19[2];
		lightCanShuMaskx._size_y_up[0][1] = canshu19[3];
		lightCanShuMaskx._size_y_down[0][1] = canshu19[4];

		SubCanShuMaskx._keneralsize[0][1] = canshu20[0];
		SubCanShuMaskx._size_x_left[0][1] = canshu20[1];
		SubCanShuMaskx._size_x_right[0][1] = canshu20[2];
		SubCanShuMaskx._size_y_up[0][1] = canshu20[3];
		SubCanShuMaskx._size_y_down[0][1] = canshu20[4];

		SubBGRCanShuMaskx._keneralsize[0][1] = canshu21[0];
		SubBGRCanShuMaskx._size_x_left[0][1] = canshu21[1];
		SubBGRCanShuMaskx._size_x_right[0][1] = canshu21[2];
		SubBGRCanShuMaskx._size_y_up[0][1] = canshu21[3];
		SubBGRCanShuMaskx._size_y_down[0][1] = canshu21[4];

		canshuExtern._sort_dxy1[0][1] = canshu22[0];
		canshuExtern._sort_dxy2[0][1] = canshu22[1];
		canshuExtern._sort_dxy3[0][1] = canshu22[2];
		canshuExtern._sort_dxy4[0][1] = canshu22[3];
		canshuExtern._houghthr[0][1] = canshu22[4];
		canshuExtern._DUSTHOLD[0][1] = canshu22[5];
		canshuExtern._DUSTSHAPE[0][1] = canshu22[6];
		canshuExtern._fill[0][1] = canshu22[7];
		canshuExtern._TFTHOLD_points[0][1] = canshu22[8];
		canshuExtern._TFTHOLD_muras[0][1] = canshu22[9];
		canshuExtern._subThresh[0][1] = canshu22[10];
		canshuExtern._bgrThresh[0][1] = canshu22[11];
		canshuExtern._subBGRThresh[0][1] = canshu22[12];
		canshuExtern._hsvThresh[0][1] = canshu22[13];


		//第三个颜色

		PointCanShureMoveFalsex._num_gradient[0][2] = canshu23[0];
		PointCanShureMoveFalsex._num_grayScale[0][2] = canshu23[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][2] = canshu23[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][2] = canshu23[3];
		PointCanShureMoveFalsex._meanValueGradient[0][2] = canshu23[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][2] = canshu23[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][2] = canshu23[6];
		PointCanShureMoveFalsex._bright_num[0][2] = canshu23[7];
		PointCanShureMoveFalsex._dark_num[0][2] = canshu23[8];

		muraCanShureMoveFalsex._num_gradient[0][2] = canshu24[0];
		muraCanShureMoveFalsex._num_grayScale[0][2] = canshu24[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][2] = canshu24[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][2] = canshu24[3];
		muraCanShureMoveFalsex._meanValueGradient[0][2] = canshu24[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][2] = canshu24[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][2] = canshu24[6];
		muraCanShureMoveFalsex._bright_num[0][2] = canshu24[7];
		muraCanShureMoveFalsex._dark_num[0][2] = canshu24[8];

		LightCanShureMoveFalsex._num_gradient[0][2] = canshu25[0];
		LightCanShureMoveFalsex._num_grayScale[0][2] = canshu25[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][2] = canshu25[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][2] = canshu25[3];
		LightCanShureMoveFalsex._meanValueGradient[0][2] = canshu25[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][2] = canshu25[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][2] = canshu25[6];
		LightCanShureMoveFalsex._bright_num[0][2] = canshu25[7];
		LightCanShureMoveFalsex._dark_num[0][2] = canshu25[8];

		SubCanShureMoveFalsex._num_gradient[0][2] = canshu26[0];
		SubCanShureMoveFalsex._num_grayScale[0][2] = canshu26[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][2] = canshu26[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][2] = canshu26[3];
		SubCanShureMoveFalsex._meanValueGradient[0][2] = canshu26[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][2] = canshu26[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][2] = canshu26[6];
		SubCanShureMoveFalsex._bright_num[0][2] = canshu26[7];
		SubCanShureMoveFalsex._dark_num[0][2] = canshu26[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][2] = canshu27[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][2] = canshu27[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][2] = canshu27[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][2] = canshu27[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][2] = canshu27[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][2] = canshu27[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][2] = canshu27[6];
		SubBGRCanShureMoveFalsex._bright_num[0][2] = canshu27[7];
		SubBGRCanShureMoveFalsex._dark_num[0][2] = canshu27[8];

		PointCanShuMaskx._keneralsize[0][2] = canshu28[0];
		PointCanShuMaskx._size_x_left[0][2] = canshu28[1];
		PointCanShuMaskx._size_x_right[0][2] = canshu28[2];
		PointCanShuMaskx._size_y_up[0][2] = canshu28[3];
		PointCanShuMaskx._size_y_down[0][2] = canshu28[4];

		MuraCanShuMaskx._keneralsize[0][2] = canshu29[0];
		MuraCanShuMaskx._size_x_left[0][2] = canshu29[1];
		MuraCanShuMaskx._size_x_right[0][2] = canshu29[2];
		MuraCanShuMaskx._size_y_up[0][2] = canshu29[3];
		MuraCanShuMaskx._size_y_down[0][2] = canshu29[4];

		lightCanShuMaskx._keneralsize[0][2] = canshu30[0];
		lightCanShuMaskx._size_x_left[0][2] = canshu30[1];
		lightCanShuMaskx._size_x_right[0][2] = canshu30[2];
		lightCanShuMaskx._size_y_up[0][2] = canshu30[3];
		lightCanShuMaskx._size_y_down[0][2] = canshu30[4];

		SubCanShuMaskx._keneralsize[0][2] = canshu31[0];
		SubCanShuMaskx._size_x_left[0][2] = canshu31[1];
		SubCanShuMaskx._size_x_right[0][2] = canshu31[2];
		SubCanShuMaskx._size_y_up[0][2] = canshu31[3];
		SubCanShuMaskx._size_y_down[0][2] = canshu31[4];

		SubBGRCanShuMaskx._keneralsize[0][2] = canshu32[0];
		SubBGRCanShuMaskx._size_x_left[0][2] = canshu32[1];
		SubBGRCanShuMaskx._size_x_right[0][2] = canshu32[2];
		SubBGRCanShuMaskx._size_y_up[0][2] = canshu32[3];
		SubBGRCanShuMaskx._size_y_down[0][2] = canshu32[4];

		canshuExtern._sort_dxy1[0][2] = canshu33[0];
		canshuExtern._sort_dxy2[0][2] = canshu33[1];
		canshuExtern._sort_dxy3[0][2] = canshu33[2];
		canshuExtern._sort_dxy4[0][2] = canshu33[3];
		canshuExtern._houghthr[0][2] = canshu33[4];
		canshuExtern._DUSTHOLD[0][2] = canshu33[5];
		canshuExtern._DUSTSHAPE[0][2] = canshu33[6];
		canshuExtern._fill[0][2] = canshu33[7];
		canshuExtern._TFTHOLD_points[0][2] = canshu33[8];
		canshuExtern._TFTHOLD_muras[0][2] = canshu33[9];
		canshuExtern._subThresh[0][2] = canshu33[10];
		canshuExtern._bgrThresh[0][2] = canshu33[11];
		canshuExtern._subBGRThresh[0][2] = canshu33[12];
		canshuExtern._hsvThresh[0][2] = canshu33[13];


		//第四个颜色

		PointCanShureMoveFalsex._num_gradient[0][3] = canshu34[0];
		PointCanShureMoveFalsex._num_grayScale[0][3] = canshu34[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][3] = canshu34[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][3] = canshu34[3];
		PointCanShureMoveFalsex._meanValueGradient[0][3] = canshu34[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][3] = canshu34[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][3] = canshu34[6];
		PointCanShureMoveFalsex._bright_num[0][3] = canshu34[7];
		PointCanShureMoveFalsex._dark_num[0][3] = canshu34[8];

		muraCanShureMoveFalsex._num_gradient[0][3] = canshu35[0];
		muraCanShureMoveFalsex._num_grayScale[0][3] = canshu35[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][3] = canshu35[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][3] = canshu35[3];
		muraCanShureMoveFalsex._meanValueGradient[0][3] = canshu35[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][3] = canshu35[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][3] = canshu35[6];
		muraCanShureMoveFalsex._bright_num[0][3] = canshu35[7];
		muraCanShureMoveFalsex._dark_num[0][3] = canshu35[8];

		LightCanShureMoveFalsex._num_gradient[0][3] = canshu36[0];
		LightCanShureMoveFalsex._num_grayScale[0][3] = canshu36[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][3] = canshu36[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][3] = canshu36[3];
		LightCanShureMoveFalsex._meanValueGradient[0][3] = canshu36[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][3] = canshu36[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][3] = canshu36[6];
		LightCanShureMoveFalsex._bright_num[0][3] = canshu36[7];
		LightCanShureMoveFalsex._dark_num[0][3] = canshu36[8];

		SubCanShureMoveFalsex._num_gradient[0][3] = canshu37[0];
		SubCanShureMoveFalsex._num_grayScale[0][3] = canshu37[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][3] = canshu37[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][3] = canshu37[3];
		SubCanShureMoveFalsex._meanValueGradient[0][3] = canshu37[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][3] = canshu37[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][3] = canshu37[6];
		SubCanShureMoveFalsex._bright_num[0][3] = canshu37[7];
		SubCanShureMoveFalsex._dark_num[0][3] = canshu37[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][3] = canshu38[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][3] = canshu38[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][3] = canshu38[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][3] = canshu38[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][3] = canshu38[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][3] = canshu38[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][3] = canshu38[6];
		SubBGRCanShureMoveFalsex._bright_num[0][3] = canshu38[7];
		SubBGRCanShureMoveFalsex._dark_num[0][3] = canshu38[8];

		PointCanShuMaskx._keneralsize[0][3] = canshu39[0];
		PointCanShuMaskx._size_x_left[0][3] = canshu39[1];
		PointCanShuMaskx._size_x_right[0][3] = canshu39[2];
		PointCanShuMaskx._size_y_up[0][3] = canshu39[3];
		PointCanShuMaskx._size_y_down[0][3] = canshu39[4];

		MuraCanShuMaskx._keneralsize[0][3] = canshu40[0];
		MuraCanShuMaskx._size_x_left[0][3] = canshu40[1];
		MuraCanShuMaskx._size_x_right[0][3] = canshu40[2];
		MuraCanShuMaskx._size_y_up[0][3] = canshu40[3];
		MuraCanShuMaskx._size_y_down[0][3] = canshu40[4];

		lightCanShuMaskx._keneralsize[0][3] = canshu41[0];
		lightCanShuMaskx._size_x_left[0][3] = canshu41[1];
		lightCanShuMaskx._size_x_right[0][3] = canshu41[2];
		lightCanShuMaskx._size_y_up[0][3] = canshu41[3];
		lightCanShuMaskx._size_y_down[0][3] = canshu41[4];

		SubCanShuMaskx._keneralsize[0][3] = canshu42[0];
		SubCanShuMaskx._size_x_left[0][3] = canshu42[1];
		SubCanShuMaskx._size_x_right[0][3] = canshu42[2];
		SubCanShuMaskx._size_y_up[0][3] = canshu42[3];
		SubCanShuMaskx._size_y_down[0][3] = canshu42[4];

		SubBGRCanShuMaskx._keneralsize[0][3] = canshu43[0];
		SubBGRCanShuMaskx._size_x_left[0][3] = canshu43[1];
		SubBGRCanShuMaskx._size_x_right[0][3] = canshu43[2];
		SubBGRCanShuMaskx._size_y_up[0][3] = canshu43[3];
		SubBGRCanShuMaskx._size_y_down[0][3] = canshu43[4];

		canshuExtern._sort_dxy1[0][3] = canshu44[0];
		canshuExtern._sort_dxy2[0][3] = canshu44[1];
		canshuExtern._sort_dxy3[0][3] = canshu44[2];
		canshuExtern._sort_dxy4[0][3] = canshu4[3];
		canshuExtern._houghthr[0][3] = canshu44[4];
		canshuExtern._DUSTHOLD[0][3] = canshu44[5];
		canshuExtern._DUSTSHAPE[0][3] = canshu44[6];
		canshuExtern._fill[0][3] = canshu44[7];
		canshuExtern._TFTHOLD_points[0][3] = canshu44[8];
		canshuExtern._TFTHOLD_muras[0][3] = canshu44[9];
		canshuExtern._subThresh[0][3] = canshu44[10];
		canshuExtern._bgrThresh[0][3] = canshu44[11];
		canshuExtern._subBGRThresh[0][3] = canshu44[12];
		canshuExtern._hsvThresh[0][3] = canshu44[13];


		//第五个颜色

		PointCanShureMoveFalsex._num_gradient[0][4] = canshu45[0];
		PointCanShureMoveFalsex._num_grayScale[0][4] = canshu45[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][4] = canshu45[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][4] = canshu45[3];
		PointCanShureMoveFalsex._meanValueGradient[0][4] = canshu45[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][4] = canshu45[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][4] = canshu45[6];
		PointCanShureMoveFalsex._bright_num[0][4] = canshu45[7];
		PointCanShureMoveFalsex._dark_num[0][4] = canshu45[8];

		muraCanShureMoveFalsex._num_gradient[0][4] = canshu46[0];
		muraCanShureMoveFalsex._num_grayScale[0][4] = canshu46[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][4] = canshu46[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][4] = canshu46[3];
		muraCanShureMoveFalsex._meanValueGradient[0][4] = canshu46[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][4] = canshu46[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][4] = canshu46[6];
		muraCanShureMoveFalsex._bright_num[0][4] = canshu46[7];
		muraCanShureMoveFalsex._dark_num[0][4] = canshu46[8];

		LightCanShureMoveFalsex._num_gradient[0][4] = canshu47[0];
		LightCanShureMoveFalsex._num_grayScale[0][4] = canshu47[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][4] = canshu47[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][4] = canshu47[3];
		LightCanShureMoveFalsex._meanValueGradient[0][4] = canshu47[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][4] = canshu47[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][4] = canshu47[6];
		LightCanShureMoveFalsex._bright_num[0][4] = canshu47[7];
		LightCanShureMoveFalsex._dark_num[0][4] = canshu47[8];

		SubCanShureMoveFalsex._num_gradient[0][4] = canshu48[0];
		SubCanShureMoveFalsex._num_grayScale[0][4] = canshu48[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][4] = canshu48[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][4] = canshu48[3];
		SubCanShureMoveFalsex._meanValueGradient[0][4] = canshu48[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][4] = canshu48[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][4] = canshu48[6];
		SubCanShureMoveFalsex._bright_num[0][4] = canshu48[7];
		SubCanShureMoveFalsex._dark_num[0][4] = canshu48[8];
		
		SubBGRCanShureMoveFalsex._num_gradient[0][4] = canshu49[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][4] = canshu49[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][4] = canshu49[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][4] = canshu49[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][4] = canshu49[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][4] = canshu49[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][4] = canshu49[6];
		SubBGRCanShureMoveFalsex._bright_num[0][4] = canshu49[7];
		SubBGRCanShureMoveFalsex._dark_num[0][4] = canshu49[8];

		PointCanShuMaskx._keneralsize[0][4] = canshu50[0];
		PointCanShuMaskx._size_x_left[0][4] = canshu50[1];
		PointCanShuMaskx._size_x_right[0][4] = canshu50[2];
		PointCanShuMaskx._size_y_up[0][4] = canshu50[3];
		PointCanShuMaskx._size_y_down[0][4] = canshu50[4];

		MuraCanShuMaskx._keneralsize[0][4] = canshu51[0];
		MuraCanShuMaskx._size_x_left[0][4] = canshu51[1];
		MuraCanShuMaskx._size_x_right[0][4] = canshu51[2];
		MuraCanShuMaskx._size_y_up[0][4] = canshu51[3];
		MuraCanShuMaskx._size_y_down[0][4] = canshu51[4];

		lightCanShuMaskx._keneralsize[0][4] = canshu52[0];
		lightCanShuMaskx._size_x_left[0][4] = canshu52[1];
		lightCanShuMaskx._size_x_right[0][4] = canshu52[2];
		lightCanShuMaskx._size_y_up[0][4] = canshu52[3];
		lightCanShuMaskx._size_y_down[0][4] = canshu52[4];

		SubCanShuMaskx._keneralsize[0][4] = canshu53[0];
		SubCanShuMaskx._size_x_left[0][4] = canshu53[1];
		SubCanShuMaskx._size_x_right[0][4] = canshu53[2];
		SubCanShuMaskx._size_y_up[0][4] = canshu53[3];
		SubCanShuMaskx._size_y_down[0][4] = canshu53[4];

		SubBGRCanShuMaskx._keneralsize[0][4] = canshu54[0];
		SubBGRCanShuMaskx._size_x_left[0][4] = canshu54[1];
		SubBGRCanShuMaskx._size_x_right[0][4] = canshu54[2];
		SubBGRCanShuMaskx._size_y_up[0][4] = canshu54[3];
		SubBGRCanShuMaskx._size_y_down[0][4] = canshu54[4];

		canshuExtern._sort_dxy1[0][4] = canshu55[0];
		canshuExtern._sort_dxy2[0][4] = canshu55[1];
		canshuExtern._sort_dxy3[0][4] = canshu55[2];
		canshuExtern._sort_dxy4[0][4] = canshu55[3];
		canshuExtern._houghthr[0][4] = canshu55[4];
		canshuExtern._DUSTHOLD[0][4] = canshu55[5];
		canshuExtern._DUSTSHAPE[0][4] = canshu55[6];
		canshuExtern._fill[0][4] = canshu55[7];
		canshuExtern._TFTHOLD_points[0][4] = canshu55[8];
		canshuExtern._TFTHOLD_muras[0][4] = canshu55[9];
		canshuExtern._subThresh[0][4] = canshu55[10];
		canshuExtern._bgrThresh[0][4] = canshu55[11];
		canshuExtern._subBGRThresh[0][4] = canshu55[12];
		canshuExtern._hsvThresh[0][4] = canshu55[13];


		//第六个颜色

		PointCanShureMoveFalsex._num_gradient[0][5] = canshu56[0];
		PointCanShureMoveFalsex._num_grayScale[0][5] = canshu56[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][5] = canshu56[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][5] = canshu56[3];
		PointCanShureMoveFalsex._meanValueGradient[0][5] = canshu56[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][5] = canshu56[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][5] = canshu56[6];
		PointCanShureMoveFalsex._bright_num[0][5] = canshu56[7];
		PointCanShureMoveFalsex._dark_num[0][5] = canshu56[8];

		muraCanShureMoveFalsex._num_gradient[0][5] = canshu57[0];
		muraCanShureMoveFalsex._num_grayScale[0][5] = canshu57[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][5] = canshu57[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][5] = canshu57[3];
		muraCanShureMoveFalsex._meanValueGradient[0][5] = canshu57[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][5] = canshu57[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][5] = canshu57[6];
		muraCanShureMoveFalsex._bright_num[0][5] = canshu57[7];
		muraCanShureMoveFalsex._dark_num[0][5] = canshu57[8];

		LightCanShureMoveFalsex._num_gradient[0][5] = canshu58[0];
		LightCanShureMoveFalsex._num_grayScale[0][5] = canshu58[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][5] = canshu58[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][5] = canshu58[3];
		LightCanShureMoveFalsex._meanValueGradient[0][5] = canshu58[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][5] = canshu58[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][5] = canshu58[6];
		LightCanShureMoveFalsex._bright_num[0][5] = canshu58[7];
		LightCanShureMoveFalsex._dark_num[0][5] = canshu58[8];

		SubCanShureMoveFalsex._num_gradient[0][5] = canshu59[0];
		SubCanShureMoveFalsex._num_grayScale[0][5] = canshu59[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][5] = canshu59[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][5] = canshu59[3];
		SubCanShureMoveFalsex._meanValueGradient[0][5] = canshu59[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][5] = canshu59[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][5] = canshu59[6];
		SubCanShureMoveFalsex._bright_num[0][5] = canshu59[7];
		SubCanShureMoveFalsex._dark_num[0][5] = canshu59[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][5] = canshu60[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][5] = canshu60[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][5] = canshu60[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][5] = canshu60[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][5] = canshu60[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][5] = canshu60[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][5] = canshu60[6];
		SubBGRCanShureMoveFalsex._bright_num[0][5] = canshu60[7];
		SubBGRCanShureMoveFalsex._dark_num[0][5] = canshu60[8];

		PointCanShuMaskx._keneralsize[0][5] = canshu61[0];
		PointCanShuMaskx._size_x_left[0][5] = canshu61[1];
		PointCanShuMaskx._size_x_right[0][5] = canshu61[2];
		PointCanShuMaskx._size_y_up[0][5] = canshu61[3];
		PointCanShuMaskx._size_y_down[0][5] = canshu61[4];

		MuraCanShuMaskx._keneralsize[0][5] = canshu62[0];
		MuraCanShuMaskx._size_x_left[0][5] = canshu62[1];
		MuraCanShuMaskx._size_x_right[0][5] = canshu62[2];
		MuraCanShuMaskx._size_y_up[0][5] = canshu62[3];
		MuraCanShuMaskx._size_y_down[0][5] = canshu62[4];

		lightCanShuMaskx._keneralsize[0][5] = canshu63[0];
		lightCanShuMaskx._size_x_left[0][5] = canshu63[1];
		lightCanShuMaskx._size_x_right[0][5] = canshu63[2];
		lightCanShuMaskx._size_y_up[0][5] = canshu63[3];
		lightCanShuMaskx._size_y_down[0][5] = canshu63[4];

		SubCanShuMaskx._keneralsize[0][5] = canshu64[0];
		SubCanShuMaskx._size_x_left[0][5] = canshu64[1];
		SubCanShuMaskx._size_x_right[0][5] = canshu64[2];
		SubCanShuMaskx._size_y_up[0][5] = canshu64[3];
		SubCanShuMaskx._size_y_down[0][5] = canshu64[4];

		SubBGRCanShuMaskx._keneralsize[0][5] = canshu65[0];
		SubBGRCanShuMaskx._size_x_left[0][5] = canshu65[1];
		SubBGRCanShuMaskx._size_x_right[0][5] = canshu65[2];
		SubBGRCanShuMaskx._size_y_up[0][5] = canshu65[3];
		SubBGRCanShuMaskx._size_y_down[0][5] = canshu65[4];

		canshuExtern._sort_dxy1[0][5] = canshu66[0];
		canshuExtern._sort_dxy2[0][5] = canshu66[1];
		canshuExtern._sort_dxy3[0][5] = canshu66[2];
		canshuExtern._sort_dxy4[0][5] = canshu66[3];
		canshuExtern._houghthr[0][5] = canshu66[4];
		canshuExtern._DUSTHOLD[0][5] = canshu66[5];
		canshuExtern._DUSTSHAPE[0][5] = canshu66[6];
		canshuExtern._fill[0][5] = canshu66[7];
		canshuExtern._TFTHOLD_points[0][5] = canshu66[8];
		canshuExtern._TFTHOLD_muras[0][5] = canshu66[9];
		canshuExtern._subThresh[0][5] = canshu66[10];
		canshuExtern._bgrThresh[0][5] = canshu66[11];
		canshuExtern._subBGRThresh[0][5] = canshu66[12];
		canshuExtern._hsvThresh[0][5] = canshu66[13];


		//第七个颜色

		PointCanShureMoveFalsex._num_gradient[0][6] = canshu67[0];
		PointCanShureMoveFalsex._num_grayScale[0][6] = canshu67[1];
		PointCanShureMoveFalsex._dst_m_gradient[0][6] = canshu67[2];
		PointCanShureMoveFalsex._dst_m_grayScale[0][6] = canshu67[3];
		PointCanShureMoveFalsex._meanValueGradient[0][6] = canshu67[4];
		PointCanShureMoveFalsex._dst_s_gradient[0][6] = canshu67[5];
		PointCanShureMoveFalsex._dst_s_grayScale[0][6] = canshu67[6];
		PointCanShureMoveFalsex._bright_num[0][6] = canshu67[7];
		PointCanShureMoveFalsex._dark_num[0][6] = canshu67[8];

		muraCanShureMoveFalsex._num_gradient[0][6] = canshu68[0];
		muraCanShureMoveFalsex._num_grayScale[0][6] = canshu68[1];
		muraCanShureMoveFalsex._dst_m_gradient[0][6] = canshu68[2];
		muraCanShureMoveFalsex._dst_m_grayScale[0][6] = canshu68[3];
		muraCanShureMoveFalsex._meanValueGradient[0][6] = canshu68[4];
		muraCanShureMoveFalsex._dst_s_gradient[0][6] = canshu68[5];
		muraCanShureMoveFalsex._dst_s_grayScale[0][6] = canshu68[6];
		muraCanShureMoveFalsex._bright_num[0][6] = canshu68[7];
		muraCanShureMoveFalsex._dark_num[0][6] = canshu68[8];

		LightCanShureMoveFalsex._num_gradient[0][6] = canshu69[0];
		LightCanShureMoveFalsex._num_grayScale[0][6] = canshu69[1];
		LightCanShureMoveFalsex._dst_m_gradient[0][6] = canshu69[2];
		LightCanShureMoveFalsex._dst_m_grayScale[0][6] = canshu69[3];
		LightCanShureMoveFalsex._meanValueGradient[0][6] = canshu69[4];
		LightCanShureMoveFalsex._dst_s_gradient[0][6] = canshu69[5];
		LightCanShureMoveFalsex._dst_s_grayScale[0][6] = canshu69[6];
		LightCanShureMoveFalsex._bright_num[0][6] = canshu69[7];
		LightCanShureMoveFalsex._dark_num[0][6] = canshu69[8];

		SubCanShureMoveFalsex._num_gradient[0][6] = canshu70[0];
		SubCanShureMoveFalsex._num_grayScale[0][6] = canshu70[1];
		SubCanShureMoveFalsex._dst_m_gradient[0][6] = canshu70[2];
		SubCanShureMoveFalsex._dst_m_grayScale[0][6] = canshu70[3];
		SubCanShureMoveFalsex._meanValueGradient[0][6] = canshu70[4];
		SubCanShureMoveFalsex._dst_s_gradient[0][6] = canshu70[5];
		SubCanShureMoveFalsex._dst_s_grayScale[0][6] = canshu70[6];
		SubCanShureMoveFalsex._bright_num[0][6] = canshu70[7];
		SubCanShureMoveFalsex._dark_num[0][6] = canshu70[8];

		SubBGRCanShureMoveFalsex._num_gradient[0][6] = canshu71[0];
		SubBGRCanShureMoveFalsex._num_grayScale[0][6] = canshu71[1];
		SubBGRCanShureMoveFalsex._dst_m_gradient[0][6] = canshu71[2];
		SubBGRCanShureMoveFalsex._dst_m_grayScale[0][6] = canshu71[3];
		SubBGRCanShureMoveFalsex._meanValueGradient[0][6] = canshu71[4];
		SubBGRCanShureMoveFalsex._dst_s_gradient[0][6] = canshu71[5];
		SubBGRCanShureMoveFalsex._dst_s_grayScale[0][6] = canshu71[6];
		SubBGRCanShureMoveFalsex._bright_num[0][6] = canshu71[7];
		SubBGRCanShureMoveFalsex._dark_num[0][6] = canshu71[8];

		PointCanShuMaskx._keneralsize[0][6] = canshu72[0];
		PointCanShuMaskx._size_x_left[0][6] = canshu72[1];
		PointCanShuMaskx._size_x_right[0][6] = canshu72[2];
		PointCanShuMaskx._size_y_up[0][6] = canshu72[3];
		PointCanShuMaskx._size_y_down[0][6] = canshu72[4];

		MuraCanShuMaskx._keneralsize[0][6] = canshu73[0];
		MuraCanShuMaskx._size_x_left[0][6] = canshu73[1];
		MuraCanShuMaskx._size_x_right[0][6] = canshu73[2];
		MuraCanShuMaskx._size_y_up[0][6] = canshu73[3];
		MuraCanShuMaskx._size_y_down[0][6] = canshu73[4];

		lightCanShuMaskx._keneralsize[0][6] = canshu74[0];
		lightCanShuMaskx._size_x_left[0][6] = canshu74[1];
		lightCanShuMaskx._size_x_right[0][6] = canshu74[2];
		lightCanShuMaskx._size_y_up[0][6] = canshu74[3];
		lightCanShuMaskx._size_y_down[0][6] = canshu74[4];

		SubCanShuMaskx._keneralsize[0][6] = canshu75[0];
		SubCanShuMaskx._size_x_left[0][6] = canshu75[1];
		SubCanShuMaskx._size_x_right[0][6] = canshu75[2];
		SubCanShuMaskx._size_y_up[0][6] = canshu75[3];
		SubCanShuMaskx._size_y_down[0][6] = canshu75[4];

		SubBGRCanShuMaskx._keneralsize[0][6] = canshu76[0];
		SubBGRCanShuMaskx._size_x_left[0][6] = canshu76[1];
		SubBGRCanShuMaskx._size_x_right[0][6] = canshu76[2];
		SubBGRCanShuMaskx._size_y_up[0][6] = canshu76[3];
		SubBGRCanShuMaskx._size_y_down[0][6] = canshu76[4];

		canshuExtern._sort_dxy1[0][6] = canshu77[0];
		canshuExtern._sort_dxy2[0][6] = canshu77[1];
		canshuExtern._sort_dxy3[0][6] = canshu77[2];
		canshuExtern._sort_dxy4[0][6] = canshu77[3];
		canshuExtern._houghthr[0][6] = canshu77[4];
		canshuExtern._DUSTHOLD[0][6] = canshu77[5];
		canshuExtern._DUSTSHAPE[0][6] = canshu77[6];
		canshuExtern._fill[0][6] = canshu77[7];
		canshuExtern._TFTHOLD_points[0][6] = canshu77[8];
		canshuExtern._TFTHOLD_muras[0][6] = canshu77[9];
		canshuExtern._subThresh[0][6] = canshu77[10];
		canshuExtern._bgrThresh[0][6] = canshu77[11];
		canshuExtern._subBGRThresh[0][6] = canshu77[12];
		canshuExtern._hsvThresh[0][6] = canshu77[13];


		//第二套参数

		SecondCanshu._point_num = canshu78[0];
		SecondCanshu._line_num = canshu78[1];
		SecondCanshu._mura_num = canshu78[2];
		SecondCanshu._tuan_num = canshu78[3];
		SecondCanshu._point_m_gray = canshu78[4];
		SecondCanshu._line_m_gray = canshu78[5];
		SecondCanshu._mura_m_gray = canshu78[6];
		SecondCanshu._tuan_m_gray = canshu78[7];
		SecondCanshu._maxDistance = canshu78[8];
		SecondCanshu._maxGParea = canshu78[9];
		SecondCanshu._point_bk_num = canshu78[10];
		SecondCanshu._point_bk_m_gray = canshu78[11];

		if (saveflag == 1)
		{
			string sProjectDir = (*sendImg1).Initpath + "//midlle_result//";
			(*sendImg1).projectDir = sProjectDir;
			(*sendImg1).saveFileDir = mkdirUseTime(sProjectDir);  //使用时间创建一个文件夹
			(*sendImg1).fir_result_file = (*sendImg1).saveFileDir + "fir_result_file.txt";
		}

		string ss = "int setCanshu ";
		string ss1 = "0 初始化参数成功！！";
		location_out << ss << endl;
		location_out << ss1 << endl;
		location_out << "****************SUCCESS******************" << endl;
		location_out.close();

		return 0;
	}
}

extern "C" __declspec(dllexport) int SetTemplate(int rowTFT, int colTFT, void *dataTFT, const char *camera, const char *color)
{
	int pictureFlag = transform(color);
	int cameraFlag = transform_camera(camera);
	Mat image = BuildImgFromLabview(rowTFT, colTFT, dataTFT);

	camera_cali_fisrt(image, neican_data, jibian_data, image);

	std::string ss0 = std::to_string(cameraFlag);
	std::string ss1 = std::to_string(pictureFlag);
	

	SetTemplate(image, cameraFlag, pictureFlag);

	string path = (*sendImg1).tmpImg_path + "Template_" + ss0 + "_" + ss1;
	imwrite(path + ".png", (*sendImg1).Template[cameraFlag][pictureFlag]);

	//(*sendImg1).CutTemplate[cameraFlag][pictureFlag] = (*sendImg1).imgTftColorCut[cameraFlag].clone();
	string path1 = (*sendImg1).tmpImg_path + "CutTemplate_" + ss0 + "_" + ss1;
	imwrite(path1 + ".png", (*sendImg1).CutTemplate[cameraFlag][pictureFlag]);
	//imwrite("D:\\project\\CutTemplate.png", (*sendImg1).CutTemplate[flag]);
	//imwrite("D:\\project\\bluetmpimgTftColorCut.png", (*sendImg1).imgTftColorCut);

	//(*sendImg1).CutTemplate_fill[cameraFlag][pictureFlag] = fill((*sendImg1).CutTemplate[cameraFlag][pictureFlag], 20);  // fill =20
	string path2 = (*sendImg1).tmpImg_path + "CutTemplate_fill_" + ss0 + "_" + ss1;
	imwrite(path2 + ".png", (*sendImg1).CutTemplate_fill[cameraFlag][pictureFlag]);

	//pointDetect((*sendImg1).CutTemplate[cameraFlag][pictureFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][pictureFlag]);
	string path3 = (*sendImg1).tmpImg_path + "GradientTemplate_fill_" + ss0 + "_" + ss1;
	imwrite(path3 + ".png", (*sendImg1).GradientTemplate_fill[cameraFlag][pictureFlag]);

	(*sendImg1).A[cameraFlag][pictureFlag] = 1;

	string a = (*sendImg1).tmptxt;
	const char *filename = a.c_str();
	//const char *filename = "..\\data\\patch.bin";      //数据存储的二进制文件
	writefile(filename, (*sendImg1).A);

	//imwrite("D://project//345.png", image);

	if (pictureFlag == 6)
	{
		ofstream location_out1;
		location_out1.open((*sendImg1).logtxt, std::ios::out | std::ios::app);
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间
		char cNowTime[64];
		strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
		string ss = "int SetTemplate";
		string ss1 = "0 设置模板图片成功！！";
		location_out1 << cNowTime << endl;
		location_out1 << ss << endl;
		location_out1 << ss1 << endl;
		location_out1 << "****************SUCCESS******************" << endl;
		location_out1.close();
	}
	return 0;
}

extern "C" __declspec(dllexport) int Init()
{
	string a = (*sendImg1).tmptxt;
	const char *filename = a.c_str();
	int B[10][20];
	readfile(filename, B);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (B[i][j] == 1)
			{
				(*sendImg1).Template[i][j] = imread((*sendImg1).tmpImg_path + "Template_" + to_string(i) + "_" + to_string(j) + ".png");
				(*sendImg1).CutTemplate[i][j] = imread((*sendImg1).tmpImg_path + "CutTemplate_" + to_string(i) + "_" + to_string(j) + ".png");
				(*sendImg1).CutTemplate_fill[i][j] = imread((*sendImg1).tmpImg_path + "CutTemplate_fill_" + to_string(i) + "_" + to_string(j) + ".png");
				(*sendImg1).GradientTemplate_fill[i][j] = imread((*sendImg1).tmpImg_path + "GradientTemplate_fill_" + to_string(i) + "_" + to_string(j) + ".png");



				//imwrite("D://project//11111111111.png",(*sendImg1).CutTemplate[i][j]);
			}
		}
	}

	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	//strftime(cNowTime, sizeof(cNowTime), "%Y年%m月%d日--%H点%M分%S秒", &t);
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	string ss = "int Init()";
	string ss1 = "0 初始化模板图片成功！！";
	location_out << cNowTime << endl;
	location_out << ss << endl;
	location_out << ss1 << "路径：" << (*sendImg1).tmpImg_path << endl;
	location_out << "****************SUCCESS******************" << endl;
	location_out.close();

	return 0;
}

extern "C" __declspec(dllexport) int detect(int rowTFT, int colTFT, void *dataTFT, const char *camera, const char *color, int &rowRoi, int &colRoi, void *dataRoi)
{
	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);

	Mat imgtran;

	int pictureFlag = transform(color);
	int cameraFlag = transform_camera(camera);
	Mat imgTFT = BuildImgFromLabview(rowTFT, colTFT, dataTFT);
	//待检测图片相机畸变校正
	camera_cali_fisrt(imgTFT, neican_data, jibian_data, imgTFT);


	/*主函数*/
	detect(imgTFT, cameraFlag, pictureFlag);

	int row = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].rows;
	int col = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].cols;

	Mat imgRoi = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].clone();

	Mat imgout = imgTFT.clone();
	Mat img_cut = imgout(cv::Rect(0, 0, imgRoi.cols, imgRoi.rows));
	imgRoi.copyTo(img_cut);

	Mat imgshow = imgout.clone();
	//imwrite("D://project//10.png", imgTFT);

	cvtColor(imgshow, imgshow, CV_RGB2RGBA);
	memcpy(dataRoi, imgshow.data, rowTFT*colTFT * 4);


	rowRoi = row;
	colRoi = col;



	if (pictureFlag == 0)
	{
		//string s = color;
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间

		localtime_s(&t, &now);   //获取当地日期和时间
		char cNowTime[64];
		strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
		//location_out << "-------------------\n" << endl;
		location_out << cNowTime << endl;
		string ss = "W screen detect done";
		location_out << ss << endl;
		location_out << "****************SUCCESS******************" << endl;
		location_out.close();
	}
	else
	{
		struct tm t;   //tm结构指针
		time_t now;  //声明time_t类型变量
		time(&now);      //获取系统日期和时间
		localtime_s(&t, &now);   //获取当地日期和时间
		char cNowTime[64];
		strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
		//location_out << "-------------------\n" << endl;
		location_out << cNowTime << endl;
		string s = color;
		string ss = s + " " + "screen detect done";
		location_out << ss << endl;
		location_out << "****************SUCCESS******************" << endl;
		location_out.close();
	}

	return 0;
}

extern "C" __declspec(dllexport) int processDust(int rowDUST, int colDUST, void *dataDUST, const char *camera)
{
	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);

	//保存Roi区域图片
	string a = (*sendImg1).tmptxt;
	const char *filename = a.c_str();
	int B[10][20];
	readfile(filename, B);

	//保存Roi区域图片
	if ((*sendImg1).if_save_midlle_result == 1)
	{
		for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)
		{
			for (int x = 0; x < 7; x++)    //遍历不同颜色的图片
			{
				if (B[cameraFlag][x] == 1)
				{
					imwrite((*sendImg1).saveFileDir + "Roi" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[cameraFlag][x]);
				}
			}
		}
	}

	int cameraFlag = transform_camera(camera);
	Mat imgDust = BuildImgFromLabview(rowDUST, colDUST, dataDUST);
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);

	processDust(imgDust, canshuX, cameraFlag);

	imwrite((*sendImg1).saveFileDir + "gradientDustThreshold.png", (*sendImg1).gradientDustThreshold);
	imwrite((*sendImg1).saveFileDir + "gradientDustThresholDilate.png", (*sendImg1).gradientDustThresholDilate);

	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	string ss = "int processDust";
	string ss1 = "0 灰尘检测成功！！";
	location_out << cNowTime << endl;
	location_out << ss << endl;
	location_out << ss1 << endl;
	location_out << "****************SUCCESS******************" << endl;
	location_out.close();

	return 0;
}

extern "C" __declspec(dllexport) int mask(int rowmask, int colmask, void *dataMask, const char *camera, const char *color, bool *array)
{

	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_point[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);   //20 10 20 50 60//创建团的Mask模板

	/************  图片1和图片4的大小一样，图片2和图片3的大小一样******/
	initChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_muras[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).gradientResultThreshold_louguangs[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).LightMaskChangeGradientDrawIn, *lightCanShuMask);

	//0219 add sub的缩进mask操作
	initChangePartOfTheRegionalGradient((*sendImg1).subtractTemplateTftImgs[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, *SubCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).gradientResultThreshold_bgr_sub[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubBGRMaskChangeGradietRCorner, (*sendImg1).SubBGRMaskChangeGradientDrawIn, *SubBGRCanShuMask);

	Mat img = BuildImgFromLabview(rowmask, colmask, dataMask);

	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);

	int pictureFlag = transform(color);
	int cameraFlag = transform_camera(camera);

	if (pictureFlag == 0)
	{
		bool defect[9] = { false };

		CombineXiaci(false);

		//imwrite("D://project//0001.png", (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag]);

		determine1();

		//imwrite("D://project//0002.png", (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag]);

		getGrounpPointResult(100, 2);

		//imwrite("D://project//0003.png", (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag]);

		drawPic();

		//imwrite("D://project//0.png", (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag]);

		Mat imgout;
		imgout = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].clone();

		//输出数组
		for (int i = 0; i < 8; i++)
		{
			//location_out << (*sendImg1).matrix[cameraFlag][0][i] << endl;

			if ((*sendImg1).matrix[cameraFlag][0][i] == 1)
			{
				defect[i] = true;
			}
			else
			{
				defect[i] = false;
			}
			//location_out << defect[i] << endl;
		}
		int R_error_num = 0;
		for (int j = 0; j < 4; j++)
		{
			if ((*sendImg1).R_compare_res[j] == 1)
			{
				R_error_num++;
			}
		}
		if (R_error_num > 0)
		{
			defect[8] = true;
		}
		else
		{
			defect[8] = false;
		}

		for (int numArray = 0; numArray < 9; numArray++)
		{
			//location_out << numArray << endl;
			*(array + numArray) = defect[numArray];
			//location_out << *(array + numArray) << endl;
		}

		if ((*sendImg1).if_show_result == 1)
		{
			namedWindow("imgout", 0);
			imshow("imgout", imgout);
			waitKey(0);
			destroyWindow("imgout");
		}

	}
	else if (pictureFlag != 0)
	{
		bool defect[9];

		Mat imgout;
		imgout = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].clone();

		//imwrite("D://project//1.png", imgout);


		//输出数组
		for (int i = 0; i < 8; i++)
		{
			if ((*sendImg1).matrix[cameraFlag][pictureFlag][i] == 1)
			{
				defect[i] = true;
			}
			else
			{
				defect[i] = false;
			}
		}
		int R_error_num = 0;
		for (int j = 0; j < 4; j++)
		{
			if ((*sendImg1).R_compare_res[j] == 1)
			{
				R_error_num++;
			}
		}
		if (R_error_num > 0)
		{
			defect[8] = true;
		}
		else
		{
			defect[8] = false;
		}

		for (int numArray = 0; numArray < 9; numArray++)
		{

			//location_out << numArray << endl;
			*(array + numArray) = defect[numArray];
			//location_out << *(array+ numArray) << endl;
		}

		if ((*sendImg1).if_show_result == 1)
		{
			namedWindow("imgout", 0);
			imshow("imgout", imgout);
			waitKey(0);
			destroyWindow("imgout");
		}
	}

	int row = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].rows;
	int col = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].cols;


	Mat imgmask = (*sendImg1).imgTftColorCuts[cameraFlag][pictureFlag].clone();

	Mat imgout = img.clone();
	Mat img_cut = imgout(cv::Rect(0, 0, imgmask.cols, imgmask.rows));
	imgmask.copyTo(img_cut);

	//imwrite("D://project//10.png", imgout);

	cvtColor(imgout, imgout, CV_RGB2RGBA);
	memcpy(dataMask, imgout.data, rowmask*colmask * 4);

	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	string camera1 = camera;
	string color1 = color;
	string ss1 = camera1 + " " + color1 + " " + "mask检测成功";
	location_out << cNowTime << endl;
	location_out << ss1 << endl;
	location_out << "****************SUCCESS******************" << endl;
	location_out.close();

	return 0;
}

extern "C" __declspec(dllexport) int get_table_result(int rowTFT, int colTFT, void *dataTFT, void *dataresult)
{
	ofstream location_out;
	location_out.open((*sendImg1).logtxt, std::ios::out | std::ios::app);

	string a = (*sendImg1).tmptxt;
	const char *filename = a.c_str();
	int B[10][20];
	readfile(filename, B);

	//保存中间结果图片
	if ((*sendImg1).if_save_midlle_result == 1)
	{
		for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)
		{
			for (int x = 0; x < 7; x++)    //遍历不同颜色的图片
			{
				if (B[cameraFlag][x] == 1)
				{
					//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png",(*sendImg1).imgTftColorCuts[cameraFlag][x]);
					imwrite((*sendImg1).saveFileDir + "RoiFill" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png", (*sendImg1).imgTftColorCutFills[cameraFlag][x]);
					//imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png",(*sendImg1).gradientResultThreshold_all[cameraFlag][x]);
					imwrite((*sendImg1).saveFileDir + "tftResultShow_final" + "_" + to_string(cameraFlag) + "_" + to_string(x) + ".png", (*sendImg1).imgTftColorCuts[cameraFlag][x]);
				}
			}
		}

		imwrite((*sendImg1).saveFileDir + "dust.png", (*sendImg1).gradientDustThreshold);
	}



	Mat res_table = BuildImgFromLabview(rowTFT, colTFT, dataTFT);
	vector<Point2i> coordinate = (*sendImg1).points;
	//int R_res[4] = (*sendImg1).R_compare_res;
	Mat final_img = (*sendImg1).back;

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
		if ((*sendImg1).R_compare_res[j] == 1)
		{
			circle(res_table, R, 16, Scalar(0, 0, 255), -1);
			R_size = 1;
			break;
		}
	}

	// 有缺陷则总结果处为红色
	if (coordinate.size() > 0 || R_size > 0)
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

	imwrite((*sendImg1).saveFileDir + "restable.png", res_table);
	//imwrite("D://project//res_table.png", res_table);

	Mat imgout = res_table.clone();
	cvtColor(imgout, imgout, CV_RGB2RGBA);
	memcpy(dataresult, imgout.data, rowTFT*colTFT * 4);

	(*sendImg1).points.clear();

	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y/%m/%d日  %H:%M:%S  %A", &t);
	string ss = "int get_final_result";
	string ss1 = "0 获取最终结果成功";
	location_out << cNowTime << endl;
	location_out << ss << endl;
	location_out << ss1 << endl;
	location_out << "****************SUCCESS******************" << endl;
	location_out.close();

	(*sendImg1).points.clear();

	memset((*sendImg1).matrix, 0, 10 * 7 * 8 * sizeof(int));

	return 0;

}

extern "C" __declspec(dllexport) int Trackbar(int rowtmp, int coltmp, void *datatmp, int rowTFT, int colTFT, void *dataTFT, int rowdust, int coldust, void *datadust, const char *camera, const char *color)
{
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）


	int cameraFlag = transform_camera(camera);
	int pictureFlag = transform(color);
	

	//模拟labview读取图片进入dll
	Mat imgtmp = BuildImgFromLabview(rowtmp, coltmp, datatmp);  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正

	//imwrite("D://project//0imgtmp.png", imgtmp);

	SetTemplate(imgtmp, camera_flag, pictureFlag);

	//imwrite("D://project//0imgtmp_after.png", imgtmp);

	//debug1();
	Mat imgFTF = BuildImgFromLabview(rowTFT, colTFT, dataTFT);  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);
	(*sendImg1).imgTFT[cameraFlag] = imgFTF;
	(*sendImg1).debug_img = imgFTF.clone();

	//imwrite("D://project//0imgTFT.png", imgFTF);


	Mat imgDust = BuildImgFromLabview(rowdust, coldust, datadust);
	(*sendImg1).debug_img_dust = imgDust.clone();
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	string configDir;

	processDust(imgDust, canshuX, cameraFlag);  //camera_flag
	//setmethodTrackbar(imgtmp, 0, 1, 0);  //其他颜色屏幕

	//imwrite("D://project//0imgDUST.png", imgFTF);

	setmethodTrackbar(imgtmp, cameraFlag, pictureFlag);

	//imwrite("D://project//0final.png", imgFTF);

	//setsecondParameterTrackbar(imgtmp, 0, 0, 0);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
