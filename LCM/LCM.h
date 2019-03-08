#pragma once
// dustGetYu.cpp: �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <iostream>
#include <string>
#include <direct.h> //_mkdir������ͷ�ļ�
#include <io.h>     //_access������ͷ�ļ�
#include <windows.h>
#include "opencv\cv.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include<cmath>
#include <thread>
#include <typeinfo>
#include <time.h>
#include <math.h>

using namespace cv;
using namespace std;

struct defectInformation {
	
	int  s_cx= 0;
	int s_cy  =0;
	int w=0;
	int h=0;
	int S=0;
	int numGray=0;
	int numGradient=0;
	double dst_m_Gradient=0.0;
	double dst_m_Gray=0.0;
	double dst_s_Gray=0.0;
	double dst_s_Gradient=0.0;
	bool TrueOrFalse=false;
	int bright_num=0;
	int dark_num=0;
	int num_yichang = 0;   
	Point2i P[4];

	bool transYorN = true;

	int typenum = -1;
	string type;
};

//struct defectInformations {
//	vector<defectInformation> xxx(9);
//};
/***************************************************����-��****************************************************************/
class sendImg

{

private: //��Ա�������Ʒ�



public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�

	Mat imgTFT[10];             //���10̨���ͬʱ����
	Mat imgTFT_gray;
	Mat imgTFTpoint_gray[10];
	Mat imgTFTmura_gray[10];
	Mat imgTFTlouguang_gray;
	Mat TFTb;
	Mat TFTg;
	Mat TFTr;
	Mat imgDust;
	Mat BorderTFT;
	Mat gradientTFT;
	Mat gradientTFT_point;
	Mat gradientTFT_pointChange;    //point�ľ��������������ݶ�ͼ
	Mat gradientTFT_muras[10][20];
	Mat gradientTFTb_muras[20];
	Mat gradientTFTg_muras[20];
	Mat gradientTFTr_muras[20];
	Mat gradientTFTbgr_muras[10][20];
	Mat gradientTFTb_point[20];
	Mat gradientTFTg_point[20];
	Mat gradientTFTr_point[20];
	Mat gradientTFTbgr_point[10][20];

	Mat gradientTFT_muraChange;     //mura�ľ��������������ݶ�ͼ
	Mat gradientTFT_louguang;
	Mat gradientTFT_louguangChange;  //©��ľ��������������ݶ�ͼ
	Mat subtractTemplateTftImg[10];


	Mat gradientTFTb;
	Mat gradientTFTg;
	Mat gradientTFTr;
	Mat gradientTFT_or;
	Mat gradientTtfMaskValue;
	Mat gradientDUST;
	Mat imgDustColorCut;
	Mat imgTftColorCut[10];
	Mat imgTftColorCuts[10][20];
	Mat CutTemplate_fill[10][20];  //add
	Mat GradientTemplate_fill[10][20];  //add


	Mat imgTftColorCutFill;
	Mat imgTftColorCutFills[10][20];
	Mat imgTftColor_point[10];
	Mat imgTftColor_mura[10];
	Mat imgTftColor_louguang[10];
	Mat imgTftColor_bgr_sub[10];
	Mat imgTftColor_sub_result[10][20];
	Mat img_border;
	Mat img_center;
	Mat img_border_gradient;
	Mat img_border_dust;
	Mat img_center_gradient;
	Mat img_center_dust;
	bool useCameras = false;   //False�������true�����
	Mat dust_points;
	Mat  imgTEMP;
	Mat gradientTftThreshold;
	Mat gradientTftThreshold_point;
	Mat gradientTftThreshold_mura;
	Mat gradientTftThreshold_louguang;
	Mat gradientDustThreshold;
	Mat showDustImg;
	Mat deteceResult;
	Mat gradientDustThresholDilate;
	Mat gradientResult;
	Mat gradientResult_point;
	Mat gradientResult_mura;
	Mat gradientResult_louguang;
	Mat gradientResultThreshold;
	Mat gradientResultThreshold_points[10][20];
	Mat gradientResultThreshold_muras[10][20];
	Mat gradientResultThreshold_louguangs[10][20];
	Mat gradientResultThreshold_subTeps[10][20];
	Mat gradientResultThreshold_bgr_sub[10][20];
	Mat gradientResultThreshold_all[10][20];


