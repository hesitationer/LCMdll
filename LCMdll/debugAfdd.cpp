#include "stdafx.h"
#include "debugAdd.h"
#include "creatDir.h"



//���û���
//0228 ���ӻҳ����ڲ���
void setmethodTrackbar(Mat imgtest, int camera_flag, int imgFlag)
{
	//�趨��ʼֵ
	int methondnow = 0;
	int mean = 10;   //���ֵӦ����Դ�ڲ���
	int Smean = 10;
	int numPoint = 20;
	int mValueGradient = 10;
	int TFTthreshold_point = 180;
	int TFTthreshold_mura = 180;
	int subThresh = 20;
	int subBGRThresh = 75;
	int hsvThresh = 20;

	int keneralsize = 5;
	int up = 5;
	int down = 5;
	int left = 5;
	int right = 5;
	int beginDug = 0;


	int DUSTHOLD = 100;
	int DUSTSHAPE = 11;


	string sProjectDir = "D://lcmProject//";
	(*sendImg1).projectDir = sProjectDir;
	(*sendImg1).sNowTimeFileDir = mkdirUseTime(sProjectDir);  //ʹ��ʱ�䴴��һ���ļ���
															  //0������������ӻ�ȡ�ֻ�border


	//��������    
	namedWindow("method", 0);
	//��������
	createTrackbar("��������", "method", &beginDug, 1, onMethod, &sendImg1);


	//�����ҳ��Ļ���
	createTrackbar("DUSTHOLD", "method", &DUSTHOLD, 200, onMethod, &sendImg1);
	createTrackbar("DUSTSHAPE", "method", &DUSTSHAPE, 50, onMethod, &sendImg1);

	//ÿ�ַ���覴�ͳ�Ƶ���ز���
	createTrackbar("methondnow", "method", &methondnow, 5, onMethod, &sendImg1);
	createTrackbar("mean", "method", &mean, 70, onMethod, &sendImg1);
	createTrackbar("Smean", "method", &Smean, 70, onMethod, &sendImg1);
	createTrackbar("numPoint", "method", &numPoint, 200, onMethod, &sendImg1);
	createTrackbar("mValueGradient", "method", &mValueGradient, 100, onMethod, &sendImg1);


	//���ַ����Ķ�ֵ������
	createTrackbar("����0", "method", &TFTthreshold_point, 220, onMethod, &sendImg1);
	createTrackbar("����1", "method", &TFTthreshold_point, 220, onMethod, &sendImg1);
	createTrackbar("����2", "method", &hsvThresh, 80, onMethod, &sendImg1);
	createTrackbar("����3", "method", &subThresh, 80, onMethod, &sendImg1);
	createTrackbar("����4", "method", &subBGRThresh, 160, onMethod, &sendImg1);

	//ÿ�ַ�����mask����
	createTrackbar("drawIn", "method", &keneralsize, 50, onMethod, &sendImg1);
	createTrackbar("up", "method", &up, 70, onMethod, &sendImg1);
	createTrackbar("down", "method", &down, 70, onMethod, &sendImg1);
	createTrackbar("left", "method", &left, 70, onMethod, &sendImg1);
	createTrackbar("right", "method", &right, 70, onMethod, &sendImg1);











	//(*canshuX)._TFTHOLD = TFTthreshold;
	//(*canshuX)._subBGRThresh = subBGRThresh;
	//(*canshuX)._hsvThresh = hsvThresh;

	//��Ҫ����
	//camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);
	detect(imgtest, camera_flag, imgFlag);

	int threadDone = 0;
	//CombineXiaci(false);


	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_16S);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_16S);
	getOneColorResult(camera_flag, imgFlag, 0, &threadDone, defectsInforGray[camera_flag][imgFlag]);
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_8U);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_8U);




	(*sendImg1).debug_camera_flag = camera_flag;
	(*sendImg1).debug_imgFlag = imgFlag;
	//(*sendImg1).debug_method = method;
	(*sendImg1).debug_m_gradient = mean;
	(*sendImg1).debug_s_gradient = Smean;
	(*sendImg1).debug_num_gradient = numPoint;
	(*sendImg1).debug_mValuegradient = mValueGradient;
	//(*sendImg1).debug_img1 = (*sendImg1).gradientResultThreshold_bgr_sub[0][0].clone();
	(*sendImg1).debug_img1 = (*sendImg1).gradientResultThreshold_bgr_sub[camera_flag][imgFlag].clone();
	(*sendImg1).debug_img_point = (*sendImg1).gradientResultThreshold_points[camera_flag][imgFlag].clone();
	(*sendImg1).debug_img_mura = (*sendImg1).gradientResultThreshold_muras[camera_flag][imgFlag].clone();
	(*sendImg1).debug_img_sub = (*sendImg1).gradientResultThreshold_subTeps[camera_flag][imgFlag].clone();
	(*sendImg1).debug_img_hsv = (*sendImg1).gradientResultThreshold_louguangs[camera_flag][imgFlag].clone();
	imwrite((*sendImg1).sNowTimeFileDir + " aftersub" + ".jpg", (*sendImg1).debug_img1);
	//0118 add test
	//CombAndRemoveGrayFalse(camera_flag, imgFlag, 0, defectsInfor[camera_flag][imgFlag]);


	//
}


