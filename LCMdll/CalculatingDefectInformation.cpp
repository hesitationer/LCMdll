#include "stdafx.h"
#include "LCMdll.h"
#include "CalculatingDefectInformation.h"


//梯度异常点计数
//===================hyw & lyl=================

//bool count_gradient(RotatedRect rect,CanShureMoveFalse *canshuMoveFalse) {
//	
//	//imwrite("D://project//tftResultShow202020.bmp", (*sendImg1).tftResultShow);
//
//	//引入参数
//	CanShureMoveFalse canshu = *canshuMoveFalse;
//	int dst_m_gradient = canshu._dst_m_gradient; //梯度异常判断阈值（均值）
//	int dst_m_grayScale = canshu._dst_m_grayScale; //灰度异常判断阈值(均值)
//
//	int dst_s_gradient = canshu._dst_s_gradient; //梯度异常判断阈值（标准差）
//	int dst_s_grayScale = canshu._dst_s_grayScale ; //灰度异常判断阈值（标准差）
//
//	int num_gradient = canshu._num_gradient;    //梯度异常总数判断阈值（像素点个数）
//	int num_grayScale = canshu._num_grayScale;  //灰度异常总数判断阈值（像素点个数）
//
//	//******************************梯度异常属性**************************
//	Mat gradient_dst;//声明梯度矩阵
//	Point2f p[4];
//	int x, y, w, h;//声明外接矩形左上角点和宽度、高度
//	int num = 0; //梯度异常像素点个数
//	int pixl;
//	Mat dst_mean, dst_stddev;
//	double dst_m1, dst_s_gra; //梯度均值，标准差
//
//	//*****************************灰度异常属性*****************************
//	Mat gray_dest, mat_mean, mat_stddev;
//	double dst_m, dst_s; //灰度均值和方差
//	int num_g = 0;     //灰度异常像素点个数
//	int pixl_g;
//
//	rect.points(p);
//	int maxX;
//	int maxY;
//	int minX;
//	int minY;
//
//	//=================test sort============
//	int x1 = p[0].x;
//	int y1 = p[0].y;
//	int x2 = p[1].x;
//	int y2 = p[1].y;
//	int x3 = p[2].x;
//	int y3 = p[2].y;
//	int x4 = p[3].x;
//	int y4 = p[3].y;
//	int zuobiaox[4] = { x1,  x2,  x3,  x4 };
//	maxX = *max_element(zuobiaox, zuobiaox + 4);
//	minX = *min_element(zuobiaox, zuobiaox + 4);
//
//	int zuobiaoy[4] = { y1,  y2,  y3,  y4 };
//	maxY = *max_element(zuobiaoy, zuobiaoy + 4);
//	minY = *min_element(zuobiaoy, zuobiaoy + 4);
//	
//	//imwrite("D://project//tftResultShow171717.bmp", (*sendImg1).tftResultShow);
//	(*sendImg1).Max_y = (*sendImg1).imgTftColorCut[0].rows;
//	(*sendImg1).Max_x = (*sendImg1).imgTftColorCut[0].cols;
//	if (maxX > (*sendImg1).Max_x - 1)
//	{
//		maxX = (*sendImg1).Max_x - 1;
//		cout << "maxX:" << maxX;
//	}
//	if (maxY > (*sendImg1).Max_y - 1)
//	{
//		maxY = (*sendImg1).Max_y - 1;
//		cout << "maxY:" << maxY;
//	}
//
//	if (minX < 0)
//	{
//		minX = 0;
//	}
//	if (minY < 0)
//	{
//		minY = 0;
//		//cout << "minY:" << minY;
//	}
//
//	//imwrite("D://project//tftResultShow181818.bmp", (*sendImg1).tftResultShow);
//
//	x = minX;
//	y = minY;
//	w = maxX - minX;
//	h = maxY - minY;
//	Mat dst1 = (*sendImg1).imgTftColorCutFill(Rect(x, y, w, h)).clone();
//
//	Mat img_copy = (*sendImg1).imgTftColorCutFill.clone();
//	Mat img_mean, img_stddev;
//	cvtColor(img_copy, img_copy, CV_RGB2GRAY);
//	int h_img = img_copy.rows;
//	int w_img = img_copy.cols;
//	meanStdDev(img_copy, img_mean, img_stddev);
//	double img_m, img_s;
//	img_m = img_mean.at<double>(0, 0);
//	img_s = img_stddev.at<double>(0, 0);
//	cout << "整张图片的灰度均值为：" << img_m << endl;
//	cout << "整张图片的灰度方差为：" << img_s << endl;
//
//	//imwrite("D://project//tftResultShow191919.bmp", (*sendImg1).tftResultShow);
//
//	Mat img_gradient, img_grad_mean, img_grad_stddev;
//	img_gradient = (*sendImg1).gradientTtfMaskValue.clone();
//	meanStdDev(img_gradient, img_grad_mean, img_grad_stddev);
//	double img_grad_m;
//	img_grad_m = img_grad_mean.at<double>(0, 0);
//	
//	cout << "整张图片的梯度均值为：" << img_grad_m << endl;
//
//
//	//imwrite("D://project//tftResultShow121212.bmp", (*sendImg1).tftResultShow);
//
//
//	//进行梯度算子计算
//	//5*5 Sobel算子
//	gradient_dst = (*sendImg1).gradientTtfMaskValue(Rect(x, y, w, h)).clone();
//	//sendImg2.gradientTFT = gradient_dst.clone();
//	int h1 = gradient_dst.rows;
//	int w1 = gradient_dst.cols;
//	//cout << "瑕疵外接矩形框的长为：" << w1 << endl;
//	//cout << "瑕疵外接矩形框的宽为：" << h1 << endl;
//	//计算梯度矩阵的均值
//	meanStdDev(gradient_dst, dst_mean, dst_stddev);
//	dst_m1 = dst_mean.at<double>(0, 0);
//	dst_s_gra = dst_stddev.at<double>(0, 0);
//	cout << "梯度的均值为：" << dst_m1 << endl;
//	cout << "梯度的标准差为：" << dst_s_gra << endl;
//
//	//imwrite("D://project//tftResultShow131313.bmp", (*sendImg1).tftResultShow);
//
//	//统计梯度异常的像素点个数
//	for (int i = 0; i < h1; i++)
//	{
//		for (int j = 0; j < w1; j++)
//		{
//			pixl = gradient_dst.at<uchar>(i, j);
//			//if (abs(pixl - dst_m1) > dst_m_gradient)
//			if (abs(pixl - img_grad_m) > dst_m_gradient)
//			{
//				num = num + 1;
//			}
//			else
//			{
//				num = num;
//			}
//		}
//	}
//
//	cout << "梯度值异常的像素点的个数为：" << num << endl;
//
//	//imwrite("D://project//tftResultShow141414.bmp", (*sendImg1).tftResultShow);
//
//	//进行灰度异常计算
//	int h1_g = dst1.rows;
//	int w1_g = dst1.cols;
//	//计算灰度图的均值和方差以及对比度
//	gray_dest = Mat::zeros(Size(w1_g, h1_g), CV_8UC3);
//	cvtColor(dst1, gray_dest, CV_RGB2GRAY);
//	meanStdDev(gray_dest, mat_mean, mat_stddev);  //计算灰度图的均值和标准偏差。
//	dst_m = mat_mean.at<double>(0, 0);
//	dst_s = mat_stddev.at<double>(0, 0);
//	cout << "灰度均值是：" << dst_m << endl;
//	cout << "灰度标准差是：" << dst_s << endl;
//	cout << "此区域的对比度是：" << 2 * dst_s << endl;
//
//	//imwrite("D://project//tftResultShow151515.bmp", (*sendImg1).tftResultShow);
//
//	//确定灰度均值异常的阈值
//	for (int i = 0; i < h1_g - 1; i++)
//	{
//		for (int j = 0; j < w1_g - 1; j++)
//		{
//			pixl_g = gray_dest.at<uchar>(i, j);
//			//if (abs(pixl_g - dst_m) >= dst_m_grayScale)
//			//思考dst_m是不是不合理，更换成整张图片的均值是不是更合适
//			if (abs(pixl_g - img_m) >= dst_m_grayScale)
//			{
//				num_g = num_g + 1;
//			}
//			else
//			{
//				num_g = num_g;
//			}
//		}
//	}
//
//	cout << "灰度值异常的像素点个数为：" << num_g << endl;
//
//	//if (num_g < 20)
//	//{
//	//	(*sendImg1).p_result = 1;
//	//}
//	//else
//	//{
//	//	(*sendImg1).l_result = 1;
//	//}
//	//imwrite("D://project//tftResultShow101010.bmp", (*sendImg1).tftResultShow);
//
//	//bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
//	//bool xianzhi2 = (dst_s > dst_s_grayScale) && (dst_s_gra > dst_s_gradient);
//
//
//	bool xianzhi3 = (dst_s_gra > 3.00) && (num >= 6);  //10
//	//bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_s_gra > 0.78) && (dst_s >1.78);			 
//	bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
//	xianzhi1 = xianzhi3 || xianzhi1;
//	bool xianzhi2 = (dst_s > dst_s_grayScale) && (dst_s_gra > dst_s_gradient);
//
//	if (xianzhi1 || xianzhi2)
//	{
//		return true;
//	}
//	else
//	{
//
//		return false;
//
//	}
//}
//保存各个屏幕图像统计信息
/*
double imgInTFTMeans[20];
double imgInTFTStddevs[20];
double 	imgInGradientMeans[20];
double imgInGradientStddevs[20];
*/
//0225 修改此函数 
void count_information_img(Mat &imgInTFT, Mat &imgInGradient, int cameraFlag, int flag)
{
	(*sendImg1).Max_y = (*sendImg1).imgTftColorCut[cameraFlag].rows;
	(*sendImg1).Max_x = (*sendImg1).imgTftColorCut[cameraFlag].cols;
	//计算TFT图片灰度相关信息：均值方差
	double imgInTFTMean, imgInTFTStddev;
	Mat imgInTFT_gray;
	Mat imgInTFT_gray_mean, imgInTFT_gray_stddev;
	//cvtColor(imgInTFT, imgInTFT_gray, CV_RGB2GRAY);
	meanStdDev(imgInTFT_gray, imgInTFT_gray_mean, imgInTFT_gray_stddev);
	(*sendImg1).imgInTFTMeans[cameraFlag][flag] = imgInTFT_gray_mean.at<double>(0, 0);   //整张图片的灰度均值
	(*sendImg1).imgInTFTStddevs[cameraFlag][flag] = imgInTFT_gray_stddev.at<double>(0, 0);  //整张图片的灰度方差





	//计算梯度图片均值方差
	double imgInGradientMean, imgInGradientStddev;
	Mat imgInGradient_gray;
	Mat imgInGradient_gray_mean, imgInGradient_gray_stddev;
	//cvtColor(imgInGradient, imgInGradient_gray, CV_RGB2GRAY);
	meanStdDev(imgInGradient, imgInGradient_gray_mean, imgInGradient_gray_stddev);
	(*sendImg1).imgInGradientMeans[cameraFlag][flag] = imgInGradient_gray_mean.at<double>(0, 0);   //整张图片的灰度均值
	(*sendImg1).imgInGradientStddevs[cameraFlag][flag] = imgInGradient_gray_stddev.at<double>(0, 0);  //整张图片的灰度方差
}

