#include "stdafx.h"
#include "fill.h"

//��ȡ�߽緽��1
Mat border(Mat& img)
{
	Mat copyImage = img.clone();

	int w = copyImage.rows;
	int h = copyImage.cols;  //����Mat���������ͻ�ȡ

	int new_h = h + 10;
	int new_w = w + 10;

	//����һ��new_h*new_wͼ�����,����Ԫ��Ϊ3ͨ���޷�������
	//Mat padImg(new_h,new_w,CV_8UC3,Scalar(255,255,255));
	Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
	//cout << padImg.size() << endl;
	copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));
	Rect ccomp; //����ˮ����еĲ����Ĳ��䡣
	Mat mask = Mat::zeros(Size(new_h + 2, new_w + 2), CV_8UC1);
	int area = floodFill(padImg, mask, Point(0, 0), Scalar(255, 255, 255), &ccomp, Scalar(100, 100, 100), Scalar(50, 50, 50), FLOODFILL_FIXED_RANGE);
	Mat temp_padImg = padImg(Rect(5, 5, h, w));
	Mat BorderTFT;
	cvtColor(temp_padImg, BorderTFT, COLOR_BGR2GRAY);
	Mat blurred;
	GaussianBlur(BorderTFT, blurred, Size(5, 5), 0); //��˹ƽ��
	Mat mask_2;
	double ret1 = cv::threshold(blurred, mask_2, 254, 255, THRESH_BINARY);
	//double ret2 = cv::threshold(high_score_img, high_score_img, 250, 255, cv::THRESH_BINARY);
	Mat mask_dilate;
	Mat mask_erode;
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(11, 11));
	dilate(mask_2, mask_dilate, kernel1);
	erode(mask_2, mask_erode, kernel1);

	Mat img1;
	absdiff(mask_dilate, mask_erode, img1);
	Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, Size(9, 9));
	dilate(img1, img1, kernel2);
	return img1;
}

//��ȡ�߽緽��2
Mat border111(Mat& img)
{
	Mat copyImage = img.clone();

	int w = copyImage.rows;
	int h = copyImage.cols;  //����Mat���������ͻ�ȡ

	int new_h = h + 10;
	int new_w = w + 10;

	//�Դ����ͼƬ����Ļ�����������䣨255,255,255)��תΪ�Ҷ�ͼBorderTFT
	Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
	//cout << padImg.size() << endl;
	copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));
	Rect ccomp; //����ˮ����еĲ����Ĳ��䡣
	Mat mask = Mat::zeros(Size(new_h + 2, new_w + 2), CV_8UC1);
	int area = floodFill(padImg, mask, Point(0, 0), Scalar(255, 255, 255), &ccomp, Scalar(100, 100, 100), Scalar(50, 50, 50), FLOODFILL_FIXED_RANGE);
	Mat temp_padImg = padImg(Rect(5, 5, h, w));
	Mat BorderTFT;
	cvtColor(temp_padImg, BorderTFT, COLOR_BGR2GRAY);

	//imwrite("D:\\project\\BorderTFT.bmp", BorderTFT);
	Mat mask_2;
	//��BorderTFT��ֵ��������(255,255,255)�Ĳ���ȫ��תΪ��ɫ
	double ret1 = cv::threshold(BorderTFT, mask_2, 254, 255, THRESH_BINARY);
	//imwrite("D:\\project\\mask_2.bmp", mask_2);


	Mat mask_3;
	Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, Size(8, 8));
	//��ͼƬ�������ͣ�����߽粿��
	dilate(mask_2, mask_3, kernel2);
	(*sendImg1).mask2 = mask_3.clone();

	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(16, 16));
	dilate(mask_2, mask_2, kernel1);


	//imwrite("D:\\project\\mask2.bmp", (*sendImg1).mask2);
	bitwise_not((*sendImg1).mask2, (*sendImg1).mask1);
	//double ret2 = cv::threshold(BorderTFT, (*sendImg1).mask1, 254, 255, THRESH_BINARY_INV);
	imwrite("D:\\project\\mask1.bmp", (*sendImg1).mask1);
	return  mask_2;
}


