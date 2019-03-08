#include "stdafx.h"
#include "debugAdd.h"

//设置滑竿，就函数，已不使用
void setTrackbar()
{
	//设定初始值
	int TFTHOLD = 130;
	int DUSTHOLD = 50;
	int DUSTSHAPE = 7;
	int dst_m_gradient = 100; //梯度异常判断阈值
	int dst_m_grayScale = 8; //灰度异常判断阈值
	int dst_s_gradient = 20;
	int dst_s_grayScale = 12; //灰度异常判断阈值
	int num_gradient = 10;    //梯度异常总数判断阈值
	int num_grayScale = 10;  //灰度异常总数判断阈值
	int fill = 20;  //灰度异常总数判断阈值
	int meanValueGradient = 20;

	namedWindow("canshu", 0);
	//createTrackbar("dst_m_gradient", "canshu", &dst_m_gradient, 255, on_trackbar, &sendImg1);
	//createTrackbar("dst_m_grayScale", "canshu", &dst_m_grayScale, 255, on_trackbar, &sendImg1);
	//createTrackbar("dst_s_gradient", "canshu", &dst_s_gradient, 355, on_trackbar, &sendImg1);
	//createTrackbar("dst_s_grayScale", "canshu", &dst_s_grayScale, 355, on_trackbar, &sendImg1);
	//createTrackbar("num_gradient", "canshu", &num_gradient, 200, on_trackbar, &sendImg1);
	//createTrackbar("num_grayScale", "canshu", &num_grayScale, 200, on_trackbar, &sendImg1);
	//createTrackbar("meanValueGradient", "canshu", &meanValueGradient, 100, on_trackbar, &sendImg1);

	//createTrackbar("TFT", "canshu", &TFTHOLD, 255, on_trackbar_1, &sendImg1);
	//createTrackbar("DUST", "canshu", &DUSTHOLD, 355, on_trackbar_1, &sendImg1);
	//createTrackbar("DUSTshape", "canshu", &DUSTSHAPE, 255, on_trackbar_1, &sendImg1);
	//createTrackbar("fill", "canshu", &fill, 255, on_trackbar_1, &sendImg1);

}


//设置分区域调整的参数的滑动窗口
void setMaskTrackbar()
{


	/*

int keneralsize = 20;
int size_x_left = 10;
int size_x_right = 20;
int size_y_up = 20;
int size_y_down = 20;


	*/
	int pointKeneralsize = 20;
	int pointsize_x_left = 10;
	int pointsize_x_right = 20;
	int pointsize_y_up = 20;
	int pointsize_y_down = 20;

	int muraKeneralsize = 20;
	int murasize_x_left = 10;
	int murasize_x_right = 20;
	int murasize_y_up = 20;
	int murasize_y_down = 20;



	namedWindow("canshuMask", 0);
	//createTrackbar("pKeneralsize", "canshuMask", &pointKeneralsize, 30, on_trackbar3, &sendImg1);
	//createTrackbar("psize_x_left", "canshuMask", &pointsize_x_left, 100, on_trackbar3, &sendImg1);
	//createTrackbar("psize_x_right", "canshuMask", &pointsize_x_right, 100, on_trackbar3, &sendImg1);
	//createTrackbar("psize_y_up", "canshuMask", &pointsize_y_up, 100, on_trackbar3, &sendImg1);
	//createTrackbar("psize_y_down", "canshuMask", &pointsize_y_down, 100, on_trackbar3, &sendImg1);
	//


	//
	//createTrackbar("TKeneralsize", "canshuMask", &muraKeneralsize, 30, on_trackbar3, &sendImg1);
	//createTrackbar("Tsize_x_left", "canshuMask", &murasize_x_left, 100, on_trackbar3, &sendImg1);
	//createTrackbar("Tsize_x_right", "canshuMask", &murasize_x_right, 100, on_trackbar3, &sendImg1);
	//createTrackbar("Tsize_y_up", "canshuMask", &murasize_y_up, 100, on_trackbar3, &sendImg1);
	//createTrackbar("Tsize_y_down", "canshuMask", &murasize_y_down, 100, on_trackbar3, &sendImg1);


}