//1.4 lyl update 添加flag 如果flag = true则显示瑕疵信息，false则隐藏瑕疵的具体信息
bool count_gradient1(Mat &gradientIn, Mat &TftIn, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)

{

	defectInformation XiaciInfo;
	//XiaciInfo.h = 60;
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);

	bool result;

	//参数初始化
	CanShureMoveFalse canshu = *canshuMoveFalse;


	//******************************梯度异常属性**************************
	Mat gradient_dst;			//声明梯度矩阵
	Point2f p[4];
	int x, y, w, h;				//声明外接矩形左上角点和宽度、高度
	int num = 0;				//梯度异常像素点个数
	int pixl;
	Mat dst_mean, dst_stddev;	//梯度均值，标准差矩阵
	double dst_m1, dst_s_gra;	//梯度均值，标准差


	//*****************************灰度异常属性*****************************
	Mat gray_des;
	double dst_m, dst_s;		//灰度均值和方差
	int num_g = 0;				 //灰度异常像素点个数
	int pixl_g;

	rect.points(p);
	int maxX;
	int maxY;
	int minX;
	int minY;

	//=================test sort============
	int x1 = p[0].x;
	int y1 = p[0].y;
	int x2 = p[1].x;
	int y2 = p[1].y;
	int x3 = p[2].x;
	int y3 = p[2].y;
	int x4 = p[3].x;
	int y4 = p[3].y;
	int zuobiaox[4] = { x1,  x2,  x3,  x4 };
	maxX = *max_element(zuobiaox, zuobiaox + 4);
	minX = *min_element(zuobiaox, zuobiaox + 4);
	int zuobiaoy[4] = { y1,  y2,  y3,  y4 };
	maxY = *max_element(zuobiaoy, zuobiaoy + 4);
	minY = *min_element(zuobiaoy, zuobiaoy + 4);

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

	x = minX;
	y = minY;
	w = maxX - minX;
	h = maxY - minY;
	int cx = x + w / 2;
	int cy = y + h / 2;


	//计算缺陷区域灰度均值和标准差
	Mat dst1 = TftIn(Rect(minX, minY, w, h)).clone();
	cvtColor(dst1, dst1, CV_RGB2GRAY);
	Mat  mat_mean, mat_stddev;
	meanStdDev(dst1, mat_mean, mat_stddev);  //计算灰度图的均值和标准偏差。
	dst_m = mat_mean.at<double>(0, 0);
	dst_s = mat_stddev.at<double>(0, 0);
	//计算缺陷区域梯度均值和标准差
	Mat img_gradient, img_grad_mean, img_grad_stddev;
	gradient_dst = gradientIn(Rect(x, y, w, h)).clone();
	//gradient_dst = (*sendImg1).GradientResult_point[flag](Rect(x, y, w, h)).clone();
	meanStdDev(gradient_dst, dst_mean, dst_stddev);
	dst_m1 = dst_mean.at<double>(0, 0);  //梯度的均值
	dst_s_gra = dst_stddev.at<double>(0, 0);  //梯度的标准差



	//统计梯度异常的像素点个数
	Mat gradient_dst_too_high, gradient_dst_too_low;
	threshold(gradient_dst, gradient_dst_too_high, (*sendImg1).imgInGradientMeans[cameraFlag][flag] + canshu._dst_m_gradient[cameraFlag][imgFlag], 255, THRESH_BINARY);
	threshold(gradient_dst, gradient_dst_too_low, (*sendImg1).imgInGradientMeans[cameraFlag][flag] - canshu._dst_m_gradient[cameraFlag][imgFlag], 255, THRESH_BINARY_INV);
	bitwise_or(gradient_dst_too_high, gradient_dst_too_low, gradient_dst);
	num = countNonZero(gradient_dst);





	//确定灰度值异常点的个数
	Mat gray_dest_too_high, gray_dest_dst_too_low;
	threshold(dst1, gray_dest_too_high, (*sendImg1).imgInTFTMeans[cameraFlag][flag] + canshu._dst_m_grayScale[cameraFlag][imgFlag], 255, THRESH_BINARY);
	threshold(dst1, gray_dest_dst_too_low, (*sendImg1).imgInTFTMeans[cameraFlag][flag] - canshu._dst_m_grayScale[cameraFlag][imgFlag], 255, THRESH_BINARY_INV);
	bitwise_or(gray_dest_too_high, gray_dest_dst_too_low, dst1);
	num_g = countNonZero(dst1);

	//1.4 是否打印缺陷区域信息
	if (flag)
	{
		cout << "梯度的均值为：" << dst_m1 << endl;
		cout << "梯度的标准差为：" << dst_s_gra << endl;
		cout << "梯度值异常的像素点的个数为：" << num << endl;


		cout << "灰度均值是：" << dst_m << endl;
		cout << "灰度标准差是：" << dst_s << endl;
		cout << "此区域的对比度是：" << 2 * dst_s << endl;
		cout << "灰度值异常的像素点个数为：" << num_g << endl;
		cout << "cx" << cx << endl;
		cout << "cy" << cy << endl;
	}


	//传入我们定义的vector里面


	XiaciInfo.w = w;
	XiaciInfo.h = h;
	XiaciInfo.s_cx = cx;
	XiaciInfo.s_cy = cy;
	XiaciInfo.numGradient = num;
	XiaciInfo.numGray = num_g;
	XiaciInfo.dst_m_Gradient = dst_m1;
	XiaciInfo.dst_m_Gray = dst_m;
	XiaciInfo.dst_s_Gray = dst_s;
	XiaciInfo.dst_s_Gradient = dst_s_gra;
	XiaciInfo.P[0] = p[0];
	XiaciInfo.P[1] = p[1];
	XiaciInfo.P[2] = p[2];
	XiaciInfo.P[3] = p[3];
	//Info = &XiaciInfo;

	(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);















	/*
	去除假缺陷
	dst_s:灰度标准差
	dst_m:灰度均值
	dst_s_gra:梯度的标准差
	dst_m1:梯度的均值
	num:梯度值异常的像素点的个数
	num_g:灰度值异常的像素点个数
	bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
	新添加一个强判定条件，关于梯度均值和梯度异常点数目的   1.4
	*/
	bool xianzhi3 = (dst_s_gra > 3.55) && (num > 10);
	bool xianzhi1 = (num > canshu._num_gradient[cameraFlag][imgFlag]) && (num_g > canshu._num_grayScale[cameraFlag][imgFlag]) && (dst_s_gra > 0.78) && (dst_s > 1.78);
	xianzhi1 = xianzhi3 || xianzhi1;
	bool xianzhi2 = (dst_s > canshu._dst_s_grayScale[cameraFlag][imgFlag]) && (dst_s_gra > canshu._dst_s_gradient[cameraFlag][imgFlag]);
	if (flag)
	{
		cout << xianzhi3 << endl;
		cout << xianzhi1 << endl;
	}

	if (xianzhi1 || xianzhi2)
	{
		result = true;
		return true;

	}
	else
	{
		result = false;
		return false;

	}
	XiaciInfo.TrueOrFalse = result;
	(*defect).push_back(XiaciInfo);

	cout << "88888" << endl;


}

