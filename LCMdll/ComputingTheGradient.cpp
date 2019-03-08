/*
计算图像梯度
12.3日lqy添加漏光检测代码
*/
#include "stdafx.h"
#include "LCMdll.h"
#include "padding.h"

//将彩色图像转为灰度图，同时将彩图分别拆分为第三个通道额灰度图
void spiltImgTFT(sendImg * sendImg2)

{
	//GaussianBlur((*sendImg2).imgTftColorCut, (*sendImg2).imgTftColorCut, Size(5, 5), 0, 0);
	Mat img_bgr[3];
	//(*sendImg2).imgTftColorCut = imread("D://project//vs2017//colorTFT_crop.bmp");
	split((*sendImg2).imgTftColorCutFill, img_bgr);

	cvtColor((*sendImg2).imgTftColorCutFill, (*sendImg2).imgTFT_gray, CV_BGR2GRAY);  // 彩图转为灰度图
	(*sendImg2).TFTb = img_bgr[0]; // 拆分出b通道
	(*sendImg2).TFTg = img_bgr[1]; // 拆分出g通道
	(*sendImg2).TFTr = img_bgr[2]; // 拆分出r通道
}


//利用9*9梯度算子计算图片梯度
Mat gradiecnt_channel_0(Mat image)
{
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
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

	Mat gradienttft;
	cv::filter2D(image, gradienttft, image.depth(), kernel2);
	return gradienttft;
}

Mat gradiecnt_channel_1(Mat image)
{


	Mat gradienttft, gradienttft_x, gradienttft_y;
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//namedWindow("GussianBlur", 0);
	//imshow("GussianBlur", image);
	/*Mat kernel2 = (Mat_<char>(3, 3) <<
	-1,-1,-1,
	-1,8,-1,
	-1,-1,-1);*/
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

	Mat gradienttft1;
	cv::filter2D(image, gradienttft1, image.depth(), kernel2);
	//namedWindow("gradienttft1", 0);
	//imshow("gradienttft1", gradienttft1);

	blur(image, image, Size(5, 5));
	Sobel(image, gradienttft_x, CV_16S, 1, 0, 5, 1, 1, BORDER_DEFAULT);
	Sobel(image, gradienttft_y, CV_16S, 0, 1, 5, 1, 1, BORDER_DEFAULT);

	//只保留正值
	gradienttft_x.convertTo(gradienttft_x, CV_8U);
	gradienttft_y.convertTo(gradienttft_y, CV_8U);

	//bitwise_or(gradienttft_x, gradienttft_y, gradienttft);
	//blur(gradienttft, gradienttft, Size(5, 5));
	//namedWindow("gradient_x", 0);
	//imshow("gradient_x", gradienttft_x);
	//namedWindow("gradient_y", 0);
	//imshow("gradient_y", gradienttft_y);
	//bitwise_or(gradienttft, gradienttft1, gradienttft);
	bitwise_or(gradienttft1, gradienttft_y, gradienttft);
	blur(gradienttft, gradienttft, Size(5, 5));

	//namedWindow("gradienttft", 0);
	//imshow("gradienttft", gradienttft);


	return gradienttft;
}

//利用sobel算子计算图片梯度
Mat gradient_sobel_channel1(Mat image)
{
	//GaussianBlur(image, image, Size(3,3),0,0,BORDER_DEFAULT);
	//对于x、y方向分别计算梯度
	Mat gradient_x, gradient_y;
	Sobel(image, gradient_x, CV_16S, 1, 0, 3);
	Sobel(image, gradient_y, CV_16S, 0, 1, 3);
	//计算绝对值
	convertScaleAbs(gradient_x, gradient_x);
	convertScaleAbs(gradient_y, gradient_y);
	Mat gradientTFT;
	//bitwise_or(gradient_x, gradient_y, gradientTFT);
	//将两个方向的梯度加权相加
	addWeighted(gradient_x, 0.5, gradient_y, 0.5, 0, gradientTFT);
	return gradientTFT;
}

