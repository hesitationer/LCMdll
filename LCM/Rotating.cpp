
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
#include "Rotating.h"
#include "creatDir.h"

using namespace std;

//获取图片的角度偏移信息
RotatedRect get_img_rotate_info(Mat& image, bool show_img)
{
	Mat img;
	cvtColor(image, img, CV_BGR2GRAY);
	//高斯平滑
	Mat blurred;
	GaussianBlur(img, blurred, Size(5, 5), 0, 0);
	//二值化
	Mat mask;
	threshold(blurred, mask, 30, 255, CV_THRESH_BINARY);

	//检测物体轮廓
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	double max_area = 100;
	//vector<Rect>boundRect(contours.size());
	//对每个找到的轮廓创建可倾斜的边界框
	//vector<RotatedRect> rect(contours.size());
	//vector<Point2f>boxPts;
	RotatedRect rect;
	Rect boundRect;
	Point centerPoint;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > max_area)
		{

			int w, h;
			max_area = contourArea(contours[i]);
			boundRect = boundingRect(contours[i]);
			rect = minAreaRect(contours[i]);
			centerPoint.x = boundRect.x + cvRound(boundRect.width / 2.0);
			centerPoint.y = boundRect.y + cvRound(boundRect.height / 2.0);
			w = boundRect.width;
			h = boundRect.height;
		}
	}
	if (show_img)
	{
		Mat image_show = image.clone();
		//外接水平矩形--绿色
		rectangle(image_show, centerPoint, boundRect.br(), Scalar(0, 255, 0), 2);
		//最小外接矩形（有角度）--红色
		Point2f rect_points[4]; rect.points(rect_points);
		for (int j = 0; j < 4; j++)
		{
			line(image_show, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 2);
		}
		//drawContours(image, rect[i], 0, Scalar(0,0,255), 2); //此处并不完全匹配python
		namedWindow("before_im_rotate", 0);
		imshow("before_im_rotate", image_show);
	}

	return rect;
}

void img_rotate(Mat& img, Point2f box[4], RotatedRect &rect, Mat &img_out, int cameraFlag, bool show_process, bool returnCutOrNotCutImg)
{
	Mat img_copy = img.clone();
	int rows = img_copy.rows;
	int cols = img_copy.cols;


	//计算手机屏幕区域最小旋转外接矩形信息，包括角度，坐标    
	rect = get_img_rotate_info(img, show_process);

	//(*sendImg1).get_rotate_rect[cameraFlag] = rect;

	// 依据最小外接框信息旋转矫正角度
	Point2f center = rect.center;
	Size2f size = rect.size;
	float angle = rect.angle;
	Mat res;
	if (angle <= -45)
	{
		angle += 90;
	}


	cout << "rotate_angle:" << angle << endl;

	//根据计算的角度进行角度旋转
	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(img_copy, res, M, Size(cols, rows));

	//矫正后的坐标点
	RotatedRect rect_res = get_img_rotate_info(res);
	(*sendImg1).rect_res = rect_res;

	//获取rect_res的四个顶点

	rect_res.points(box);

	//角度矫正结果
	Mat img_crop = res(cv::Rect(box[1].x, box[1].y, box[2].x - box[1].x, box[0].y - box[1].y));
	img_crop.copyTo(img_out);  //返回旋转裁剪后框
	if (returnCutOrNotCutImg == false)  //返回不裁剪原图旋转
		res.copyTo(img_out);


	if (show_process)
	{
		namedWindow("after_im_rotate", 0);
		imshow("after_im_rotate", img_out);
		waitKey(0);
		destroyAllWindows();
	}

}

//  =========================== ysq =================================
void  shrink_poly(Point2f poly[4], vector<Point2f>& new_poly, int size_y)
{
	// 如果采取按长宽的比例收缩
	float x_y_rito = (poly[1].y - poly[2].y) / (poly[3].x - poly[2].x);
	int size_x = (int)size_y*x_y_rito;

	// 如果长宽都按照固定像素值收缩
	size_x = size_y;

	Point2f new_p1(poly[0].x + size_x, poly[0].y - size_y);
	Point2f new_p2(poly[1].x + size_x, poly[1].y + size_y);
	Point2f new_p3(poly[2].x - size_x, poly[2].y + size_y);
	Point2f new_p4(poly[3].x - size_x, poly[3].y - size_y);

	new_poly.push_back(new_p1);
	new_poly.push_back(new_p2);
	new_poly.push_back(new_p3);
	new_poly.push_back(new_p4);
}

