/*
����ͼ���ݶ�
12.3��lqy���©�������
*/
#include "stdafx.h"
#include "LCMdll.h"
#include "padding.h"

//����ɫͼ��תΪ�Ҷ�ͼ��ͬʱ����ͼ�ֱ���Ϊ������ͨ����Ҷ�ͼ
void spiltImgTFT(sendImg * sendImg2)

{
	//GaussianBlur((*sendImg2).imgTftColorCut, (*sendImg2).imgTftColorCut, Size(5, 5), 0, 0);
	Mat img_bgr[3];
	//(*sendImg2).imgTftColorCut = imread("D://project//vs2017//colorTFT_crop.bmp");
	split((*sendImg2).imgTftColorCutFill, img_bgr);

	cvtColor((*sendImg2).imgTftColorCutFill, (*sendImg2).imgTFT_gray, CV_BGR2GRAY);  // ��ͼתΪ�Ҷ�ͼ
	(*sendImg2).TFTb = img_bgr[0]; // ��ֳ�bͨ��
	(*sendImg2).TFTg = img_bgr[1]; // ��ֳ�gͨ��
	(*sendImg2).TFTr = img_bgr[2]; // ��ֳ�rͨ��
}


//����9*9�ݶ����Ӽ���ͼƬ�ݶ�
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

	//ֻ������ֵ
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

