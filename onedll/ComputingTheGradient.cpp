#include "stdafx.h"
#include "onedll.h"
#include "ComputingTheGradient.h"


void spiltImgTFT(sendImg * sendImg2)

{
	//GaussianBlur((*sendImg2).imgTftColorCut, (*sendImg2).imgTftColorCut, Size(5, 5), 0, 0);
	Mat img_bgr[3];
	//(*sendImg2).imgTftColorCut = imread("D://project//vs2017//colorTFT_crop.bmp");
	split((*sendImg2).imgTftColorCutFill, img_bgr);

	cvtColor((*sendImg2).imgTftColorCutFill, (*sendImg2).imgTFT_gray, CV_BGR2GRAY);
	(*sendImg2).TFTb = img_bgr[0];
	(*sendImg2).TFTg = img_bgr[1];
	(*sendImg2).TFTr = img_bgr[2];
	//(*sendImg2).TFTb = imread("D://project//vs2017//bb.bmp", 0);
	//(*sendImg2).TFTg = imread("D://project//vs2017//gg.bmp", 0);
	//(*sendImg2).TFTr = imread("D://project//vs2017//rr.bmp", 0);
}

Mat gradiecnt_channel(Mat image)
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

Mat gradient_sobel_channel1(Mat image)
{
	//GaussianBlur(image, image, Size(3,3),0,0,BORDER_DEFAULT);
	Mat gradient_x, gradient_y;
	Sobel(image, gradient_x, CV_16S, 1, 0, 3);
	Sobel(image, gradient_y, CV_16S, 0, 1, 3);
	//计算绝对值
	convertScaleAbs(gradient_x, gradient_x);
	convertScaleAbs(gradient_y, gradient_y);
	Mat gradientTFT;
	//bitwise_or(gradient_x, gradient_y, gradientTFT);
	addWeighted(gradient_x, 0.5, gradient_y, 0.5, 0, gradientTFT);
	return gradientTFT;
}
//11.26
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


void gradient_sobel_channel3_or(sendImg * sendImg2)
{
	(*sendImg2).gradientTFT = gradiecnt_channel_1((*sendImg2).imgTFT_gray);
	(*sendImg2).gradientTFTb = gradiecnt_channel_1((*sendImg2).TFTb);
	(*sendImg2).gradientTFTg = gradiecnt_channel_1((*sendImg2).TFTg);
	(*sendImg2).gradientTFTr = gradiecnt_channel_1((*sendImg2).TFTr);
	//(*sendImg2).gradientTFT = imread("D://project//vs2017//TFT.bmp", 0);
	//(*sendImg2).gradientTFTb = imread("D://project//vs2017//b.bmp", 0);
	//(*sendImg2).gradientTFTg = imread("D://project//vs2017//g.bmp", 0);
	//(*sendImg2).gradientTFTr = imread("D://project//vs2017//r.bmp", 0);
	Mat img_gray_or_b, img_g_or_r;
	bitwise_or((*sendImg2).gradientTFT, (*sendImg2).gradientTFTb, img_gray_or_b);
	bitwise_or((*sendImg2).gradientTFTg, (*sendImg2).gradientTFTr, img_g_or_r);
	bitwise_or(img_gray_or_b, img_g_or_r, (*sendImg2).gradientTFT_or);
	//(*sendImg2).gradientTFT = (*sendImg2).gradientTFT;
}