void sort_points(vector<Point>&poly, map<string, vector<Point>>&final_poly, vector<int>&dxy)
{
	//寻找坐标点的极值
	int x_max = 0, y_max = 0, x_min = 1000, y_min = 1000, x_mid, y_mid;
	for (int i = 0; i < poly.size(); i++)
	{
		if (poly[i].x > x_max)
			x_max = poly[i].x;
		if (poly[i].y > y_max)
			y_max = poly[i].y;
		if (poly[i].x < x_min)
			x_min = poly[i].x;
		if (poly[i].y < y_min)
			y_min = poly[i].y;
	}
	x_mid = (x_max + x_min) / 2;
	y_mid = (y_max + y_min) / 2;

	int x_dif, y_dif;
	x_dif = (x_max - x_min);
	y_dif = (y_max - y_min);


	// 对坐标点划分四个区域
	vector<Point> polyx = poly;
	vector<Point> polyy = poly;
	//确定顺时针顺序
	//final_poly["x_min"].push_back(Point(0,0));
	//final_poly["x_min"].pop_back();
	//final_poly["y_min"].push_back(Point(0, 0));
	//final_poly["y_min"].pop_back();
	//final_poly["x_max"].push_back(Point(0, 0));
	//final_poly["x_max"].pop_back();
	//final_poly["y_max"].push_back(Point(0, 0));
	//final_poly["y_max"].pop_back();
	//无法调整map的键顺序，在我们插入<key, value>键值对时，就会默认按照key的大小顺序进行存储
	//故默认顺序为x_max,x_min,y_max,y_min.

	vector<int>y_min_abs, y_max_abs, x_min_abs, x_max_abs;  //20190219

	//针对y轴方向
	for (int j = 0; j < polyy.size(); j++)
	{
		if (polyy[j].y < y_min + y_dif / 3)
		{
			y_min_abs.push_back(abs(polyy[j].y - y_min));

			//cout << "y_min_abs" << abs(polyy[j].y - y_min) << endl;
			if (abs(polyy[j].y - y_min) < dxy[0]) //70  15
			{
				final_poly["y_min"].push_back(polyy[j]);
				if (abs(polyy[j].x - x_max) < 3)
					continue;
				if (abs(polyy[j].x - x_min) < 3)
					continue;
				polyx.erase(remove(polyx.begin(), polyx.end(), polyy[j]), polyx.end());
			}
		}
		else if (polyy[j].y > y_max - y_dif / 3)
		{
			y_max_abs.push_back(abs(polyy[j].y - y_max));

			if (abs(polyy[j].y - y_max) < dxy[1])  //32,72 12
			{
				//cout << "y_max_abs" << abs(polyy[j].y - y_max) << endl;
				final_poly["y_max"].push_back(polyy[j]);
				if (abs(polyy[j].x - x_max) < 3)
					continue;
				if (abs(polyy[j].x - x_min) < 3)
					continue;
				polyx.erase(remove(polyx.begin(), polyx.end(), polyy[j]), polyx.end());
			}
		}
	}
	//针对x轴方向
	for (int j = 0; j < polyx.size(); j++)
	{
		if (polyx[j].x < x_min + x_dif / 3)
		{
			x_min_abs.push_back(abs(polyx[j].x - x_min));

			//cout << "x_min_abs" << abs(polyx[j].x - x_min) << endl;
			if (abs(polyx[j].x - x_min) < dxy[2])  //100 50  20
				final_poly["x_min"].push_back(polyx[j]);
		}
		else if (polyx[j].x > x_max - x_dif / 3)
		{
			x_max_abs.push_back(abs(polyx[j].x - x_max));

			//cout << "x_max_abs" << abs(polyx[j].x - x_max) << endl;
			if (abs(polyx[j].x - x_max) < dxy[3])  // 200  20
				final_poly["x_max"].push_back(polyx[j]);
		}
	}

	//2019-02-19-ysq 自己计算阈值，尝试自适应
	// 排序
	sort(x_min_abs.begin(), x_min_abs.end());
	sort(y_min_abs.begin(), y_min_abs.end());
	sort(x_max_abs.begin(), x_max_abs.end());
	sort(y_max_abs.begin(), y_max_abs.end());
	// 计算分类阈值
	int s1, s2, s3, s4, m1, m2, m3, m4 = 0; 0; 0; 0; 0; 0; 0; 0;
	s1 = 0;
	s2 = 0;
	s3 = 0;
	s4 = 0;
	cout << "y_min_abs:" << endl;
	for (int n = 0; n < y_min_abs.size(); n++)
	{
		cout << y_min_abs[n] << endl;
		if (n < y_min_abs.size() * 4 / 5)
		{
			s1 += y_min_abs[n];
			m1 = s1 / (n + 1);
		}
	}

	cout << "y_max_abs:" << endl;
	for (int n = 0; n < y_max_abs.size(); n++)
	{
		cout << y_max_abs[n] << endl;
		if (n < y_max_abs.size() * 4 / 5)
		{
			s2 += y_max_abs[n];
			m2 = s2 / (n + 1);
		}
	}

	cout << "x_min_abs:" << endl;
	int index = 0;
	for (int n = 0; n < x_min_abs.size(); n++)
	{
		cout << x_min_abs[n] << endl;
		if (n > 1 && (x_min_abs[n] - x_min_abs[n - 1] > 20))
			index = n;
	}
	for (int n = 0; n <= index; n++)
		s3 += x_min_abs[n];
	m3 = s3 / index;

	cout << "x_max_abs:" << endl;
	int k = 0;
	for (int n = 0; n < x_max_abs.size(); n++)
	{
		cout << x_max_abs[n] << endl;
		k++;
	}
	m4 = x_max_abs[k - 1];
	cout << "1:" << endl;
	cout << m1 << endl;
	cout << "2:" << endl;
	cout << m2 << endl;
	cout << "3:" << endl;
	cout << m3 << endl;
	cout << "4:" << endl;
	cout << m4 << endl;
}