Mat border1(Mat& img)
{
	Mat copyImage = img.clone();

	int w = copyImage.rows;
	int h = copyImage.cols;  //����Mat���������ͻ�ȡ

	int new_h = h + 10;
	int new_w = w + 10;

	//�Դ����ͼƬ����Ļ�����������䣨255,255,255)��תΪ�Ҷ�ͼBorderTFT
	Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
	//cout << padImg.size() << endl;
	copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));
	Rect ccomp; //����ˮ����еĲ����Ĳ��䡣
	Mat mask = Mat::zeros(Size(new_h + 2, new_w + 2), CV_8UC1);
	int area = floodFill(padImg, mask, Point(0, 0), Scalar(255, 255, 255), &ccomp, Scalar(100, 100, 100), Scalar(50, 50, 50), FLOODFILL_FIXED_RANGE);
	Mat temp_padImg = padImg(Rect(5, 5, h, w));
	Mat BorderTFT;
	cvtColor(temp_padImg, BorderTFT, COLOR_BGR2GRAY);

	//imwrite("D:\\project\\BorderTFT.bmp", BorderTFT);
	Mat mask_2;
	//��BorderTFT��ֵ��������(255,255,255)�Ĳ���ȫ��תΪ��ɫ
	double ret1 = cv::threshold(BorderTFT, mask_2, 254, 255, THRESH_BINARY);
	//imwrite("D:\\project\\mask_2.bmp", mask_2);


	Mat mask_3;
	Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, Size(8, 8));
	//��ͼƬ�������ͣ�����߽粿��
	dilate(mask_2, mask_3, kernel2);
	(*sendImg1).mask2 = mask_3.clone();

	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(17, 17));
	dilate(mask_2, mask_2, kernel1);


	//imwrite("D:\\project\\mask2.bmp", (*sendImg1).mask2);
	bitwise_not((*sendImg1).mask2, (*sendImg1).mask1);
	//double ret2 = cv::threshold(BorderTFT, (*sendImg1).mask1, 254, 255, THRESH_BINARY_INV);
	imwrite("D:\\project\\mask1.bmp", (*sendImg1).mask1);
	return  mask_2;
}
//���ͼƬ
Mat fill_image(Mat img, int fill) {
	Mat copyImage = img.clone();
	if (!img.data)
	{
		cout << "Error: This image is not built!" << endl;
	}

	//padding pic
	int w = copyImage.rows;
	int h = copyImage.cols;  //����Mat���������ͻ�ȡ

	int new_h = h + 10;
	int new_w = w + 10;
	//cout << new_h << endl;
	//cout << new_w << endl;

	//����һ��new_h*new_wͼ�����,����Ԫ��Ϊ3ͨ���޷�������
	Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
	Mat padImg1 = Mat::zeros(Size(h, w), CV_8UC3);

	//��copyimage������ֵ�˲�
	//blur(copyImage, copyImage, Size(7, 7));
	//medianBlur(copyImage, copyImage, 7);
	//Mat border;
	//add(copyImage, padImg1, border,(*sendImg1).mask2);
	////imwrite("D:\\project\\border.bmp", border);

	Mat center;
	add(img, padImg1, center, (*sendImg1).mask1);
	//imwrite("D:\\project\\center.bmp", center);

	//add(center, border, copyImage);
	//imwrite("D:\\project\\copyimage.bmp", copyImage);


	//��copyimage�滻��padImg��5,5������w+5,h+5����С��λ��
	//copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));
	copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));//����center�������
	//center.copyTo(padImg(Range(new_w-100, new_w +100), Range(new_h-100, new_h + 100)));//����center�������
	//��ȡ�����ɫ
	Mat colorFillArea = copyImage(Rect(50, 50, 50, 50));
	//Mat colorFillArea = copyImage(Rect(100, 100, 100, 100));
	cout << "colorFillArea.size:" << colorFillArea.size() << endl;
	//����һ��������ɫ�����飬�˴�ע���Ƿ���Ҫ�޸������С
	int colorList[3];
	int colorFillAreaShape = colorFillArea.cols*colorFillArea.rows;
	cout << "colorFillAreaShape" << colorFillAreaShape << endl;
	//�ֱ����ͼƬ����ͨ������ֵ���������
	int color_area_sum[3] = { 0 };
	for (int r = 0; r < colorFillArea.rows; r++)
	{
		for (int c = 0; c < colorFillArea.cols; c++)
		{
			//��ɫͨ��
			color_area_sum[0] += colorFillArea.at<Vec3b>(r, c)[0];
			//��ɫͨ��
			color_area_sum[1] += colorFillArea.at<Vec3b>(r, c)[1];
			//��ɫͨ��
			color_area_sum[2] += colorFillArea.at<Vec3b>(r, c)[2];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		colorList[i] = (color_area_sum[i] / colorFillAreaShape);
	}

	Rect ccomp; //����ˮ����еĲ����Ĳ��䡣

	//msk��ʾ������ģ��Ϊ��ͨ����8λ�����Ϳ��϶�������ͼ�� image ���������ص��ͼ��
	Mat mask = Mat::zeros(Size(new_h + 2, new_w + 2), CV_8UC1);

	//����floodfill�����������
	int area = floodFill(padImg, mask, Point(0, 0), Scalar(colorList[0], colorList[1], colorList[2]), &ccomp, Scalar(100, 100, 100), Scalar(fill, fill, fill), FLOODFILL_FIXED_RANGE);
	//int area = floodFill(padImg, mask, Point(0, 0), Scalar(85, 78, 81), &ccomp, Scalar(100, 100, 100), Scalar(fill, fill, fill), FLOODFILL_FIXED_RANGE);
	//��ȡ�����ͼƬ
	Mat temp_padImg = padImg(Rect(5, 5, h, w));
	//imwrite("D:\\project\\fill.bmp", temp_padImg);
	return temp_padImg;
}