	Mat tftResultShow;
	Mat tftResultShow_point;
	Mat tftResultShow_mura;
	Mat tftResultShow_louguang;
	Mat tftResultShow_subTmp;
	Mat tftResultShow_gradient[10][20][4];
	Mat tftResultShow_gray[20];
	Mat tftResultShow_all[20];

	Mat tftResultShow_final;
	string projectDir;
	string sNowTimeFileDir;
	Mat gradient_dst;
	//RotatedRect rect[10];

	Mat mask1;
	Mat mask2;

	int  shrink_size = 4;   //�������ұ�Ե���Գߴ�
	int  shrink_size_mura = 20;   //�������ұ�Ե���Գߴ�  ����ŵ�size�Ƚϴ�ȡ8  //8 before   20
	int  shrink_size_point = 10;   //�������ұ�Ե���Գߴ�  ��Ե���ߵ�size�Ƚ�Сȡ4
	int  shrink_size_louguang = 40;   //�������ұ�Ե���Գߴ�  ���©���size�Ƚ�Сȡ4
	int  shrink_size_subTmp = 35;   //�������ұ�Ե���Գߴ�  ���©���size�Ƚ�Сȡ4
	int  shrink_size_subBGR = 5;   //�������ұ�Ե���Գߴ�  ���©���size�Ƚ�Сȡ4
	Mat BorderTftDilate;

	int Max_x;
	int Max_y;

	//ROI
	//get_img_rotate_info�����ֻ���Ļ������С��ת��Ӿ�����Ϣ�������Ƕȣ�����    
	RotatedRect get_rotate_rect[10];   // [10]�洢���ʮ�����������
	Point2f box[10][4];

	vector<Point2f> pts1[10];  //�ɰ������������Ļ�����ĸ��ǵ�����, ����[10]���10�����
	vector<Point2f>pts1_tmp;  //ģ��ͼ���ĸ��ǵ�����
	string pts1_tmp_file; // = projectDir + "R_tmmp//pts1_tmp.txt";  //ģ��ͼ���ĸ��ǵ�����

	//������ͳһ�����ĸ��ǵ����꣬��͸�ӱ任�е�pts2
	vector<Point2f> WhiteScreenQuadrilateral;

	//͸�ӱ任�� ���Ϳ� Size(2315, 1115));
	int PersPectiveTransformation_weight = 2315;
	int PersPectiveTransformation_hight = 1115;


	// R��
	vector<string> res;
	string res_tmp_dir; //= projectDir + "R_tmmp//";
	string FileDir;  //������ļ���·��
	Mat tmp;
	Mat R_tmp;
	int R_compare_res[4] = {0,0,0,0}; //R�Ǽ����


	vector<vector<Point>>  contours3;
	vector<vector<Point>>  contours;
	vector<vector<Point>>  contours_points[10][20];
	vector<vector<Point>>  contours_muras[10][20];
	vector<vector<Point>>  contours_louguangs[10][20];
	vector<vector<Point>>  contours_subTmps[10][20];
	vector<vector<Point>>  contours_BGRsub[10][20];
	vector<vector<Point>>  contours_all[10][20];

	Mat GradientResult[20];
	Mat GradientResult_point[10][20];
	Mat GradientResult_mura[20];
	Mat GradientResult_louguang[10][20];
	Mat subtractTemplateTftImgs[10][20];
	Mat bgrsubTemplateImgs[10][20];
	Mat GradientResult_or[20];
	Mat GradientResult_final;
	Mat Template[10][20];    //���10�������ÿ�����20��ͼƬ
	Mat CutTemplate[10][20];

	int h; //roi����ĸ�
	int w;  //roi����Ŀ�

	int new_x;
	int new_y;

	Mat img_detect[20];


	int  p_result;
	int  l_result;


