
// dustGetYu.cpp: �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <iostream>
#include <string>
#include <direct.h> //_mkdir������ͷ�ļ�
#include <io.h>     //_access������ͷ�ļ�
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

//��ȡͼƬ�ĽǶ�ƫ����Ϣ
RotatedRect get_img_rotate_info(Mat& image, bool show_img)
{
	Mat img;
	cvtColor(image, img, CV_BGR2GRAY);
	//��˹ƽ��
	Mat blurred;
	GaussianBlur(img, blurred, Size(5, 5), 0, 0);
	//��ֵ��
	Mat mask;
	threshold(blurred, mask, 30, 255, CV_THRESH_BINARY);

	//�����������
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	double max_area = 100;
	//vector<Rect>boundRect(contours.size());
	//��ÿ���ҵ���������������б�ı߽��
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
		//���ˮƽ����--��ɫ
		rectangle(image_show, centerPoint, boundRect.br(), Scalar(0, 255, 0), 2);
		//��С��Ӿ��Σ��нǶȣ�--��ɫ
		Point2f rect_points[4]; rect.points(rect_points);
		for (int j = 0; j < 4; j++)
		{
			line(image_show, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 2);
		}
		//drawContours(image, rect[i], 0, Scalar(0,0,255), 2); //�˴�������ȫƥ��python
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


	//�����ֻ���Ļ������С��ת��Ӿ�����Ϣ�������Ƕȣ�����    
	rect = get_img_rotate_info(img, show_process);

	//(*sendImg1).get_rotate_rect[cameraFlag] = rect;

	// ������С��ӿ���Ϣ��ת�����Ƕ�
	Point2f center = rect.center;
	Size2f size = rect.size;
	float angle = rect.angle;
	Mat res;
	if (angle <= -45)
	{
		angle += 90;
	}


	cout << "rotate_angle:" << angle << endl;

	//���ݼ���ĽǶȽ��нǶ���ת
	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(img_copy, res, M, Size(cols, rows));

	//������������
	RotatedRect rect_res = get_img_rotate_info(res);
	(*sendImg1).rect_res = rect_res;

	//��ȡrect_res���ĸ�����

	rect_res.points(box);

	//�ǶȽ������
	Mat img_crop = res(cv::Rect(box[1].x, box[1].y, box[2].x - box[1].x, box[0].y - box[1].y));
	img_crop.copyTo(img_out);  //������ת�ü����
	if (returnCutOrNotCutImg == false)  //���ز��ü�ԭͼ��ת
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
	// �����ȡ������ı�������
	float x_y_rito = (poly[1].y - poly[2].y) / (poly[3].x - poly[2].x);
	int size_x = (int)size_y*x_y_rito;

	// ����������չ̶�����ֵ����
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
	//Ѱ�������ļ�ֵ
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


	// ������㻮���ĸ�����
	vector<Point> polyx = poly;
	vector<Point> polyy = poly;
	//ȷ��˳ʱ��˳��
	//final_poly["x_min"].push_back(Point(0,0));
	//final_poly["x_min"].pop_back();
	//final_poly["y_min"].push_back(Point(0, 0));
	//final_poly["y_min"].pop_back();
	//final_poly["x_max"].push_back(Point(0, 0));
	//final_poly["x_max"].pop_back();
	//final_poly["y_max"].push_back(Point(0, 0));
	//final_poly["y_max"].pop_back();
	//�޷�����map�ļ�˳�������ǲ���<key, value>��ֵ��ʱ���ͻ�Ĭ�ϰ���key�Ĵ�С˳����д洢
	//��Ĭ��˳��Ϊx_max,x_min,y_max,y_min.

	vector<int>y_min_abs, y_max_abs, x_min_abs, x_max_abs;  //20190219

	//���y�᷽��
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
	//���x�᷽��
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

	//2019-02-19-ysq �Լ�������ֵ����������Ӧ
	// ����
	sort(x_min_abs.begin(), x_min_abs.end());
	sort(y_min_abs.begin(), y_min_abs.end());
	sort(x_max_abs.begin(), x_max_abs.end());
	sort(y_max_abs.begin(), y_max_abs.end());
	// ���������ֵ
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


//  2019-02-19ysq�޸� ��������Ӧ���㶼��Ҫ��������ÿ�ζ��ֶ�����
void sort_points1(vector<Point>&poly, map<string, vector<Point>>&final_poly, vector<int>&dxy)
{
	//Ѱ�������ļ�ֵ
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


	// ������㻮���ĸ�����
	vector<Point> polyx = poly;
	vector<Point> polyy = poly;
	//ȷ��˳ʱ��˳��
	//final_poly["x_min"].push_back(Point(0,0));
	//final_poly["x_min"].pop_back();
	//final_poly["y_min"].push_back(Point(0, 0));
	//final_poly["y_min"].pop_back();
	//final_poly["x_max"].push_back(Point(0, 0));
	//final_poly["x_max"].pop_back();
	//final_poly["y_max"].push_back(Point(0, 0));
	//final_poly["y_max"].pop_back();
	//�޷�����map�ļ�˳�������ǲ���<key, value>��ֵ��ʱ���ͻ�Ĭ�ϰ���key�Ĵ�С˳����д洢
	//��Ĭ��˳��Ϊx_max,x_min,y_max,y_min.

	vector<int>y_min_abs, y_max_abs, x_min_abs, x_max_abs;  //20190219

	//���y�᷽��
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
	//���x�᷽��
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

	//2019-02-19-ysq �Լ�������ֵ����������Ӧ
	// ����
	sort(x_min_abs.begin(), x_min_abs.end());
	sort(y_min_abs.begin(), y_min_abs.end());
	sort(x_max_abs.begin(), x_max_abs.end());
	sort(y_max_abs.begin(), y_max_abs.end());
	// ���������ֵ
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
		//if (n > 1 && (x_min_abs[n] - x_min_abs[n - 1] > 30))  //�������Ψһ��Ƕ���صĲ�--����ֵ
		//	index = n-1;
	}
	//for (int n = 0; n <= index; n++)
	//	s3 += x_min_abs[n];
	//m3 = s3 / (index+1);

	// ���û��������Ļ��x_minȡֵ�б仯
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
	//���x�᷽��
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
	//�� L1б�ʲ�����
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
	// ��L2б�ʲ�����
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
	//��˹ƽ��
	Mat blurred;
	GaussianBlur(gray, blurred, Size(5, 5), 0, 0);
	//��ֵ��
	Mat binary;
	threshold(blurred, binary, binary_thres, 255, CV_THRESH_BINARY);

	//�����������
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//�ںڱ����ϻ�������
	Mat tmp_contour(rows, cols, CV_8UC1, Scalar(0));  //8λ�޷���char�͵�ͨ��
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




	//ȡcanny�߽���������߽�
	Mat edge = tmp_contour.clone();
	//Canny(blurred, edge, 2, 50, 3);

	/* �ۻ����ʻ���ֱ�߱任*/
	vector<Vec4i>lines;
	//////////11.26���˻���任�Ĳ���/////////
	HoughLinesP(edge, lines, 1, CV_PI / 180, 60, 100, 80);  // 0.75 0.68   120   100 00

	//����⵽���߻�����
	vector<Point>cnt;
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//��4��[x1,y1,x2,y2]ת��Ϊ2��[x1,y1],[x2,y2]
		Point p1(l[0], l[1]);
		Point p2(l[2], l[3]);
		cnt.push_back(p1);
		cnt.push_back(p2);
		line(img_draw, Point(l[0], l[1]), Point(l[2], l[3]),
			Scalar(0, 0, 255), 1, CV_AA);
	}

	//namedWindow("lines", 0);
	//imshow("lines", img_draw);

	//�������߶εĶ˵㻭����
	Mat tmp_points(rows, cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < cnt.size(); i++)
	{
		circle(tmp_points, cnt[i], 5, Scalar(255, 255, 255), -1);
	}

	//namedWindow("points", 0);
	//imshow("points", tmp_points);

	long th3 = GetTickCount();

	/*============ ɸѡ��������Щ��Ϊ���� ============*/
	Mat tmp_lines(rows, cols, CV_8UC1, Scalar(0));
	map<string, vector<Point>>classfied_point;  //��Ӧpython�е��ֵ�
	map<string, vector<Point>>::iterator iter;
	//cout << "cnt:" << endl;
	//cout << cnt << endl;
	//sort_points(cnt, classfied_point, sort_dxy);
	sort_points1(cnt, classfied_point, sort_dxy);

	//cout << "classfied_point:" << endl;

	long th4 = GetTickCount();

	//����ĵ�ֱ������ֱ��
	vector<Vec4i>final_points;
	for (iter = classfied_point.begin(); iter != classfied_point.end(); iter++)
	{
		//cout << iter->first << ' ' << iter->second << endl;
		vector<Point>pxy = iter->second;
		for (int i = 0; i < pxy.size(); i++)
			//�ں�ɫ�����ϻ�����ϵ�
			circle(tmp_lines, pxy[i], 5, Scalar(255, 255, 255), -1);
		if (show_img)
		{
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

		Vec4f line_res;
		//ֱ�����
		fitLine(pxy, line_res, DIST_L1, 0, 1e-2, 1e-2);
		//cout << "line_res" << line_res << endl;
		//��ȡ��бʽ�ĵ��б��
		Point point0;
		point0.x = line_res[2];
		point0.y = line_res[3];
		double k = line_res[1] / line_res[0];

		//����ֱ�ߵĶ˵�(y = k(x - x0) + y0)
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


		// �������б�����޴����� ==== 2018/11/22
		if (line_res[0] < 0.0001)
		{
			//cout << "bad k" << line_res[0] << endl;
			p[0] = pxy[0].x;
			p[1] = pxy[0].y;
			p[2] = pxy[1].x;
			p[3] = pxy[1].y;
		}
		//cout << "p" << p << endl;
		// �������б�����޴����� ==== 2018/11/22


		final_points.push_back(p);
		if (show_img)
		{
			//�ں�ɫ�����ϻ������ֱ��
			line(tmp_lines, point1, point2, Scalar(255, 255, 255), 2);
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

	}
	//for (int a = 0; a < final_points.size(); a++)
	//cout << "final_points" << final_points[a] << endl;
	// ��ֱ�߽��㣬�����ؽ��
	//classfied_point�м���Ĭ��˳��Ϊx_max,x_min,y_max,y_min.
	//�������python��˳ʱ�벻һ������Ҫ��˳��
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
	cout << "huf�����������е�ʱ��Ϊ��" << (th2 - th1) << endl;
	cout << "huf��⺯�����е�ʱ��Ϊ��" << (th3 - th2) << endl;
	cout << "sort point�������е�ʱ��Ϊ��" << (th4 - th3) << endl;
	cout << "ֱ����Ϻ������е�ʱ��Ϊ��" << (th5 - th4) << endl;


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

//0301������汾
void hough_detect(Mat& image, vector<Point2f>& res_points, vector<int>sort_dxy, int binary_thres, bool show_img)
{
	long th1 = GetTickCount();

	int rows = image.rows;
	int cols = image.cols;
	Mat img_draw = image.clone();
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	//��˹ƽ��
	Mat blurred;
	GaussianBlur(gray, blurred, Size(5, 5), 0, 0);
	//��ֵ��
	Mat binary;
	threshold(blurred, binary, binary_thres, 255, CV_THRESH_BINARY);
	//threshold(blurred, binary, binary_thres, 255, CV_THRESH_OTSU);

	//�����������
	vector<vector<Point>> contours;
	Mat hierarchy;
	//vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//�ںڱ����ϻ�������
	Mat tmp_contour(rows, cols, CV_8UC1, Scalar(0));  //8λ�޷���char�͵�ͨ��
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




	//ȡcanny�߽���������߽�
	Mat edge = tmp_contour.clone();
	//Canny(blurred, edge, 2, 50, 3);

	/* �ۻ����ʻ���ֱ�߱任*/
	vector<Vec4i>lines;
	//////////11.26���˻���任�Ĳ���/////////
	HoughLinesP(edge, lines, 1, CV_PI / 180, (*canshuX)._houghP[0], (*canshuX)._houghP[1], (*canshuX)._houghP[2]);  //  0.68   120   100 00

	//����⵽���߻�����
	vector<Point>cnt;
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//��4��[x1,y1,x2,y2]ת��Ϊ2��[x1,y1],[x2,y2]
		Point p1(l[0], l[1]);
		Point p2(l[2], l[3]);
		cnt.push_back(p1);
		cnt.push_back(p2);
		line(img_draw, Point(l[0], l[1]), Point(l[2], l[3]),
			Scalar(0, 0, 255), 1, CV_AA);
	}

	//namedWindow("lines", 0);
	//imshow("lines", img_draw);

	//�������߶εĶ˵㻭����
	Mat tmp_points(rows, cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < cnt.size(); i++)
	{
		circle(tmp_points, cnt[i], 5, Scalar(255, 255, 255), -1);
	}

	//namedWindow("points", 0);
	//imshow("points", tmp_points);

	long th3 = GetTickCount();

	/*============ ɸѡ��������Щ��Ϊ���� ============*/
	Mat tmp_lines(rows, cols, CV_8UC1, Scalar(0));
	map<string, vector<Point>>classfied_point;  //��Ӧpython�е��ֵ�
	map<string, vector<Point>>::iterator iter;
	//cout << "cnt:" << endl;
	//cout << cnt << endl;

	//sort_points(cnt, classfied_point, sort_dxy);
	sort_points1(cnt, classfied_point, sort_dxy);  //20190227
	//cout << "classfied_point:" << endl;

	long th4 = GetTickCount();

	//����ĵ�ֱ������ֱ��
	vector<Vec4i>final_points;
	for (iter = classfied_point.begin(); iter != classfied_point.end(); iter++)
	{
		//cout << iter->first << ' ' << iter->second << endl;
		vector<Point>pxy = iter->second;
		for (int i = 0; i < pxy.size(); i++)
			//�ں�ɫ�����ϻ�����ϵ�
			circle(tmp_lines, pxy[i], 5, Scalar(255, 255, 255), -1);
		if (show_img)
		{
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

		Vec4f line_res;
		//ֱ�����
		fitLine(pxy, line_res, DIST_L1, 0, 1e-2, 1e-2);
		//cout << "line_res" << line_res << endl;
		//��ȡ��бʽ�ĵ��б��
		Point point0;
		point0.x = line_res[2];
		point0.y = line_res[3];
		double k = line_res[1] / line_res[0];

		//����ֱ�ߵĶ˵�(y = k(x - x0) + y0)
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


		// �������б�����޴����� ==== 2018/11/22
		if (line_res[0] < 0.0001)
		{
			//cout << "bad k" << line_res[0] << endl;
			p[0] = pxy[0].x;
			p[1] = pxy[0].y;
			p[2] = pxy[1].x;
			p[3] = pxy[1].y;
		}
		//cout << "p" << p << endl;
		// �������б�����޴����� ==== 2018/11/22


		final_points.push_back(p);
		if (show_img)
		{
			//�ں�ɫ�����ϻ������ֱ��
			line(tmp_lines, point1, point2, Scalar(255, 255, 255), 2);
			namedWindow("tmp_lines", 0);
			imshow("tmp_lines", tmp_lines);
			waitKey(0);
		}

	}
	//for (int a = 0; a < final_points.size(); a++)
	//cout << "final_points" << final_points[a] << endl;
	// ��ֱ�߽��㣬�����ؽ��
	//classfied_point�м���Ĭ��˳��Ϊx_max,x_min,y_max,y_min.
	//�������python��˳ʱ�벻һ������Ҫ��˳��
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
	cout << "huf�����������е�ʱ��Ϊ��" << (th2 - th1) << endl;
	cout << "huf��⺯�����е�ʱ��Ϊ��" << (th3 - th2) << endl;
	cout << "sort point�������е�ʱ��Ϊ��" << (th4 - th3) << endl;
	cout << "ֱ����Ϻ������е�ʱ��Ϊ��" << (th5 - th4) << endl;


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

//������ȡ����IOU������Ϣ
void getRoi(int cameraFlag, bool isBuildtemplate, bool if_showimg)
{
	Mat imgTFT = (*sendImg1).imgTFT[cameraFlag].clone();

	/*��ȡ��ת���*/
	RotatedRect rect;
	Mat dst;
	Point2f box[4];

	/*************  ����任��������С�Ҵ������  2018/11/22******************/
	long tr1 = GetTickCount();

	//1.��ͼƬ���нǶȽ���
	img_rotate(imgTFT, box, rect, (*sendImg1).imgTftColorCut[cameraFlag], cameraFlag, false, false);
	//Mat imgTFT_out_copy = imgTFT_out.clone();
	//imwrite("D:\\project\\bluetmpimgTftColorCut.png", (*sendImg1).imgTftColorCut);

	long tr2 = GetTickCount();


	//2.����ǶȽ���֮���ֻ�����Ļ���ĸ�����
	//pts1 = np.float32([[0, 0], [1130, 0], [1130, 2280], [0, 2280]])

	vector<int>sort_dxy;  //12.29�޸�sort_point������
	switch (cameraFlag) // ���ڡ���ͬ�����ȡ��ͬ�Ļ���������� --0123
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
		//	//vector<int>sort_dxy_dust;  //12.29�޸�sort_point������
		//	sort_dxy.push_back(210);
		//	sort_dxy.push_back(20);
		//	sort_dxy.push_back(80);
		//	sort_dxy.push_back(100);
		//	hough_detect((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).pts1[cameraFlag], sort_dxy, 50, false);
		//	break;
	}

	//imwrite("D:\\project\\bluetmpimgTftColorCut2.png", (*sendImg1).imgTftColorCut);

	long tr3 = GetTickCount();
	cout << "ROI��ת���е�ʱ��Ϊ��" << (tr2 - tr1) << endl;
	cout << "ROI����任���е�ʱ��Ϊ��" << (tr3 - tr2) << endl;


	//imwrite("D://project//test.bmp", (*sendImg1).imgTFT);

	//imwrite("D:\\project\\imgTFT_out.bmp", imgTFT_out);
	/*************  ����任��������С�Ҵ������  2018/11/22******************/


	//������ת���̵Ĳ���
	//(*sendImg1).rect[cameraFlag] = rect;
	(*sendImg1).get_rotate_rect[cameraFlag] = rect;


	//�������任��Ĳ���
	for (int ptrMove = 0; ptrMove < 4; ptrMove++)
		(*sendImg1).box[cameraFlag][ptrMove] = box[ptrMove];




	if (isBuildtemplate == true)
	{
		//�����ģ�壬������0���1/23���򱣴����pts1����,Ȼ�����͸�ӱ任
		if (cameraFlag == 0)
		{
			// ����ģ��ͼ���ĸ���������txt�ļ��У����ڰ���ͼ���ʱ�ĶԱ�
			ofstream OutFile((*sendImg1).pts1_tmp_file); //���ù��캯������txt�ı������Ҵ򿪸��ı�
			for (int i = 0; i < 4; i++)
			{
				OutFile << (*sendImg1).pts1[cameraFlag][i].x; //���ַ�������"This is a Test!"��д��Test.txt�ļ�
				OutFile << "\n";
				OutFile << (*sendImg1).pts1[cameraFlag][i].y; //���ַ�������"This is a Test!"��д��Test.txt�ļ�
				OutFile << "\n";
			}
			OutFile.close();//�ر�Test.txt�ļ�

			//������ز���   2315*1115  ͸�ӱ任�Ĵ�С��ÿ�εİ���ģ��ͼȷ�������ҹ̶�Ϊ0�����--2019/01/22��
			(*sendImg1).PersPectiveTransformation_hight = (*sendImg1).pts1[0][2].y - (*sendImg1).pts1[0][0].y;
			(*sendImg1).PersPectiveTransformation_weight = (*sendImg1).pts1[0][1].x - (*sendImg1).pts1[0][0].x;
		}

		vector<Point2f> pts2;
		pts2.push_back(Point2f(0, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, 0));
		pts2.push_back(Point2f((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		pts2.push_back(Point2f(0, (*sendImg1).PersPectiveTransformation_hight));
		(*sendImg1).WhiteScreenQuadrilateral = pts2;

		//����͸�ӱ任����
		Mat M = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);


		//����͸�ӱ任
		warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));
		//cout<<"PersPectiveTransformation_weight:"<<(*sendImg1).PersPectiveTransformation_weight<<endl;
		//cout<<"PersPectiveTransformation_hight:"<<(*sendImg1).PersPectiveTransformation_hight<<endl;
		imwrite("..\\ColorCut.png", (*sendImg1).imgTftColorCut[cameraFlag]);
		//namedWindow("beforeresize", 0);
		//imshow("beforeresize",(*sendImg1).imgTftColorCut );


	}

	else {  //�������ģ�壬��ԡ�ֻ0���--01/23����⵽���ĸ��ǵ����������ģ��Ľǵ�ƥ���⣬Ȼ�����͸�ӱ任

		if (cameraFlag == 0)
		{
			// ��ȡģ����ĸ��ǵ���Ϣtxt�ļ�
			string line;
			Point2f p;
			int f = 0;

			ifstream InFile((*sendImg1).pts1_tmp_file);;
			if (InFile) // �и��ļ�  
			{
				//getline���ж���
				while (getline(InFile, line)) // line�в�����ÿ�еĻ��з�  
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
			else // û�и��ļ�  
			{
				//throw "no pts1_tmp_file !!";

				int x = MessageBox(GetForegroundWindow(), "pts1_tmp_file��ȡʧ�ܣ���", "", MB_OKCANCEL); //һ��ȷ����ť��һ��ȡ����ť
				if (x == 2)//��ȡ����ť
					exit(0);
			}


			//��������⵽��ԭ��ͼ���ĸ��ǵ�pts1��ģ��Ľ��бȽ� 
			//���̫��ͱ���
			for (int i = 0; i < 4; i++)
			{
				Point2f p_img = (*sendImg1).pts1[cameraFlag][i];
				Point2f p_tmp = (*sendImg1).pts1_tmp[i];
				if (abs(p_img.x - p_tmp.x) > 50 || abs(p_img.y - p_tmp.y) > 50)
				{
					//throw "img get wrong edges!!";
					int x = MessageBox(GetForegroundWindow(), "ͼƬ�߽��ȡ�쳣����", "", MB_OKCANCEL); //һ��ȷ����ť��һ��ȡ����ť
					if (x == 2)//��ȡ����ť
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


		//����͸�ӱ任����
		Mat M = getPerspectiveTransform((*sendImg1).pts1[cameraFlag], (*sendImg1).WhiteScreenQuadrilateral);




		//����͸�ӱ任
		warpPerspective((*sendImg1).imgTftColorCut[cameraFlag], (*sendImg1).imgTftColorCut[cameraFlag], M, Size((*sendImg1).PersPectiveTransformation_weight, (*sendImg1).PersPectiveTransformation_hight));

		long tr4 = GetTickCount();
		cout << "ROI͸�ӱ任���е�ʱ��Ϊ��" << (tr4 - tr3) << endl;

		//imwrite(".\\2camera-grid\\grid40_up_res.png", (*sendImg1).imgTftColorCut);

		//namedWindow("beforeresize", 0);
		//imshow("beforeresize",(*sendImg1).imgTftColorCut );

		//waitKey(0);
		//resize((*sendImg1).imgTftColorCut, (*sendImg1).imgTftColorCut, Size((*sendImg1).PersPectiveTransformation_hight, (*sendImg1).PersPectiveTransformation_weight));


	}


}

vector<Point2f> getRoi1(int cameraFlag)
{
	Mat imgTFT = (*sendImg1).imgTFT[cameraFlag].clone();//��������
	/*��ȡ��ת���*/
	RotatedRect rect;
	Mat dst;
	Point2f box[4];
	Mat imgTFT_out;
	//img_rotate(imgTFT, res_path, box, rect, imgTFT_out);



	/*************  ����任��������С�Ҵ������  2018/11/22******************/
	img_rotate(imgTFT, box, rect, imgTFT_out, false, false);
	Mat imgTFT_out_copy = imgTFT_out.clone();
	//�任��ֱ������ϡ����ϡ����¡������ĸ���
	//pts2 = np.float32([[0, 0], [1130, 0], [1130, 2280], [0, 2280]])
	vector<Point2f> pts2222;
	vector<int>sort_dxy2;  //12.29�޸�sort_point������
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

	//����͸�ӱ任����
	Mat M = getPerspectiveTransform(pts2222, pts2);
	//����͸�ӱ任
	warpPerspective(imgTFT_out_copy, imgTFT_out, M, Size(1115, 2315));
	resize(imgTFT_out, imgTFT_out, Size(2315, 1115));
	cout << "imgTFT_out.rows" << imgTFT_out.rows << "imgTFT_out.cols" << imgTFT_out.cols << endl;
	//imwrite("D:\\project\\imgTFT_out.bmp", imgTFT_out);
	/*************  ����任��������С�Ҵ������  2018/11/22******************/



	(*sendImg1).get_rotate_rect[cameraFlag] = rect;

	for (int ptrMove = 0; ptrMove < 4; ptrMove++)
	{
		(*sendImg1).box[cameraFlag][ptrMove] = box[ptrMove];

	}
	//roi����߽��������10������
	int shrink_size = (*sendImg1).shrink_size;
	int w_TFT, h_TFT;
	w_TFT = imgTFT_out.rows;
	h_TFT = imgTFT_out.cols;
	//imgTFT = imgTFT_out(Range(shrink_size, w_TFT - shrink_size), Range(shrink_size, h_TFT - shrink_size));
	//ֻ�������Һ���
	imwrite("D:\\project\\imgTFT_formal.bmp", imgTFT_out);
	imgTFT = imgTFT_out(Range(shrink_size, w_TFT - 2), Range(shrink_size, h_TFT - shrink_size));
	cout << "imgTFT.rows" << imgTFT.rows << "imgTFT.cols" << imgTFT.cols << endl;
	imwrite("D:\\project\\imgTFT_shrink.bmp", imgTFT);

	//TFT�����ü�����
	(*sendImg1).imgTftColorCut[cameraFlag] = imgTFT.clone();

	//��ȡͼƬ�ĳ���

	(*sendImg1).Max_y = (*sendImg1).imgTftColorCut[cameraFlag].rows;
	(*sendImg1).Max_x = (*sendImg1).imgTftColorCut[cameraFlag].cols;
	//��ȡiou����




	return pts2222;
}

// �������������룬������������ͼ�ĵ�һ������
/*
float neican_data[] = { 124046.170922198990,0,1672.8744383824846000,0, 123576.1775990077400,1436.80076020515030,0,0,1 };
float jibian_data[] = { -115.529366790098550, -13.3238477842862260, -0.0822106292981709260,0.00036432147392857344,-0.00288576837799785390 };
Mat newimage = imageSource.clone(); //У�������ͼƬ
*/

//R�Ǽ��
//R�Ƕ�λ����������Ϊ����任��⵽�Ľǵ�
//R�Ǽ��
//R�Ƕ�λ����������Ϊ����任��⵽�Ľǵ�


//��ȡͼƬ��R����Ϣ
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

	//���ڷ���R�ǵ�4���߽�ı�������ʹ�ð�ɫ������٣��Ŵ�ȱ�ݴ����Ĳ�ͬ
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

// ��һ�ű�׼��ģ�崴��R�Ǽ�������ͼ
//��ֻ��0����İ���ģ�屣��R��ģ��--2019/01/23��
void detectRCornorInit(int cameraFlag)
{
	/************************** ģ��R�Ǽ�Ⲣ����  *******************************/
	int R_size = 120;  //R�ǲü���С
	string res_tmp_dir = (*sendImg1).res_tmp_dir;
	newmkdir(res_tmp_dir);

	// ����ģ��ü����ͼ
	Mat R_tmp_cut = (*sendImg1).imgTftColorCut[cameraFlag].clone();

	// �ѽ����ü����ͼpaddingһȦ
	Mat R_tmp_cut_padding;
	int padding_size = 50;
	copyMakeBorder(R_tmp_cut, R_tmp_cut_padding, padding_size, padding_size, padding_size, padding_size, BORDER_CONSTANT, 0); //BORDER_REPLICATE

	// ����������paddingͼ�е�4���ǵ�����
	vector<Point2f> pts;
	pts.push_back(Point2f(padding_size, padding_size));
	pts.push_back(Point2f(R_tmp_cut.cols + padding_size, padding_size));
	pts.push_back(Point2f(R_tmp_cut.cols + padding_size, R_tmp_cut.rows + padding_size));
	pts.push_back(Point2f(padding_size, R_tmp_cut.rows + padding_size));

	vector<string>res_tmp;
	find_R_cornor(R_tmp_cut_padding, pts, R_size, res_tmp_dir, res_tmp, false);

}

// R��ȱ�ݼ�⣬��Ҫһ�Ŵ����ͼ �� �������һ����ȱ�ݵ�ģ��ͼ��
void compare_R_cornor(vector<string> cor1, vector<string> cor2, string path)
{
	Mat kernel_open = getStructuringElement(MORPH_CROSS, Size(9, 9));
	Mat kernel_close = getStructuringElement(MORPH_RECT, Size(9, 9));
	Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
	Mat element1 = getStructuringElement(MORPH_CROSS, Size(9, 9));
	Mat element2 = getStructuringElement(MORPH_CROSS, Size(7, 7));
	Mat element3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

	int area_thresh = 50;

	// ����ģ��ͼ���ĸ���������txt�ļ��У����ڰ���ͼ���ʱ�ĶԱ�
	ofstream OutFile(path + "diff_area.txt"); //���ù��캯������txt�ı������Ҵ򿪸��ı�


	for (int i = 0; i < 4; i++)
	{
		//��̬ѧ����������ͼ�Ŵ�ȱ�ݣ�����û���ϣ�
		Mat img = imread(cor1[i], 0);
		Mat img2 = imread(cor2[i], 0);
		//dilate(img, img, element);  // ����
		//dilate(img2, img2, element);  // ����


		Mat dil, ero, res;
		dilate(img, dil, element);  // ����
		erode(img, ero, element);  // ��ʴ
		absdiff(dil, ero, res);

		Mat opened, closed;
		morphologyEx(img, opened, MORPH_OPEN, kernel_open);
		morphologyEx(img, closed, MORPH_CLOSE, kernel_close);

		//��ȡ��ģ���Ӧ4��R�ǵĲ��
		Mat diff;
		absdiff(img, img2, diff);
		//erode(diff, diff, element);  // ��ʴ
		dilate(diff, diff, element3);  // pz  3*3
		//morphologyEx(diff, diff, MORPH_CLOSE, element);
		morphologyEx(diff, diff, MORPH_OPEN, element2);  //7*7
		dilate(diff, diff, element1);  // pz   9*9
		//morphologyEx(diff, diff, MORPH_TOPHAT, element2);

		//imshow("diff", diff);
		//waitKey(0);
		imwrite(path + "diff_" + to_string(i) + ".png", diff);



		//�����Ӱ��������� ��Ϊȱ���ж�����
		vector<vector<Point>> contours;
		Mat hierarchy;
		cv::findContours(diff, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		float max_area = 0;
		float total_area = 0;

		for (int j = 0; j < contours.size(); j++)
		{
			float area = contourArea(contours[j]);

			//��diff_area�����д��txt�ļ�
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
			(*sendImg1).R_compare_res[i] = 1;  //R�������Ᵽ����ȫ�ֱ����У����ں���覴ý������
			cout << "===!!!== R cornor gets wrong:" << i << "===!!!==" << endl;
		}
		else
			cout << "R_cornor:" << i << " OK." << endl;
	}
	OutFile.close();//�ر�Test.txt�ļ�

}


//��ֻ��0����İ�������R�Ǽ��--2019/01/23��
void getRCornorResult(int cameraFlag)
{
	/************************** R�Ǽ��  *******************************/
	int R_size = 120;  //R�ǲü���С
	// �����ü����ͼ
	Mat R_tft_cut = (*sendImg1).imgTftColorCut[cameraFlag].clone();

	// �ѽ����ü����ͼpaddingһȦ
	Mat R_tft_cut_padding;
	int padding_size = 50;
	copyMakeBorder(R_tft_cut, R_tft_cut_padding, padding_size, padding_size, padding_size, padding_size, BORDER_CONSTANT, 0); //BORDER_REPLICATE
																															  // ����������paddingͼ�е�4���ǵ�����
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