//  2019-02-19ysq修改 用于自适应计算都需要，而无需每次都手动输入
void sort_points1(vector<Point>&poly, map<string, vector<Point>>&final_poly, vector<int>&dxy)
{
	//寻找坐标点的极值
	int x_max = 0, y_max = 0, x_min = 1000, y_min = 1000, x_mid, y_mid;
	for (int i = 0; i < poly.size(); i++)
	{
		if (poly[i].x > x_max)
			x_max = poly[i].x;
		if (poly[i].y > y_max)
			y_max = poly[i].y;
		if (poly[i].x < x_min)
			x_min = poly[i].x;
		if (poly[i].y < y_min)
			y_min = poly[i].y;
	}
	x_mid = (x_max + x_min) / 2;
	y_mid = (y_max + y_min) / 2;

	int x_dif, y_dif;
	x_dif = (x_max - x_min);
	y_dif = (y_max - y_min);


	// 对坐标点划分四个区域
	vector<Point> polyx = poly;
	vector<Point> polyy = poly;
	//确定顺时针顺序
	//final_poly["x_min"].push_back(Point(0,0));
	//final_poly["x_min"].pop_back();
	//final_poly["y_min"].push_back(Point(0, 0));
	//final_poly["y_min"].pop_back();
	//final_poly["x_max"].push_back(Point(0, 0));
	//final_poly["x_max"].pop_back();
	//final_poly["y_max"].push_back(Point(0, 0));
	//final_poly["y_max"].pop_back();
	//无法调整map的键顺序，在我们插入<key, value>键值对时，就会默认按照key的大小顺序进行存储
	//故默认顺序为x_max,x_min,y_max,y_min.

	vector<int>y_min_abs, y_max_abs, x_min_abs, x_max_abs;  //20190219

	//针对y轴方向
	for (int j = 0; j < polyy.size(); j++)
	{
		if (polyy[j].y < y_min + y_dif / 4)
		{
			y_min_abs.push_back(abs(polyy[j].y - y_min));
		}
		else if (polyy[j].y > y_max - y_dif / 4)
		{
			y_max_abs.push_back(abs(polyy[j].y - y_max));

		}
	}
	//针对x轴方向
	for (int j = 0; j < polyx.size(); j++)
	{
		if ((polyx[j].x < x_min + x_dif / 3) && (abs(polyx[j].y - y_min) > 10) && (abs(polyx[j].y - y_max) > 10))
		{
			x_min_abs.push_back(abs(polyx[j].x - x_min));
		}
		else if ((polyx[j].x > x_max - x_dif / 3) && (abs(polyx[j].y - y_min) > 10) && (abs(polyx[j].y - y_max) > 10))
		{
			x_max_abs.push_back(abs(polyx[j].x - x_max));
		}
	}

	//2019-02-19-ysq 自己计算阈值，尝试自适应
	// 排序
	sort(x_min_abs.begin(), x_min_abs.end());
	sort(y_min_abs.begin(), y_min_abs.end());
	sort(x_max_abs.begin(), x_max_abs.end());
	sort(y_max_abs.begin(), y_max_abs.end());
	// 计算分类阈值
	int s1, s2, s3, s4, m1, m2, m3, m4 = 0; 0; 0; 0; 0; 0; 0; 0;
	s1 = 0;
	s2 = 0;
	s3 = 0;
	s4 = 0;
	cout << "y_min_abs:" << y_min_abs.size() << endl;
	for (int n = 0; n < y_min_abs.size(); n++)
	{
		cout << y_min_abs[n] << endl;
		if (n < y_min_abs.size() * 4 / 5)
		{
			s1 += y_min_abs[n];
			m1 = s1 / (n + 1);
		}
	}

	cout << "y_max_abs:" << endl;
	for (int n = 0; n < y_max_abs.size(); n++)
	{
		cout << y_max_abs[n] << endl;
		if (n < y_max_abs.size() * 7 / 8)
		{
			s2 += y_max_abs[n];
			m2 = s2 / (n + 1);
		}
	}

	cout << "x_min_abs:" << endl;
	int index = 0;
	for (int n = 0; n < x_min_abs.size(); n++)
	{
		cout << x_min_abs[n] << endl;
		if (n < x_min_abs.size() * 5 / 6)
		{
			s3 += x_min_abs[n];
			m3 = s3 / (n + 1);
		}
		//if (index > 0)
		//	continue;
		//if (n > 1 && (x_min_abs[n] - x_min_abs[n - 1] > 30))  //这里出现唯一与角度相关的参--该阈值
		//	index = n-1;
	}
	//for (int n = 0; n <= index; n++)
	//	s3 += x_min_abs[n];
	//m3 = s3 / (index+1);

	// 针对没刘海的屏幕，x_min取值有变化
	bool no_liuhai = true;
	int l = x_min_abs.size();
	if (no_liuhai)
		m3 = (x_min_abs[l - 1] + x_min_abs[l - 2]) / 2;

	cout << "x_max_abs:" << endl;
	int k = 0;
	for (int n = 0; n < x_max_abs.size(); n++)
	{
		cout << x_max_abs[n] << endl;
		k++;
	}
	m1 += 5;
	m2 += 1;
	m3 += 1;
	m4 += 5;
	m4 = (x_max_abs[k - 1] + x_max_abs[k - 2] + x_max_abs[k - 3]) / 3;
	cout << "1:" << endl;
	cout << m1 << endl;
	cout << "2:" << endl;
	cout << m2 << endl;
	cout << "3:" << endl;
	cout << m3 << endl;
	cout << "4:" << endl;
	cout << m4 << endl;


	for (int j = 0; j < polyy.size(); j++)
	{
		if (polyy[j].y < y_min + y_dif / 3)
		{
			//cout << "y_min_abs" << abs(polyy[j].y - y_min) << endl;
			if (abs(polyy[j].y - y_min) < m1) //70  15
			{
				final_poly["y_min"].push_back(polyy[j]);
				if (abs(polyy[j].x - x_max) < 3)
					continue;
				if (abs(polyy[j].x - x_min) < 3)
					continue;
				polyx.erase(remove(polyx.begin(), polyx.end(), polyy[j]), polyx.end());
			}
		}
		else if (polyy[j].y > y_max - y_dif / 3)
		{
			if (abs(polyy[j].y - y_max) < m2)  //32,72 12
			{
				//cout << "y_max_abs" << abs(polyy[j].y - y_max) << endl;
				final_poly["y_max"].push_back(polyy[j]);
				if (abs(polyy[j].x - x_max) < 3)
					continue;
				if (abs(polyy[j].x - x_min) < 3)
					continue;
				polyx.erase(remove(polyx.begin(), polyx.end(), polyy[j]), polyx.end());
			}
		}
	}
	//针对x轴方向
	for (int j = 0; j < polyx.size(); j++)
	{
		if (polyx[j].x < x_min + x_dif / 3)
		{
			//cout << "x_min_abs" << abs(polyx[j].x - x_min) << endl;
			if (abs(polyx[j].x - x_min) < m3)  //100 50  20
				final_poly["x_min"].push_back(polyx[j]);
		}
		else if (polyx[j].x > x_max - x_dif / 3)
		{
			//cout << "x_max_abs" << abs(polyx[j].x - x_max) << endl;
			if (abs(polyx[j].x - x_max) < m4)  // 200  20
				final_poly["x_max"].push_back(polyx[j]);
		}
	}


}


void cross_point(Vec4i line1, Vec4i line2, Point2f& res_p)
{
	int x1, y1, x2, y2;
	x1 = line1[0];
	y1 = line1[1];
	x2 = line1[2];
	y2 = line1[3];
	int x3, y3, x4, y4;
	x3 = line2[0];
	y3 = line2[1];
	x4 = line2[2];
	y4 = line2[3];
	double k1, k2, b1, b2;
	//若 L1斜率不存在
	if (abs(x2 - x1) < 2)
	{
		k1 = 9999.0;
		b1 = 0.0;
	}
	else
	{
		k1 = (y2 - y1)*1.0 / (x2 - x1);
		b1 = y1 * 1.0 - x1 * k1*1.0;
	}
	// 若L2斜率不存在
	if (abs(x4 - x3) < 2)
	{
		k2 = 9999.0;
		b2 = 0.0;
	}
	else
	{
		k2 = (y4 - y3)*1.0 / (x4 - x3);
		b2 = y3 * 1.0 - x3 * k2*1.0;
	}

	if (k1 == 9999.0)
	{
		res_p.x = x1;
		res_p.y = k2 * res_p.x + b2;
	}
	else if (k2 == 9999.0)
	{
		res_p.x = x3;
		res_p.y = k1 * res_p.x + b1;
	}
	else
	{
		res_p.x = (b2 - b1) / (k1 - k2);
		res_p.y = k1 * res_p.x + b1;
	}
}