	int MaxFlag[10] = {0,0,0,0,0,0,0,0,0,0};    //ÿ���������˶�����ͼƬ
	int Flag_White = 0;
	int Flag_Green = 0;
	int Flag_Yellow = 0;
	int Flag_Blue = 0;



	//��������
	vector<vector<Point>> pointResults[10][20];
	vector<vector<Point>> muraResults[10][20];
	vector<vector<Point>> louguangResults[10][20];
	vector<vector<Point>> subTmpResults[10][20];
	vector<vector<Point>> BGRsubResults[10][20];


	

	//**************��������ֵ�м�ͼƬ*****************
	Mat plusResult;    //�߽���ֵ��С��Ľ��ͼƬ
	Mat ResultBorderinvgradTFT; //�������м䲿���ݶȵ�ͼƬ
	Mat BorderTFTthreshold;
	Mat ResultBorderthreshgradTFT; //�����˱�Ե�ݶȵ�ͼƬ

								   //��������
	int  shrink_x = 3;
	int  shrink_y = 6;
	Point2f box_TFT[4];
	Mat  OuterBorderTFTImg;

	//���궨
	Mat getMouseChooseimg;
	Rect rect_getIOU;
	Mat ResultMouse;

	Mat onMouseResultShow;

	//����������ݶȵ���maskͼƬ

	//R��Mask
	Mat PointMaskChangeGradietRCorner;      //�����ݶȵ�R������ģ��  
	Mat MuraMaskChangeGradietRCorner;      //�����ݶȵ�R������ģ��  
	Mat LightMaskChangeGradietRCorner;      //�����ݶȵ�R������ģ��  
	Mat SubMaskChangeGradietRCorner;      //�����ݶȵ�R������ģ��  
	Mat SubBGRMaskChangeGradietRCorner;      //�����ݶȵ�R������ģ��  

	
	
	//����Mask
	Mat PointMaskChangeGradientDrawIn;		//�����ݶȵ���������ģ��
	Mat MuraMaskChangeGradientDrawIn;		//�����ݶȵ���������ģ��
	Mat LightMaskChangeGradientDrawIn;
	Mat SubMaskChangeGradientDrawIn;
	Mat SubBGRMaskChangeGradientDrawIn;


	//�Զ���Mask
	Mat  PointMaskChangeGradientMouseArea;	//�����ݶȵ����ѡ������ģ��
	Mat  MuraMaskChangeGradientMouseArea;
	Mat  LightMaskChangeGradientMouseArea;
	Mat  SubMaskChangeGradientMouseArea;
	Mat  SubBGRMaskChangeGradientMouseArea;
	
	
	//���ڴ���ͬͼƬʱ�ļ��ֵ

	

	RotatedRect rect_res;   ////������������

	//ͼƬ�Ҷȣ��ݶȾ�ֵ����
	double imgInTFTMeans[10][20];
	double imgInTFTStddevs[10][20];
	double 	imgInGradientMeans[10][20];
	double imgInGradientStddevs[10][20];

	Mat imgOut_g;   //������
	

	// ����ͼ
	vector<Point2i> points; //������ͼ���̵��ã�ͳ��ȱ������
	Mat back;  // �����ܾŹ�ͼ�ã���ȱ��λ�ñ���ͱ�ʾȱ�����͵�����


	vector<Point2d> group_points[10][20];

	int matrix[10][7][8] = { 0 };

	Point2f P[4][2];



	//��ǰ�����������
	int debug_camera_flag;
	int debug_imgFlag;
	int debug_method;
	int debug_m_gradient;
	int debug_s_gradient;
	int debug_mValuegradient;
	int debug_num_gradient;
	int debug_thre_point;
	int debug_thre_mura;
	int debug_subThre;
	int debug_subBGR;
	int debug_hsvThre;
	Mat debug_cutImg;
	Mat debug_img;
	Mat debug_img1;
	Mat debug_img_point;
	Mat debug_img_mura;
	Mat debug_img_sub;
	Mat debug_img_hsv;