Mat gradiecnt_channel_scharr_1(Mat image)
{

	Mat gradienttft, gradienttft_x, gradienttft_y;
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	namedWindow("GussianBlur", 0);
	imshow("GussianBlur", image);

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

	Mat gradienttft1;
	cv::filter2D(image, gradienttft1, image.depth(), kernel2);
	namedWindow("gradienttft1", 0);
	imshow("gradienttft1", gradienttft1);
	//scharr算子
	Scharr(image, gradienttft_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	Scharr(image, gradienttft_y, CV_16S, 0, 1, 0, 1, BORDER_DEFAULT);
	convertScaleAbs(gradienttft_x, gradienttft_x);
	convertScaleAbs(gradienttft_y, gradienttft_y);
	bitwise_or(gradienttft_x, gradienttft_y, gradienttft);
	//bitwise_or(gradienttft, gradienttft1, gradienttft);

	namedWindow("gradient_x", 0);
	imshow("gradient_x", gradienttft_x);
	namedWindow("gradient_y", 0);
	imshow("gradient_y", gradienttft_y);


	namedWindow("gradienttft", 0);
	imshow("gradienttft", gradienttft);

	return gradienttft;
}
//计算梯度
void gradient_sobel_channel3_or(sendImg * sendImg2)
{
	//对灰度图以及b，g，r三个通道图像分别进行梯度计算
	(*sendImg2).gradientTFT = gradiecnt_channel_1((*sendImg2).imgTFT_gray);
	(*sendImg2).gradientTFTb = gradiecnt_channel_1((*sendImg2).TFTb);
	(*sendImg2).gradientTFTg = gradiecnt_channel_1((*sendImg2).TFTg);
	(*sendImg2).gradientTFTr = gradiecnt_channel_1((*sendImg2).TFTr);
	//imwrite("D:\\project\\TFTr.bmp", (*sendImg2).gradientTFTr);
	//imwrite("D:\\project\\TFTb.bmp", (*sendImg2).gradientTFTb);
	//imwrite("D:\\project\\TFTg.bmp", (*sendImg2).gradientTFTg);
	//对处理结果进行“或”运算
	Mat img_gray_or_b, img_g_or_r;
	bitwise_or((*sendImg2).gradientTFT, (*sendImg2).gradientTFTb, img_gray_or_b);
	bitwise_or((*sendImg2).gradientTFTg, (*sendImg2).gradientTFTr, img_g_or_r);
	bitwise_or(img_gray_or_b, img_g_or_r, (*sendImg2).gradientTFT_or);
	//(*sendImg2).gradientTFT = (*sendImg2).gradientTFT;
	//imwrite("D:\\project\\gradientTFT_or.bmp", (*sendImg2).gradientTFT_or);
}
//12.19 添加漏光检测部分代码，修改函数接口
//输出一个二值化之前的图片和二值化之后的图片
//void getLightLeakArea(Mat &imgIn, Mat &imgOut1, Mat &imgOut2, int cameraflag, int imgflag)
//{
//	int thresh = 20;       //RGB方式二值化阈值开放
//	int kernelsize1 = 500;
//	int kernelsize2 = 20;
//
//	/******RGB方法********/
//	Mat channels[3];
//	split(imgIn, channels);//分割imgTftColor_mura的通道
//						   //b通道
//
//	Mat RcutB;
//	Mat BcutG;
//	Mat GcutR;
//
//	Mat RcutB1;
//	Mat BcutG1;
//	Mat GcutR1;
//
//	absdiff(channels[2], channels[0], RcutB);
//	cv::threshold(RcutB, RcutB1, 10, 255, CV_THRESH_TOZERO);  //100
//	absdiff(channels[0], channels[1], BcutG);
//	cv::threshold(BcutG, BcutG1, 10, 255, CV_THRESH_TOZERO);  //100
//	absdiff(channels[1], channels[2], GcutR);
//	cv::threshold(GcutR, GcutR1, 10, 255, CV_THRESH_TOZERO);  //100
//
//	Mat addcutfinal1;
//	add(RcutB1, BcutG1, addcutfinal1);
//	add(addcutfinal1, GcutR1, addcutfinal1);
//	imwrite("D://project//addcutfinal1.jpg", addcutfinal1);
//
//	/*结果2*/
//	Mat addcutfinal1_after;
//	cv::threshold(addcutfinal1, addcutfinal1_after, (*canshuX)._bgrThresh[cameraflag][imgflag], 255, THRESH_BINARY);  //100
//	imwrite("D://project//addcutfinal1_after.jpg", addcutfinal1_after);;
//
//	Mat addcutfinal;
//	add(RcutB, BcutG, addcutfinal);
//	add(addcutfinal, GcutR, addcutfinal);
//	imwrite("D://project//addcutfinal.jpg", addcutfinal);
//	cv::threshold(addcutfinal, addcutfinal, (*canshuX)._bgrThresh[cameraflag][imgflag], 255, THRESH_BINARY);  //100
//	imwrite("D://project//addcutfinal_after.jpg", addcutfinal);
//
//	Mat orcutfinal;
//	bitwise_or(RcutB, BcutG, orcutfinal);
//	bitwise_or(orcutfinal, GcutR, orcutfinal);
//	cv::threshold(orcutfinal, orcutfinal, 35, 255, THRESH_BINARY);  //100
//	imwrite("D://project//orcutfinal.jpg", orcutfinal);
//
//
//	Mat b500;
//	Mat b30;
//	Mat bsub;
//	blur(channels[0], b500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
//	blur(channels[0], b30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
//	absdiff(b500, b30, bsub);
//	//g通道
//	Mat g500;
//	Mat g30;
//	Mat gsub;
//	blur(channels[1], g500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
//	blur(channels[1], g30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数
//	absdiff(g500, g30, gsub);
//	//r通道
//	Mat r500;
//	Mat r30;
//	Mat rsub;
//	blur(channels[2], r500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
//	blur(channels[2], r30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
//	absdiff(r500, r30, rsub);
//
//	Mat result;
//	add(bsub, gsub, result);
//	add(result, rsub, result);
//
//	imwrite("D://project//zRGBresult.jpg", result);
//
//	cv::threshold(bsub, bsub, thresh, 255, THRESH_BINARY);  //100
//	cv::threshold(gsub, gsub, thresh, 255, THRESH_BINARY);  //100
//	cv::threshold(rsub, rsub, thresh, 255, THRESH_BINARY);  //100
//
//	imwrite("D://project//zb.jpg", channels[0]);
//	imwrite("D://project//zg.jpg", channels[1]);
//	imwrite("D://project//zr.jpg", channels[2]);
//
//	imwrite("D://project//zb30.jpg", b30);
//	imwrite("D://project//zg30.jpg", g30);
//	imwrite("D://project//zr30.jpg", r30);
//
//	imwrite("D://project//zb500.jpg", b500);
//	imwrite("D://project//zg500.jpg", g500);
//	imwrite("D://project//zr500.jpg", r500);
//
//	imwrite("D://project//zbsub.jpg", bsub);
//	imwrite("D://project//zgsub.jpg", gsub);
//	imwrite("D://project//zrsub.jpg", rsub);
//
//	//Mat result;
//	Mat resultthreshold;
//	add(bsub, gsub, resultthreshold);
//	add(resultthreshold, rsub, resultthreshold);
//	imwrite("D://project//zRGBresult_thre.png", resultthreshold);
//
//
//
//	//cv::threshold(b0_50, b0_50threshold, 20, 255, THRESH_BINARY);  //100
//	//cv::threshold(g0_50, g0_50threshold, 20, 255, THRESH_BINARY);  //100
//	//cv::threshold(r0_50, r0_50threshold, 20, 255, THRESH_BINARY);  //100
//	//cv::threshold(result, resultthreshold, 0, 255, THRESH_BINARY);  //100
//	//waitKey(0);
//	//split( (*sendImg1).imgTFT, LAB[3]);  // 分离original
//	// = cvtColor(imgTFT, cv2.COLOR_BGR2LAB)
//
//	/*HSV方法*/
//	int threshHSV = 30;        //HSV二值化阈值开放
//	Mat HSV, HSV3[3];
//	cvtColor(imgIn, HSV, CV_BGR2HSV);
//	split(HSV, HSV3);//分割imgTftColor_mura的通道
//	imwrite("D://project//zH.jpg", HSV3[0]);
//	imwrite("D://project//zS.jpg", HSV3[1]);
//	imwrite("D://project//zV.jpg", HSV3[2]);
//
//	Mat H500;
//	Mat H30;
//	blur(HSV3[0], H500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
//	blur(HSV3[0], H30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
//	imwrite("D://project//zH500.jpg", H500);
//	imwrite("D://project//zH30.jpg", H30);
//
//	absdiff(H500, H30, H30);
//	imwrite("D://project//zHsub.jpg", H30);
//	Mat resultHSVandRGB;
//	add(H30, result, resultHSVandRGB);
//
//	/*结果1*/
//	cv::threshold(H30, H30, (canshuX)->_hsvThresh[cameraflag][imgflag], 255, THRESH_BINARY);  //100
//	imwrite("D://project//zHsub_thre.jpg", H30);
//
//
//	//Mat final;
//	bitwise_or(H30, addcutfinal1_after, imgOut2);
//	imgOut1 = imgOut2;
//	imwrite("D://project//zfinal.jpg", imgOut2);
//
//}

void getLightLeakArea(Mat &imgIn, Mat &imgOut1, Mat &imgOut2 ,int cameraflag, int imgflag)
{
	int thresh = (*canshuX)._hsvThresh[cameraflag][imgflag];       //RGB方式二值化阈值开放
	int kernelsize1 = 500;
	int kernelsize2 = 20;

	/******RGB方法********/
	Mat channels[3];
	split(imgIn, channels);//分割imgTftColor_mura的通道
						   //b通道

	/*RGB三通道互动cut方法*/
	Mat RcutB;
	Mat BcutG;
	Mat GcutR;

	Mat RcutB1;
	Mat BcutG1;
	Mat GcutR1;

	absdiff(channels[2], channels[0], RcutB);
	cv::threshold(RcutB, RcutB1, 5, 255, CV_THRESH_TOZERO);  //100
	absdiff(channels[0], channels[1], BcutG);
	cv::threshold(BcutG, BcutG1, 5, 255, CV_THRESH_TOZERO);  //100
	absdiff(channels[1], channels[2], GcutR);
	cv::threshold(GcutR, GcutR1, 5, 255, CV_THRESH_TOZERO);  //100
	Mat addcutfinal1;
	add(RcutB1, BcutG1, addcutfinal1);
	add(addcutfinal1, GcutR1, addcutfinal1);
	//imwrite("D://project//addcutfinal1.jpg", addcutfinal1);
	Mat addcutfinal1_after;
	cv::threshold(addcutfinal1, addcutfinal1_after, 20, 255, THRESH_BINARY);  //100
	//imwrite("D://project//addcutfinal1_after.jpg", addcutfinal1_after);;

	Mat addcutfinal;
	add(RcutB, BcutG, addcutfinal);
	add(addcutfinal, GcutR, addcutfinal);

	cv::threshold(addcutfinal, addcutfinal, 55, 255, THRESH_BINARY);  //100
	//imwrite("D://project//addcutfinal.jpg", addcutfinal);

	Mat orcutfinal;
	bitwise_or(RcutB, BcutG, orcutfinal);
	bitwise_or(orcutfinal, GcutR, orcutfinal);
	cv::threshold(orcutfinal, orcutfinal, 35, 255, THRESH_BINARY);  //100
	//imwrite("D://project//orcutfinal.jpg", orcutfinal);

	/*BGR方法*/
	Mat b500;
	Mat b30;
	Mat bsub;
	blur(channels[0], b500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
	blur(channels[0], b30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
	absdiff(b500, b30, bsub);
	//g通道
	Mat g500;
	Mat g30;
	Mat gsub;
	blur(channels[1], g500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
	blur(channels[1], g30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数
	absdiff(g500, g30, gsub);
	//r通道
	Mat r500;
	Mat r30;
	Mat rsub;
	blur(channels[2], r500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
	blur(channels[2], r30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
	absdiff(r500, r30, rsub);

	Mat result;
	add(bsub, gsub, result);
	add(result, rsub, result);

	//imwrite("D://project//zRGBresult.jpg", result);

	cv::threshold(bsub, bsub, thresh, 255, THRESH_BINARY);  //100
	cv::threshold(gsub, gsub, thresh, 255, THRESH_BINARY);  //100
	cv::threshold(rsub, rsub, thresh, 255, THRESH_BINARY);  //100

	//imwrite("D://project//zb.jpg", channels[0]);
	//imwrite("D://project//zg.jpg", channels[1]);
	//imwrite("D://project//zr.jpg", channels[2]);

	//imwrite("D://project//zb30.jpg", b30);
	//imwrite("D://project//zg30.jpg", g30);
	//imwrite("D://project//zr30.jpg", r30);

	//imwrite("D://project//zb500.jpg", b500);
	//imwrite("D://project//zg500.jpg", g500);
	//imwrite("D://project//zr500.jpg", r500);

	//imwrite("D://project//zbsub.jpg", bsub);
	//imwrite("D://project//zgsub.jpg", gsub);
	//imwrite("D://project//zrsub.jpg", rsub);

	//Mat result;
	Mat resultthreshold;
	add(bsub, gsub, resultthreshold);
	add(resultthreshold, rsub, resultthreshold);
	//imwrite("D://project//zRGBresult_thre.png", resultthreshold);



	//cv::threshold(b0_50, b0_50threshold, 20, 255, THRESH_BINARY);  //100
	//cv::threshold(g0_50, g0_50threshold, 20, 255, THRESH_BINARY);  //100
	//cv::threshold(r0_50, r0_50threshold, 20, 255, THRESH_BINARY);  //100
	//cv::threshold(result, resultthreshold, 0, 255, THRESH_BINARY);  //100
	//waitKey(0);
	//split( (*sendImg1).imgTFT, LAB[3]);  // 分离original
	// = cvtColor(imgTFT, cv2.COLOR_BGR2LAB)

	/*HSV方法*/
	int threshHSV = 30;        //HSV二值化阈值开放
	Mat HSV, HSV3[3];
	cvtColor(imgIn, HSV, CV_BGR2HSV);
	split(HSV, HSV3);//分割imgTftColor_mura的通道
	//imwrite("D://project//zH.jpg", HSV3[0]);
	//imwrite("D://project//zS.jpg", HSV3[1]);
	//imwrite("D://project//zV.jpg", HSV3[2]);

	Mat H500;
	Mat H30;
	blur(HSV3[0], H500, Size(kernelsize1, kernelsize1));//openCV库自带的均值滤波函数													   
	blur(HSV3[0], H30, Size(kernelsize2, kernelsize2));//openCV库自带的均值滤波函数	
	//imwrite("D://project//zH500.jpg", H500);
	//imwrite("D://project//zH30.jpg", H30);
	absdiff(H500, H30, H30);
	//imwrite("D://project//zHsub.jpg", H30);

	Mat resultHSVandRGB;
	add(H30, result, resultHSVandRGB);

	cv::threshold(H30, H30, threshHSV, 255, THRESH_BINARY);  //100
	//imwrite("D://project//zHsub_thre.jpg", H30);
	//imwrite("D://project//zresultHSVandRGB.jpg", resultHSVandRGB);

	Mat resultHSVandRGB_thre;
	cv::threshold(resultHSVandRGB, resultHSVandRGB_thre, 30, 255, THRESH_BINARY);  //100
	//imwrite("D://project//zresultHSVandRGB_thre.jpg", resultHSVandRGB_thre);

	Mat final;
	bitwise_or(resultHSVandRGB, addcutfinal1, imgOut1);
	//imwrite("D://project//zfinal.jpg", imgOut1);
	//(*sendImg1).GradientResult_louguang

	imgOut1 = resultthreshold.clone();
	imgOut2 = imgOut1;

}




//2.输入时固定大小
//白屏
//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   点，团，漏光
//内缩图片
void drawIn(Mat &imgIn, Mat &imgOut, int shrink)
{
	//4.对ROI区域进行内缩
	int shrink_size = shrink;
	int w_in, h_in;
	w_in = imgIn.rows;
	h_in = imgIn.cols;
	cout << "imgIn.rows:" << w_in << endl;
	cout << "imgIn.cols:" << h_in << endl;
	//imgTFT = imgTFT_out(Range(shrink_size, w_TFT - shrink_size), Range(shrink_size, h_TFT - shrink_size));
	//只內缩左、右和上
	//imgOut = imgIn(Range(shrink_size, w_in ), Range(shrink_size, h_in - shrink_size));
	//现在改为内缩 左、右、上、下
	imgOut = imgIn(Range(shrink_size, w_in - shrink_size), Range(shrink_size, h_in - shrink_size));
	//imgOut = imgIn(Range(2*shrink_size, w_in- shrink_size), Range(shrink_size, h_in - shrink_size));
	cout << "imgOut.rows" << imgOut.rows << "imgOut.cols" << imgOut.cols << endl;

	//获取图片的长宽
	//获取iou区域

}

//void pointDetect(Mat & ingIn ,Mat & imgOut,canshu )       //计算梯度图
//计算点的梯度图
void pointDetect(Mat & imgIn, Mat & imgOut)
{
	Mat image = imgIn.clone();
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
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

	//Mat gradienttft;
	cv::filter2D(image, imgOut, image.depth(), kernel2);


	//Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	//erode(imgOut, imgOut, kernel1);
	// test  0114 add 3*3的滤波 过滤掉小的误检瑕疵，只保留我们本身自己的mura和大的瑕疵
	blur(imgOut, imgOut, Size(3, 3));
	GaussianBlur(imgOut, imgOut, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//cv::medianBlur(imgOut_g, imgOut_g, 3);


}

//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)

//void repadding(Mat & imgIn, Mat & imgOut,int padding)
//{
//	int padding_size = padding;
//	Mat copyImage = imgIn.clone();
//	if (!imgIn.data)
//	{
//		cout << "Error: This image is not built!" << endl;
//	}
//
//	//padding pic
//	//获取待padding图像的尺寸大小
//	int w = copyImage.rows;
//	int h = copyImage.cols;  //利用Mat的数据类型获取
//
//	int new_h = h + padding_size;
//	int new_w = w + padding_size;
//	int top, bottom, left, right;
//	top = left = right = padding_size;
//	Scalar value;
//	value = Scalar(0, 0, 0);
//	copyMakeBorder(copyImage, imgOut, top, bottom, left, right, BORDER_CONSTANT, value);
//	cout << "imgOut.row:" << imgOut.rows << endl;
//	cout << "imgOut.col:" << imgOut.cols << endl;
//
//}
//膨胀
//
//找到缺陷   
//去除假的缺陷(canshu)
//couterPoint


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   点，团，漏光

//void louguangDetect(Mat & ingIn ,Mat & imgOut,canshu)   //计算梯度图
void louguangDetect(Mat & imgIn, Mat & imgOut)
{
	Mat image = imgIn.clone();
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
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

	//Mat gradienttft;
	cv::filter2D(image, imgOut, image.depth(), kernel2);


}
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//膨胀
//
//找到缺陷   
//去除假的缺陷(canshu)
//couterlouguang


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   点，团，漏光
//void muraDetect(MMat & ingIn ,Mat & imgOut,canshu)
//0114 lyl 经过测试，sobel梯度算子 7*7的不合适，结果很花，目前5*5的效果最佳
//0114 修改sobel算子的计算过程 
void muraDetect(Mat & imgIn, Mat & imgOut)
{
	Mat image = imgIn.clone();
	Mat gradienttft, gradienttft_x, gradienttft_y;
	//0114  
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//Mat kernel2 = (Mat_<char>(9, 9) <<
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	//	-1, -1, -1, 8, 8, 8, -1, -1, -1,
	//	-1, -1, -1, 8, 8, 8, -1, -1, -1,
	//	-1, -1, -1, 8, 8, 8, -1, -1, -1,
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	//	-1, -1, -1, -1, -1, -1, -1, -1, -1);

	//Mat gradienttft1;
	//cv::filter2D(image, gradienttft1, image.depth(), kernel2);
	//namedWindow("gradienttft1", 0);
	//imshow("gradienttft1", gradienttft1);

	//0114
	//blur(image, image, Size(5, 5));
	Sobel(image, gradienttft_x, CV_16S, 1, 0, 5, 1, 1, BORDER_DEFAULT);
	Sobel(image, gradienttft_y, CV_16S, 0, 1, 5, 1, 1, BORDER_DEFAULT);

	//只保留正值
	//gradienttft_x.convertTo(gradienttft_x, CV_8U);
	//gradienttft_y.convertTo(gradienttft_y, CV_8U);
	convertScaleAbs(gradienttft_x, gradienttft_x);
	convertScaleAbs(gradienttft_y, gradienttft_y);


	//bitwise_or(gradienttft_x, gradienttft_y, gradienttft);
	//blur(gradienttft, gradienttft, Size(5, 5));
	//namedWindow("gradient_x", 0);
	//imshow("gradient_x", gradienttft_x);
	//namedWindow("gradient_y", 0);
	//imshow("gradient_y", gradienttft_y);
	bitwise_or(gradienttft_x, gradienttft_y, gradienttft);
	blur(gradienttft, gradienttft, Size(5, 5));

	//0114 add  腐蚀
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradienttft, gradienttft, kernel1);
	// test  0114 add 3*3的滤波 过滤掉小的误检瑕疵，只保留我们本身自己的mura和大的瑕疵
	blur(gradienttft, gradienttft, Size(3, 3));
	imgOut = gradienttft.clone();
}
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//膨胀
//
//找到缺陷   
//去除假的缺陷(canshu)
//COUTREmura


//1.4 对 分三通道或的图片进行减去border和灰尘的操作
void subTemplate(Mat &imgIn, Mat &imgOut)
{
	//Mat subtractTemplateTftImg;
	//absdiff((*sendImg1).imgTftColorCut, (*sendImg1).CutTemplate[flag], subtractTemplateTftImg);

	Mat channels[3];

	//必须分通道，建议二值化值设为20或者15

	//分通道或合成一张图片
	split(imgIn, channels);//分割image1的通道
	bitwise_or(channels[0], channels[1], imgOut);
	bitwise_or(channels[2], imgOut, imgOut);

}





//cut    show   countrePoint   blue   show   countrelougunag    yellow  