//����sobel���Ӽ���ͼƬ�ݶ�
Mat gradient_sobel_channel1(Mat image)
{
	//GaussianBlur(image, image, Size(3,3),0,0,BORDER_DEFAULT);
	//����x��y����ֱ�����ݶ�
	Mat gradient_x, gradient_y;
	Sobel(image, gradient_x, CV_16S, 1, 0, 3);
	Sobel(image, gradient_y, CV_16S, 0, 1, 3);
	//�������ֵ
	convertScaleAbs(gradient_x, gradient_x);
	convertScaleAbs(gradient_y, gradient_y);
	Mat gradientTFT;
	//bitwise_or(gradient_x, gradient_y, gradientTFT);
	//������������ݶȼ�Ȩ���
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
	//scharr����
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
//�����ݶ�
void gradient_sobel_channel3_or(sendImg * sendImg2)
{
	//�ԻҶ�ͼ�Լ�b��g��r����ͨ��ͼ��ֱ�����ݶȼ���
	(*sendImg2).gradientTFT = gradiecnt_channel_1((*sendImg2).imgTFT_gray);
	(*sendImg2).gradientTFTb = gradiecnt_channel_1((*sendImg2).TFTb);
	(*sendImg2).gradientTFTg = gradiecnt_channel_1((*sendImg2).TFTg);
	(*sendImg2).gradientTFTr = gradiecnt_channel_1((*sendImg2).TFTr);
	//imwrite("D:\\project\\TFTr.bmp", (*sendImg2).gradientTFTr);
	//imwrite("D:\\project\\TFTb.bmp", (*sendImg2).gradientTFTb);
	//imwrite("D:\\project\\TFTg.bmp", (*sendImg2).gradientTFTg);
	//�Դ��������С�������
	Mat img_gray_or_b, img_g_or_r;
	bitwise_or((*sendImg2).gradientTFT, (*sendImg2).gradientTFTb, img_gray_or_b);
	bitwise_or((*sendImg2).gradientTFTg, (*sendImg2).gradientTFTr, img_g_or_r);
	bitwise_or(img_gray_or_b, img_g_or_r, (*sendImg2).gradientTFT_or);
	//(*sendImg2).gradientTFT = (*sendImg2).gradientTFT;
	//imwrite("D:\\project\\gradientTFT_or.bmp", (*sendImg2).gradientTFT_or);
}
//12.19 ���©���ⲿ�ִ��룬�޸ĺ����ӿ�
//���һ����ֵ��֮ǰ��ͼƬ�Ͷ�ֵ��֮���ͼƬ
//void getLightLeakArea(Mat &imgIn, Mat &imgOut1, Mat &imgOut2, int cameraflag, int imgflag)
//{
//	int thresh = 20;       //RGB��ʽ��ֵ����ֵ����
//	int kernelsize1 = 500;
//	int kernelsize2 = 20;
//
//	/******RGB����********/
//	Mat channels[3];
//	split(imgIn, channels);//�ָ�imgTftColor_mura��ͨ��
//						   //bͨ��
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
//	/*���2*/
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
//	blur(channels[0], b500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
//	blur(channels[0], b30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
//	absdiff(b500, b30, bsub);
//	//gͨ��
//	Mat g500;
//	Mat g30;
//	Mat gsub;
//	blur(channels[1], g500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
//	blur(channels[1], g30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����
//	absdiff(g500, g30, gsub);
//	//rͨ��
//	Mat r500;
//	Mat r30;
//	Mat rsub;
//	blur(channels[2], r500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
//	blur(channels[2], r30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
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
//	//split( (*sendImg1).imgTFT, LAB[3]);  // ����original
//	// = cvtColor(imgTFT, cv2.COLOR_BGR2LAB)
//
//	/*HSV����*/
//	int threshHSV = 30;        //HSV��ֵ����ֵ����
//	Mat HSV, HSV3[3];
//	cvtColor(imgIn, HSV, CV_BGR2HSV);
//	split(HSV, HSV3);//�ָ�imgTftColor_mura��ͨ��
//	imwrite("D://project//zH.jpg", HSV3[0]);
//	imwrite("D://project//zS.jpg", HSV3[1]);
//	imwrite("D://project//zV.jpg", HSV3[2]);
//
//	Mat H500;
//	Mat H30;
//	blur(HSV3[0], H500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
//	blur(HSV3[0], H30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
//	imwrite("D://project//zH500.jpg", H500);
//	imwrite("D://project//zH30.jpg", H30);
//
//	absdiff(H500, H30, H30);
//	imwrite("D://project//zHsub.jpg", H30);
//	Mat resultHSVandRGB;
//	add(H30, result, resultHSVandRGB);
//
//	/*���1*/
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
	int thresh = (*canshuX)._hsvThresh[cameraflag][imgflag];       //RGB��ʽ��ֵ����ֵ����
	int kernelsize1 = 500;
	int kernelsize2 = 20;

	/******RGB����********/
	Mat channels[3];
	split(imgIn, channels);//�ָ�imgTftColor_mura��ͨ��
						   //bͨ��

	/*RGB��ͨ������cut����*/
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

	/*BGR����*/
	Mat b500;
	Mat b30;
	Mat bsub;
	blur(channels[0], b500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
	blur(channels[0], b30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
	absdiff(b500, b30, bsub);
	//gͨ��
	Mat g500;
	Mat g30;
	Mat gsub;
	blur(channels[1], g500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
	blur(channels[1], g30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����
	absdiff(g500, g30, gsub);
	//rͨ��
	Mat r500;
	Mat r30;
	Mat rsub;
	blur(channels[2], r500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
	blur(channels[2], r30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
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
	//split( (*sendImg1).imgTFT, LAB[3]);  // ����original
	// = cvtColor(imgTFT, cv2.COLOR_BGR2LAB)

	/*HSV����*/
	int threshHSV = 30;        //HSV��ֵ����ֵ����
	Mat HSV, HSV3[3];
	cvtColor(imgIn, HSV, CV_BGR2HSV);
	split(HSV, HSV3);//�ָ�imgTftColor_mura��ͨ��
	//imwrite("D://project//zH.jpg", HSV3[0]);
	//imwrite("D://project//zS.jpg", HSV3[1]);
	//imwrite("D://project//zV.jpg", HSV3[2]);

	Mat H500;
	Mat H30;
	blur(HSV3[0], H500, Size(kernelsize1, kernelsize1));//openCV���Դ��ľ�ֵ�˲�����													   
	blur(HSV3[0], H30, Size(kernelsize2, kernelsize2));//openCV���Դ��ľ�ֵ�˲�����	
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




//2.����ʱ�̶���С
//����
//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//����ͼƬ
void drawIn(Mat &imgIn, Mat &imgOut, int shrink)
{
	//4.��ROI�����������
	int shrink_size = shrink;
	int w_in, h_in;
	w_in = imgIn.rows;
	h_in = imgIn.cols;
	cout << "imgIn.rows:" << w_in << endl;
	cout << "imgIn.cols:" << h_in << endl;
	//imgTFT = imgTFT_out(Range(shrink_size, w_TFT - shrink_size), Range(shrink_size, h_TFT - shrink_size));
	//ֻ�������Һ���
	//imgOut = imgIn(Range(shrink_size, w_in ), Range(shrink_size, h_in - shrink_size));
	//���ڸ�Ϊ���� ���ҡ��ϡ���
	imgOut = imgIn(Range(shrink_size, w_in - shrink_size), Range(shrink_size, h_in - shrink_size));
	//imgOut = imgIn(Range(2*shrink_size, w_in- shrink_size), Range(shrink_size, h_in - shrink_size));
	cout << "imgOut.rows" << imgOut.rows << "imgOut.cols" << imgOut.cols << endl;

	//��ȡͼƬ�ĳ���
	//��ȡiou����

}

//void pointDetect(Mat & ingIn ,Mat & imgOut,canshu )       //�����ݶ�ͼ
//�������ݶ�ͼ
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
	// test  0114 add 3*3���˲� ���˵�С�����覴ã�ֻ�������Ǳ����Լ���mura�ʹ��覴�
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
//	//��ȡ��paddingͼ��ĳߴ��С
//	int w = copyImage.rows;
//	int h = copyImage.cols;  //����Mat���������ͻ�ȡ
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
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterPoint


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��

//void louguangDetect(Mat & ingIn ,Mat & imgOut,canshu)   //�����ݶ�ͼ
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
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterlouguang


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void muraDetect(MMat & ingIn ,Mat & imgOut,canshu)
//0114 lyl �������ԣ�sobel�ݶ����� 7*7�Ĳ����ʣ�����ܻ���Ŀǰ5*5��Ч�����
//0114 �޸�sobel���ӵļ������ 
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

	//ֻ������ֵ
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

	//0114 add  ��ʴ
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradienttft, gradienttft, kernel1);
	// test  0114 add 3*3���˲� ���˵�С�����覴ã�ֻ�������Ǳ����Լ���mura�ʹ��覴�
	blur(gradienttft, gradienttft, Size(3, 3));
	imgOut = gradienttft.clone();
}
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//COUTREmura


//1.4 �� ����ͨ�����ͼƬ���м�ȥborder�ͻҳ��Ĳ���
void subTemplate(Mat &imgIn, Mat &imgOut)
{
	//Mat subtractTemplateTftImg;
	//absdiff((*sendImg1).imgTftColorCut, (*sendImg1).CutTemplate[flag], subtractTemplateTftImg);

	Mat channels[3];

	//�����ͨ���������ֵ��ֵ��Ϊ20����15

	//��ͨ����ϳ�һ��ͼƬ
	split(imgIn, channels);//�ָ�image1��ͨ��
	bitwise_or(channels[0], channels[1], imgOut);
	bitwise_or(channels[2], imgOut, imgOut);

}





//cut    show   countrePoint   blue   show   countrelougunag    yellow  



