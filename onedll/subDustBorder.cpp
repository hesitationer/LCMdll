#include "stdafx.h"
#include "onedll.h"
#include "subDustBorder.h"


//0114 add test
//0114
//������ȫ�ֱ��� (*sendImg1).gradientTFTg_point ��ԭʼ�ĻҶ��ݶ�ͼ��ֵ����Ľ������һ��
void subDustBorder(Mat &imgIn, Mat &imgOut, vector<vector<Point>> &contours, int cameraFlag, int imgFlag)
{
	contours.clear();
	Mat gradientTftThreshold_point, gradientResult_point, gradientResultThreshold_point;
	//gradientTftThreshold_point = imgIn.clone();
	cv::threshold(imgIn, gradientTftThreshold_point, (*canshuX)._TFTHOLD, 255, THRESH_BINARY);
	bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTbgr_point[cameraFlag][imgFlag], gradientTftThreshold_point);
	//(*sendImg1).gradientTFTbgr_point[imgFlag] = gradientResultThreshold_point.clone();
	//0114
	//������ȫ�ֱ��� (*sendImg1).gradientTFTg_point ��ԭʼ�ĻҶ��ݶ�ͼ��ֵ����Ľ������һ��

	//bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTb_point[imgFlag], gradientTftThreshold_point);
	//bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTr_point[imgFlag], gradientTftThreshold_point);


	//�Ƚ�ȱ�ݶ�ֵ��ͼgradientTftThreshold���ҳ���ֵ��ͼgradientDustThresholDilate�Լ��߽�BorderTFTתΪCV_16S������ͼ���
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_16S);

	gradientResult_point = gradientTftThreshold_point - (*sendImg1).gradientDustThresholDilate - (*sendImg1).BorderTFT;//���ܲ��� 										 
																																   //�ָ�
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_8U);
	//(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_8U);
	//(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_8U);


	//���ݶ�ͼ�ϳ�ȥ�ҳ��ͱ߽粿��
	cv::threshold(gradientResult_point, gradientResultThreshold_point, 0, 255, THRESH_BINARY);
	gradientResultThreshold_point.convertTo(gradientResultThreshold_point, CV_8U);
	Mat gradientTft = imgIn.clone();
	// ��ȡ��ʵ�ݶ�ֵ,�������ݶ���Ϊ��
	bitwise_and(gradientTft, gradientResultThreshold_point, gradientTft);
	//��ʴ��Ե����
	Mat maskKeepGradientTft;
	bitwise_and(gradientTft, (*sendImg1).BorderTftDilate, maskKeepGradientTft);//�߼��룬�󽻼�
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradientTft, gradientTft, kernel1);
	bitwise_or(gradientTft, maskKeepGradientTft, (*sendImg1).gradientTtfMaskValue);

	//����ȱ������
	Mat kernel6 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	Mat dilateResultTFT;
	dilate(gradientResultThreshold_point, dilateResultTFT, kernel6);

	imgOut = gradientResultThreshold_point.clone();
	//��������ܴ���ȱ�ݵ�����
	//vector<vector<Point>>contours3;
	Mat hierarchy;
	cv::findContours(dilateResultTFT, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


}