//0118 add lyl
//只统计梯度信息，利用梯度信息进行筛选
bool count_gradient_only(Mat &gradientIn, Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)

{

	defectInformation XiaciInfo;
	//XiaciInfo.h = 60;
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);

	bool result;

	//参数初始化
	CanShureMoveFalse canshu = *canshuMoveFalse;


	//******************************梯度异常属性**************************
	Mat gradient_dst;			//声明梯度矩阵
	Point2f p[4];
	int x, y, w, h;				//声明外接矩形左上角点和宽度、高度
	int num = 0;				//梯度异常像素点个数
	int pixl;
	Mat dst_mean, dst_stddev;	//梯度均值，标准差矩阵
	double dst_m1, dst_s_gra;	//梯度均值，标准差


								//*****************************灰度异常属性*****************************
	Mat gray_des;
	double dst_m, dst_s;		//灰度均值和方差
	int num_g = 0;				 //灰度异常像素点个数
	int pixl_g;

	rect.points(p);
	int maxX;
	int maxY;
	int minX;
	int minY;

	//=================test sort============
	int x1 = p[0].x;
	int y1 = p[0].y;
	int x2 = p[1].x;
	int y2 = p[1].y;
	int x3 = p[2].x;
	int y3 = p[2].y;
	int x4 = p[3].x;
	int y4 = p[3].y;
	int zuobiaox[4] = { x1,  x2,  x3,  x4 };
	maxX = *max_element(zuobiaox, zuobiaox + 4);
	minX = *min_element(zuobiaox, zuobiaox + 4);
	int zuobiaoy[4] = { y1,  y2,  y3,  y4 };
	maxY = *max_element(zuobiaoy, zuobiaoy + 4);
	minY = *min_element(zuobiaoy, zuobiaoy + 4);

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

	x = minX;
	y = minY;
	w = maxX - minX;
	h = maxY - minY;
	int cx = x + w / 2;
	int cy = y + h / 2;


	////计算缺陷区域灰度均值和标准差
	//Mat dst1 = TftIn(Rect(minX, minY, w, h)).clone();
	//cvtColor(dst1, dst1, CV_RGB2GRAY);
	//Mat  mat_mean, mat_stddev;
	//meanStdDev(dst1, mat_mean, mat_stddev);  //计算灰度图的均值和标准偏差。
	//dst_m = mat_mean.at<double>(0, 0);
	//dst_s = mat_stddev.at<double>(0, 0);
	//计算缺陷区域梯度均值和标准差
	Mat img_gradient, img_grad_mean, img_grad_stddev;
	gradient_dst = gradientIn(Rect(x, y, w, h)).clone();

	//gradient_dst = (*sendImg1).GradientResult_point[flag](Rect(x, y, w, h)).clone();
	meanStdDev(gradient_dst, dst_mean, dst_stddev);
	dst_m1 = dst_mean.at<double>(0, 0);  //梯度的均值
	dst_s_gra = dst_stddev.at<double>(0, 0);  //梯度的标准差

	//统计梯度异常的像素点个数
	Mat gradient_dst_too_high, gradient_dst_too_low;
	threshold(gradient_dst, gradient_dst_too_high, (*sendImg1).imgInGradientMeans[cameraFlag][flag] + canshu._meanValueGradient[cameraFlag][imgFlag], 255, THRESH_BINARY);
	threshold(gradient_dst, gradient_dst_too_low, (*sendImg1).imgInGradientMeans[cameraFlag][flag] - canshu._meanValueGradient[cameraFlag][imgFlag], 255, THRESH_BINARY_INV);
	bitwise_or(gradient_dst_too_high, gradient_dst_too_low, gradient_dst);
	num = countNonZero(gradient_dst);

	////确定灰度值异常点的个数
	//Mat gray_dest_too_high, gray_dest_dst_too_low;
	//threshold(dst1, gray_dest_too_high, (*sendImg1).imgInTFTMeans[flag] + canshu._dst_m_grayScale, 255, THRESH_BINARY);
	//threshold(dst1, gray_dest_dst_too_low, (*sendImg1).imgInTFTMeans[flag] - canshu._dst_m_grayScale, 255, THRESH_BINARY_INV);
	//bitwise_or(gray_dest_too_high, gray_dest_dst_too_low, dst1);
	//num_g = countNonZero(dst1);

	//1.4 是否打印缺陷区域信息
	if (flag)
	{
		cout << "梯度的均值为：" << dst_m1 << endl;
		cout << "梯度的标准差为：" << dst_s_gra << endl;
		cout << "梯度值异常的像素点的个数为：" << num << endl;

		//cout << "灰度均值是：" << dst_m << endl;
		//cout << "灰度标准差是：" << dst_s << endl;
		//cout << "此区域的对比度是：" << 2 * dst_s << endl;
		//cout << "灰度值异常的像素点个数为：" << num_g << endl;
		//cout << "cx" << cx << endl;
		//cout << "cy" << cy << endl;
	}


	//传入我们定义的vector里面


	//XiaciInfo.w = w;
	//XiaciInfo.h = h;
	//XiaciInfo.s_cx = cx;
	//XiaciInfo.s_cy = cy;
	//XiaciInfo.numGradient = num;
	//XiaciInfo.dst_m_Gradient = dst_m1;
	//XiaciInfo.dst_s_Gradient = dst_s_gra;
	////XiaciInfo.dst_m_Gray = dst_m;
	////XiaciInfo.dst_s_Gray = dst_s;
	////XiaciInfo.numGray = num_g;
	//XiaciInfo.P[0] = p[0];
	//XiaciInfo.P[1] = p[1];
	//XiaciInfo.P[2] = p[2];
	//XiaciInfo.P[3] = p[3];
	//Info = &XiaciInfo;

	//(*defect).push_back(XiaciInfo);





	/*
	去除假缺陷
	dst_s:灰度标准差
	dst_m:灰度均值
	dst_s_gra:梯度的标准差
	dst_m1:梯度的均值
	num:梯度值异常的像素点的个数
	num_g:灰度值异常的像素点个数
	bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
	新添加一个强判定条件，关于梯度均值和梯度异常点数目的   1.4
	*/
	//bool xianzhi3 = (dst_s_gra > 3.55) && (num>10);
	//bool xianzhi3 = (dst_s_gra> canshu._dst_s_gradient) && (num>  canshu._num_gradient);
	bool xianzhi3 = (dst_s_gra > canshu._dst_s_gradient[cameraFlag][imgFlag]) && (dst_m1 > canshu._dst_m_gradient[cameraFlag][imgFlag]);
	//bool xianzhi1 = (num > canshu._num_gradient) && (dst_s_gra > 0.78) &&(dst_m1 > canshu._dst_m_gradient);
	//bool xianzhi1 = (num > canshu._num_gradient)  &&(dst_m1 > canshu._dst_m_gradient);
	bool xianzhi1 = (num > canshu._num_gradient[cameraFlag][imgFlag]);
	xianzhi1 = xianzhi3 && xianzhi1;
	//bool xianzhi2 =  (dst_s_gra > canshu._dst_s_gradient);
	if (flag)
	{
		cout << "xianzhi1:" << xianzhi1 << endl;
		//cout << "xianzhi2:"<<xianzhi2 << endl;
	}

	if (xianzhi1)
	{
		result = true;
		//return true;

	}
	else
	{
		result = false;

		//bitwise_not(gradientThreshold_dst, );
		//Mat mm = gradientThreshold_dst(Rect(0, 0, gradientThreshold_dst.cols / 2, gradientThreshold_dst.rows));//ROI设置

		//gradientThreshold_dst = { Scalar(0,0,0) };//把ROI中的像素值改为黑色

		Mat gradientThreshold_dst;
		imgOut = gradientThresholdIn.clone();
		gradientThreshold_dst = imgOut(Rect(x, y, w, h));
		gradientThreshold_dst.setTo(0);

		//Mat mask = Mat::zeros(image.size(), CV_8UC1);//zero为与image相同大小的全0图像

		//return false;

	}

	//imgOut = img.clone();
	XiaciInfo.TrueOrFalse = result;
	XiaciInfo.w = w;
	XiaciInfo.h = h;
	XiaciInfo.s_cx = cx;
	XiaciInfo.s_cy = cy;
	XiaciInfo.numGradient = num;
	XiaciInfo.dst_m_Gradient = dst_m1;
	XiaciInfo.dst_s_Gradient = dst_s_gra;

	XiaciInfo.P[0] = p[0];
	XiaciInfo.P[1] = p[1];
	XiaciInfo.P[2] = p[2];
	XiaciInfo.P[3] = p[3];
	//Info = &XiaciInfo;

	//(*defect).push_back(XiaciInfo);





	(*defect).push_back(XiaciInfo);



	//(*defect).clear();
	//defectInformation XiaciInfo1;
	//XiaciInfo.h = 99;
	//(*defect).push_back(XiaciInfo1);


	return result;


}