//wzx ���ͼƬ���·��� 12.21 //
//Mat fill(Mat img, int fill)
//{
//	LARGE_INTEGER nFreq;
//	LARGE_INTEGER nBeginTime;
//	LARGE_INTEGER nEndTime;
//	double time;
//	QueryPerformanceFrequency(&nFreq);
//	QueryPerformanceCounter(&nBeginTime);
//
//	Mat copyImage = img.clone();
//	imwrite("D://project//copyImage.bmp", copyImage);
//	if (!img.data)
//	{
//		cout << "Error: This image is not built!" << endl;
//	}
//
//	int w = copyImage.rows;
//	int h = copyImage.cols;
//	cout << "777777777" << endl;
//	//int new_h = h + 10;
//	//int new_w = w + 10;
//	////�Դ����ͼƬ����Ļ�����������䣨255,255,255)��תΪ�Ҷ�ͼBorderTFT
//	//Mat padImg = Mat::zeros(Size(new_h, new_w), CV_8UC3);
//	////cout << padImg.size() << endl;
//	//copyImage.copyTo(padImg(Range(5, w + 5), Range(5, h + 5)));
//	//Rect ccomp; //����ˮ����еĲ����Ĳ��䡣
//	//Mat mask = Mat::zeros(Size(new_h + 2, new_w + 2), CV_8UC1);
//	//int area = floodFill(padImg, mask, Point(0, 0), Scalar(255, 255, 255), &ccomp, Scalar(100, 100, 100), Scalar(50, 50, 50), FLOODFILL_FIXED_RANGE);
//	//Mat BorderTFT;
//	//cvtColor(padImg, BorderTFT, COLOR_BGR2GRAY);
//	//Mat mask2;
//	////��BorderTFT��ֵ��������(255,255,255)�Ĳ���ȫ��תΪ��ɫ
//	//double ret1 = cv::threshold(BorderTFT, mask2, 254, 255, THRESH_BINARY);
//	//mask2 = mask2(Rect(5, 5, h, w));
//	//imwrite("D://project//mask2.bmp", mask2);
//	//Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, Size(7, 7));
//	////��ͼƬ�������ͣ�����߽粿��
//	//dilate(mask2, mask2, kernel2);
//	//imwrite("D://project//mask2_dilate.bmp", mask2);
//	//bitwise_not(mask2, mask2);
//	//imwrite("D://project//mask2.bmp", mask2);
//	//(*sendImg1).mask1 = mask2(Rect(5, 5, h, w));
//
//	Mat padImg1 = Mat::zeros(Size(h, w), CV_8UC3);
//	Mat center;
//	add(img, padImg1, center, (*sendImg1).mask1);
//	imwrite("D://project//center.bmp", center);
//
//	center = copyImage.clone();  //����ע�͵�
//
//	int size = 120;
//
//	Mat R_cornor_cut_1 = center(Rect(0, 0, size, size));
//	imwrite("D://project//R_cornor_cut_1.bmp", R_cornor_cut_1);
//	Mat R_cornor_cut_1_thre = R_cornor_cut_1.clone();
//	threshold(R_cornor_cut_1_thre, R_cornor_cut_1_thre, 50, 255, CV_THRESH_BINARY);
//	imwrite("D://project//R_cornor_cut_1_thre.bmp", R_cornor_cut_1_thre);
//
//	Mat R_cornor_cut_2 = center(Rect(h - size, 0, size, size));
//	imwrite("D://project//R_cornor_cut_2.bmp", R_cornor_cut_2);
//	Mat R_cornor_cut_2_thre = R_cornor_cut_2.clone();
//	threshold(R_cornor_cut_2_thre, R_cornor_cut_2_thre, 50, 255, CV_THRESH_BINARY);
//	imwrite("D://project//R_cornor_cut_1_thre.bmp", R_cornor_cut_1_thre);
//
//	Mat R_cornor_cut_3 = center(Rect(h - size, w - size, size, size));
//	imwrite("D://project//R_cornor_cut_3.bmp", R_cornor_cut_3);
//	Mat R_cornor_cut_3_thre = R_cornor_cut_3.clone();
//	threshold(R_cornor_cut_3_thre, R_cornor_cut_3_thre, 50, 255, CV_THRESH_BINARY);
//	imwrite("D://project//R_cornor_cut_3_thre.bmp", R_cornor_cut_3_thre);
//
//	Mat R_cornor_cut_4 = center(Rect(0, w - size, size, size));
//	imwrite("D://project//R_cornor_cut_4.bmp", R_cornor_cut_4);
//	Mat R_cornor_cut_4_thre = R_cornor_cut_4.clone();
//	threshold(R_cornor_cut_4_thre, R_cornor_cut_4_thre, 50, 255, CV_THRESH_BINARY);
//	//imwrite("D://project//R_cornor_cut_1.bmp", R_cornor_cut_1);
//	imwrite("D://project//R_cornor_cut_4_thre.bmp", R_cornor_cut_4_thre);
//
//	Mat liuhai = center(Rect(0, 300, 100, 500));
//	imwrite("D://project//liuhai.bmp", liuhai);
//	Mat liuhai_thre = liuhai.clone();
//	threshold(liuhai_thre, liuhai_thre, 50, 255, CV_THRESH_BINARY);
//	imwrite("D://project//liuhai_thre.bmp", liuhai_thre);
//	//
//	//Mat left1 = center(Rect(0, size, 10, 300 - size));
//	//imwrite("D://project//left1.bmp", left1);
//	//Mat left1_thre = left1.clone();
//	//threshold(left1_thre, left1_thre, 20, 255, CV_THRESH_BINARY);
//	////imwrite("D://project//liuhai_thre.bmp", liuhai_thre);
//	//
//	//Mat left2 = center(Rect(0, 800, 10, w-size-800));
//	//imwrite("D://project//left2.bmp", left2);
//	//Mat left2_thre = left2.clone();
//	//threshold(left2_thre, left2_thre, 20, 255, CV_THRESH_BINARY);
//	////imwrite("D://project//liuhai_thre.bmp", liuhai_thre);
//	//
//	//Mat right = center(Rect(h - 10, size, 10, w - 2 * size)).clone();
//	//imwrite("D://project//right.bmp", right);
//	//Mat right_thre = right.clone();
//	//threshold(right_thre, right_thre, 20, 255, CV_THRESH_BINARY);
//	//imwrite("D://project//right_thre.bmp", right_thre);
//
//	//�������
//	for (int i0 = 0; i0 < 500; i0++)
//	{
//		int loc0 = 100;
//		for (int j0 = 0; j0 < 100; j0++)
//		{
//			if (liuhai_thre.at<Vec3b>(i0, j0)[0] == 255 || liuhai_thre.at<Vec3b>(i0, j0)[1] == 255 || liuhai_thre.at<Vec3b>(i0, j0)[2] == 255)
//			{
//				if (j0 < loc0)
//				{
//					loc0 = j0;
//				}
//			}
//		}
//
//		if (loc0 != 0)
//		{
//			loc0 = loc0 + 7;    //7
//		}
//		else
//		{
//			loc0 = loc0 + 3;
//		}
//
//		Scalar color0 = liuhai.at<Vec3b>(i0, loc0);
//		//cout << "88888" << endl;
//		//cout << "("<<i0<<","<<loc0<<")" << endl;
//		//while (1);
//		//cout << color0 << endl;
//		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
//		for (int k0 = 0; k0 < loc0; k0++)
//		{
//			liuhai.at<Vec3b>(i0, k0)[0] = color0(0);
//			liuhai.at<Vec3b>(i0, k0)[1] = color0(1);
//			liuhai.at<Vec3b>(i0, k0)[2] = color0(2);
//		}
//	}
//	imwrite("D://project//liuhai_fill.bmp", liuhai);
//	cout << "ok" << endl;
//	//���R_cornor_cut_1
//	for (int i = 0; i < size; i++)
//	{
//		int loc = 120;
//		for (int j = 0; j < size; j++)
//		{
//			if (R_cornor_cut_1_thre.at<Vec3b>(i, j)[0] == 255 || R_cornor_cut_1_thre.at<Vec3b>(i, j)[1] == 255 || R_cornor_cut_1_thre.at<Vec3b>(i, j)[2] == 255)
//			{
//				if (j < loc)
//				{
//					loc = j;
//				}
//			}
//		}
//		if (loc != 0)
//		{
//			loc = loc + 7;
//		}
//		else
//		{
//			loc = loc + 3;
//		}
//
//		Scalar color = R_cornor_cut_1.at<Vec3b>(i, loc);
//		//cout << "("<<i<<","<<loc<<")" << endl;
//		cout << color << endl;
//		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
//		for (int k = 0; k < loc; k++)
//		{
//			R_cornor_cut_1.at<Vec3b>(i, k)[0] = color(0);
//			R_cornor_cut_1.at<Vec3b>(i, k)[1] = color(1);
//			R_cornor_cut_1.at<Vec3b>(i, k)[2] = color(2);
//		}
//	}
//	imwrite("D://project//R_cornor_cut_1_fill.bmp", R_cornor_cut_1);
//
//	//���R_cornor_cut_2
//	for (int i2 = 0; i2 < size; i2++)
//	{
//		int loc2 = 0;
//		for (int j2 = size - 1; j2 > 0; j2--)
//		{
//			if (R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[0] == 255 || R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[1] == 255 || R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[2] == 255)
//			{
//				if (j2 > loc2)
//				{
//					loc2 = j2;
//				}
//			}
//		}
//
//		if (loc2 != size - 1)
//		{
//			loc2 = loc2 - 7;
//		}
//		else
//		{
//			loc2 = loc2 - 3;
//		}
//
//		Scalar color2 = R_cornor_cut_2.at<Vec3b>(i2, loc2);
//		//cout << "("<<i<<","<<loc<<")" << endl;
//		//cout << color << endl;
//		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
//		for (int k2 = size - 1; k2 > loc2; k2--)
//		{
//			R_cornor_cut_2.at<Vec3b>(i2, k2)[0] = color2(0);
//			R_cornor_cut_2.at<Vec3b>(i2, k2)[1] = color2(1);
//			R_cornor_cut_2.at<Vec3b>(i2, k2)[2] = color2(2);
//		}
//	}
//	imwrite("D://project//R_cornor_cut_2_fill.bmp", R_cornor_cut_2);
//
//	//���R_cornor_cut_3
//	for (int i3 = 0; i3 < size; i3++)
//	{
//		int loc3 = 0;
//		for (int j3 = size - 1; j3 >0; j3--)
//		{
//			if (R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[0] == 255 || R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[1] == 255 || R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[2] == 255)
//			{
//				if (j3 > loc3)
//				{
//					loc3 = j3;
//				}
//			}
//		}
//
//		if (loc3 != size - 1)
//		{
//			loc3 = loc3 - 7;
//		}
//		else
//		{
//			loc3 = loc3 - 3;
//		}
//		Scalar color = R_cornor_cut_3.at<Vec3b>(i3, loc3);
//		//cout << "("<<i<<","<<loc<<")" << endl;
//		//cout << color << endl;
//		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
//		for (int k3 = size - 1; k3 > loc3; k3--)
//		{
//			R_cornor_cut_3.at<Vec3b>(i3, k3)[0] = color(0);
//			R_cornor_cut_3.at<Vec3b>(i3, k3)[1] = color(1);
//			R_cornor_cut_3.at<Vec3b>(i3, k3)[2] = color(2);
//		}
//	}
//	imwrite("D://project//R_cornor_cut_3_fill.bmp", R_cornor_cut_3);
//
//	//���R_cornor_cut_4
//
//
//	for (int i = 0; i < size; i++)
//	{
//		int loc4 = 120;
//		for (int j = 0; j < size; j++)
//		{
//			if (R_cornor_cut_4_thre.at<Vec3b>(i, j)[0] == 255 || R_cornor_cut_4_thre.at<Vec3b>(i, j)[1] == 255 || R_cornor_cut_4_thre.at<Vec3b>(i, j)[2] == 255)
//			{
//				if (j < loc4)
//				{
//					loc4 = j;
//				}
//			}
//		}
//		if (loc4 != 0)
//		{
//			loc4 = loc4 + 7;
//		}
//		else
//		{
//			loc4 = loc4 + 3;
//		}
//
//		Scalar color = R_cornor_cut_4.at<Vec3b>(i, loc4);
//		//cout << "("<<i<<","<<loc4<<")" << endl;
//		//cout << color << endl;
//		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
//		for (int k = 0; k < loc4; k++)
//		{
//			R_cornor_cut_4.at<Vec3b>(i, k)[0] = color(0);
//			R_cornor_cut_4.at<Vec3b>(i, k)[1] = color(1);
//			R_cornor_cut_4.at<Vec3b>(i, k)[2] = color(2);
//		}
//	}
//	imwrite("D://project//R_cornor_cut_4_fill.bmp", R_cornor_cut_4);
//
//
//
//	QueryPerformanceCounter(&nEndTime);
//	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
//	printf("%f\n", time);
//
//	imwrite("D://project//center_fill.bmp", center);
//	return center;
//}