//0112 lyl �޸��˸ú���
//������ȫ�ֱ��� (*sendImg1).gradientTFTg_muras ��ԭʼ�ĻҶ��ݶ�ͼ��ֵ����Ľ������һ��
void subDustBorder_mura(Mat &imgIn, Mat &imgOut, vector<vector<Point>>&contours, int cameraFlag, int imgFlag)
{

	contours.clear();
	Mat gradientTftThreshold_point, gradientResult_point, gradientResultThreshold_point;
	//gradientTftThreshold_point = imgIn.clone();

	//imgIn = imread("..\\gradientResultThreshold_mura.bmp",0);
	//cv::threshold(imgIn, gradientTftThreshold_point, (*canshuX)._TFTHOLD, 255, THRESH_BINARY);
	cv::threshold(imgIn, gradientTftThreshold_point, (*canshuX)._TFTHOLD, 255, THRESH_BINARY);
	bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag], gradientTftThreshold_point);
	//(*sendImg1).gradientTFTbgr_muras[imgFlag] = gradientTftThreshold_point.clone();

	//0112
	//������ȫ�ֱ��� (*sendImg1).gradientTFTg_muras ��ԭʼ�ĻҶ��ݶ�ͼ��ֵ����Ľ������һ��
	//bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTb_muras[imgFlag], gradientTftThreshold_point);
	//bitwise_or(gradientTftThreshold_point, (*sendImg1).gradientTFTr_muras[imgFlag], gradientTftThreshold_point);
	//gradientTftThreshold_point = imread("D://project//tft.png", 0);
	//imwrite("D://project//tft1.png", gradientTftThreshold_point);
	//�Ƚ�ȱ�ݶ�ֵ��ͼgradientTftThreshold���ҳ���ֵ��ͼgradientDustThresholDilate�Լ��߽�BorderTFTתΪCV_16S������ͼ���
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_16S);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_16S);
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_16S);

	gradientResult_point = gradientTftThreshold_point - (*sendImg1).gradientDustThresholDilate - (*sendImg1).BorderTFT;//���ܲ��� 										 
																													   //�ָ�
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_8U);



	//���ݶ�ͼ�ϳ�ȥ�ҳ��ͱ߽粿��
	cv::threshold(gradientResult_point, gradientResultThreshold_point, 0, 255, THRESH_BINARY);
	gradientResultThreshold_point.convertTo(gradientResultThreshold_point, CV_8U);
	//namedWindow("gradientResultThreshold_mura", 0);
	//imshow("gradientResultThreshold_mura", gradientResultThreshold_point);
	//namedWindow("gradientResultThreshold_mura" + to_string(imgFlag), 0);
	//imshow("gradientResultThreshold_mura" + to_string(imgFlag), gradientResultThreshold_point);
	//waitKey(0);
	Mat gradientTft = imgIn.clone();
	// ��ȡ��ʵ�ݶ�ֵ,�������ݶ���Ϊ��

	bitwise_and(gradientTft, gradientResultThreshold_point, gradientTft);
	//��ʴ��Ե����
	Mat maskKeepGradientTft;
	bitwise_and(gradientTft, (*sendImg1).BorderTftDilate, maskKeepGradientTft);//�߼��룬�󽻼�
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradientTft, gradientTft, kernel1);
	bitwise_or(gradientTft, maskKeepGradientTft, (*sendImg1).gradientTtfMaskValue);

	//����ȱ������
	Mat kernel6 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	Mat dilateResultTFT;
	dilate(gradientResultThreshold_point, dilateResultTFT, kernel6);
	namedWindow("dilateResultTFT", 0);
	imshow("dilateResultTFT", dilateResultTFT);
	imgOut = gradientResultThreshold_point.clone();
	//��������ܴ���ȱ�ݵ�����
	//vector<vector<Point>>contours3;
	Mat hierarchy;
	cv::findContours(dilateResultTFT, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

}