//只统计灰度信息，利用灰度信息进行筛选
//0119 add 统计信息 lyl
bool count_gray_only(Mat &gradientThresholdIn, Mat &TftIn, Mat &imgOut, RotatedRect rect, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)

{

	defectInformation XiaciInfo;
	//XiaciInfo.h = 60;
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);
	//(*defect).push_back(XiaciInfo);

	bool result;

	//参数初始化
	CanShureMoveFalse canshu = *canshuMoveFalse;


	//******************************梯度异常属性**************************
	Mat gradient_dst;			//声明梯度矩阵
	Point2f p[4];
	int x, y, w, h, S;				//声明外接矩形左上角点和宽度、高度和面积
	int num = 0;				//梯度异常像素点个数
	int pixl;
	Mat dst_mean, dst_stddev;	//梯度均值，标准差矩阵
	double dst_m1, dst_s_gra;	//梯度均值，标准差


								//*****************************灰度异常属性*****************************
	Mat gray_des;
	double dst_m, dst_s;		//灰度均值和方差
	int num_g = 0;				 //灰度异常像素点个数
	int pixl_g;

	rect.points(p);
	int maxX;
	int maxY;
	int minX;
	int minY;

	//=================test sort============
	int x1 = p[0].x;
	int y1 = p[0].y;
	int x2 = p[1].x;
	int y2 = p[1].y;
	int x3 = p[2].x;
	int y3 = p[2].y;
	int x4 = p[3].x;
	int y4 = p[3].y;
	int zuobiaox[4] = { x1,  x2,  x3,  x4 };
	maxX = *max_element(zuobiaox, zuobiaox + 4);
	minX = *min_element(zuobiaox, zuobiaox + 4);
	int zuobiaoy[4] = { y1,  y2,  y3,  y4 };
	maxY = *max_element(zuobiaoy, zuobiaoy + 4);
	minY = *min_element(zuobiaoy, zuobiaoy + 4);

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

	x = minX;
	y = minY;
	w = maxX - minX;
	h = maxY - minY;
	S = w * h;
	int cx = x + w / 2;
	int cy = y + h / 2;


	////计算缺陷区域灰度均值和标准差
	Mat dst1 = TftIn(Rect(minX, minY, w, h)).clone();
	cvtColor(dst1, dst1, CV_RGB2GRAY);
	Mat  mat_mean, mat_stddev;
	meanStdDev(dst1, mat_mean, mat_stddev);  //计算灰度图的均值和标准偏差。
	dst_m = mat_mean.at<double>(0, 0);
	double dst_m_chazhi;
	dst_m_chazhi = dst_m - (*sendImg1).imgInTFTMeans[cameraFlag][imgFlag];
	cout << "dst_m_chazhi:" << dst_m_chazhi << endl;

	dst_s = mat_stddev.at<double>(0, 0);
	cout << "dst_s:" << dst_s << endl;


	//0124 统计二值化图对应的瑕疵区域的异常点的个数
	Mat dst_yichang = gradientThresholdIn(Rect(minX, minY, w, h)).clone();
	int num_yichang = countNonZero(dst_yichang);



	//计算缺陷区域梯度均值和标准差
	//Mat img_gradient, img_grad_mean, img_grad_stddev;
	//gradient_dst = gradientIn(Rect(x, y, w, h)).clone();
	////gradient_dst = (*sendImg1).GradientResult_point[flag](Rect(x, y, w, h)).clone();
	//meanStdDev(gradient_dst, dst_mean, dst_stddev);
	//dst_m1 = dst_mean.at<double>(0, 0);  //梯度的均值
	//dst_s_gra = dst_stddev.at<double>(0, 0);  //梯度的标准差



	//统计极亮点、极暗点的像素点个数
	Mat gray_dst_too_bright, gray_dst_too_dark;
	int bright_num;
	int dark_num;
	int bright_yuzhi, dark_yuzhi;
	bright_yuzhi = (*canshuMoveFalse)._bright_num[cameraFlag][imgFlag];
	dark_yuzhi = (*canshuMoveFalse)._dark_num[cameraFlag][imgFlag];
	cout << "bright_yuzhi:" << (*canshuMoveFalse)._bright_num << endl;
	cout << "dark_yuzhi:" << (*canshuMoveFalse)._bright_num << endl;
	threshold(dst1, gray_dst_too_bright, (*sendImg1).imgInTFTMeans[cameraFlag][imgFlag] + (*canshuMoveFalse)._bright_num[cameraFlag][imgFlag], 255, THRESH_BINARY);
	threshold(dst1, gray_dst_too_dark, (*sendImg1).imgInTFTMeans[cameraFlag][imgFlag] - (*canshuMoveFalse)._dark_num[cameraFlag][imgFlag], 255, THRESH_BINARY_INV);
	//bitwise_or(gray_dst_too_high, gray_dst_too_low, gradient_dst);
	bright_num = countNonZero(gray_dst_too_bright);
	dark_num = countNonZero(gray_dst_too_dark);




	//统计梯度异常的像素点个数
