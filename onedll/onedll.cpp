
// dustGetYu.cpp: 定义控制台应用程序的入口点。
//0119 新添判断阈值 极亮点、极暗点
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
#include "onedll.h"
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

//参数初始化
int TFTHOLD = 130;   //130
int DUSTHOLD = 104;
int DUSTSHAPE = 5;


//int num_gradient = 100;    //梯度异常总数判断阈值
//int num_grayScale = 100;  //灰度异常总数判断阈值
//int dst_m_gradient = 100; //梯度异常判断阈值
//int dst_m_grayScale = 8; //灰度异常判断阈值
//int dst_s_gradient = 20;
//int dst_s_grayScale = 3; //灰度异常判断阈值
//int meanValueGradient = 20;

int num_gradient = 3;    //梯度异常总数判断阈值
int num_grayScale = 5;  //灰度异常总数判断阈值
int dst_m_gradient = 10; //梯度异常判断阈值
int dst_m_grayScale = 8; //灰度异常判断阈值
int dst_s_gradient = 8;//梯度标准差 20 before
int dst_s_grayScale = 3; //灰度异常判断阈值
int meanValueGradient = 20;

int fill1 = 20;  //灰度异常总数判断阈值
int bright_num = 4;  //灰度异常总数判断阈值
int dark_num = 4;  //灰度异常总数判断阈值


sendImg sendImgExtern;
CanShu canshuExtern(TFTHOLD, DUSTHOLD, DUSTSHAPE, num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, fill1, meanValueGradient, dst_s_gradient, dst_s_grayScale, bright_num, dark_num);

//CanShu BrightDark_Num(bright_num, dark_num);


//Mask参数

int keneralsize = 20;
int size_x_left = 5;
int size_x_right = 0;
int size_y_up = 5;
int size_y_down = 15;

int point_num = 0;
int line_num = 0;
int mura_num = 0;
int tuan_num = 0;
double point_m_gray = 0;
double line_m_gray = 0;
double mura_m_gray = 0;
double tuan_m_gray = 0;


CanShuFinal SecondCanshu(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray);
CanShuMask PointCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down);
//CanShuMask MuraCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down);
CanShuMask MuraCanShuMaskx(25, 10, 10, 10, 30);
CanShuMask lightCanShuMaskx(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down);
//初始化Mask参数

//(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int meanValueGradient, int dst_s_gradient, int  dst_s_grayScale)
//移除假缺陷参数
//CanShureMoveFalse    PointCanShureMoveFalsex(100,100,100,8,20,3,20);
//
//CanShureMoveFalse  muraCanShureMoveFalsex(4, 5, 100, 8, 20,3, 20);

CanShureMoveFalse    PointCanShureMoveFalsex(3, 5, 10, 8, 20, 8, 3, 20, 20);

CanShureMoveFalse  muraCanShureMoveFalsex(3, 5, 10, 8, 20, 8, 3, 4, 4);

//vector <defectInformationss> defectInformations(30);

//defectInformation detectInfor[30][4][200];
//设定为全局变量
sendImg *sendImg1 = &sendImgExtern;

CanShu * canshuX = &canshuExtern;

//CanShu * canshu_num = &BrightDark_Num;

CanShuMask *PointCanShuMask = &PointCanShuMaskx;

CanShuMask *MuraCanShuMask = &MuraCanShuMaskx;

CanShuMask *lightCanShuMask = &lightCanShuMaskx;



CanShureMoveFalse  *muraCanShureMoveFalse = &muraCanShureMoveFalsex;

CanShureMoveFalse    *PointCanShureMoveFalse = &PointCanShureMoveFalsex;




//相机畸变校准用到的参数
float neican_data[] = { 124046.170922198990,0,1672.8744383824846000,0, 123576.1775990077400,1436.80076020515030,0,0,1 };
float jibian_data[] = { -115.529366790098550, -13.3238477842862260, -0.0822106292981709260,0.00036432147392857344,-0.00288576837799785390 };

float neican_data_A[] = { 74783.7435244331720,0,1678.03692091216480,0, 78656.741412721021000,1372.67656859171460000,0,0,1 };
float jibian_data_A[] = { -8.86938558282631110, 2.26982865811383980, -0.121296204933671560,0.151996295500854470,0.0010363396229630410 };

float neican_data_E[] = { 14057.49668362299100,0,1914.823952172616400,0, 13696.913440609065000,792.15791617674211000,0,0,1 };
float jibian_data_E[] = { -0.54469973088577128000, -38.23095327363589700, 0.03861500246177145700,-0.01644584532002359500,1743.74914915230310000000 };
//缺陷信息存储

vector<defectInformation> defectsInforGray[10][20][4];


vector<defectInformation> defectsInfor[10][20][4];


Mat BuildImgFromLabview(int rows, int cols, void *data)
{
	Mat image_src(rows, cols, CV_8UC4, data);
	cvtColor(image_src, image_src, CV_RGBA2RGB);
	//imshow("test1", image_src);
	return image_src;
}
//缺陷信息存储
//vector<defectInformation> defectsInfor[10][20][4];

//labview传图片数据初始化为图片
void InitMat(Mat& m, float* num)
{
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			m.at<float>(i, j) = *(num + i * m.rows + j);
}


//!!!
void processImg(int cameraFlag, sendImg * sendImg2)
{

	Mat imgTFT = (*sendImg2).imgTFT[cameraFlag].clone();//白屏拍照
	string res_path = "D:\\project\\vs2017\\Data\\3.png";
	//*** ******************  获取边界轮廓    ******************************************//
	/*复制彩色白屏图片*/
	/*去除边界*/
	(*sendImg2).BorderTFT = border1((*sendImg2).imgTftColorCut[cameraFlag]);
	cout << "imgTftColorCut.rows:" << (*sendImg2).imgTftColorCut[cameraFlag].rows << endl;
	cout << "imgTftColorCut.cols:" << (*sendImg2).imgTftColorCut[cameraFlag].cols << endl;
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	dilate((*sendImg2).BorderTFT, (*sendImg2).BorderTftDilate, kernel1);
	(*sendImg2).BorderTftDilate = ~(*sendImg2).BorderTftDilate;
	//*** ******************  获取白屏检测结果    ******************************************//
	/*计算缺陷+灰尘+轮廓*/
	/*填充图片*/
}

