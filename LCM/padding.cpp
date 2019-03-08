#include "stdafx.h"
#include <iostream>
#include <string>
#include <direct.h> //_mkdir函数的头文件
#include <io.h>     //_access函数的头文件
#include <windows.h>
#include "opencv\cv.h"
#include "padding.h"

using namespace std;

//padding图片
Mat padding_image(Mat img)
{
	int a1 = (sendImg1)->shrink_size_mura;    //8
	//int a2 = (sendImg1)->shrink_size_dian;   //4
											 //补齐差值
	int b = abs(a1);
	Mat copyImage = img.clone();
	if (!img.data)
	{
		cout << "Error: This image is not built!" << endl;
	}

	//padding pic
	//获取待padding图像的尺寸大小
	int w = copyImage.rows;
	int h = copyImage.cols;  //利用Mat的数据类型获取

	int new_h = h + b;
	int new_w = w + b;
	//cout << new_h << endl;
	//cout << new_w << endl;

	//构建一个new_h*new_w图像矩阵,矩阵元素为3通道无符号整数
	//Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
	//Mat padImg1 = Mat::zeros(Size(h, w), CV_8UC3);


	int top, bottom, left, right;
	top = bottom = left = right = b;
	Scalar value;
	value = Scalar(0, 0, 0);
	////按最长边计算左右或上下待扩充大小
	//if (copyImage.cols <= copyImage.rows) {
	//	top = 0;
	//	bottom = 0;
	//	left = abs(copyImage.rows - copyImage.cols) / 2;
	//	right = abs(copyImage.rows - copyImage.cols) / 2;
	//}
	//else {
	//	top = abs(copyImage.rows - copyImage.cols) / 2;
	//	bottom = abs(copyImage.rows - copyImage.cols) / 2;
	//	left = 0;
	//	right = 0;
	//}

	//
	Mat padImg;
	copyMakeBorder(copyImage, padImg, top, bottom, left, right, BORDER_CONSTANT, value);
	//(*sendImg2).padImg = padImg.clone();
	//namedWindow("padImg", 0);
	//imshow("padImg", padImg);
	return padImg;


	//return temp_padImg;
}


//1.9 修改repadding函数 lyl
void repadding(Mat & imgIn, Mat & imgOut, int padding)
{
	int padding_size = padding;
	Mat copyImage = imgIn.clone();
	if (!imgIn.data)
	{
		cout << "Error: This image is not built!" << endl;
	}

	//padding pic
	//获取待padding图像的尺寸大小
	int w = copyImage.rows;
	int h = copyImage.cols;  //利用Mat的数据类型获取

	int top, bottom, left, right;
	top = left = right = padding_size;
	//left = 2 * padding_size;
	//bottom = 0;
	bottom = padding_size;
	Scalar value;
	value = Scalar(0, 0, 0);
	copyMakeBorder(copyImage, imgOut, top, bottom, left, right, BORDER_CONSTANT, value);
	//cout << "imgOut.row:" << imgOut.rows << endl;
	//cout << "imgOut.col:" << imgOut.cols << endl;

}