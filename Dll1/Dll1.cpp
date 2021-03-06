// Dll2.cpp: 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include<opencv2\opencv.hpp>
#include<opencv2\core\core.hpp>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件
#include <windows.h>
#include <vector>
#define my_pi       3.14159265358979323846

using namespace cv;
using namespace std;

vector<string> files;   //能够存放任意类型的动态数组

						//判断是否存在指定文件夹，不存在则创建
__declspec(dllexport) void mkdir(string dir)
{
	int m = 0, n;
	string str1, str2;
	str1 = dir;
	str2 = str1.substr(0, 2);   //substr是C++语言函数，主要功能是复制子字符串，要求从指定位置开始，并具有指定的长度
	str1 = str1.substr(3, str1.size()); //  '//'算一个字符，转义字符
	while (m >= 0)
	{
		m = str1.find('\\');     //.find()是寻找该字符串下标位置
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //判断该目录是否存在   //c_str()：生成一个const char*指针，指向str2
									  //_access,确定文件或文件夹的访问权限。如果指定的存取方式有效，则函数返回0，否则函数返回-1。
		if (n == -1)
		{
			_mkdir(str2.c_str());     //创建目录
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}



//计算一个点旋转后的映射位置
__declspec(dllexport) Point get_romate_point(Point point, Point center, double angle)
{
	Point new_point;
	Point distance;
	distance.x = point.x -467- center.x;
	distance.y = point.y - -21-center.y;
	new_point.x = round(distance.x * cos(angle) + distance.y * sin(angle) + center.x);
	new_point.y = round(-distance.x * sin(angle) + distance.y * cos(angle) + center.y);
	return new_point;
}

//计算四个点旋转后的映射位置
__declspec(dllexport) void get_many_romate_point(Point Pts[4], Point new_pts[4], Point centerRotation, double angle)
{
	int i;
	for (i = 0; i< 4; i++)
	{
		new_pts[i] = get_romate_point(Pts[i], centerRotation, angle * my_pi / 180);
	}
}


//根据选择的角度和尺度创建图片
__declspec(dllexport) void creat_img(string im_name, double angle, int Scale, string  result_dir, int num )
{
	vector<Point>point_big_boxes = { Point(708, 333), Point(1669, 717) };
	int a, b, c, d;
	a = 708, b = 333, c = 1669, d = 717;
	Point orgin_change(21, 497);
	int set_angle = 20;
	Point centerRotation(round((point_big_boxes[0].x + point_big_boxes[1].x) / 2.0),
		round((point_big_boxes[0].y + point_big_boxes[1].y) / 2.0));
	float w = abs(c - a);
	w = w / 2;
	int h = abs(d - b);
	h = h / 2;
	//对角线的长度为l
	int l = sqrt(pow(a - c, 2) + pow(b - d, 2));
	l = l / 2;
	float hw = h / w;
	float beta = atan(hw);
	//angle角度转成弧度
	float alpha = set_angle + beta;
	float gamma = beta - set_angle;
	int hmax = (int)(l * sin(alpha));
	int wmax = (int)(l * cos(gamma));
	int o1 = centerRotation.x;
	int o2 = centerRotation.y;

	int a1, b1, c1, d1;
	a1 = (int)(o1 - 1.2 * wmax);
	if (a1 < 0)
	{
		a1 = 0;
	}
	b1 = (int)(o2 - 1.2 * hmax);
	if (b1 < 0)
	{
		b1 = 0;
	}
	c1 = (int)(o1 + 1.2 * wmax);
	d1 = (int)(o2 + 1.2 * hmax);
	
	int w1 = 1.2 * wmax;
	int h1 = 1.2 * hmax;
	l = sqrt(pow(w1, 2) + pow(h1, 2));
	hw = h1 / w1;
	float beta1 = atan(hw);
	float alpha1 = angle + beta1;
	float gamma1 = beta1 - angle;
	hmax = (int)(l * sin(alpha1));
	wmax = (int)(l * cos(gamma1));

	//m是正方形的边长
	int m = max (hmax, wmax);
	int a2, b2, c2, d2;
	a2 = (int)(o1 - 1.2 * m);
	if (a2 < 0)
	{
		a2 = 0;
	}
	b2 = (int)(o2 - 1.2 * m);
	if (b2 < 0)
	{
		b2 = 0;
	}
	c2 = (int)(o1 + 1.2 * m);
	d2 = (int)(o2 + 1.2 * m);
		
	//框的顶点进行平移处理
	point_big_boxes = { Point(708 - orgin_change.y, 333 - orgin_change.x),
		Point(1669 - orgin_change.y, 717 - orgin_change.x) };      //左上，右下
	Point centerRotation1(round((point_big_boxes[0].x + point_big_boxes[1].x) / 2),
		round((point_big_boxes[0].y + point_big_boxes[1].y) / 2));
	vector<Point>k1{ Point(a1 - 275, b1 - 21),
		Point(c1 - 275, d1 - 21) };
	vector<Point>k2{ Point(a2 - 275, b2 - 21),
		Point(c2 - 275, d2 - 21) };





	//创建截取模板的文件夹
	string cut_dir = result_dir + "cut" + "\\";
	mkdir(cut_dir);

	////读取模板图片
	Mat img = imread(im_name, 0);
	img = img(Rect(a2 + 142, b2, c2-a2-290, d2-b2-290));
	int h_img = img.rows;	//rows 1942 高
	int w_img = img.cols;  //cols 2590 宽
	Point center(w_img / 2, h_img / 2);
	int i, j;
	
	double angles;
	float change_Scale;
	Mat imageROI;

	////创建一个矩阵M存放旋转矩阵
	Mat M(w_img, h_img, img.type());
	Mat N;
	string img_result_file;
	string cut_img_name;
	////创建旋转后图像
	Mat dst, dst1;

	Point PointBigBox_1(759 - orgin_change.y, 400 - orgin_change.x), PointBigBox_2(1669 - orgin_change.y, 717 - orgin_change.x);
	Point PointLogo_1(799, 463), PointLogo_2(1581, 460), PointLogo_3(1580, 623), PointLogo_4(798, 620);
	Point Logo_points[4] = { PointLogo_1,PointLogo_2,PointLogo_3, PointLogo_4 };
	Point Pts_si[4] = { Point(984, 463), Point(1024, 463), Point(1024, 504), Point(984, 504) };
	Point Pts_xiao[4] = { Point(1349, 467), Point(1390, 467), Point(1390, 508), Point(1349, 508) };
	Point Pts_china[4] = { Point(801, 516), Point(913, 516), Point(913, 560), Point(801, 560) };
	Point Pts_de[4] = { Point(1520, 522), Point(1577, 522), Point(1577, 566), Point(1520, 566) };
	Point Pts_16[4] = { Point(842, 573),Point(886, 573),Point(886, 611), Point(842, 611) };
	Point Pts_dt[4] = { Point(1474, 578), Point(1531, 578), Point(1531, 620),Point(1474, 620) };
	Point new_Pts[4], new_Pts_si[4], new_Pts_xiao[4], new_Pts_china[4], new_Pts_de[4], new_Pts_16[4], new_Pts_dt[4];

	////写入csv文件第一行
	vector<string> points;
	ofstream outFile;          //ofstream 是从内存读到硬盘； ifstream 是从硬盘读到内存。
	outFile.open("D:\\project\\vs2017\\create_xiaozi_done\\csv_data_down.csv", ios::out); // 打开模式可省略   ios::out：文件以输出方式打开（内存数据输出到文件）
	outFile << "angle" << ',';
	for (i = 1; i < 8; i++)
	{

		for (j = 0; j < 8; j++)
		{
			points.push_back("point" + to_string(i) + to_string(j));
			// 写文件
			outFile << points[(i - 1) * 8 + j] << ',';
		}
	}
	outFile << "dir" << endl;


	for (i = 0; i < num; i++)
	{
		//复制图片


		//创建角度等差数列
		angles = -angle + ((angle * 2) / double(num - 1))*i;

		// 随机一个尺度变化
		a = 1 - Scale / 100., b = 1 + Scale / 100.;
		//change_Scale = ((rand() % (int(b * 100) - int(a * 100)))) + (a * 100) / 100;  

		// 旋转图片

		M = getRotationMatrix2D(centerRotation1, angles, 1);
		warpAffine(img, dst, M, img.size());



		////生成的多角度模板的名称
		img_result_file = result_dir + to_string(angles) + "_" + to_string(0) + ".bmp";

		////画大框
		////Point point1(617, 307), point2(1771, 727);
		cv::rectangle(dst, PointBigBox_1, PointBigBox_2, Scalar(0, 0, 255), 3, 1, 0);

		////小框顶点坐标
		////Point point3(799, 463), point4(1581, 460), point5(1580, 623), point6(798, 620);
		//Point Pts[4] = { Point(799, 463),Point(1581, 460),Point(1580, 623), Point(798, 620) };
		//
		////小字位置
		//Point Pts_si[4] = { Point(984, 463), Point(1024, 463), Point(1024, 504), Point(984, 504) };
		//Point Pts_xiao[4] = { Point(1349, 467), Point(1390, 467), Point(1390, 508), Point(1349, 508) };
		//Point Pts_china[4] = { Point(801, 516), Point(913, 516), Point(913, 560), Point(801, 560) };
		//Point Pts_de[4] = { Point(1520, 522), Point(1577, 522), Point(1577, 566), Point(1520, 566) };
		//Point Pts_16[4] = { Point(842, 573),Point(886, 573),Point(886, 611), Point(842, 611) };
		//Point Pts_dt[4] = { Point(1474, 578), Point(1531, 578), Point(1531, 620),Point(1474, 620) };

		////计算实际模板对应的新的位置
		//Point new_Pts[4], new_Pts_si[4], new_Pts_xiao[4],new_Pts_china[4],new_Pts_de[4], new_Pts_16[4], new_Pts_dt[4];

		get_many_romate_point(Logo_points, new_Pts, centerRotation1, angles);
		get_many_romate_point(Pts_si, new_Pts_si, centerRotation1, angles);
		get_many_romate_point(Pts_xiao, new_Pts_xiao, centerRotation1, angles);
		get_many_romate_point(Pts_china, new_Pts_china, centerRotation1, angles);
		get_many_romate_point(Pts_de, new_Pts_de, centerRotation1, angles);
		get_many_romate_point(Pts_16, new_Pts_16, centerRotation1, angles);
		get_many_romate_point(Pts_dt, new_Pts_dt, centerRotation1, angles);
		//
		////将所有坐标保存到一个数组里
		int All_Pts[] = { new_Pts[0].x,new_Pts[0].y,new_Pts[1].x,new_Pts[1].y, new_Pts[2].x,new_Pts[2].y,new_Pts[3].x,new_Pts[3].y,
			new_Pts_si[0].x,new_Pts_si[0].y,new_Pts_si[1].x,new_Pts_si[1].y, new_Pts_si[2].x,new_Pts_si[2].y,new_Pts_si[3].x,new_Pts_si[3].y,
			new_Pts_xiao[0].x,new_Pts_xiao[0].y, new_Pts_xiao[1].x,new_Pts_xiao[1].y,new_Pts_xiao[2].x,new_Pts_xiao[2].y, new_Pts_xiao[3].x,new_Pts_xiao[3].y,
			new_Pts_china[0].x,new_Pts_china[0].y, new_Pts_china[1].x,new_Pts_china[1].y,new_Pts_china[2].x,new_Pts_china[2].y, new_Pts_china[3].x,new_Pts_china[3].y,
			new_Pts_de[0].x,new_Pts_de[0].y, new_Pts_de[1].x,new_Pts_de[1].y,new_Pts_de[2].x,new_Pts_de[2].y, new_Pts_de[3].x,new_Pts_de[3].y,
			new_Pts_16[0].x,new_Pts_16[0].y, new_Pts_16[1].x,new_Pts_16[1].y,new_Pts_16[2].x,new_Pts_16[2].y, new_Pts_16[3].x,new_Pts_16[3].y,
			new_Pts_dt[0].x,new_Pts_dt[0].y, new_Pts_dt[1].x,new_Pts_dt[1].y ,new_Pts_dt[2].x,new_Pts_dt[2].y, new_Pts_dt[3].x,new_Pts_dt[3].y
		};

		cut_img_name = cut_dir + to_string(angles) + ".bmp";
		//写入csv剩下行
		outFile << angles << ',';
		for (j = 0; j<56; j++)
		{
			outFile << All_Pts[j] << ',';
		}
		outFile << cut_img_name << endl;

		//	//保存一级模板
		//

		imageROI = dst(Rect(759 - orgin_change.y, 400 - orgin_change.x, 910, 317));
		imwrite(cut_img_name, imageROI);

		//	//显示旋转后的二级模板
		//	Point ppt1[] = { new_Pts[0] ,new_Pts[1] ,new_Pts[2] ,new_Pts[3] };
		//	Point ppt2[] = { new_Pts_si[0] ,new_Pts_si[1] ,new_Pts_si[2] ,new_Pts_si[3] };
		//	const Point* pts1[] = { ppt1 };
		//	const Point* pts2[] = { ppt2 };
		//	int npt[] = { 4 };
		//	dst1 = dst.clone();
		//	polylines(dst1, pts1, npt, 1, true, //是否闭合,改为false则不闭合
		//		Scalar(0, 0, 255), 2, 8);
		//	polylines(dst1, pts2, npt, 1, true, 
		//		Scalar(0, 0, 255), 2, 8);
		//imwrite(img_result_file, dst);
		//}

	}
	outFile.close();

}