//初始化参数
CanShu initCanshu()
{
	int TFTHOLD = 187;
	int DUSTHOLD = 134;
	int DUSTSHAPE = 5;

	int num_gradient = 300;
	int num_grayScale = 200;
	int dst_m_gradient = 20;
	int dst_m_grayScale = 44;
	int dst_s_gradient = 20;
	int dst_s_grayScale = 40;
	int fill = 100;
	int  meanValueGradient = 10;
	int  bright_num = 4;
	int  dark_num = 4;
	CanShu canshu(TFTHOLD, DUSTHOLD, DUSTSHAPE, num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, fill, meanValueGradient, dst_s_gradient, dst_s_grayScale, bright_num, dark_num);
	return canshu;
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
	(*sendImg1).Template[cameraFlag][pictureFlag] = image;
	(*sendImg1).imgTFT[cameraFlag] = image;

	if (pictureFlag == 0)  // 白屏模板
	{
		string sProjectDir = "D://lcmProject//";
		(*sendImg1).projectDir = sProjectDir;
		(*sendImg1).res_tmp_dir = sProjectDir + "R_tmmp//";
		(*sendImg1).pts1_tmp_file = sProjectDir + "R_tmmp//pts1_tmp.txt";

		//【获取ROI图】imgTftColorCut，true表示此时为模板图，会【保存四个角点信息】写在文件中
		getRoi(cameraFlag, true);

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

	//imwrite("D:\\project\\CutTemplate.png", (*sendImg1).CutTemplate[flag]);
	//imwrite("D:\\project\\bluetmpimgTftColorCut.png", (*sendImg1).imgTftColorCut);

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
void processDust(Mat imgDust, string configfile, CanShu * canshuX, int cameraFlag)
{
	(*sendImg1).imgDust = imgDust;

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
	//namedWindow("imgDustColorCut", 0);
	//imshow("imgDustColorCut", (*sendImg1).imgDustColorCut);
	//waitKey(0);
	//imwrite(".\\2camera-grid\\grid40_left_res.png", (*sendImg1).imgDustColorCut);


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

	double ret3 = cv::threshold((*sendImg1).gradientDUST, (*sendImg1).gradientDustThreshold, (*canshuX)._DUSTHOLD, 255, THRESH_BINARY);

	//膨胀二值化的灰尘梯度图，方便查看灰尘
	Mat showDUST = (*sendImg1).gradientDustThreshold.clone();
	Mat kernel4 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	dilate(showDUST, showDUST, kernel4);
	vector<vector<Point>>contours2;
	Mat hierarchy2;
	cv::findContours(showDUST, contours2, hierarchy2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	//在彩图上显示灰尘缺陷
	cv::drawContours((*sendImg1).imgDustColorCut, contours2, -1, Scalar(0, 0, 255), 2);
	int DUSTshape = (*canshuX)._DUSTSHAPE;
	Mat kernel5 = cv::getStructuringElement(cv::MORPH_RECT, Size(DUSTshape, DUSTshape));
	dilate((*sendImg1).gradientDustThreshold, (*sendImg1).gradientDustThresholDilate, kernel5);


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
		(*sendImg1).imgTftColor_point[cameraFlag] = fill((*sendImg1).imgTftColor_point[cameraFlag], (*canshuX)._fill);


		Mat imgOut_b;
		Mat imgOut_g;
		Mat imgOut_r;
		Mat channels[3];
		int RGB_Threshold;
		//白屏阈值与其他不一样
		if (pictureFlag == 0)
		{
			RGB_Threshold = 160;
		}
		else
		{
			RGB_Threshold = 130;
		}

		//必须分通道
		split((*sendImg1).imgTftColor_point[cameraFlag], channels);//分割imgTftColor_mura的通道
													   //gradientTFT = muraDetect(imgTFT_gray);
		pointDetect(channels[0], imgOut_b);
		pointDetect(channels[1], imgOut_g);
		pointDetect(channels[2], imgOut_r);
		cv::threshold(imgOut_b, imgOut_b, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_g, imgOut_g, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_r, imgOut_r, RGB_Threshold, 255, THRESH_BINARY);  //100
																			   //增加了全局变量 (*sendImg1).gradientTFTg_point 与原始的灰度梯度图二值化后的结果或在一起
		bitwise_or(imgOut_b, imgOut_g, (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag]);
		bitwise_or(imgOut_r, (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag]);



		cvtColor((*sendImg1).imgTftColor_point[cameraFlag], (*sendImg1).imgTFTpoint_gray[cameraFlag], CV_BGR2GRAY);
		pointDetect((*sendImg1).imgTFTpoint_gray[cameraFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag]);
		//bitwise_or((*sendImg1).gradientTFTbgr_point[flag], (*sendImg1).GradientResult_point[flag], (*sendImg1).gradientTFTbgr_point[flag]);
		repadding((*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][pictureFlag], (*sendImg1).shrink_size_point);
		repadding((*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).GradientResult_point[cameraFlag][pictureFlag], (*sendImg1).shrink_size_point);


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
		(*sendImg1).imgTftColor_mura[cameraFlag] = fill(imgMura, (*canshuX)._fill);
		Mat imgOut_b;
		Mat imgOut_g;
		Mat imgOut_r;
		Mat channels[3];
		int RGB_Threshold;

		if (pictureFlag == 0)
		{
			RGB_Threshold = 170;
		}
		else
		{
			RGB_Threshold = 130;
		}
		//必须分通道
		split((*sendImg1).imgTftColor_mura[cameraFlag], channels);//分割imgTftColor_mura的通道
													  //gradientTFT = muraDetect(imgTFT_gray);
		muraDetect(channels[0], imgOut_b);
		muraDetect(channels[1], imgOut_g);
		muraDetect(channels[2], imgOut_r);
		cv::threshold(imgOut_b, imgOut_b, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_g, imgOut_g, RGB_Threshold, 255, THRESH_BINARY);  //100
		cv::threshold(imgOut_r, imgOut_r, RGB_Threshold, 255, THRESH_BINARY);  //100
																			   //增加了全局变量 (*sendImg1).gradientTFTg_muras 与原始的灰度梯度图二值化后的结果或在一起
		bitwise_or(imgOut_b, imgOut_g, (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);
		bitwise_or(imgOut_r, (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);

		// 灰度图检测
		cvtColor((*sendImg1).imgTftColor_mura[cameraFlag], (*sendImg1).imgTFTmura_gray[cameraFlag], CV_BGR2GRAY);
		muraDetect((*sendImg1).imgTFTmura_gray[cameraFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag]);
		//bitwise_or((*sendImg1).gradientTFTbgr_muras[flag], (*sendImg1).gradientTFT_muras[flag], (*sendImg1).gradientTFTbgr_muras[flag]);

		//!!!
		repadding((*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag], (*sendImg1).shrink_size_mura);
		repadding((*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).gradientTFT_muras[cameraFlag][pictureFlag], (*sendImg1).shrink_size_mura);
		//cv::threshold((*sendImg1).gradientTFTbgr_muras[flag], (*sendImg1).gradientTFTbgr_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		//GaussianBlur(imgOut_g, imgOut_g, Size(9, 9), 0, 0, BORDER_DEFAULT);
		//cv::medianBlur(imgOut_g, imgOut_g, 3);
		//GaussianBlur(imgOut_g, imgOut_g, Size(5, 5), 0, 0, BORDER_DEFAULT);
		//repadding(imgOut_g, imgOut_g, (*sendImg1).shrink_size_mura);
		//imwrite((*sendImg1).sNowTimeFileDir + "imgOut_g" + to_string(flag) + ".jpg", imgOut_g);
		//cv::threshold(imgOut_g, (*sendImg1).gradientTFTg_muras[flag], RGB_Threshold, 255, THRESH_BINARY);  //100

		////GaussianBlur(imgOut_b, imgOut_b, Size(5, 5), 0, 0, BORDER_DEFAULT);
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
		imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras" + to_string(cameraFlag) + "_" + to_string(pictureFlag) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][pictureFlag]);
		//cout << "repadding2.row:" << (*sendImg1).GradientResult_mura[flag].rows << endl;
		//cout << "repadding2.col:" << (*sendImg1).GradientResult_mura[flag].cols << endl;

		break; // 可选的

	};


			 // 检测漏光
	case(2): {

		Mat imgLouguang = (*sendImg1).imgTftColorCut[cameraFlag].clone();
		drawIn(imgLouguang, imgLouguang, (*sendImg1).shrink_size_louguang);
		(*sendImg1).imgTftColor_louguang[cameraFlag] = fill(imgLouguang, (*canshuX)._fill);
		// 彩图检测
		getLightLeakArea((*sendImg1).imgTftColor_louguang[cameraFlag], (*sendImg1).GradientResult_louguang[cameraFlag][pictureFlag]);
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
			}


			//告诉detect检测到第几张图的标准位清零
			(*sendImg1).MaxFlag[cameraFlag] = 0;

			//白屏时候进行漏光检测
			//getLightLeakArea();


			//检测图【ROI获取】（同时对检测到的四个角点坐标进行与模板的角点匹配检测【只对0相机进行角点匹配--0123】）
			getRoi(cameraFlag, false);
			//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123
			//if (cameraFlag == 1)
			//	return;


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

		long t2 = GetTickCount();
		cout << "Roi—time：" << (t2 - t1) << endl;
		//imwrite("D:\\project\\imgTftColorCut111111.png", (*sendImg1).imgTftColorCut);
		//roi区域比对



		/************************ result1为第二次进一步判断检测图与模板图差异是否太大的结果 （在第一次为true前提下） ***************/

		long t55 = GetTickCount();
		////4.！！！需要修改成多相机模式
		bool result1 = judgment1((*sendImg1).imgTftColorCut[cameraFlag], cameraFlag, pictureFlag);
		long t56 = GetTickCount();
		cout << "judgment-2函数运行的时间为：" << (t56 - t55) << endl;  //0.2S-0.25S


		int back_row = (*sendImg1).imgTftColorCut[cameraFlag].rows;
		int back_col = (*sendImg1).imgTftColorCut[cameraFlag].cols;


		(*sendImg1).P[0][0].x = 0;
		(*sendImg1).P[0][0].y = 0;
		(*sendImg1).P[0][1].x = 100;
		(*sendImg1).P[0][1].y = 100;
		(*sendImg1).P[1][0].x = back_row - 101;
		(*sendImg1).P[1][0].y = 0;
		(*sendImg1).P[1][1].x = back_row - 1;
		(*sendImg1).P[1][1].y = 100;
		(*sendImg1).P[2][0].x = back_row - 101;
		(*sendImg1).P[2][0].y = back_col - 101;
		(*sendImg1).P[2][1].x = back_row - 1;
		(*sendImg1).P[2][1].y = back_col - 1;
		(*sendImg1).P[3][0].x = 0;
		(*sendImg1).P[3][0].y = back_col - 101;
		(*sendImg1).P[3][1].x = 100;
		(*sendImg1).P[3][1].y = back_col - 1;


		Mat img_back(back_row, back_col, CV_8UC3, Scalar(255, 255, 255));
		(*sendImg1).back = img_back.clone();
		if (result1 == true)  // 这次judgment1在于看是否存在大面积缺陷
		{
			//内缩  之前的内缩方法，现在放弃掉了
			//drawIn1();

			//ROI图像预处理  【获取边界轮廓】
			long t57 = GetTickCount();
			processImg(cameraFlag, sendImg1);
			long t58 = GetTickCount();
			cout << "process函数获取边界运行的时间为：" << (t58 - t57) << endl;

			//无内缩直接fill填充图片
			//long t66 = GetTickCount();
			(*sendImg1).imgTftColorCutFills[cameraFlag][pictureFlag] = fill((*sendImg1).imgTftColorCut[cameraFlag], (*canshuX)._fill);
			//long t67 = GetTickCount();
			//cout << "fill -1函数运行的时间为：" << (t67 - t66) << endl;

			////12.21
			//拆分图片
			//spiltImgTFT(sendImg1);
			//////计算梯度 
			//gradient_sobel_channel3_or(sendImg1);





			// fill 【填充】（在内缩之后）
			/////////////////////////////////////////

			//拆分图片
			//spiltImgTFT(sendImg1);


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
			for (int numthread = 0; numthread < 4; numthread++)
			{
				thread t(detectDifferentTypeDefect, numthread, cameraFlag, pictureFlag);
				t.detach();
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
void getOneColorPointResult(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	(*defect).clear();
	defectInformation xxx;
	//xxx.h = 52;
	//(*defect).push_back(xxx);
	//(*defect).push_back(xxx);
	//(*defect).push_back(xxx);
	//(*defect).push_back(xxx);
	//(*defect).push_back(xxx);


	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).GradientResult_point[imgFlag], (*sendImg1).GradientResult_point[imgFlag], 0.0, 0.0);

	ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], 0.0, 0.0);

	//subDustBorder_mura((*sendImg1).gradientTFT_muras[imgFlag], (*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).contours_muras[imgFlag], imgFlag);
	subDustBorder((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_points" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_points[imgFlag]);

	removePointFalse((*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], (*sendImg1).pointResults[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, flag, defect);  //true

																																											 //cout << "point end" << endl;

}


//0118 lyl test
void getOneColorPointResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{





	//ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, (*sendImg1).GradientResult_point[imgFlag], (*sendImg1).GradientResult_point[imgFlag], 0.0, 0.0);

	ChangePartOfTheRegionalGradient((*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], 0.0, 0.0);

	//subDustBorder_mura((*sendImg1).gradientTFT_muras[imgFlag], (*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).contours_muras[imgFlag], imgFlag);
	subDustBorder((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_points" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_points[imgFlag]);

	//removePointFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_points[imgFlag], (*sendImg1).pointResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect);  //true


	removeGradientFalse((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).contours_points[cameraFlag][imgFlag], (*sendImg1).pointResults[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 0);  //true

	cout << "lalalalal";

	//cout << "point end" << endl;

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
void getOneColorMuraResult(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect) {

	//0114 add test lyl
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTg_muras[imgFlag], (*sendImg1).gradientTFTg_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTb_muras[imgFlag], (*sendImg1).gradientTFTb_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTr_muras[imgFlag], (*sendImg1).gradientTFTr_muras[imgFlag], 0.0, 0.0);


	//0115 修改 lyl 简化流程
	ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], 0.0, 0.0);
	ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder_mura((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写 test 0109
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_muras" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_muras[imgFlag]);
	removeMuraFalse((*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], (*sendImg1).muraResults[cameraFlag][imgFlag], muraCanShureMoveFalse, cameraFlag, imgFlag, flag, defect);   //flag
}


//0118 lyl add test
void getOneColorMuraResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect) {

	//0114 add test lyl
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTg_muras[imgFlag], (*sendImg1).gradientTFTg_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTb_muras[imgFlag], (*sendImg1).gradientTFTb_muras[imgFlag], 0.0, 0.0);
	//ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTr_muras[imgFlag], (*sendImg1).gradientTFTr_muras[imgFlag], 0.0, 0.0);


	//0115 修改 lyl 简化流程
	ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], 0.0, 0.0);
	ChangePartOfTheRegionalGradient((*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], 0.0, 0.0);
	subDustBorder_mura((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], cameraFlag, imgFlag);

	//添加梯度图的写 test 0109
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_muras" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_muras[imgFlag]);
	//removeMuraFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_muras[imgFlag], (*sendImg1).muraResults[imgFlag], muraCanShureMoveFalse, imgFlag, flag, defect);   //flag

	//0118
	removeGradientFalse((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).contours_muras[cameraFlag][imgFlag], (*sendImg1).muraResults[cameraFlag][imgFlag], muraCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 1);

}




//该函数流程：先减边除尘，然后去除假缺陷
void getOneColorLouguangResult1(int cameraFlag, int imgFlag, bool flag)
{
	subDustBorder_louguang((*sendImg1).GradientResult_louguang[cameraFlag][imgFlag], (*sendImg1).GradientResult_louguang[cameraFlag][imgFlag], (*sendImg1).contours_louguangs[cameraFlag][imgFlag]);
	//removeLouguangFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_louguangs[imgFlag], (*sendImg1).louguangResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag);
}

//该函数流程：先减边除尘，然后去除假缺陷
void getOneColorSubTemplateResult1(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	subDustBorder_subTmp((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag]);
	//添加梯度图的写
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_subTeps" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_subTeps[imgFlag]);
	//removeSubTmpFalse((*sendImg1).imgTftColorCuts[imgFlag], (*sendImg1).contours_subTmps[imgFlag], (*sendImg1).subTmpResults[imgFlag], PointCanShureMoveFalse, imgFlag, flag, defect);

	//0118
	removeGradientFalse((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).contours_subTmps[cameraFlag][imgFlag], (*sendImg1).subTmpResults[cameraFlag][imgFlag], PointCanShureMoveFalse, cameraFlag, imgFlag, flag, defect, 3);

}

//0118 add test lyl
//将进行梯度筛选后的二值化后的图片合在一起，然后进行灰度筛选
//CombAndRemoveGrayFalse((*sendImg1).gradientResultThreshold_points[imgFlag], (*sendImg1).gradientResultThreshold_muras[imgFlag], (*sendImg1).gradientResultThreshold_subTeps[imgFlag], (*sendImg1).gradientResultThreshold_all[imgFlag], imgFlag, flag, defectsInfor);
void CombAndRemoveGrayFalse(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	(*defect).clear();
	bitwise_or((*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_muras[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
	bitwise_or((*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_subTeps[cameraFlag][imgFlag], (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);

	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_all" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_all[cameraFlag][imgFlag]);
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
		cout << "hahahahahahahhahahahahahahahaha" << endl;
		*ifdone = 1;
		break; // 可选的
	};

	case(1): {

		//getOneColorMuraResult(imgFlag, flag, defectsInfor);
		getOneColorMuraResult1(cameraFlag, imgFlag, flag, defectsInfor);
		*ifdone = 1;
		cout << "22222222222222222222" << endl;
		break; // 可选的

	};

	case(2): {



		getOneColorLouguangResult1(cameraFlag, imgFlag, flag);
		cout << "333333333333333333333333" << endl;
		*ifdone = 1;
		break; // 可选的


	}

	case(3): {


		//getOneColorSubTemplateResult(imgFlag, flag, defectsInfor);
		getOneColorSubTemplateResult1(cameraFlag, imgFlag, flag, defectsInfor);
		cout << "444444444444444444444444" << endl;
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
	imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + to_string(imgFlag) + ".jpg", imgOut);




}
//**********多线程运行四种检测方法，得到检测结果*************//
void getOneColorResult(int cameraFlag, int imgFlag, bool flag, int *ifthreadDonn, vector<defectInformation> defectsInfor[4])
{



	count_information_img((*sendImg1).CutTemplate_fill[cameraFlag][imgFlag], (*sendImg1).GradientTemplate_fill[cameraFlag][flag], cameraFlag, imgFlag);
	int threadDone[4] = { 0,0,0,0 };
	for (int DefectType = 0; DefectType < 4; DefectType++) {

		getDifferentTypeDefectResult(DefectType, &threadDone[DefectType], cameraFlag, imgFlag, flag, &defectsInfor[DefectType]);

		//thread t(getDifferentTypeDefectResult, DefectType, &threadDone[DefectType], cameraFlag,imgFlag, flag, &defectsInfor[DefectType]);
		//t.detach();
	}
	while (threadDone[0] != 1 || threadDone[1] != 1 || threadDone[2] != 1 || threadDone[3] != 1)
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
	vector<Point2f> pointGroundTrue[30];

	for (int cameraFlag = 0; cameraFlag < 10; cameraFlag++)
	{
		for (int imgFlag = 0; imgFlag < (*sendImg1).MaxFlag[cameraFlag]; imgFlag++)    //遍历不同颜色的图片

		{
			vector<int>  NumberOfApproachesPoints;


			for (int pointNumCenter = 0; pointNumCenter < (*sendImg1).group_points[imgFlag].size(); pointNumCenter++)
			{

				int  NumberOfApproachesPoint = 0;
				//计算与该点相近的点数，最少为一，也就是本身
				for (int pointNum = 0; pointNum < (*sendImg1).group_points[imgFlag].size(); pointNum++)
				{
					double  xDistance = ((*sendImg1).group_points[imgFlag][pointNumCenter].x - (*sendImg1).group_points[imgFlag][pointNum].x);
					double  yDistance = ((*sendImg1).group_points[imgFlag][pointNumCenter].y - (*sendImg1).group_points[imgFlag][pointNum].y);
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
					for (int checkDuplicate = 0; checkDuplicate < pointGroundTrue[imgFlag].size(); checkDuplicate++)
					{

						int centerDistanceX = abs((*sendImg1).group_points[imgFlag][pointNumCenter].x - pointGroundTrue[imgFlag][checkDuplicate].x);
						int centerDistanceY = abs((*sendImg1).group_points[imgFlag][pointNumCenter].y - pointGroundTrue[imgFlag][checkDuplicate].y);

						if ((centerDistanceX < maxDistance) && (centerDistanceY < maxDistance)) {

							ifExistPoint = false;
						}
					}
					//记录该群点位置
					if (ifExistPoint) {
						pointGroundTrue[imgFlag].push_back((*sendImg1).group_points[imgFlag][pointNumCenter]);
						defectInformation Xiaci;
						Xiaci.s_cx = (*sendImg1).group_points[imgFlag][pointNumCenter].x;
						Xiaci.s_cy = (*sendImg1).group_points[imgFlag][pointNumCenter].y;



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
						Xiaci.P[2].x = Xiaci.s_cx + int(Xiaci.w / 2 + 2);
						Xiaci.P[2].y = Xiaci.s_cy + int(Xiaci.h / 2 + 2);
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
			(*sendImg1).group_points[x].clear();
			int num_group_point = 0;
			if (sizeof(defectsInfor[cameraFlag][x]) != NULL)    //结构体不为空
			{
				for (int type = 0; type < 1; type++)    //遍历不同检测方法结果
				{
					if (sizeof(defectsInfor[cameraFlag][x][type]) != 0)   //不为空
					{
						for (int i = 0; i < (defectsInfor[cameraFlag][x][type]).size(); i++)    //遍历每一个结果
						{
							if (defectsInfor[cameraFlag][x][type][i].TrueOrFalse == true)
							{
								//point的判定条件
								bool xianzhi_size_point = (defectsInfor[cameraFlag][x][type][i].h < 10 + 19) && (defectsInfor[cameraFlag][x][type][i].w < 10 + 19);    //点的大小限制
								cout << "颜色：" << x << "类型：" << type << "顺序：" << i << endl;
								bool xianzhi_if_group_point = defectsInfor[cameraFlag][x][type][i].numGray > 50;     //判定单点、群点定义
								bool xianzhi_point_bright = (defectsInfor[cameraFlag][x][type][i].bright_num > defectsInfor[cameraFlag][x][type][i].dark_num);  //亮点
								bool xianzhi_point_dark = (defectsInfor[cameraFlag][x][type][i].dark_num > defectsInfor[cameraFlag][x][type][i].bright_num);    //暗点
								//point的第二套参数判定条件
								bool point_2 = (defectsInfor[cameraFlag][x][type][i].numGray >= SecondCanshu._point_num) && (abs(defectsInfor[cameraFlag][x][type][i].dst_m_Gray) > SecondCanshu._point_m_gray);

								//line的判定条件
								bool xianzhi_line_1 = (defectsInfor[cameraFlag][x][type][i].h - 18) > 2 * (defectsInfor[cameraFlag][x][type][i].w - 18);
								bool xianzhi_line_2 = (defectsInfor[cameraFlag][x][type][i].w - 18) > 2 * (defectsInfor[cameraFlag][x][type][i].h - 18);       //满足长宽比关系
								//bool xianzhi_line_3 = abs(defectsInfor[x][type][i].dst_m_Gray) > 1;    //满足对比度关系
								bool xianzhi_line = (xianzhi_line_1 || xianzhi_line_2);
								bool xianzhi_line_bright = defectsInfor[cameraFlag][x][type][i].dst_m_Gray > 0;
								bool xianzhi_line_dark = defectsInfor[cameraFlag][x][type][i].dst_m_Gray < 0;     //判定亮暗
								//line的第二套参数判定条件
								bool line_2 = (defectsInfor[cameraFlag][x][type][i].numGray >= SecondCanshu._line_num) && (abs(defectsInfor[cameraFlag][x][type][i].dst_m_Gray) > SecondCanshu._line_m_gray);

								//mura的判定条件
								bool xianzhi_mura_1 = defectsInfor[cameraFlag][x][type][i].numGray > 500;     //mura满足面积关系
								bool xianzhi_mura_2 = abs(defectsInfor[cameraFlag][x][type][i].dst_m_Gray) < 1;   //满足对比度关系
								bool xianzhi_mura = xianzhi_mura_1 && xianzhi_mura_2;
								//mura的第二套参数判定条件
								bool mura_2 = (defectsInfor[cameraFlag][x][type][i].numGray >= SecondCanshu._mura_num) && (abs(defectsInfor[cameraFlag][x][type][i].dst_m_Gray) > SecondCanshu._mura_m_gray);

								//tuan的判定条件
								bool xianzhi_tuan_bright = (defectsInfor[cameraFlag][x][type][i].bright_num > defectsInfor[cameraFlag][x][type][i].dark_num);
								bool xianzhi_tuan_dark = (defectsInfor[cameraFlag][x][type][i].dark_num > defectsInfor[cameraFlag][x][type][i].bright_num);
								//tuan的第二套参数判定条件
								bool tuan_2 = (defectsInfor[cameraFlag][x][type][i].numGray >= SecondCanshu._tuan_num) && (abs(defectsInfor[cameraFlag][x][type][i].dst_m_Gray) > SecondCanshu._tuan_m_gray);

								if (xianzhi_size_point)
								{
									if (xianzhi_if_group_point)  //判定是否为群点
									{
										//判定亮暗
										if (xianzhi_point_bright)
										{
											y = 0;
											defectsInfor[cameraFlag][x][type][i].type = "b_p";
											if (point_2)
											{
												defectsInfor[cameraFlag][x][type][i].transYorN = true;  //满足第二套参数输出
											}
										}
										else if (xianzhi_point_dark)
										{
											y = 1;
											defectsInfor[cameraFlag][x][type][i].type = "d_p";
											if (point_2)
											{
												defectsInfor[cameraFlag][x][type][i].transYorN = true;  //满足第二套参数输出
											}
										}

									}
									else
									{
										defectsInfor[cameraFlag][x][type][i].type = "unknown";
										defectsInfor[cameraFlag][x][type][i].TrueOrFalse = false;
										//将满足群点定义的坐标写入group_points中，并返回group_points
										group_temp.x = defectsInfor[cameraFlag][x][type][i].s_cx;
										group_temp.y = defectsInfor[cameraFlag][x][type][i].s_cy;
										//x表示颜色，num_group_point表示该图中的第几个
										(*sendImg1).group_points[x].push_back(group_temp);

									}
								}

								else if (xianzhi_line)
								{
									if (xianzhi_line_bright)
									{
										y = 2;
										defectsInfor[cameraFlag][x][type][i].type = "b_l";
										if (line_2)
										{
											defectsInfor[cameraFlag][x][type][i].transYorN = true;
										}
									}
									else if (xianzhi_line_dark)
									{
										y = 3;
										defectsInfor[cameraFlag][x][type][i].type = "d_l";
										if (line_2)
										{
											defectsInfor[cameraFlag][x][type][i].transYorN = true;
										}
									}

								}

								else if (xianzhi_mura)
								{
									y = 4;
									defectsInfor[cameraFlag][x][type][i].type = "mura";

									if (mura_2)
									{
										defectsInfor[cameraFlag][x][type][i].transYorN = true;
									}
								}

								else
								{
									if (xianzhi_tuan_bright)
									{
										y = 5;
										defectsInfor[cameraFlag][x][type][i].type = "b_t";
										if (tuan_2)
										{
											defectsInfor[cameraFlag][x][type][i].transYorN = true;
										}
									}
									else if (xianzhi_tuan_dark)
									{
										y = 6;
										defectsInfor[cameraFlag][x][type][i].type = "d_t";
										if (tuan_2)
										{
											defectsInfor[cameraFlag][x][type][i].transYorN = true;
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

void drawPic()
{
	int h = (*sendImg1).PersPectiveTransformation_hight;
	int w = (*sendImg1).PersPectiveTransformation_weight;
	int judge_h = int(h / 2);
	int judge_w = int(w / 2);

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
							int font = CV_FONT_HERSHEY_SIMPLEX;
							//for (int j = 0; j < 4; j++)
							//{
							//	line((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].P[j], defectsInfor[x][type][i].P[(j + 1) % 4], Scalar(0, 0, 255), 2);
							//}
							rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], defectsInfor[cameraFlag][x][type][i].P[0], defectsInfor[cameraFlag][x][type][i].P[2], (255, 255, 255), 2);
							//cv::putText((*sendImg1).imgTftColorCuts[x], defectsInfor[x][type][i].type, Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 0, 0), 2);
							cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(i + 1), Point(defectsInfor[cameraFlag][x][type][i].s_cx, defectsInfor[cameraFlag][x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);

							//for (int j = 0; j < 4; j++)
							//{
							//	line((*sendImg1).back, defectsInfor[x][type][i].P[j], defectsInfor[x][type][i].P[(j + 1) % 4], Scalar(0, 0, 255), 2);
							//}

							rectangle((*sendImg1).back, defectsInfor[cameraFlag][x][type][i].P[0], defectsInfor[cameraFlag][x][type][i].P[2], (0, 0, 255), 2);

							if (defectsInfor[cameraFlag][x][type][i].transYorN == true)
							{
								Point2f p2[2];
								p2[0].x = defectsInfor[cameraFlag][x][type][i].s_cx - int(defectsInfor[cameraFlag][x][type][i].w / 2 - 2);
								p2[0].y = defectsInfor[cameraFlag][x][type][i].s_cy - int(defectsInfor[cameraFlag][x][type][i].h / 2 - 2);
								p2[1].x = defectsInfor[cameraFlag][x][type][i].s_cx + int(defectsInfor[cameraFlag][x][type][i].w / 2 + 2);
								p2[1].y = defectsInfor[cameraFlag][x][type][i].s_cy + int(defectsInfor[cameraFlag][x][type][i].h / 2 + 2);
								rectangle((*sendImg1).imgTftColorCuts[cameraFlag][x], p2[0], p2[1], (255, 255, 0), 1);
								float m = defectsInfor[cameraFlag][x][type][i].dst_m_Gray;

								//只保留小数点后两位
								m = round(m * 100) / 100.0;
								ostringstream oss;
								oss << m;
								string dst_m_gray;
								dst_m_gray = oss.str();
								//cout << "dst_m_gray:"<<dst_m_gray << endl;
								//cout << 99999 << endl;


								// 判定瑕疵书写的部位
								if (defectsInfor[cameraFlag][x][type][i].s_cx > judge_w)
								{
									// 在第四象限
									if (defectsInfor[cameraFlag][x][type][i].s_cy > judge_h)
									{


										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectsInfor[cameraFlag][x][type][i].type, Point(p2[1].x - 30, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x - 30, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectsInfor[cameraFlag][x][type][i].numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
									//在第二象限
									else
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectsInfor[cameraFlag][x][type][i].type, Point(p2[1].x - 20, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[1].y + 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectsInfor[cameraFlag][x][type][i].numGray), Point(p2[0].x, p2[1].y + 25), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
								}
								else
								{
									//在第三象限
									if (defectsInfor[cameraFlag][x][type][i].s_cy > judge_h)
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectsInfor[cameraFlag][x][type][i].type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[0].x, p2[0].y - 15), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectsInfor[cameraFlag][x][type][i].numGray), Point(p2[0].x - 20, p2[0].y), font, 0.6, cvScalar(255, 255, 0, 1), 2);

									}
									//在第一象限
									else
									{
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], defectsInfor[cameraFlag][x][type][i].type, Point(p2[1].x, p2[0].y), font, 0.6, cvScalar(0, 0, 255, 1), 2);
										//cv::putText((*sendImg1).imgTftColorCuts[x], to_string(i + 1), Point(defectsInfor[x][type][i].s_cx, defectsInfor[x][type][i].s_cy), font, 1.2, (255, 255, 255), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], dst_m_gray, Point(p2[1].x, p2[1].y), font, 0.6, cvScalar(255, 255, 255, 1), 2);
										cv::putText((*sendImg1).imgTftColorCuts[cameraFlag][x], to_string(defectsInfor[cameraFlag][x][type][i].numGray), Point(p2[0].x, p2[1].y + 10), font, 0.6, cvScalar(255, 255, 0, 1), 2);

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
				rectangle((*sendImg1).back, (*sendImg1).P[i][0], (*sendImg1).P[i][1], (0, 0, 255), 2);
			}
		}

		imwrite((*sendImg1).sNowTimeFileDir + "final_result.png", (*sendImg1).back);
	}
}
// 用于九宫格和红绿点输出总的瑕疵统计结果--ysq0123
void get_table_result(vector<Point2i>coordinate, Mat final_img, Mat& res_table)
{
	Point2i p;
	/*
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			p.x = 835 + i * 60;
			p.y = 125 + j * 60;
			circle(res_table, p, 16, Scalar(0, 255, 0), -1);
		}
	}
	rectangle(res_table, Point(984, 1118), Point(1340, 1171), Scalar(0, 255, 0), -1);
	imwrite(".\\result_table\\LCM_res_table.png", res_table);
	*/
	//在相应位置画红点
	for (int i = 0; i < coordinate.size(); i++)
	{
		p.x = coordinate[i].x * 60 + 835;
		p.y = coordinate[i].y * 60 + 125;
		circle(res_table, p, 16, Scalar(0, 0, 255), -1);
	}
	// 有缺陷则总结果处为红色
	if (coordinate.size() > 0)
		rectangle(res_table, Point(984, 1118), Point(1340, 1171), Scalar(0, 0, 255), -1);

	// 左边贴上结果红框图
	Mat dst;
	transpose(final_img, dst);
	flip(dst, dst, 1);
	resize(dst, dst, Size(605, 1130));
	Mat imgROI = res_table(Rect(35, 40, dst.cols, dst.rows));
	dst.copyTo(imgROI);
	line(res_table, Point(238, 38), Point(238, 1174), 0, 3);
	line(res_table, Point(442, 38), Point(442, 1174), 0, 3);
	line(res_table, Point(36, 456), Point(642, 456), 0, 3);
	line(res_table, Point(36, 818), Point(642, 818), 0, 3);
}




//模拟labview输入所有数据进行测试


/*************************************************   debug  模式     ****************************************************************/




//debug模式主函数
void debug()
{

	/**************************   设置输入图路径/轨迹条     ***********************************/
	string configDir;
	//setTestData();  //现已不在这设置图片路径01/05
	//BuildingTemplate();

	setremoveFalseTrackbar();



	/******************  增加camera_flag相机标志位 ---  2019/1/22  ****************************/
	int camera_flag = 0;  // 0表示正拍主相机，1表示灰尘相机，其余2,3,4...（暂定）



	/**************************   构建模板  （模板颜色flag1）   ***********************************/

	// ++++++++++++++++++++++++++  cameraFlag =0 （正拍主相机）


	//构建白屏模板，不同flag表示不同颜色的模板屏幕输入，flag1 = 0表示白屏，至少需要构建白屏模板
	int flag1 = 0;   //模板颜色flag1
	//Mat imgtmp = imread("..\\A4.png");  //white
	Mat imgtmp = imread("..\\white.png");  //white
	camera_cali_fisrt(imgtmp, neican_data, jibian_data, imgtmp);  //相机矫正

	/*输入：Template[flag]  模板图
	输出：CutTemplate[flag] 模板ROI*/
	//SetTemplate(imgtmp, camera_flag, flag1);  // 增加camera_flag


	//构建flag= 1的颜色屏幕模板
	flag1 = 1;
	//Mat imgtmp1 = imread("..\\A4.png");
	Mat imgtmp1 = imread("..\\green.png");
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
	Mat Dust_tmp = imread("..\\dust.png");  //1相机的0颜色即为灰尘模板图
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



	camera_flag = 2;
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);


	camera_flag = 3;
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);



	camera_flag = 4;
	SetTemplate(imgtmp, camera_flag, 0);
	SetTemplate(imgtmp1, camera_flag, 1);
	//SetTemplate(imgtmp1, camera_flag, 2);
	//SetTemplate(imgtmp1, camera_flag, 3);
	//SetTemplate(imgtmp1, camera_flag, 4);
	//SetTemplate(imgtmp1, camera_flag, 5);
	//SetTemplate(imgtmp1, camera_flag, 6);







	/**************************   进行缺陷（梯度）检测   （检测图的颜色flag2）  ***********************************/
	// ++++++++++++++++++++++++++  cameraFlag =0 （正拍主相机）
	long t11 = GetTickCount();
	int flag2 = 0;   // 检测图的颜色flag2
	camera_flag = 0;
	//Mat imgtest = imread("..\\A4.png");
	Mat imgtest = imread("..\\w_h_39.png");

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

	//Mat imgtest1 = imread("..\\test.png");
	//Mat imgtest1 = imread("..\\test1.png");
	//Mat imgtest1 = imread("..\\A4.png");
	Mat imgtest1 = imread("..\\test.png");
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
	Mat imgDust_tmp = imread("..\\dust.png");  //1相机的0颜色即为灰尘模板图
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
	imgDust_tmp = imread("..\\dust.png");  //1相机的0颜色即为灰尘模板图
	camera_cali_fisrt(imgDust_tmp, neican_data, jibian_data, imgDust_tmp);
	//对于【灰尘相机位cameraFlag=1，只获取其白屏的roi即返回】，而【不继续进行白屏的梯度检测】--ysq0123




	camera_flag = 2;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);
	//detect(imgtest1, camera_flag, 2);
	//detect(imgtest1, camera_flag, 3);
	//detect(imgtest1, camera_flag, 4);
	//detect(imgtest1, camera_flag, 5);
	//detect(imgtest1, camera_flag, 6);




	//detect(imgDust_tmp, camera_flag, 5);



	camera_flag = 3;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);

	camera_flag = 4;
	detect(imgtest, camera_flag, 0);
	detect(imgtest1, camera_flag, 1);


	/**************************   灰尘图处理 （增加多相机场景2019/01/05,多相机场景修改0123）   ***********************************/
	long t51 = GetTickCount();

	//Mat imgDust = imread("..\\E4.png");  //灰灰尘图路径
	Mat imgDust = imread("..\\dust.png");  //灰灰尘图路径
	camera_flag = 1;
	camera_cali_fisrt(imgDust, neican_data, jibian_data, imgDust);
	// 灰尘检测多相机场景修改--ysq0123
	processDust(imgDust, configDir, canshuX, 0);  //camera_flag


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
	//repadding((*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).MuraMaskChangeGradientDrawIn, (*sendImg1).shrink_size_mura);
	//initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_louguang[0], (*sendImg1).BorderTFT,
	//	(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);


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
	determine();
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
	get_table_result((*sendImg1).points, (*sendImg1).back, result_table);
	long ttt2 = GetTickCount();
	cout << "result_table函数运行的时间为：" << (ttt2 - ttt1) << endl;

	namedWindow("result_final_show", 0);
	imshow("result_final_show", result_table);


	//on_trackbar(0, sendImg1);
	waitKey(0);
	destroyAllWindows();
	//saveImg();
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

int main()
{




	debug();
	//testReadAndWrite();
	//testDetect();
	return 0;
}

//12.21 wzx 创建模板