//设置滤除假缺陷的滑动窗口
void setremoveFalseTrackbar()
{


	/*

	int dst_m_gradient = 100; //梯度异常判断阈值
	int dst_m_grayScale = 8; //灰度异常判断阈值
	int dst_s_gradient = 20;
	int dst_s_grayScale = 3; //灰度异常判断阈值
	int num_gradient = 4;    //梯度异常总数判断阈值
	int num_grayScale = 5;  //灰度异常总数判断阈值
	int meanValueGradient = 20;


	*/

	int point_num_gradient = 4;    //梯度异常总数判断阈值
	int point_num_grayScale = 5;  //灰度异常总数判断阈值
	int point_dst_m_gradient = 100; //梯度异常判断阈值
	int point_dst_m_grayScale = 8; //灰度异常判断阈值
	int point_dst_s_gradient = 20;
	int point_dst_s_grayScale = 3; //灰度异常判断阈值
	int point_meanValueGradient = 20;



	int mura_num_gradient = 4;    //梯度异常总数判断阈值
	int mura_num_grayScale = 5;  //灰度异常总数判断阈值
	int mura_dst_m_gradient = 100; //梯度异常判断阈值
	int mura_dst_m_grayScale = 8; //灰度异常判断阈值
	int mura_dst_s_gradient = 20;
	int mura_dst_s_grayScale = 3; //灰度异常判断阈值
	int mura_meanValueGradient = 20;



	namedWindow("removeFalse", 0);

	//createTrackbar("pnum_gradient", "removeFalse", &point_num_gradient, 200, on_trackbar4, &sendImg1);
	//createTrackbar("pnum_grayScale", "removeFalse", &point_num_grayScale, 200, on_trackbar4, &sendImg1);
	//createTrackbar("pdst_m_gradient", "removeFalse", &point_dst_m_gradient, 255, on_trackbar4, &sendImg1);
	//createTrackbar("pdst_m_grayScale", "removeFalse", &point_dst_m_grayScale, 255, on_trackbar4, &sendImg1);
	//createTrackbar("pdst_s_gradient", "removeFalse", &point_dst_s_gradient, 355, on_trackbar4, &sendImg1);
	//createTrackbar("pdst_s_grayScale", "removeFalse", &point_dst_s_grayScale, 355, on_trackbar4, &sendImg1);
	//createTrackbar("pmeanValueGradient", "removeFalse", &point_num_grayScale, 100, on_trackbar4, &sendImg1);

	//createTrackbar("mnum_gradient", "removeFalse", &mura_num_gradient, 200, on_trackbar4, &sendImg1);
	//createTrackbar("mnum_grayScale", "removeFalse", &mura_num_grayScale, 200, on_trackbar4, &sendImg1);
	//createTrackbar("mdst_m_gradient", "removeFalse", &mura_dst_m_gradient, 255, on_trackbar4, &sendImg1);
	//createTrackbar("mdst_m_grayScale", "removeFalse", &mura_dst_m_grayScale, 255, on_trackbar4, &sendImg1);
	//createTrackbar("mdst_s_gradient", "removeFalse", &mura_dst_s_gradient, 355, on_trackbar4, &sendImg1);
	//createTrackbar("mdst_s_grayScale", "removeFalse", &mura_dst_s_grayScale, 355, on_trackbar4, &sendImg1);
	//createTrackbar("mmeanValueGradient", "removeFalse", &mura_num_grayScale, 100, on_trackbar4, &sendImg1);


}

//读取参数
void setCanshuUseTrackbar()
{

	int TFTHOLD = getTrackbarPos("TFT", "canshu");
	int DUSTHOLD = getTrackbarPos("DUST", "canshu");
	int DUSTSHAPE = getTrackbarPos("DUSTshape", "canshu");

	int num_gradient = getTrackbarPos("num_gradient", "canshu");
	int num_grayScale = getTrackbarPos("num_grayScale", "canshu");

	int dst_m_gradient = getTrackbarPos("dst_m_gradient", "canshu");
	int dst_m_grayScale = getTrackbarPos("dst_m_grayScale", "canshu");

	int  dst_s_gradient = getTrackbarPos("dst_s_gradient", "canshu");
	int  dst_s_grayScale = getTrackbarPos("dst_s_grayScale", "canshu");

	int fill = getTrackbarPos("fill", "canshu");
	int  meanValueGradient = getTrackbarPos("meanValueGradient", "canshu");
	int  bright_num = getTrackbarPos("bright_num", "canshu");
	int  dark_num = getTrackbarPos("dark_num", "canshu");

	CanShu canshu(TFTHOLD, DUSTHOLD, DUSTSHAPE, num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, fill, meanValueGradient, dst_s_gradient, dst_s_grayScale, bright_num, dark_num);
}