void hough_detect1(Mat& image, vector<Point2f>& res_points, vector<int>sort_dxy, int binary_thres, bool show_img)
{
	long th1 = GetTickCount();

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
													  // Mat tmp_contour = Mat::zeros(Size(rows, cols), CV_8UC1);
	drawContours(tmp_contour, contours, -1, Scalar(255), 2);

	long th2 = GetTickCount();

	if (show_img == true)
	{
		namedWindow("binary", 0);
		imshow("binary", binary);
		namedWindow("c", 0);
		imshow("c", tmp_contour);
		waitKey(0);
	}


	/***********test*******************/
	//Mat img_test = image.clone();
	//drawContours(img_test, contours, -1, Scalar(0,0,255), 1);
	//namedWindow("cont", 0);
	//imshow("cont", img_test);
	//imwrite("D:\\project\\img_test.bmp", img_test);
	/***********test*******************/




	//取canny边界或者轮廓边界
	Mat edge = tmp_contour.clone();
	//Canny(blurred, edge, 2, 50, 3);

	/* 累积概率霍夫直线变换*/
	vector<Vec4i>lines;
	//////////11.26改了霍夫变换的参数/////////
	HoughLinesP(edge, lines, 1, CV_PI / 180, 60, 100, 80);  // 0.75 0.68   120   100 00

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

	//namedWindow("lines", 0);
	//imshow("lines", img_draw);

	//将检测的线段的端点画出来
	Mat tmp_points(rows, cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < cnt.size(); i++)
	{
		circle(tmp_points, cnt[i], 5, Scalar(255, 255, 255), -1);
	}

	//namedWindow("points", 0);
	//imshow("points", tmp_points);

	long th3 = GetTickCount();

	/*============ 筛选分类这这些点为四类 ============*/
	Mat tmp_lines(rows, cols, CV_8UC1, Scalar(0));
	map<string, vector<Point>>classfied_point;  //对应python中的字典
	map<string, vector<Point>>::iterator iter;
	//cout << "cnt:" << endl;
	//cout << cnt << endl;
	//sort_points(cnt, classfied_point, sort_dxy);
	sort_points1(cnt, classfied_point, sort_dxy);

	//cout << "classfied_point:" << endl;

	long th4 = GetTickCount();

	//四类的点分别求拟合直线
	vector<Vec4i>final_points;
	for (iter = classfied_point.begin(); iter != classfied_point.end(); iter++)
	{
		//cout << iter->first << ' ' << iter->second << endl;
		vector<Point>pxy = iter->second;
		for (int i = 0; i < pxy.size(); i++)
			//在黑色背景上画出拟合点
			circle(tmp_lines, pxy[i], 5, Scalar(255, 255, 255), -1);
		if (show_img)
		{
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

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
		if (show_img)
		{
			//在黑色背景上画出拟合直线
			line(tmp_lines, point1, point2, Scalar(255, 255, 255), 2);
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

	}
	//for (int a = 0; a < final_points.size(); a++)
	//cout << "final_points" << final_points[a] << endl;
	// 求直线交点，并返回结果
	//classfied_point中键的默认顺序为x_max,x_min,y_max,y_min.
	//故这里和python的顺时针不一样，需要改顺序
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



	vector<Point2f> res_points_tmp;
	res_points_tmp.push_back(p1);
	res_points_tmp.push_back(p2);
	res_points_tmp.push_back(p3);
	res_points_tmp.push_back(p4);
	res_points = res_points_tmp;

	line(img_draw, p1, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p4, Scalar(255, 255, 0), 1);
	line(img_draw, p1, p4, Scalar(255, 255, 0), 1);

	long th5 = GetTickCount();
	cout << "huf轮廓函数运行的时间为：" << (th2 - th1) << endl;
	cout << "huf检测函数运行的时间为：" << (th3 - th2) << endl;
	cout << "sort point函数运行的时间为：" << (th4 - th3) << endl;
	cout << "直线拟合函数运行的时间为：" << (th5 - th4) << endl;


	if (show_img)
	{
		namedWindow("tmp_points", 0);
		imshow("tmp_points", tmp_points);
		namedWindow("img_res_draw", 0);
		imshow("img_res_draw", img_draw);
		imwrite("D:\\project\\img_draw.bmp", img_draw);
		waitKey(0);
		destroyAllWindows();
	}

}

//0301多相机版本
void hough_detect(Mat& image, vector<Point2f>& res_points, vector<int>sort_dxy, int binary_thres, bool show_img)
{
	long th1 = GetTickCount();

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
	//threshold(blurred, binary, binary_thres, 255, CV_THRESH_OTSU);

	//检测物体轮廓
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//在黑背景上画出轮廓
	Mat tmp_contour(rows, cols, CV_8UC1, Scalar(0));  //8位无符号char型单通道
													  // Mat tmp_contour = Mat::zeros(Size(rows, cols), CV_8UC1);
	drawContours(tmp_contour, contours, -1, Scalar(255), 2);

	long th2 = GetTickCount();

	if (show_img == true)
	{
		namedWindow("binary", 0);
		imshow("binary", binary);
		namedWindow("c", 0);
		imshow("c", tmp_contour);
		waitKey(0);
	}


	/***********test*******************/
	//Mat img_test = image.clone();
	//drawContours(img_test, contours, -1, Scalar(0,0,255), 1);
	//namedWindow("cont", 0);
	//imshow("cont", img_test);
	//imwrite("D:\\project\\img_test.bmp", img_test);
	/***********test*******************/




	//取canny边界或者轮廓边界
	Mat edge = tmp_contour.clone();
	//Canny(blurred, edge, 2, 50, 3);

	/* 累积概率霍夫直线变换*/
	vector<Vec4i>lines;
	//////////11.26改了霍夫变换的参数/////////
	HoughLinesP(edge, lines, 1, CV_PI / 180, (*canshuX)._houghP[0], (*canshuX)._houghP[1], (*canshuX)._houghP[2]);  //  0.68   120   100 00

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

	//namedWindow("lines", 0);
	//imshow("lines", img_draw);

	//将检测的线段的端点画出来
	Mat tmp_points(rows, cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < cnt.size(); i++)
	{
		circle(tmp_points, cnt[i], 5, Scalar(255, 255, 255), -1);
	}

	//namedWindow("points", 0);
	//imshow("points", tmp_points);

	long th3 = GetTickCount();

	/*============ 筛选分类这这些点为四类 ============*/
	Mat tmp_lines(rows, cols, CV_8UC1, Scalar(0));
	map<string, vector<Point>>classfied_point;  //对应python中的字典
	map<string, vector<Point>>::iterator iter;
	//cout << "cnt:" << endl;
	//cout << cnt << endl;

	//sort_points(cnt, classfied_point, sort_dxy);
	sort_points1(cnt, classfied_point, sort_dxy);  //20190227
	//cout << "classfied_point:" << endl;

	long th4 = GetTickCount();

	//四类的点分别求拟合直线
	vector<Vec4i>final_points;
	for (iter = classfied_point.begin(); iter != classfied_point.end(); iter++)
	{
		//cout << iter->first << ' ' << iter->second << endl;
		vector<Point>pxy = iter->second;
		for (int i = 0; i < pxy.size(); i++)
			//在黑色背景上画出拟合点
			circle(tmp_lines, pxy[i], 5, Scalar(255, 255, 255), -1);
		if (show_img)
		{
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

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
		if (show_img)
		{
			//在黑色背景上画出拟合直线
			line(tmp_lines, point1, point2, Scalar(255, 255, 255), 2);
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

	}
	//for (int a = 0; a < final_points.size(); a++)
	//cout << "final_points" << final_points[a] << endl;
	// 求直线交点，并返回结果
	//classfied_point中键的默认顺序为x_max,x_min,y_max,y_min.
	//故这里和python的顺时针不一样，需要改顺序
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



	vector<Point2f> res_points_tmp;
	res_points_tmp.push_back(p1);
	res_points_tmp.push_back(p2);
	res_points_tmp.push_back(p3);
	res_points_tmp.push_back(p4);
	res_points = res_points_tmp;

	line(img_draw, p1, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p2, Scalar(255, 255, 0), 1);
	line(img_draw, p3, p4, Scalar(255, 255, 0), 1);
	line(img_draw, p1, p4, Scalar(255, 255, 0), 1);

	long th5 = GetTickCount();
	cout << "huf轮廓函数运行的时间为：" << (th2 - th1) << endl;
	cout << "huf检测函数运行的时间为：" << (th3 - th2) << endl;
	cout << "sort point函数运行的时间为：" << (th4 - th3) << endl;
	cout << "直线拟合函数运行的时间为：" << (th5 - th4) << endl;


	if (show_img)
	{
		namedWindow("tmp_points", 0);
		imshow("tmp_points", tmp_points);
		namedWindow("img_res_draw", 0);
		imshow("img_res_draw", img_draw);
		imwrite("D:\\project\\img_draw.bmp", img_draw);
		waitKey(0);
		destroyAllWindows();
	}

}

//用来获取白屏IOU区域信息
void getRoi(int cameraFlag, bool isBuildtemplate, bool if_showimg)
{
	Mat imgTFT = (*sendImg1).imgTFT[cameraFlag].clone();

	/*获取旋转结果*/
	RotatedRect rect;
	Mat dst;
	Point2f box[4];

	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/
	long tr1 = GetTickCount();

	//1.对图片进行角度矫正
	img_rotate(imgTFT, box, rect, (*sendImg1).imgTftColorCut[cameraFlag], cameraFlag, false, false);
	//Mat imgTFT_out_copy = imgTFT_out.clone();
	//imwrite("D:\\project\\bluetmpimgTftColorCut.png", (*sendImg1).imgTftColorCut);

	long tr2 = GetTickCount();


	//2.计算角度矫正之后手机屏屏幕的四个顶点
	//pts1 = np.float32([[0, 0], [1130, 0], [1130, 2280], [0, 2280]])

	vector<int>sort_dxy;  //12.29修改sort_point带参数
	switch (cameraFlag) // 对于【不同相机采取不同的霍夫检测参数】 --0123
	{
	default:
		cout << "default:" << (*canshuX)._sort_dxy1 << endl;
		sort_dxy.push_back(15);
		sort_dxy.push_back(12);
		sort_dxy.push_back(20);
		sort_dxy.push_back(20);
		hough_detect((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).pts1[cameraFlag], sort_dxy, (*canshuX)._houghthr[cameraFlag][0], if_showimg);  //100
		break;
	case 0:
		sort_dxy.push_back((*canshuX)._sort_dxy1[cameraFlag][0]);
		sort_dxy.push_back((*canshuX)._sort_dxy2[cameraFlag][0]);
		sort_dxy.push_back((*canshuX)._sort_dxy3[cameraFlag][0]);
		sort_dxy.push_back((*canshuX)._sort_dxy4[cameraFlag][0]);
		hough_detect((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).pts1[cameraFlag], sort_dxy, (*canshuX)._houghthr[cameraFlag][0], if_showimg);   //100
		break;
		//case 1:
		//	//vector<int>sort_dxy_dust;  //12.29修改sort_point带参数
		//	sort_dxy.push_back(210);
		//	sort_dxy.push_back(20);
		//	sort_dxy.push_back(80);
		//	sort_dxy.push_back(100);
		//	hough_detect((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).pts1[cameraFlag], sort_dxy, 50, false);
		//	break;
	}

	//imwrite("D:\\project\\bluetmpimgTftColorCut2.png", (*sendImg1).imgTftColorCut);

	long tr3 = GetTickCount();
	cout << "ROI旋转运行的时间为：" << (tr2 - tr1) << endl;
	cout << "ROI霍夫变换运行的时间为：" << (tr3 - tr2) << endl;


	//imwrite("D://project//test.bmp", (*sendImg1).imgTFT);

	//imwrite("D:\\project\\imgTFT_out.bmp", imgTFT_out);
	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/


	//保存旋转过程的参数
	//(*sendImg1).rect[cameraFlag] = rect;
	(*sendImg1).get_rotate_rect[cameraFlag] = rect;


	//保存霍夫变换后的参数
	for (int ptrMove = 0; ptrMove < 4; ptrMove++)
		(*sendImg1).box[cameraFlag][ptrMove] = box[ptrMove];




	if (isBuildtemplate == true)
	{
		//如果是模板，【且是0相机1/23】则保存相关pts1参数,然后进行透视变换
		if (cameraFlag == 0)
		{
			// 保存模板图的四个点坐标于txt文件中，用于白屏图检测时的对比
			ofstream OutFile((*sendImg1).pts1_tmp_file); //利用构造函数创建txt文本，并且打开该文本
			for (int i = 0; i < 4; i++)
			{
				OutFile << (*sendImg1).pts1[cameraFlag][i].x; //把字符串内容"This is a Test!"，写入Test.txt文件
				OutFile << "\n";
				OutFile << (*sendImg1).pts1[cameraFlag][i].y; //把字符串内容"This is a Test!"，写入Test.txt文件
				OutFile << "\n";
			}
			OutFile.close();//关闭Test.txt文件

			//保存相关参数   2315*1115  透视变换的大小由每次的白屏模板图确定，【且固定为0相机的--2019/01/22】
			(*sendImg1).PersPectiveTransformation_hight = (*sendImg1).pts1[0][2].y - (*sendImg1).pts1[0][0].y;
			(*sendImg1).PersPectiveTransformation_weight = (*sendImg1).pts1[0][1].x - (*sendImg1).pts1[0][0].x;
		}

		vector<Point2f> pts2;
		pts2.push_back(Point2f(0, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		pts2.push_back(Point2f(0, (*sendImg1).PersPectiveTransformation_hight));
		(*sendImg1).WhiteScreenQuadrilateral = pts2;

		//生成透视变换矩阵
		Mat M = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);


		//进行透视变换
		warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		//cout<<"PersPectiveTransformation_weight:"<<(*sendImg1).PersPectiveTransformation_weight<<endl;
		//cout<<"PersPectiveTransformation_hight:"<<(*sendImg1).PersPectiveTransformation_hight<<endl;
		imwrite("..\\ColorCut.png", (*sendImg1).imgTftColorCut[cameraFlag]);
		//namedWindow("beforeresize", 0);
		//imshow("beforeresize",(*sendImg1).imgTftColorCut );


	}

	else {  //如果不是模板，则对【只0相机--01/23】检测到的四个角点坐标进行与模板的角点匹配检测，然后进行透视变换

		if (cameraFlag == 0)
		{
			// 读取模板的四个角点信息txt文件
			string line;
			Point2f p;
			int f = 0;

			ifstream InFile((*sendImg1).pts1_tmp_file);;
			if (InFile) // 有该文件  
			{
				//getline逐行读入
				while (getline(InFile, line)) // line中不包括每行的换行符  
				{
					//cout << line << endl;
					if (f == 0)
					{
						p.x = atof(line.c_str());
						f++;
					}
					else
					{
						p.y = atof(line.c_str());
						f = 0;
						(*sendImg1).pts1_tmp.push_back(p);
					}
				}
			}
			else // 没有该文件  
			{
				//throw "no pts1_tmp_file !!";

				int x = MessageBox(GetForegroundWindow(), "pts1_tmp_file读取失败！！", "", MB_OKCANCEL); //一个确定按钮，一个取消按钮
				if (x == 2)//按取消按钮
					exit(0);
			}


			//将白屏检测到的原生图的四个角点pts1与模板的进行比较 
			//差别太大就报错
			for (int i = 0; i < 4; i++)
			{
				Point2f p_img = (*sendImg1).pts1[cameraFlag][i];
				Point2f p_tmp = (*sendImg1).pts1_tmp[i];
				if (abs(p_img.x - p_tmp.x) > 50 || abs(p_img.y - p_tmp.y) > 50)
				{
					//throw "img get wrong edges!!";
					int x = MessageBox(GetForegroundWindow(), "图片边界获取异常！！", "", MB_OKCANCEL); //一个确定按钮，一个取消按钮
					if (x == 2)//按取消按钮
						exit(0);
				}

			}
		}


		vector<Point2f> pts2;
		pts2.push_back(Point2f(0, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		pts2.push_back(Point2f(0, (*sendImg1).PersPectiveTransformation_hight));
		(*sendImg1).WhiteScreenQuadrilateral = pts2;
		//


		//生成透视变换矩阵
		Mat M = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);




		//进行透视变换
		warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));

		long tr4 = GetTickCount();
		cout << "ROI透视变换运行的时间为：" << (tr4 - tr3) << endl;

		//imwrite(".\\2camera-grid\\grid40_up_res.png", (*sendImg1).imgTftColorCut);

		//namedWindow("beforeresize", 0);
		//imshow("beforeresize",(*sendImg1).imgTftColorCut );

		//waitKey(0);
		//resize((*sendImg1).imgTftColorCut, (*sendImg1).imgTftColorCut, Size((*sendImg1).PersPectiveTransformation_hight, (*sendImg1).PersPectiveTransformation_weight));


	}


}

vector<Point2f> getRoi1(int cameraFlag)
{
	Mat imgTFT = (*sendImg1).imgTFT[cameraFlag].clone();//白屏拍照
	/*获取旋转结果*/
	RotatedRect rect;
	Mat dst;
	Point2f box[4];
	Mat imgTFT_out;
	//img_rotate(imgTFT, res_path, box, rect, imgTFT_out);



	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/
	img_rotate(imgTFT, box, rect, imgTFT_out, false, false);
	Mat imgTFT_out_copy = imgTFT_out.clone();
	//变换后分别在左上、右上、右下、左下四个点
	//pts2 = np.float32([[0, 0], [1130, 0], [1130, 2280], [0, 2280]])
	vector<Point2f> pts2222;
	vector<int>sort_dxy2;  //12.29修改sort_point带参数
	sort_dxy2.push_back(15);
	sort_dxy2.push_back(12);
	sort_dxy2.push_back(20);
	sort_dxy2.push_back(20);
	hough_detect(imgTFT_out, pts2222, sort_dxy2, false);
	(*sendImg1).pts1[cameraFlag] = pts2222;

	//imwrite("D://project//test.bmp", (*sendImg1).imgTFT);

	vector<Point2f> pts2;
	pts2.push_back(Point2f(0, 0));
	pts2.push_back(Point2f(1115, 0));
	pts2.push_back(Point2f(1115, 2315));
	pts2.push_back(Point2f(0, 2315));

	//生成透视变换矩阵
	Mat M = getPerspectiveTransform(pts2222, pts2);
	//进行透视变换
	warpPerspective(imgTFT_out_copy, imgTFT_out, M, Size(1115, 2315));
	resize(imgTFT_out, imgTFT_out, Size(2315, 1115));
	cout << "imgTFT_out.rows" << imgTFT_out.rows << "imgTFT_out.cols" << imgTFT_out.cols << endl;
	//imwrite("D:\\project\\imgTFT_out.bmp", imgTFT_out);
	/*************  仿射变换，改善左小右大的问题  2018/11/22******************/



	(*sendImg1).get_rotate_rect[cameraFlag] = rect;

	for (int ptrMove = 0; ptrMove < 4; ptrMove++)
	{
		(*sendImg1).box[cameraFlag][ptrMove] = box[ptrMove];

	}
	//roi区域边界各自内缩10个像素
	int shrink_size = (*sendImg1).shrink_size;
	int w_TFT, h_TFT;
	w_TFT = imgTFT_out.rows;
	h_TFT = imgTFT_out.cols;
	//imgTFT = imgTFT_out(Range(shrink_size, w_TFT - shrink_size), Range(shrink_size, h_TFT - shrink_size));
	//只內缩左、右和上
	imwrite("D:\\project\\imgTFT_formal.bmp", imgTFT_out);
	imgTFT = imgTFT_out(Range(shrink_size, w_TFT - 2), Range(shrink_size, h_TFT - shrink_size));
	cout << "imgTFT.rows" << imgTFT.rows << "imgTFT.cols" << imgTFT.cols << endl;
	imwrite("D:\\project\\imgTFT_shrink.bmp", imgTFT);

	//TFT矫正裁剪后结果
	(*sendImg1).imgTftColorCut[cameraFlag] = imgTFT.clone();

	//获取图片的长宽

	(*sendImg1).Max_y = (*sendImg1).imgTftColorCut[cameraFlag].rows;
	(*sendImg1).Max_x = (*sendImg1).imgTftColorCut[cameraFlag].cols;
	//获取iou区域




	return pts2222;
}

// 相机畸变矫正代码，用在所有输入图的第一步处理
/*
float neican_data[] = { 124046.170922198990,0,1672.8744383824846000,0, 123576.1775990077400,1436.80076020515030,0,0,1 };
float jibian_data[] = { -115.529366790098550, -13.3238477842862260, -0.0822106292981709260,0.00036432147392857344,-0.00288576837799785390 };
Mat newimage = imageSource.clone(); //校正后输出图片
*/

//R角检测
//R角定位，输入坐标为霍夫变换检测到的角点
//R角检测
//R角定位，输入坐标为霍夫变换检测到的角点


//获取图片的R角信息
void find_R_cornor(Mat pic, vector<Point2f> pt, int size, string path, vector<string> &res, bool show_img = false)
{
	Mat pic_show = pic.clone();
	Mat im;
	cvtColor(pic, im, CV_BGR2GRAY);
	Mat blurred;
	GaussianBlur(im, blurred, Size(5, 5), 0, 0);
	Mat binary;
	threshold(blurred, binary, 50, 255, CV_THRESH_BINARY);

	float x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = pt[0].x;
	y1 = pt[0].y;
	x2 = pt[1].x;
	y2 = pt[1].y;
	x3 = pt[2].x;
	y3 = pt[2].y;
	x4 = pt[3].x;
	y4 = pt[3].y;

	//用于放松R角的4个边界的背景区域，使得白色区域更少，放大缺陷带来的不同
	if (true)
	{
		x4 -= 20;
		y4 += 20;
		x3 += 20;
		y3 += 20;
		x2 += 20;
		y2 -= 20;
		x1 -= 20;
		y1 -= 20;
	}
	rectangle(pic_show, Point(x1, y1), Point(x1 + size, y1 + size), Scalar(0, 255, 0), 3);
	Mat R_cornor_cut_1 = binary(Rect(x1, y1, size, size));
	circle(pic_show, pt[0], 10, Scalar(0, 255, 0), -1);

	rectangle(pic_show, Point(x2 - size, y2), Point(x2, y2 + size), Scalar(0, 255, 0), 3);
	Mat R_cornor_cut_2 = binary(Rect(x2 - size, y2, size, size));
	circle(pic_show, pt[1], 10, Scalar(0, 255, 0), -1);

	rectangle(pic_show, Point(x3 - size, y3 - size), Point(x3, y3), Scalar(0, 255, 0), 3);
	Mat R_cornor_cut_3 = binary(Rect(x3 - size, y3 - size, size, size));
	circle(pic_show, pt[2], 10, Scalar(0, 255, 0), -1);

	rectangle(pic_show, Point(x4, y4 - size), Point(x4 + size, y4), Scalar(0, 255, 0), 3);
	Mat R_cornor_cut_4 = binary(Rect(x4, y4 - size, size, size));
	circle(pic_show, pt[3], 10, Scalar(0, 255, 0), -1);

	imwrite(path + "R_cut_1.png", R_cornor_cut_1);
	imwrite(path + "R_cut_2.png", R_cornor_cut_2);
	imwrite(path + "R_cut_3.png", R_cornor_cut_3);
	imwrite(path + "R_cut_4.png", R_cornor_cut_4);

	res.push_back(path + "R_cut_1.png");
	res.push_back(path + "R_cut_2.png");
	res.push_back(path + "R_cut_3.png");
	res.push_back(path + "R_cut_4.png");

	if (show_img)
	{
		namedWindow("R_cornor_cut_1", 0);
		imshow("R_cornor_cut_1", R_cornor_cut_1);

		namedWindow("R_cornor_cut_1", 0);
		imshow("R_cornor_cut_2", R_cornor_cut_2);

		namedWindow("R_cornor_cut_1", 0);
		imshow("R_cornor_cut_1", R_cornor_cut_1);

		namedWindow("R_cornor_cut_1", 0);
		imshow("R_cornor_cut_1", R_cornor_cut_1);
		waitKey(0);
	}
}

// 以一张标准的模板创建R角检测所需的图
//【只在0相机的白屏模板保存R角模板--2019/01/23】
void detectRCornorInit(int cameraFlag)
{
	/************************** 模板R角检测并保存  *******************************/
	int R_size = 120;  //R角裁剪大小
	string res_tmp_dir = (*sendImg1).res_tmp_dir;
	newmkdir(res_tmp_dir);

	// 白屏模板裁剪后的图
	Mat R_tmp_cut = (*sendImg1).imgTftColorCut[cameraFlag].clone();

	// 已矫正裁剪后的图padding一圈
	Mat R_tmp_cut_padding;
	int padding_size = 50;
	copyMakeBorder(R_tmp_cut, R_tmp_cut_padding, padding_size, padding_size, padding_size, padding_size, BORDER_CONSTANT, 0); //BORDER_REPLICATE

	// 白屏区域在padding图中的4个角点坐标
	vector<Point2f> pts;
	pts.push_back(Point2f(padding_size, padding_size));
	pts.push_back(Point2f(R_tmp_cut.cols + padding_size, padding_size));
	pts.push_back(Point2f(R_tmp_cut.cols + padding_size, R_tmp_cut.rows + padding_size));
	pts.push_back(Point2f(padding_size, R_tmp_cut.rows + padding_size));

	vector<string>res_tmp;
	find_R_cornor(R_tmp_cut_padding, pts, R_size, res_tmp_dir, res_tmp, false);

}

// R角缺陷检测，需要一张待检测图 和 【额外的一张无缺陷的模板图】
void compare_R_cornor(vector<string> cor1, vector<string> cor2, string path)
{
	Mat kernel_open = getStructuringElement(MORPH_CROSS, Size(9, 9));
	Mat kernel_close = getStructuringElement(MORPH_RECT, Size(9, 9));
	Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
	Mat element1 = getStructuringElement(MORPH_CROSS, Size(9, 9));
	Mat element2 = getStructuringElement(MORPH_CROSS, Size(7, 7));
	Mat element3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

	int area_thresh = 50;

	// 保存模板图的四个点坐标于txt文件中，用于白屏图检测时的对比
	ofstream OutFile(path + "diff_area.txt"); //利用构造函数创建txt文本，并且打开该文本


	for (int i = 0; i < 4; i++)
	{
		//形态学操作用于试图放大缺陷（可能没用上）
		Mat img = imread(cor1[i], 0);
		Mat img2 = imread(cor2[i], 0);
		//dilate(img, img, element);  // 膨胀
		//dilate(img2, img2, element);  // 膨胀


		Mat dil, ero, res;
		dilate(img, dil, element);  // 膨胀
		erode(img, ero, element);  // 腐蚀
		absdiff(dil, ero, res);

		Mat opened, closed;
		morphologyEx(img, opened, MORPH_OPEN, kernel_open);
		morphologyEx(img, closed, MORPH_CLOSE, kernel_close);

		//求取与模板对应4个R角的差别
		Mat diff;
		absdiff(img, img2, diff);
		//erode(diff, diff, element);  // 腐蚀
		dilate(diff, diff, element3);  // pz  3*3
		//morphologyEx(diff, diff, MORPH_CLOSE, element);
		morphologyEx(diff, diff, MORPH_OPEN, element2);  //7*7
		dilate(diff, diff, element1);  // pz   9*9
		//morphologyEx(diff, diff, MORPH_TOPHAT, element2);

		//imshow("diff", diff);
		//waitKey(0);
		imwrite(path + "diff_" + to_string(i) + ".png", diff);



		//计算差影的轮廓面积 作为缺陷判断依据
		vector<vector<Point>> contours;
		Mat hierarchy;
		cv::findContours(diff, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		float max_area = 0;
		float total_area = 0;

		for (int j = 0; j < contours.size(); j++)
		{
			float area = contourArea(contours[j]);

			//把diff_area的面积写入txt文件
			OutFile << to_string(i) + "_diff_area:" << area << endl;

			total_area += area;
			if (area > max_area)
				max_area = area;
		}
		OutFile << to_string(i) + "_total_area:" << total_area << endl;
		OutFile << endl;
		//cout << i << "area:" << max_area << endl;


		if (total_area > area_thresh)
		{
			(*sendImg1).R_compare_res[i] = 1;  //R角有问题保存在全局变量中，用于后续瑕疵结果汇总
			cout << "===!!!== R cornor gets wrong:" << i << "===!!!==" << endl;
		}
		else
			cout << "R_cornor:" << i << " OK." << endl;
	}
	OutFile.close();//关闭Test.txt文件

}


//【只在0相机的白屏进行R角检测--2019/01/23】
void getRCornorResult(int cameraFlag)
{
	/************************** R角检测  *******************************/
	int R_size = 120;  //R角裁剪大小
	// 白屏裁剪后的图
	Mat R_tft_cut = (*sendImg1).imgTftColorCut[cameraFlag].clone();

	// 已矫正裁剪后的图padding一圈
	Mat R_tft_cut_padding;
	int padding_size = 50;
	copyMakeBorder(R_tft_cut, R_tft_cut_padding, padding_size, padding_size, padding_size, padding_size, BORDER_CONSTANT, 0); //BORDER_REPLICATE
																															  // 白屏区域在padding图中的4个角点坐标
	vector<Point2f> pts;
	pts.push_back(Point2f(padding_size, padding_size));
	pts.push_back(Point2f(R_tft_cut.cols + padding_size, padding_size));
	pts.push_back(Point2f(R_tft_cut.cols + padding_size, R_tft_cut.rows + padding_size));
	pts.push_back(Point2f(padding_size, R_tft_cut.rows + padding_size));

	string path = (*sendImg1).projectDir + "R_tft//";
	newmkdir(path);
	find_R_cornor(R_tft_cut_padding, pts, R_size, path, (*sendImg1).res);

	string path_diff = (*sendImg1).projectDir + "R_dif//";
	newmkdir(path_diff);

	vector<string>res_tmp;
	res_tmp.push_back((*sendImg1).res_tmp_dir + "R_cut_1.png");
	res_tmp.push_back((*sendImg1).res_tmp_dir + "R_cut_2.png");
	res_tmp.push_back((*sendImg1).res_tmp_dir + "R_cut_3.png");
	res_tmp.push_back((*sendImg1).res_tmp_dir + "R_cut_4.png");

	compare_R_cornor((*sendImg1).res, res_tmp, path_diff);

}