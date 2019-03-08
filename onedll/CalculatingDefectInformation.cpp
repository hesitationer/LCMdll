#include "stdafx.h"
#include "onedll.h"
#include "CalculatingDefectInformation.h"


//�ݶ��쳣�����
//===================hyw & lyl=================



bool count_gradient(RotatedRect rect) {

	//imwrite("D://project//tftResultShow202020.bmp", (*sendImg1).tftResultShow);

	//�������
	CanShu canshu = *canshuX;
	int dst_m_gradient = canshu._dst_m_gradient; //�ݶ��쳣�ж���ֵ����ֵ��
	int dst_m_grayScale = canshu._dst_m_grayScale; //�Ҷ��쳣�ж���ֵ(��ֵ)

	int dst_s_gradient = canshu._dst_s_gradient; //�ݶ��쳣�ж���ֵ����׼�
	int dst_s_grayScale = canshu._dst_s_grayScale; //�Ҷ��쳣�ж���ֵ����׼�

	int num_gradient = canshu._num_gradient;    //�ݶ��쳣�����ж���ֵ�����ص������
	int num_grayScale = canshu._num_grayScale;  //�Ҷ��쳣�����ж���ֵ�����ص������

	//******************************�ݶ��쳣����**************************
	Mat gradient_dst;//�����ݶȾ���
	Point2f p[4];
	int x, y, w, h;//������Ӿ������Ͻǵ�Ϳ�ȡ��߶�
	int num = 0; //�ݶ��쳣���ص����
	int pixl;
	Mat dst_mean, dst_stddev;
	double dst_m1, dst_s_gra; //�ݶȾ�ֵ����׼��

	//*****************************�Ҷ��쳣����*****************************
	Mat gray_dest, mat_mean, mat_stddev;
	double dst_m, dst_s; //�ҶȾ�ֵ�ͷ���
	int num_g = 0;     //�Ҷ��쳣���ص����
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

	//imwrite("D://project//tftResultShow171717.bmp", (*sendImg1).tftResultShow);

	if (maxX > (*sendImg1).Max_x - 1)
	{
		maxX = (*sendImg1).Max_x - 1;
	}
	if (maxY > (*sendImg1).Max_y - 1)
	{
		maxY = (*sendImg1).Max_y - 1;
		cout << "maxY:" << maxY;
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

	//imwrite("D://project//tftResultShow181818.bmp", (*sendImg1).tftResultShow);

	x = minX;
	y = minY;
	w = maxX - minX;
	h = maxY - minY;
	Mat dst1 = (*sendImg1).imgTftColorCutFill(Rect(x, y, w, h)).clone();

	Mat img_copy = (*sendImg1).imgTftColorCutFill.clone();
	Mat img_mean, img_stddev;
	cvtColor(img_copy, img_copy, CV_RGB2GRAY);
	int h_img = img_copy.rows;
	int w_img = img_copy.cols;
	meanStdDev(img_copy, img_mean, img_stddev);
	double img_m, img_s;
	img_m = img_mean.at<double>(0, 0);
	img_s = img_stddev.at<double>(0, 0);
	cout << "����ͼƬ�ĻҶȾ�ֵΪ��" << img_m << endl;
	cout << "����ͼƬ�ĻҶȷ���Ϊ��" << img_s << endl;

	//imwrite("D://project//tftResultShow191919.bmp", (*sendImg1).tftResultShow);

	Mat img_gradient, img_grad_mean, img_grad_stddev;
	img_gradient = (*sendImg1).gradientTtfMaskValue.clone();
	meanStdDev(img_gradient, img_grad_mean, img_grad_stddev);
	double img_grad_m;
	img_grad_m = img_grad_mean.at<double>(0, 0);

	cout << "����ͼƬ���ݶȾ�ֵΪ��" << img_grad_m << endl;


	//imwrite("D://project//tftResultShow121212.bmp", (*sendImg1).tftResultShow);


	//�����ݶ����Ӽ���
	//5*5 Sobel����
	gradient_dst = (*sendImg1).gradientTtfMaskValue(Rect(x, y, w, h)).clone();
	//sendImg2.gradientTFT = gradient_dst.clone();
	int h1 = gradient_dst.rows;
	int w1 = gradient_dst.cols;
	//cout << "覴���Ӿ��ο�ĳ�Ϊ��" << w1 << endl;
	//cout << "覴���Ӿ��ο�Ŀ�Ϊ��" << h1 << endl;
	//�����ݶȾ���ľ�ֵ
	meanStdDev(gradient_dst, dst_mean, dst_stddev);
	dst_m1 = dst_mean.at<double>(0, 0);
	dst_s_gra = dst_stddev.at<double>(0, 0);
	cout << "�ݶȵľ�ֵΪ��" << dst_m1 << endl;
	cout << "�ݶȵı�׼��Ϊ��" << dst_s_gra << endl;

	//imwrite("D://project//tftResultShow131313.bmp", (*sendImg1).tftResultShow);

	//ͳ���ݶ��쳣�����ص����
	for (int i = 0; i < h1; i++)
	{
		for (int j = 0; j < w1; j++)
		{
			pixl = gradient_dst.at<uchar>(i, j);
			//if (abs(pixl - dst_m1) > dst_m_gradient)
			if (abs(pixl - img_grad_m) > dst_m_gradient)
			{
				num = num + 1;
			}
			else
			{
				num = num;
			}
		}
	}

	cout << "�ݶ�ֵ�쳣�����ص�ĸ���Ϊ��" << num << endl;

	//imwrite("D://project//tftResultShow141414.bmp", (*sendImg1).tftResultShow);

	//���лҶ��쳣����
	int h1_g = dst1.rows;
	int w1_g = dst1.cols;
	//����Ҷ�ͼ�ľ�ֵ�ͷ����Լ��Աȶ�
	gray_dest = Mat::zeros(Size(w1_g, h1_g), CV_8UC3);
	cvtColor(dst1, gray_dest, CV_RGB2GRAY);
	meanStdDev(gray_dest, mat_mean, mat_stddev);  //����Ҷ�ͼ�ľ�ֵ�ͱ�׼ƫ�
	dst_m = mat_mean.at<double>(0, 0);
	dst_s = mat_stddev.at<double>(0, 0);
	cout << "�ҶȾ�ֵ�ǣ�" << dst_m << endl;
	cout << "�Ҷȱ�׼���ǣ�" << dst_s << endl;
	cout << "������ĶԱȶ��ǣ�" << 2 * dst_s << endl;

	//imwrite("D://project//tftResultShow151515.bmp", (*sendImg1).tftResultShow);

	//ȷ���ҶȾ�ֵ�쳣����ֵ
	for (int i = 0; i < h1_g - 1; i++)
	{
		for (int j = 0; j < w1_g - 1; j++)
		{
			pixl_g = gray_dest.at<uchar>(i, j);
			//if (abs(pixl_g - dst_m) >= dst_m_grayScale)
			//˼��dst_m�ǲ��ǲ���������������ͼƬ�ľ�ֵ�ǲ��Ǹ�����
			if (abs(pixl_g - img_m) >= dst_m_grayScale)
			{
				num_g = num_g + 1;
			}
			else
			{
				num_g = num_g;
			}
		}
	}

	cout << "�Ҷ�ֵ�쳣�����ص����Ϊ��" << num_g << endl;


	//imwrite("D://project//tftResultShow101010.bmp", (*sendImg1).tftResultShow);

	bool xianzhi1 = (num > num_gradient) && (num_g > num_grayScale) && (dst_m1 > 0.5);
	bool xianzhi2 = (dst_s > dst_s_grayScale) && (dst_s_gra > dst_s_gradient);

	if (xianzhi1 || xianzhi2)
	{
		if (num_g = 0)
		{
			(*sendImg1).p_result = 0;
			(*sendImg1).l_result = 0;
		}
		else if (0 < num_g < 20)
		{
			(*sendImg1).p_result = 1;
			(*sendImg1).l_result = 0;
		}
		else
		{
			(*sendImg1).p_result = 0;
			(*sendImg1).l_result = 1;
		}

		return true;
	}
	else
	{
		return false;

	}
}