	int beginDebug = 0;
	int beginDebug1 = 0;
	Mat debug_img_dust;
	Mat debug_img_secondCanshu;



	//0301����
	Mat img_roi_debug[10];
	int hough_binary_debug[10];

	//0307����
	Mat RemovefixedDefect;



protected:


};


//1.ȫ�ֱ���
//Canshu lougunagCanshu =() 
//Canshu pointDetect = ()
//Canshu muraDetect = ()

/*�¼�����*/
class CanShu

{

private: //��Ա�������Ʒ�

public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�

	//�ҳ�����
	int _DUSTHOLD[10][7];
	int _DUSTSHAPE[10][7];
	//������
	int _fill[10][7];

	int _TFTHOLD_muras[10][7];
	int _TFTHOLD_points[10][7];

	//��ĻУ������
	int _houghthr[10][7];
	int _sort_dxy1[10][7];
	int _sort_dxy2[10][7];
	int _sort_dxy3[10][7];
	int _sort_dxy4[10][7];
	int _Rarea[10][7];


	int _subThresh[10][7];
	int _subBGRThresh[10][7];
	int _bgrThresh[10][7];
	int _hsvThresh[10][7];

	//0301��������
	int _houghP[10];



	CanShu(int subThresh, int subBGRThresh, int bgrThresh, int hsvThresh, int sort_dxy1, int sort_dxy2, int sort_dxy3, int sort_dxy4, int houghthr, int DUSTHOLD, int DUSTSHAPE, int fill, int Rarea, int TFTHOLD_muras, int TFTHOLD_points, int houghP, int i, int j)
	{

		initCanShu(subThresh, subBGRThresh, bgrThresh, hsvThresh, sort_dxy1, sort_dxy2, sort_dxy3, sort_dxy4, houghthr, DUSTHOLD, DUSTSHAPE, fill, Rarea, TFTHOLD_muras, TFTHOLD_points,houghP,  i, j);

	}

	void initCanShu(int subThresh, int subBGRThresh, int bgrThresh, int hsvThresh, int sort_dxy1, int sort_dxy2, int sort_dxy3, int sort_dxy4, int houghthr, int DUSTHOLD, int DUSTSHAPE, int fill, int Rarea, int TFTHOLD_muras, int TFTHOLD_points, int houghP, int i, int j)
	{
		_subThresh[i][j] = subThresh;
		_subBGRThresh[i][j] = subBGRThresh;
		_bgrThresh[i][j] = bgrThresh;
		_hsvThresh[i][j] = hsvThresh;

		_sort_dxy1[i][j] = sort_dxy1;
		_sort_dxy2[i][j] = sort_dxy2;
		_sort_dxy3[i][j] = sort_dxy3;
		_sort_dxy4[i][j] = sort_dxy4;
		_houghthr[i][j] = houghthr;
		_Rarea[i][j] = Rarea;

		_DUSTHOLD[i][j] = DUSTHOLD;
		_DUSTSHAPE[i][j] = DUSTSHAPE;

		_fill[i][j] = fill;

		_TFTHOLD_points[i][j] = TFTHOLD_points;
		_TFTHOLD_muras[i][j] = TFTHOLD_points;

		_houghP[i] = houghP;
	}
protected:
};

//������������ֵ�Ĳ���
/*
int _keneralsize;            �ֻ�����������ϵ����Խ�󣬼�������ԽС
int _size_x_left;			 �ֻ���߽�������������
int _size_x_right;			 �ֻ��ұ߽�������������
int _size_y_up;				 �ֻ��ϱ߽�������������
int _size_y_down;			 �ֻ��±߽�������������

*/

class CanShuMask
{
private: //��Ա�������Ʒ�

public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�


	int _keneralsize[10][7];
	int _size_x_left[10][7];
	int _size_x_right[10][7];
	int _size_y_up[10][7];
	int _size_y_down[10][7];
	int i;
	int j;

	//���캯��������Ĭ�ϸ�ֵ
	//CanShuMask(int keneralsize[10][7], int size_x_left[10][7], int size_x_right[10][7], int size_y_up[10][7], int size_y_down[10][7])
	//{
	//	initCanShuMask(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down);