//读取分区域设置阈值参数
void setCanshuMaskUseTrackbar()
{



	int pointKeneralsize = getTrackbarPos("pKeneralsize", "canshuMask");
	int pointsize_x_left = getTrackbarPos("psize_x_left", "canshuMask");
	int pointsize_x_right = getTrackbarPos("psize_x_right", "canshuMask");
	int pointsize_y_up = getTrackbarPos("psize_y_up", "canshuMask");
	int pointsize_y_down = getTrackbarPos("psize_y_down", "canshuMask");

	int muraKeneralsize = getTrackbarPos("TKeneralsize", "canshuMask");
	int murasize_x_left = getTrackbarPos("Tsize_x_left", "canshuMask");
	int murasize_x_right = getTrackbarPos("Tsize_x_right", "canshuMask");
	int murasize_y_up = getTrackbarPos("Tsize_y_up", "canshuMask");
	int murasize_y_down = getTrackbarPos("Tsize_y_down", "canshuMask");
	;
	(*PointCanShuMask).initCanShuMask(pointKeneralsize, pointsize_x_left, pointsize_x_right, pointsize_y_up, pointsize_y_down);
	(*MuraCanShuMask).initCanShuMask(muraKeneralsize, murasize_x_left, murasize_x_right, murasize_y_up, murasize_y_down);
}

//读取去除假缺陷参数
void UseremoveFalseTrackbar()
{

	int pnum_gradient = getTrackbarPos("pnum_gradient", "removeFalse");    //梯度异常总数判断阈值
	int pnum_grayScale = getTrackbarPos("pnum_grayScale", "removeFalse");  //灰度异常总数判断阈值
	int pdst_m_gradient = getTrackbarPos("pdst_m_gradient", "removeFalse"); //梯度异常判断阈值
	int pdst_m_grayScale = getTrackbarPos("pdst_m_grayScale", "removeFalse"); //灰度异常判断阈值
	int pdst_s_gradient = getTrackbarPos("pdst_s_gradient", "removeFalse");
	int pdst_s_grayScale = getTrackbarPos("pdst_s_grayScale", "removeFalse"); //灰度异常判断阈值
	int pmeanValueGradient = getTrackbarPos("pmeanValueGradient", "removeFalse");
	//读取mura缺陷的remove参数值

	int mnum_gradient = getTrackbarPos("mnum_gradient", "removeFalse");    //梯度异常总数判断阈值
	int mnum_grayScale = getTrackbarPos("mnum_grayScale", "removeFalse");  //灰度异常总数判断阈值
	int mdst_m_gradient = getTrackbarPos("mdst_m_gradient", "removeFalse"); //梯度异常判断阈值
	int mdst_m_grayScale = getTrackbarPos("mdst_m_grayScale", "removeFalse"); //灰度异常判断阈值
	int mdst_s_gradient = getTrackbarPos("mdst_s_gradient", "removeFalse");
	int mdst_s_grayScale = getTrackbarPos("mdst_s_grayScale", "removeFalse"); //灰度异常判断阈值
	int mmeanValueGradient = getTrackbarPos("mmeanValueGradient", "removeFalse");
	int bright_num = getTrackbarPos("bright_num", "removeFalse");
	int dark_num = getTrackbarPos("dark_num", "removeFalse");



	(*PointCanShureMoveFalse).initCanShureMoveFalse(pnum_gradient, pnum_grayScale, pdst_m_gradient, pdst_m_grayScale, pdst_s_gradient, pdst_s_grayScale, pmeanValueGradient, bright_num, dark_num);
	(*muraCanShureMoveFalse).initCanShureMoveFalse(mnum_gradient, mnum_grayScale, mdst_m_gradient, mdst_m_grayScale, mdst_s_gradient, mdst_s_grayScale, mmeanValueGradient, bright_num, dark_num);




}