void setsecondParameterTrackbar(Mat imgtest, int camera_flag, int imgFlag, int method) {

	//�趨��ʼֵ
	int p_m = 130;   //���ֵӦ����Դ�ڲ���
	int p_n = 30;
	int l_m = 130;
	int l_n = 30;
	int t_m = 130;
	int t_n = 30;
	int m_m = 130;
	int m_n = 30;
	int maxDistance = 130;
	int maxGParea = 30;
	int point_bk_num = 130;
	int point_bk_m_gray = 30;


	namedWindow("secondParameter", 0);
	createTrackbar("p_m", "secondParameter", &p_m, 255, onSecondParameter, &sendImg1);
	createTrackbar("p_n", "secondParameter", &p_n, 500, onSecondParameter, &sendImg1);
	createTrackbar("l_m", "secondParameter", &l_m, 255, onSecondParameter, &sendImg1);
	createTrackbar("l_n", "secondParameter", &l_n, 500, onSecondParameter, &sendImg1);
	createTrackbar("m_m", "secondParameter", &m_m, 255, onSecondParameter, &sendImg1);
	createTrackbar("m_n", "secondParameter", &m_n, 500, onSecondParameter, &sendImg1);

	createTrackbar("t_m", "secondParameter", &t_m, 255, onSecondParameter, &sendImg1);
	createTrackbar("t_n", "secondParameter", &t_n, 500, onSecondParameter, &sendImg1);
	createTrackbar("maxDistance", "secondParameter", &maxDistance, 255, onSecondParameter, &sendImg1);
	createTrackbar("maxGParea", "secondParameter", &maxGParea, 500, onSecondParameter, &sendImg1);
	createTrackbar("point_bk_num", "secondParameter", &point_bk_num, 255, onSecondParameter, &sendImg1);
	createTrackbar("point_bk_m_gray", "secondParameter", &point_bk_m_gray, 500, onSecondParameter, &sendImg1);

	//��Ҫ����
	//camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);
	detect(imgtest, camera_flag, imgFlag);

	int threadDone = 0;
	//CombineXiaci(false);


	setEachMethodMask();
	(*sendImg1).gradientDustThresholDilate.convertTo((*sendImg1).gradientDustThresholDilate, CV_16S);
	(*sendImg1).BorderTFT.convertTo((*sendImg1).BorderTFT, CV_16S);
	getOneColorResult(camera_flag, imgFlag, method, &threadDone, defectsInforGray[camera_flag][imgFlag]);
	(*sendImg1).debug_camera_flag = camera_flag;
	(*sendImg1).debug_imgFlag = imgFlag;
	(*sendImg1).debug_method = method;
	(*sendImg1).debug_cutImg = (*sendImg1).imgTftColorCuts[camera_flag][method].clone();


}





//�ҳ����˺���
void setDustTrackbar(Mat imgtest, int camera_flag, int imgFlag, int method)
{

	//�趨��ʼֵ
	//int mean = 130;   //���ֵӦ����Դ�ڲ���
	//int numPoint = 30;

	int DUSTHOLD = 100;  //104
	int DUSTSHAPE = 11;   //5



	namedWindow("DustDilate", 0);
	createTrackbar("DUSTHOLD", "secondParameter", &DUSTHOLD, 255, onDustSlicer, &sendImg1);
	createTrackbar("DUSTSHAPE", "secondParameter", &DUSTSHAPE, 50, onDustSlicer, &sendImg1);

	//��Ҫ����
	//camera_cali_fisrt(imgtest1, neican_data, jibian_data, imgtest1);
	detect(imgtest, camera_flag, imgFlag);




}


//���û��ͣ��ͺ������Ѳ�ʹ��
void setTrackbar()
{
	//�趨��ʼֵ
	int TFTHOLD = 130;
	int DUSTHOLD = 50;
	int DUSTSHAPE = 7;
	int dst_m_gradient = 100; //�ݶ��쳣�ж���ֵ
	int dst_m_grayScale = 8; //�Ҷ��쳣�ж���ֵ
	int dst_s_gradient = 20;
	int dst_s_grayScale = 12; //�Ҷ��쳣�ж���ֵ
	int num_gradient = 10;    //�ݶ��쳣�����ж���ֵ
	int num_grayScale = 10;  //�Ҷ��쳣�����ж���ֵ
	int fill = 20;  //�Ҷ��쳣�����ж���ֵ
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