	//}

	//void initCanShuMask(int keneralsize[10][7], int size_x_left[10][7], int size_x_right[10][7], int size_y_up[10][7], int size_y_down[10][7])
	//{
	//	for (int i = 0; i < 10; i++)
	//	{
	//		for (int j = 0; j < 10; j++)
	//		{
	//			_keneralsize[i][j] = keneralsize[i][j];
	//			_size_x_left[i][j] = size_x_left[i][j];
	//			_size_x_right[i][j] = size_x_right[i][j];
	//			_size_y_up[i][j] = size_y_up[i][j];
	//			_size_y_down[i][j] = size_y_down[i][j];
	//		}
	//	}
	//}

	CanShuMask(int keneralsize, int size_x_left, int size_x_right, int size_y_up, int size_y_down, int i, int j)
	{
		initCanShuMask(keneralsize, size_x_left, size_x_right, size_y_up, size_y_down, i, j);

	}

	void initCanShuMask(int keneralsize, int size_x_left, int size_x_right, int size_y_up, int size_y_down, int i, int j)
	{

		_keneralsize[i][j] = keneralsize;
		_size_x_left[i][j] = size_x_left;
		_size_x_right[i][j] = size_x_right;
		_size_y_up[i][j] = size_y_up;
		_size_y_down[i][j] = size_y_down;

	}

protected:
};

class CanShuFinal
{
private: //��Ա�������Ʒ�

public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�


	int _point_num;
	int _line_num;
	int _mura_num;
	int _tuan_num;
	double _point_m_gray;
	double _line_m_gray;
	double _mura_m_gray;
	double _tuan_m_gray;
	int _maxDistance;
	int _maxGParea;
	int _point_bk_num;
	int _point_bk_m_gray;


	//���캯��������Ĭ�ϸ�ֵ
	CanShuFinal(int point_num, int line_num, int mura_num, int tuan_num, double point_m_gray, double line_m_gray, double mura_m_gray, double tuan_m_gray, int maxDistance, int maxGParea, int point_bk_num, int point_bk_m_gray)
	{
		initCanShuMask(point_num, line_num, mura_num, tuan_num, point_m_gray, line_m_gray, mura_m_gray, tuan_m_gray, maxDistance, maxGParea, point_bk_num, point_bk_m_gray);

	}

	//��ʼ�����������ڸı�Ĭ�ϸ�ֵ
	void initCanShuMask(int point_num, int line_num, int mura_num, int tuan_num, double point_m_gray, double line_m_gray, double mura_m_gray, double tuan_m_gray, int maxDistance, int maxGParea, int point_bk_num, int point_bk_m_gray)
	{
		_point_num = point_num;
		_line_num = line_num;
		_mura_num = mura_num;
		_tuan_num = tuan_num;
		_point_m_gray = point_m_gray;
		_line_m_gray = line_m_gray;
		_mura_m_gray = mura_m_gray;
		_tuan_m_gray = tuan_m_gray;
		_maxDistance = maxDistance;
		_maxGParea = maxGParea;
		_point_bk_num = point_bk_num;
		_point_bk_m_gray = point_bk_m_gray;

	}

protected:
};

//ȥ����ȱ�ݵĲ���
/*
int _num_gradient, _num_grayScale; //�ݶȺͻҶ��쳣���ص����
double _dst_m_gradient, _dst_s_gradient; //�ݶȾ�ֵ�ͷ���
double _dst_m_grayScale, _dst_s_grayScale; //�ҶȾ�ֵ�ͷ���;
double _meanValueGradient;
*/
class CanShureMoveFalse

{
private: //��Ա�������Ʒ�

public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�
		//ȱ���쳣������


	int _num_gradient[10][7], _num_grayScale[10][7]; //�ݶȺͻҶ��쳣���ص����
	double _dst_m_gradient[10][7], _dst_s_gradient[10][7]; //�ݶȾ�ֵ
	double _dst_m_grayScale[10][7], _dst_s_grayScale[10][7]; //�ҶȾ�ֵ�ͷ���;
	double _meanValueGradient[10][7];
	int _bright_num[10][7];  int _dark_num[10][7];

