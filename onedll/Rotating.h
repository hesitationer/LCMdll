/*
所有与矫正相关的函数

*/


#pragma once
#include "onedll.h"

//获取图片的角度偏移信息
RotatedRect get_img_rotate_info(Mat& image, bool show_img = false);
void img_rotate(Mat& img, Point2f box[4], RotatedRect &rect, Mat &img_out, int cameraFlag, bool show_process = false, bool returnCutOrNotCutImg = true);
void  shrink_poly(Point2f poly[4], vector<Point2f>& new_poly, int size_y);
void sort_points(vector<Point>&poly, map<string, vector<Point>>&final_poly, vector<int>&dx);
void cross_point(Vec4i line1, Vec4i line2, Point2f& res_p);
void hough_detect(Mat& image, vector<Point2f>& res_points, vector<int>sort_dxy, int binary_thres = 70, bool show_img = false);
void getRoi(int cameraFlag, bool isBuildtemplate = false);
void camera_cali_fisrt(Mat imageSource, float neican_data[], float jibian_data[], Mat newimage);
void find_R_cornor(Mat pic, vector<Point2f> pt, int size, string path, vector<string> &res, bool show_img);
// 以一张标准的模板创建R角检测所需的图
// 相机畸变矫正代码，用在所有输入图的第一步处理
void detectRCornorInit(int cameraFlag);
void compare_R_cornor(vector<string> cor1, vector<string> cor2, string path);

void getRCornorResult(int cameraFlag);
vector<Point2f> getRoi1(int cameraFlag);