//显示处理过程中的一些图片`
void showProcessImg(sendImg * sendImg2)
{

	namedWindow("imgBorder", 0);
	imshow("imgBorder", (*sendImg2).BorderTFT);


	namedWindow("fill_image", 0);
	imshow("fill_image", (*sendImg2).imgTftColorCutFill);



	namedWindow("gradientDUST", 0);
	imshow("gradientDUST", (*sendImg2).gradientDUST);


	//显示梯度二值化图片
	namedWindow("gradientTftThreshold", 0);
	imshow("gradientTftThreshold", (*sendImg2).gradientTftThreshold);

	/*显示缺陷和灰尘图*/
	//namedWindow("gradientDustThreshold", 0);
	//imshow("gradientDustThreshold", (*sendImg2).gradientDustThreshold);

	//膨胀二值化的灰尘梯度图，方便查看灰尘
	//namedWindow("ShowDustColorCut", 0);
	//imshow("ShowDustColorCut", (*sendImg2).imgDustColorCut);

	//显示差值二值化后的结果
	namedWindow("imgresult", 0);
	imshow("imgresult", (*sendImg2).gradientResultThreshold);

	//在彩图上显示缺陷检测结果
	namedWindow("showResult", 0);
	imshow("showResult", (*sendImg2).tftResultShow);

};

//
void showProcessImg()
{

	namedWindow("imgBorder", 0);
	imshow("imgBorder", (*sendImg1).BorderTFT);


	namedWindow("fill_image", 0);
	imshow("fill_image", (*sendImg1).imgTftColorCutFill);



	namedWindow("gradientDUST", 0);
	imshow("gradientDUST", (*sendImg1).gradientDUST);


	//显示梯度二值化图片
	//namedWindow("gradientTftThreshold", 0);
	//imshow("gradientTftThreshold", (*sendImg1).gradientTftThreshold);

	/*显示缺陷和灰尘图*/
	//namedWindow("gradientDustThreshold", 0);
	//imshow("gradientDustThreshold", (*sendImg2).gradientDustThreshold);

	//膨胀二值化的灰尘梯度图，方便查看灰尘
	//namedWindow("ShowDustColorCut", 0);
	//imshow("ShowDustColorCut", (*sendImg2).imgDustColorCut);

	//显示差值二值化后的结果
	//namedWindow("imgresult", 0);
	//imshow("imgresult", (*sendImg1).gradientResultThreshold);

	////在彩图上显示缺陷检测结果
	namedWindow("showResult", 0);
	imshow("showResult", (*sendImg1).tftResultShow_final);

};

//保存所有检测图片
//void saveImg1()
//{
//
//	imwrite((*sendImg1).sNowTimeFileDir + "imgTFT.bmp", (*sendImg1).imgTFT);
//	imwrite((*sendImg1).sNowTimeFileDir + "imgDust.bmp", (*sendImg1).imgDust);
//	imwrite((*sendImg1).sNowTimeFileDir + " BorderTFT.bmp", (*sendImg1).BorderTFT);
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientTFT.bmp", (*sendImg1).gradientTFT);
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientDUST.bmp", (*sendImg1).gradientDUST);
//	imwrite((*sendImg1).sNowTimeFileDir + "imgDust.bmp", (*sendImg1).imgDust);
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientResultThreshold.bmp", (*sendImg1).gradientResultThreshold);
//	imwrite((*sendImg1).sNowTimeFileDir + " tftResultShow.bmp", (*sendImg1).tftResultShow);
//	imwrite((*sendImg1).sNowTimeFileDir + " imgTftColorCutFill.bmp", (*sendImg1).imgTftColorCutFill);
//
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientTftThreshold.bmp", (*sendImg1).gradientTftThreshold);
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientDustThresholDilate.bmp", (*sendImg1).gradientDustThresholDilate);
//
//	imwrite((*sendImg1).sNowTimeFileDir + " BorderTFT.bmp", (*sendImg1).BorderTFT);
//	imwrite((*sendImg1).sNowTimeFileDir + " gradientResult.bmp", (*sendImg1).gradientResult);
//
//}