	CanShureMoveFalse(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, int bright_num, int dark_num, int i, int j)
	{
		//ȱ���쳣���
		initCanShureMoveFalse(num_gradient, num_grayScale, dst_m_gradient, dst_m_grayScale, dst_s_gradient, dst_s_grayScale, meanValueGradient, bright_num, dark_num, i, j);
	}

	void initCanShureMoveFalse(int num_gradient, int num_grayScale, int dst_m_gradient, int  dst_m_grayScale, int dst_s_gradient, int  dst_s_grayScale, int meanValueGradient, int bright_num, int dark_num, int i, int j)
	{

		_num_gradient[i][j] = num_gradient;
		_num_grayScale[i][j] = num_grayScale;
		_dst_m_gradient[i][j] = dst_m_gradient;
		_dst_m_grayScale[i][j] = dst_m_grayScale;
		_dst_s_gradient[i][j] = dst_s_gradient;
		_dst_s_grayScale[i][j] = dst_s_grayScale;
		_meanValueGradient[i][j] = float(meanValueGradient) / 10;
		_bright_num[i][j] = bright_num;
		_dark_num[i][j] = dark_num;

	}

protected:
};



//



//class BorderCanShu
//{
//
//private: //��Ա�������Ʒ�
//
//public: //public����֮ǰ�ĳ�Ա�������������õ�˽�е�
//
//	//ȱ���쳣������
//	int _b_num_gradient, _b_num_grayScale; //�ݶȺͻҶ��쳣���ص����
//	double _b_dst_m_gradient, _b_dst_s_gradient; //�ݶȾ�ֵ
//	double _b_dst_m_grayScale, _b_dst_s_grayScale; //�ҶȾ�ֵ�ͷ���;
//
//	BorderCanShu(int b_num_gradient, int b_num_grayScale, int b_dst_m_gradient, int  b_dst_m_grayScale,  int b_dst_s_gradient, int  b_dst_s_grayScale)
//	{
//
//	
//		//ȱ���쳣���
//		_b_num_gradient = b_num_gradient;
//		_b_num_grayScale = b_num_grayScale;
//		_b_dst_m_gradient = b_dst_m_gradient;
//		_b_dst_m_grayScale = b_dst_m_grayScale;
//		_b_dst_s_grayScale = b_dst_s_grayScale;
//		_b_dst_s_gradient = b_dst_s_gradient;
//
//	}
//
//protected:
//};



//ȫ�ֱ�������
extern sendImg *sendImg1 ;
extern  CanShu *canshuX;
extern  CanShu *canshuMura;
extern  CanShu *canshuLouguang;
extern  CanShuMask *PointCanShuMask;
extern  CanShuMask *MuraCanShuMask;
extern  CanShuMask *lightCanShuMask;
extern  CanShuMask *SubCanShuMask;
extern  CanShuMask *SubBGRCanShuMask;
extern CanShureMoveFalse  *muraCanShureMoveFalse;
extern CanShureMoveFalse *PointCanShureMoveFalse;
extern vector<defectInformation>  defectsInforGray[40][20][5];
extern vector<defectInformation> defectsInfor[40][20][5];


//extern  BorderCanShu *canshuY;

//extern sendImg *sendImg1 = &sendImgExtern;
//extern  CanShu * canshuX = &canshuExtern;


//���������ú�������
void onWhiteSlicer(int, void * names);
void detect(Mat imgTFT, int cameraFlag, int pictureFlag, bool if_showimg);
void getOneColorResult(int cameraFlag, int imgFlag, bool flag, int *ifthreadDonn, vector<defectInformation> defectsInfor[5]);
void CombAndRemoveGrayFalse(int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect);
void CombineXiaci(bool flag);
void onMethod(int, void * names);
void onSecondParameter(int, void * names);
void onDustSlicer(int, void * names);