Mat fill(Mat img, int fill)
{
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);

	Mat copyImage = img.clone();
	imwrite("D://project//copyImage.bmp", copyImage);
	if (!img.data)
	{
		cout << "Error: This image is not built!" << endl;
	}

	int w = copyImage.rows;
	int h = copyImage.cols;

	//Mat padImg1 = Mat::zeros(Size(h, w), CV_8UC3);
	Mat center;
	//add(img, padImg1, center, (*sendImg1).mask1);
	//imwrite("D://project//center.bmp", center);

	center = copyImage.clone();  //����ע�͵�

	int size = 120;

	Mat R_cornor_cut_1 = center(Rect(0, 0, size, size));
	imwrite("D://project//R_cornor_cut_1.bmp", R_cornor_cut_1);
	Mat R_cornor_cut_1_thre = R_cornor_cut_1.clone();
	threshold(R_cornor_cut_1_thre, R_cornor_cut_1_thre, 50, 255, CV_THRESH_BINARY);
	imwrite("D://project//R_cornor_cut_1_thre.bmp", R_cornor_cut_1_thre);

	Mat R_cornor_cut_2 = center(Rect(h - size, 0, size, size));
	imwrite("D://project//R_cornor_cut_2.bmp", R_cornor_cut_2);
	Mat R_cornor_cut_2_thre = R_cornor_cut_2.clone();
	threshold(R_cornor_cut_2_thre, R_cornor_cut_2_thre, 50, 255, CV_THRESH_BINARY);
	imwrite("D://project//R_cornor_cut_1_thre.bmp", R_cornor_cut_1_thre);

	Mat R_cornor_cut_3 = center(Rect(h - size, w - size, size, size));
	imwrite("D://project//R_cornor_cut_3.bmp", R_cornor_cut_3);
	Mat R_cornor_cut_3_thre = R_cornor_cut_3.clone();
	threshold(R_cornor_cut_3_thre, R_cornor_cut_3_thre, 50, 255, CV_THRESH_BINARY);
	imwrite("D://project//R_cornor_cut_3_thre.bmp", R_cornor_cut_3_thre);

	Mat R_cornor_cut_4 = center(Rect(0, w - size, size, size));
	imwrite("D://project//R_cornor_cut_4.bmp", R_cornor_cut_4);
	Mat R_cornor_cut_4_thre = R_cornor_cut_4.clone();
	threshold(R_cornor_cut_4_thre, R_cornor_cut_4_thre, 50, 255, CV_THRESH_BINARY);
	//imwrite("D://project//R_cornor_cut_1.bmp", R_cornor_cut_1);
	imwrite("D://project//R_cornor_cut_4_thre.bmp", R_cornor_cut_4_thre);

	Mat liuhai = center(Rect(0, 300, 100, 500));
	imwrite("D://project//liuhai.bmp", liuhai);
	Mat liuhai_thre = liuhai.clone();
	threshold(liuhai_thre, liuhai_thre, 50, 255, CV_THRESH_BINARY);
	imwrite("D://project//liuhai_thre.bmp", liuhai_thre);
	//


	//�������
	for (int i0 = 0; i0 < 500; i0++)
	{
		int loc0 = 100;
		for (int j0 = 0; j0 < 100; j0++)
		{
			if (liuhai_thre.at<Vec3b>(i0, j0)[0] == 255 || liuhai_thre.at<Vec3b>(i0, j0)[1] == 255 || liuhai_thre.at<Vec3b>(i0, j0)[2] == 255)
			{
				if (j0 < loc0)
				{
					loc0 = j0;
				}
			}
		}

		if (loc0 != 0)
		{
			loc0 = loc0 + 10;    //7
		}
		else
		{
			loc0 = loc0 + 3;
		}

		Scalar color0 = liuhai.at<Vec3b>(i0, loc0);
		//cout << "("<<i0<<","<<loc0<<")" << endl;
		//while (1);
		//cout << color0 << endl;
		//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
		for (int k0 = 0; k0 < loc0; k0++)
		{
			liuhai.at<Vec3b>(i0, k0)[0] = color0(0);
			liuhai.at<Vec3b>(i0, k0)[1] = color0(1);
			liuhai.at<Vec3b>(i0, k0)[2] = color0(2);
		}
	}
	imwrite("D://project//liuhai_fill.bmp", liuhai);

	//���R_cornor_cut_1
	for (int i = 0; i < size; i++)
	{
		int loc = 120;
		for (int j = 0; j < size; j++)
		{
			if (R_cornor_cut_1_thre.at<Vec3b>(i, j)[0] == 255 || R_cornor_cut_1_thre.at<Vec3b>(i, j)[1] == 255 || R_cornor_cut_1_thre.at<Vec3b>(i, j)[2] == 255)
			{
				if (j < loc)
				{
					loc = j;
				}
			}
		}
		if (loc != 119)
		{
			if (loc != 0)
			{
				loc = loc + 7;
			}
			else
			{
				loc = loc + 3;
			}
			if (loc > 119)
			{
				loc = 119;
			}
			Scalar color = R_cornor_cut_1.at<Vec3b>(i, loc);
			//cout << "("<<i<<","<<loc<<")" << endl;
			//cout << color << endl;
			//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
			for (int k = 0; k < loc; k++)
			{
				R_cornor_cut_1.at<Vec3b>(i, k)[0] = color(0);
				R_cornor_cut_1.at<Vec3b>(i, k)[1] = color(1);
				R_cornor_cut_1.at<Vec3b>(i, k)[2] = color(2);
			}
		}
	}
	imwrite("D://project//R_cornor_cut_1_fill.bmp", R_cornor_cut_1);

	//���R_cornor_cut_2
	for (int i2 = 0; i2 < size; i2++)
	{
		int loc2 = 0;
		for (int j2 = size - 1; j2 > 0; j2--)
		{
			if (R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[0] == 255 || R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[1] == 255 || R_cornor_cut_2_thre.at<Vec3b>(i2, j2)[2] == 255)
			{
				if (j2 > loc2)
				{
					loc2 = j2;
				}
			}
		}

		if (loc2 != 0)
		{
			if (loc2 != size - 1)
			{
				loc2 = loc2 - 7;
			}
			else
			{
				loc2 = loc2 - 3;
			}
			if (loc2 < 0)
			{
				loc2 = 0;
			}
			Scalar color2 = R_cornor_cut_2.at<Vec3b>(i2, loc2);
			//cout << "("<<i<<","<<loc<<")" << endl;
			//cout << color << endl;
			//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
			for (int k2 = size - 1; k2 > loc2; k2--)
			{
				R_cornor_cut_2.at<Vec3b>(i2, k2)[0] = color2(0);
				R_cornor_cut_2.at<Vec3b>(i2, k2)[1] = color2(1);
				R_cornor_cut_2.at<Vec3b>(i2, k2)[2] = color2(2);
			}
		}

	}
	imwrite("D://project//R_cornor_cut_2_fill.bmp", R_cornor_cut_2);

	//���R_cornor_cut_3
	for (int i3 = 0; i3 < size; i3++)
	{
		int loc3 = 0;
		for (int j3 = size - 1; j3 > 0; j3--)
		{
			if (R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[0] == 255 || R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[1] == 255 || R_cornor_cut_3_thre.at<Vec3b>(i3, j3)[2] == 255)
			{
				if (j3 > loc3)
				{
					loc3 = j3;
				}
			}
		}
		if (loc3 != 0)
		{
			if (loc3 != size - 1)
			{
				loc3 = loc3 - 7;
			}
			else
			{
				loc3 = loc3 - 3;
			}
			if (loc3 < 0)
			{
				loc3 = 0;
			}
			Scalar color = R_cornor_cut_3.at<Vec3b>(i3, loc3);
			//cout << "("<<i<<","<<loc<<")" << endl;
			//cout << color << endl;
			//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
			for (int k3 = size - 1; k3 > loc3; k3--)
			{
				R_cornor_cut_3.at<Vec3b>(i3, k3)[0] = color(0);
				R_cornor_cut_3.at<Vec3b>(i3, k3)[1] = color(1);
				R_cornor_cut_3.at<Vec3b>(i3, k3)[2] = color(2);
			}
		}

	}
	imwrite("D://project//R_cornor_cut_3_fill.bmp", R_cornor_cut_3);

	//���R_cornor_cut_4


	for (int i = 0; i < size; i++)
	{
		int loc4 = 120;
		for (int j = 0; j < size; j++)
		{
			if (R_cornor_cut_4_thre.at<Vec3b>(i, j)[0] == 255 || R_cornor_cut_4_thre.at<Vec3b>(i, j)[1] == 255 || R_cornor_cut_4_thre.at<Vec3b>(i, j)[2] == 255)
			{
				if (j < loc4)
				{
					loc4 = j;
				}
			}
		}
		if (loc4 != 119)
		{
			if (loc4 != 0)
			{
				loc4 = loc4 + 7;
			}
			else
			{
				loc4 = loc4 + 3;
			}
			if (loc4 > 119)
			{
				loc4 = 119;
			}
			Scalar color = R_cornor_cut_4.at<Vec3b>(i, loc4);
			//cout << "("<<i<<","<<loc4<<")" << endl;
			//cout << color << endl;
			//cout << "(" << R_cornor_cut_1.at<Vec3b>(i, loc)[0] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[1] << "," << R_cornor_cut_1.at<Vec3b>(i, loc)[2] << ")" << endl;
			for (int k = 0; k < loc4; k++)
			{
				R_cornor_cut_4.at<Vec3b>(i, k)[0] = color(0);
				R_cornor_cut_4.at<Vec3b>(i, k)[1] = color(1);
				R_cornor_cut_4.at<Vec3b>(i, k)[2] = color(2);
			}
		}

	}
	imwrite("D://project//R_cornor_cut_4_fill.bmp", R_cornor_cut_4);


	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	printf("%f\n", time);

	imwrite("D://project//center_fill.bmp", center);
	return center;
}