//Mat gradient_dst_too_high, gradient_dst_too_low;
//threshold(gradient_dst, gradient_dst_too_high, (*sendImg1).imgInGradientMeans[flag] + canshu._dst_m_gradient, 255, THRESH_BINARY);
//threshold(gradient_dst, gradient_dst_too_low, (*sendImg1).imgInGradientMeans[flag] - canshu._dst_m_gradient, 255, THRESH_BINARY_INV);
//bitwise_or(gradient_dst_too_high, gradient_dst_too_low, gradient_dst);
//num = countNonZero(gradient_dst);


//确定灰度值异常点的个数
	Mat gray_dest_too_high, gray_dest_dst_too_low;
	threshold(dst1, gray_dest_too_high, (*sendImg1).imgInTFTMeans[cameraFlag][imgFlag] + canshu._dst_m_grayScale[cameraFlag][imgFlag], 255, THRESH_BINARY);
	threshold(dst1, gray_dest_dst_too_low, (*sendImg1).imgInTFTMeans[cameraFlag][imgFlag] - canshu._dst_m_grayScale[cameraFlag][imgFlag], 255, THRESH_BINARY_INV);
	bitwise_or(gray_dest_too_high, gray_dest_dst_too_low, dst1);
	num_g = countNonZero(dst1);
	//int num_g_chazhi = num_g - canshu._num_grayScale;

	num_g = bright_num + dark_num;

	//1.4 是否打印缺陷区域信息
	if (flag)
	{
		//cout << "梯度的均值为：" << dst_m1 << endl;
		//cout << "梯度的标准差为：" << dst_s_gra << endl;
		//cout << "梯度值异常的像素点的个数为：" << num << endl;

		cout << "灰度均值是：" << dst_m << endl;
		cout << "灰度标准差是：" << dst_s << endl;
		cout << "此区域的对比度是：" << 2 * dst_s << endl;
		cout << "灰度值异常的像素点个数为：" << num_g << endl;
		cout << "梯度异常的像素点个数为：" << num_yichang << endl;
		//cout << "cx" << cx << endl;
		//cout << "cy" << cy << endl;
	}


	//传入我们定义的vector里面


	//XiaciInfo.s_cx = cx;
	//XiaciInfo.s_cy = cy;
	//XiaciInfo.numGradient = num;
	//XiaciInfo.dst_m_Gradient = dst_m1;
	//XiaciInfo.dst_s_Gradient = dst_s_gra;



	XiaciInfo.w = w;
	XiaciInfo.h = h;
	XiaciInfo.S = S;
	XiaciInfo.s_cx = cx;
	XiaciInfo.s_cy = cy;
	XiaciInfo.dst_m_Gray = dst_m_chazhi;
	XiaciInfo.dst_s_Gray = dst_s;
	XiaciInfo.numGray = num_g;
	XiaciInfo.bright_num = bright_num;
	XiaciInfo.dark_num = dark_num;
	XiaciInfo.P[0] = p[0];
	XiaciInfo.P[1] = p[1];
	XiaciInfo.P[2] = p[2];
	XiaciInfo.P[3] = p[3];
	XiaciInfo.num_yichang = num_yichang;
	//Info = &XiaciInfo;

	//(*defect).push_back(XiaciInfo);





	/*
	去除假缺陷
	dst_s:灰度标准差
	dst_m:灰度均值
	dst_s_gra:梯度的标准差
	dst_m1:梯度的均值
	num:梯度值异常的像素点的个数
	num_g:灰度值异常的像素点个数
	bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
	新添加一个强判定条件，关于梯度均值和梯度异常点数目的   1.4
	*/
	//bool xianzhi3 = (dst_s_gra > 3.55) && (num>10);

	//bool xianzhi1 = (2 * dst_s > 1.61);
	//bool xianzhi3 = (num_yichang >= 30);   //30之后设定为参数阈值
	//bool xianzhi1 = (num_g > canshu._num_grayScale)  && (dst_s >1.78);
	bool xianzhi1 = (num_g > canshu._num_grayScale[cameraFlag][imgFlag]);
	//xianzhi1 = xianzhi3 && xianzhi1;
	//bool xianzhi2 = (dst_s > canshu._dst_s_grayScale);
	bool xianzhi2 = (dst_s > canshu._dst_s_grayScale[cameraFlag][imgFlag]) && (abs(dst_m_chazhi) > canshu._dst_m_grayScale[cameraFlag][imgFlag]);
	if (flag)
	{
		cout << xianzhi2 << endl;
		cout << xianzhi1 << endl;
	}

	if (xianzhi1 || xianzhi2)
	{
		XiaciInfo.TrueOrFalse = result = true;
		//return true;

	}
	else
	{
		XiaciInfo.TrueOrFalse = result = false;
		Mat gradientThreshold_dst;
		imgOut = gradientThresholdIn.clone();
		gradientThreshold_dst = imgOut(Rect(x, y, w, h));
		gradientThreshold_dst.setTo(0);


		//return false;

	}


	//XiaciInfo.TrueOrFalse = result;
	(*defect).push_back(XiaciInfo);

	cout << "83388383" << endl;
	cout << "83388383" << endl;
	return result;


}