void subDustBorder_louguang(Mat &imgIn, Mat &imgOut, vector<vector<Point>> &contours)
{
	//(*sendImg1).GradientResult_final = (*sendImg1).GradientResult[0];
	//for (int flag = 1; flag < (*sendImg1).MaxFlag; flag++) {

	//	bitwise_or((*sendImg1).GradientResult_final, (*sendImg1).GradientResult[flag], (*sendImg1).GradientResult_final);

	//}
	contours.clear();
	//��GradientResult_final���ж�ֵ��
	Mat gradientTftThreshold_point, gradientResult_point, gradientResultThreshold_point;
	cv::threshold(imgIn, gradientTftThreshold_point, (*canshuX)._TFTHOLD, 255, THRESH_BINARY);



	//�Ƚ�ȱ�ݶ�ֵ��ͼgradientTftThreshold���ҳ���ֵ��ͼgradientDustThresholDilate�Լ��߽�BorderTFTתΪCV_16S������ͼ���
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_16S);

	gradientResult_point = gradientTftThreshold_point - (*sendImg1).gradientDustThresholDilate - (*sendImg1).BorderTFT;//���ܲ��� 										 
																													   //�ָ�
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_8U);



	//���ݶ�ͼ�ϳ�ȥ�ҳ��ͱ߽粿��
	cv::threshold(gradientResult_point, gradientResultThreshold_point, 0, 255, THRESH_BINARY);
	gradientResultThreshold_point.convertTo(gradientResultThreshold_point, CV_8U);
	Mat gradientTft = imgIn.clone();
	// ��ȡ��ʵ�ݶ�ֵ,�������ݶ���Ϊ��

	bitwise_and(gradientTft, gradientResultThreshold_point, gradientTft);

	//��ʴ��Ե����
	Mat maskKeepGradientTft;
	bitwise_and(gradientTft, (*sendImg1).BorderTftDilate, maskKeepGradientTft);//�߼��룬�󽻼�
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradientTft, gradientTft, kernel1);
	bitwise_or(gradientTft, maskKeepGradientTft, (*sendImg1).gradientTtfMaskValue);

	//����ȱ������
	Mat kernel6 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	Mat dilateResultTFT;
	dilate(gradientResultThreshold_point, dilateResultTFT, kernel6);

	imgOut = gradientResultThreshold_point.clone();
	//namedWindow("gradientResultThreshold_louguang", 0);
	//imshow("gradientResultThreshold_louguang", gradientResultThreshold_point);
	//��������ܴ���ȱ�ݵ�����
	//vector<vector<Point>>contours3;
	Mat hierarchy;
	cv::findContours(dilateResultTFT, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


}

void subDustBorder_subTmp(Mat &imgIn, Mat &imgOut, vector<vector<Point>> &contours)
{
	contours.clear();
	Mat gradientTftThreshold_point, gradientResult_point, gradientResultThreshold_point;
	GaussianBlur(imgIn, imgIn, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//��Ҫ�ĳɲ�������ʽ
	cv::threshold(imgIn, gradientTftThreshold_point, 10, 255, THRESH_BINARY);
	//imgIn = gradientTftThreshold_point.clone();


	//�Ƚ�ȱ�ݶ�ֵ��ͼgradientTftThreshold���ҳ���ֵ��ͼgradientDustThresholDilate�Լ��߽�BorderTFTתΪCV_16S������ͼ���
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_16S);
	gradientResult_point = gradientTftThreshold_point - (*sendImg1).gradientDustThresholDilate - (*sendImg1).BorderTFT;//���ܲ��� 										 																									   //�ָ�
	gradientTftThreshold_point.convertTo(gradientTftThreshold_point, CV_8U);



	//���ݶ�ͼ�ϳ�ȥ�ҳ��ͱ߽粿��
	cv::threshold(gradientResult_point, gradientResultThreshold_point, 0, 255, THRESH_BINARY);
	gradientResultThreshold_point.convertTo(gradientResultThreshold_point, CV_8U);
	namedWindow("gradientResultThreshold_subTmp", 0);
	imshow("gradientResultThreshold_subTmp", gradientResultThreshold_point);
	Mat gradientTft = imgIn.clone();
	// ��ȡ��ʵ�ݶ�ֵ,�������ݶ���Ϊ��

	bitwise_and(gradientTft, gradientResultThreshold_point, gradientTft);
	//��ʴ��Ե����
	Mat maskKeepGradientTft;
	bitwise_and(gradientTft, (*sendImg1).BorderTftDilate, maskKeepGradientTft);//�߼��룬�󽻼�
	Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));
	erode(gradientTft, gradientTft, kernel1);
	bitwise_or(gradientTft, maskKeepGradientTft, (*sendImg1).gradientTtfMaskValue);

	//����ȱ������
	Mat kernel6 = cv::getStructuringElement(cv::MORPH_RECT, Size(20, 20));
	Mat dilateResultTFT;
	dilate(gradientResultThreshold_point, dilateResultTFT, kernel6);
	namedWindow("dilateResultTFT", 0);
	imshow("dilateResultTFT", dilateResultTFT);
	imgOut = gradientResultThreshold_point.clone();
	//��������ܴ���ȱ�ݵ�����
	//vector<vector<Point>>contours3;
	Mat hierarchy;
	cv::findContours(dilateResultTFT, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}
