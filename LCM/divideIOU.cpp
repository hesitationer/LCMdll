#include "stdafx.h"
#include "divideIOU.h"
#include"LCM.h"
#include "CalculatingDefectInformation.h"
#include "removeFalseDetect.h"

bool draw;
Mat roi;
Point cursor=Point(-1,-1);  //���λ��
Point peak=Point(-1,-1); //�������
Point mousePosition=Point(-1,-1); //������궨��
RNG g_rng(12345);
//extern Rect rect;      //�Ҷ������ܵ�ϸ�ڲ�����Ȥ��




//�ı�һ��ͼƬ�Ĳ��������ݶ�ֵ
//Mat &imgIn,	����ͼƬ 
//Mat &imgMask,	��Ҫ�ı�Ķ�����Ϊ255������ı������Ϊ0
//Mat &imgOut,	���ͼƬ
//float percentum  �ı��ϵ��
void changeIouGradirnt(Mat &imgIn, Mat &imgMask, Mat &imgOut, float percentum)
{
	//borderȡ��
	Mat imgMaskinverse;
	bitwise_not(imgMask, imgMaskinverse);

	//(*sendImg1).gradientTFT��
	//Mat tmp_gradientTFT = (*sendImg1).gradientTFT.clone();//TFT ���ͼ

	bitwise_and(imgMaskinverse, imgIn, imgOut); //�������м䲿���ݶ�

	Mat tmpImgIn, Origntmp;																							 //boder��ֵ������ԭͼ
	bitwise_and(imgMask, imgIn, tmpImgIn); //�����˱�Ե�ݶ�

										   //��ͼ����ݶȽ����������

	Origntmp = tmpImgIn.clone();
	Origntmp = percentum * Origntmp;
	//IplImage* Origntmp = cvCloneImage(&(IplImage)tmpImgIn);   //Mat->cvArr����ͨ��IplImage+&�ķ���
	//double f2 = 0.01*f1; //����trackerbarֻ��int��Ӱ��
	//cvConvertScale(Origntmp, Origntmp, percentum, 0);
	//Mat convImg = cvarrToMat(Origntmp, true);     //cvArr->MAt								  //��ͼ���
	imgOut = (Origntmp + imgOut);//������ս��ͼ
}


//���ѡ��ʱ������ȷ��
void DrawRectangle(Mat& img, Rect box)
{
	rectangle(img, box.tl(), box.br(), Scalar(0, 255, 0), 2);
}

//****************����¼�ѡ����С�ݶ�ֵROI����************
void onMouse(int event, int x, int y, int flags, void *param)
{
	Mat img = (*sendImg1).getMouseChooseimg.clone();   //�˴�Դͼ��Ϊ�ɸ��ĵ�
	switch (event)
	{
		//����������
	case CV_EVENT_LBUTTONDOWN:
		//****������ͼ��ʱ�Ĳ���
		//���֮ǰROIͼ�����ʾ����
		cvDestroyWindow("ROI");
		//��ʼ��
		cursor = Point(x, y);
		//Rect rect = (*sendImg1).rect_getIOU;
		(*sendImg1).rect_getIOU = Rect(x, y, 0, 0);
		draw = true;



		break;

		//�ɿ�������
	case CV_EVENT_LBUTTONUP:
		if ((*sendImg1).rect_getIOU.height > 0 && (*sendImg1).rect_getIOU.width > 0)
		{
			draw = false;


			//��img�еľ��θ��Ƹ�ROI
			roi = img(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));
			rectangle(img, (*sendImg1).rect_getIOU, Scalar(0, 255, 0), 2);
			namedWindow("SignROI");
			imshow("SignROI", img);

			//���ú������л���




			DrawRectangle(img, (*sendImg1).rect_getIOU);

			//changeIOUthreshod((*sendImg1).rect_getIOU);

			//removeFalseDefects2();

		}

		break;

		//�ƶ����
	case CV_EVENT_MOUSEMOVE:
		if (draw)
		{
			//��MIN�õ����ϵ���Ϊ���ο����ʼ���꣬���򻭾���ʱֻ����һ������
			(*sendImg1).rect_getIOU.x = MIN(x, cursor.x);
			(*sendImg1).rect_getIOU.y = MIN(y, cursor.y);
			(*sendImg1).rect_getIOU.width = abs(cursor.x - x);
			(*sendImg1).rect_getIOU.height = abs(cursor.y - y);
			//��ֹ�������򳬳�ͼ��ķ�Χ
			(*sendImg1).rect_getIOU &= Rect(0, 0, img.cols, img.rows);


		}
		break;

	}
}
// ����ʵʱ����
void onMouse1(int event, int x, int y, int flags, void *param)
{
	//Mat img = (*sendImg1).getMouseChooseimg.clone();   //�˴�Դͼ��Ϊ�ɸ��ĵ�
	Mat & img = *(cv::Mat*) param;
	char temp[16];
	switch (event)
	{
		//����������
	case CV_EVENT_LBUTTONDOWN:
	{	//****������ͼ��ʱ�Ĳ���
		//���֮ǰROIͼ�����ʾ����
		cvDestroyWindow("ROI");
		//��ʼ��
		cursor = Point(x, y);
		//Rect rect = (*sendImg1).rect_getIOU;
		(*sendImg1).rect_getIOU = Rect(x, y, 0, 0);
		//ʵʱ����
		sprintf_s(temp, "(%d,%d)", x, y);
		putText(img, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		draw = true;
		peak = Point(x, y);


		break;
	}
	//�ɿ�������
	case CV_EVENT_LBUTTONUP:
	{
		

		
		
		
		if ((*sendImg1).rect_getIOU.height > 0 && (*sendImg1).rect_getIOU.width > 0)
		{
			draw = false;
			//��img�еľ��θ��Ƹ�ROI
			roi = img(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));

			//ʵʱ����
			sprintf_s(temp, "(%d,%d)", x, y);
			putText(img, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			rectangle(img, peak, mousePosition, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));

			// ���ú���
			//changeIOUthreshod((*sendImg1).rect_getIOU);
			vector<defectInformation> defect[10];


			
			//��img�еľ��θ��Ƹ�ROI
			roi = (*sendImg1).RemovefixedDefect(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));
			roi.setTo(0);
			namedWindow("SrcImage1", 0);
			imshow("SrcImage1", (*sendImg1).RemovefixedDefect);



		
			//removePointFalse((*sendImg1).ResultMouse, (*sendImg1).contours_points[0][0], (*sendImg1).pointResults[0][0],PointCanShureMoveFalse ,0,0,false , defect);
		}

	}	break;

	//�ƶ����
	case CV_EVENT_MOUSEMOVE:
	{
		mousePosition = Point(x, y);
		if (draw)
		{
			//��MIN�õ����ϵ���Ϊ���ο����ʼ���꣬���򻭾���ʱֻ����һ������
			(*sendImg1).rect_getIOU.x = MIN(x, cursor.x);   //������������֪��Ϊ�κ������Ϊ0
			(*sendImg1).rect_getIOU.y = MIN(y, cursor.y);
			(*sendImg1).rect_getIOU.width = abs(cursor.x - x);
			(*sendImg1).rect_getIOU.height = abs(cursor.y - y);
			//��ֹ�������򳬳�ͼ��ķ�Χ
			(*sendImg1).rect_getIOU &= Rect(0, 0, img.cols, img.rows);


		}
	}	break;

	}
}

//**************���ѡ����ı���ֵ******************
//bug��¼��֮ǰ��src���������⣬֮ǰ�İ汾��src����Ϊ������ʾ���ͼ��ʵ��������ͼֻ����������ȷ����ȱ�ݵ�λ�ã�����Ҫ�޸ĵ�ʵ��Ϊ�ݶ�ͼ
//�Լ�ȱ�����ų�������Ҳ���ݶ�ͼ�����Ҫ�������º�����src�����Դͼ��count_gradient3�еļ���ͼ
//
void changeIOUthreshod(Rect rect)
{
	double f4 = 0.0;
	Mat src = (*sendImg1).gradientTtfMaskValue.clone();
	imshow("before mask zero", src);

	//������ԭͼ�������ϵ��f4
	Mat Originimg = src.clone();
	IplImage* Origntmp = cvCloneImage(&(IplImage)Originimg);   //Mat->cvArr����ͨ��IplImage+&�ķ���
	cvConvertScale(Origntmp, Origntmp, f4, 0);
	Mat convImg = cvarrToMat(Origntmp, true);     //cvArr->MAt

												  //��convImg��ȡ��rect�����ݶ�
	Mat roi_changeThreshold = convImg(Rect(rect.x, rect.y, rect.width, rect.height));

	//��ճ����ͼƬ��Ҫ���һ��ճ��ͼƬ��С��λ�ã�����ΪMat����
	Mat roi = src(Rect(rect.x, rect.y, rect.width, rect.height));
	roi_changeThreshold.copyTo(roi);

	imshow("after mask zero", src);
	(*sendImg1).ResultMouse = src;     //������ͼ�п������Կ�������ȥ����֮ǰ���ݶ�


}

//�� �����ݶȵ�R������ģ��  ���д���    
void getBorderThreshold(float f1,Mat &maskChangeGradietRCorner)
{
	changeIouGradirnt((*sendImg1).gradientTFT ,maskChangeGradietRCorner, (*sendImg1).plusResult, f1);
	//namedWindow("plusResult", 0);
	//imshow("plusResult", (*sendImg1).plusResult);
	//imwrite("C:\\cppProjects\\Application\\lcmProject\\Data\\plusResult.png", (*sendImg1).plusResult);
}
//�� ���������ݶȵ���������ģ�� ���д���
void getShrinkThreshold(float f3,Mat &maskChangeGradientDrawIn)
{

	changeIouGradirnt((*sendImg1).gradientTFT, maskChangeGradientDrawIn, (*sendImg1).OuterBorderTFTImg, f3);
	//namedWindow("OuterBorderResult", 0);
	//imshow("OuterBorderResult", (*sendImg1).OuterBorderTFTImg);

}
//���������ݶȵ�R������ģ��  
void getRCornerMask(Mat &imgIn, Mat &imgOut, int kernelSize)

{
	Mat kerne20 = cv::getStructuringElement(cv::MORPH_RECT, Size(kernelSize, kernelSize));
	Mat dilateBorderTFT;
	dilate(imgIn, imgOut, kerne20);  //���ͺ���ݶ�ͼ
}
//���������ݶȵ���������ģ�� 
void getDrawInAreaMask(Mat &imgIn, Mat &imgOut, int size_x_left, int size_x_right, int size_y_up, int size_y_down)
{
	
	Mat tmp_gradientTFT = Mat::zeros((*sendImg1).PersPectiveTransformation_hight, (*sendImg1).PersPectiveTransformation_weight, CV_8UC1);

	//********�����������*********
	// roi����߽糤����6�����أ�������3������
	vector<Point2f>new_poly;
	Point2f new_p1(0 + size_x_left, tmp_gradientTFT.rows - size_y_down);
	Point2f new_p2(0 + size_x_left, 0 + size_y_up);
	Point2f new_p3(tmp_gradientTFT.cols - size_x_right, 0 + size_y_up);
	Point2f new_p4(tmp_gradientTFT.cols - size_x_right, tmp_gradientTFT.rows - size_y_down);

	new_poly.push_back(new_p1);
	new_poly.push_back(new_p2);
	new_poly.push_back(new_p3);
	new_poly.push_back(new_p4);

	int h = new_p1.y - new_p2.y;
	int w = new_p4.x - new_p1.x;

	//*****����������ͼ****
	Mat tmp_gradienttft = tmp_gradientTFT(Rect(new_p2.x, new_p2.y, w, h));

	//����������ͼͬ����С��0ͼ
	Mat padImg = Mat::zeros(Size(tmp_gradienttft.cols, tmp_gradienttft.rows), CV_8UC1); //��Ϊc3ʱ������ʱ�ڴ��쳣

																						//������ԭͼͬ����С��255ͼ
	Mat templateImg = Mat::zeros(Size(tmp_gradientTFT.cols, tmp_gradientTFT.rows), CV_8UC1);

	Mat templateImg_inverse;
	bitwise_not(templateImg, templateImg_inverse);

	//padImg���ӵ�templateImg_inverse
	Mat roi = templateImg_inverse(Rect(new_p2.x, new_p2.y, w, h));
	padImg.copyTo(roi);

	imgOut = templateImg_inverse;
	//changeIouGradirnt((*sendImg1).gradientTFT, templateImg_inverse, (*sendImg1).OuterBorderTFTImg, f3);
}
//���������ݶȵ����ѡ������ģ�� 
//
//maskChangeGradientMouseArea


//��Ҫ�õ�ȫ�ֱ���  ����һ��MaskͼƬ (*sendImg1).RemovefixedDefect    
void setMouseChoseArea(Mat &imgIn)
{
	(*sendImg1).RemovefixedDefect = Mat(imgIn.rows, imgIn.cols, CV_8UC1, Scalar(255, 255, 255));
	//namedWindow("SrcImage", 0);
	//imshow("SrcImage",(*sendImg1).RemovefixedDefect);
	//waitKey(0);
	//���Ȧ��
	(*sendImg1).getMouseChooseimg = imgIn.clone();  //Ϊ���Ȧ����ͼ��ֵ
	waitKey(0);
	system("color 9F");
	Mat temp1;
	while (waitKey(30) != 27)
	{
		//long t1 = GetTickCount();
		(*sendImg1).getMouseChooseimg.copyTo(temp1);
		namedWindow("SrcImage",0);
		imshow("SrcImage", (*sendImg1).getMouseChooseimg);
		setMouseCallback("SrcImage", onMouse1, (void*)&(*sendImg1).getMouseChooseimg);
		//setMouseCallback("SrcImage", onMouse, NULL);
		if (draw)
		{
			rectangle(temp1, peak, mousePosition, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));
			putText(temp1, "(" + std::to_string(mousePosition.x) + "," + std::to_string(mousePosition.y) + ")", mousePosition, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			imshow("SrcImage", temp1);
		}
	}
}
//������Ҫ�õ�������ͼƬ
//int size_x_left, int size_x_right, int size_y_up, int size_y_down, double f3,if useMuoseChose,MouseChose float


/*
//��ʼ���������������mask����
// �����ݶ�ͼ�ͱ߽�ͼ���õ��߽�mask������mask
*/
void initChangePartOfTheRegionalGradient(Mat ImgIn, Mat bordertft, Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, CanShuMask canshumask)
{
	//��border�������ͣ���ȡmaskģ��  ��Ҫ�����ĵط�Ϊ255�����������Ϊ0
	getRCornerMask(bordertft, maskChangeGradietRCorner, canshumask._keneralsize[0][0]);
	//��ȡ��Ҫ�����ݶȵ���������ģ�� �� ��Ҫ�����ĵط�Ϊ255�����������Ϊ0
	getDrawInAreaMask(ImgIn, maskChangeGradientDrawIn, canshumask._size_x_left[0][0], canshumask._size_x_right[0][0], canshumask._size_y_up[0][0], canshumask._size_y_down[0][0]);
	//�ı�Բ�������ݶ�ֵ    A=  A*f1
}

void initChangePartOfTheRegionalGradient(Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn,CanShuMask canshumask)
{

	//��border�������ͣ���ȡmaskģ��  ��Ҫ�����ĵط�Ϊ255�����������Ϊ0
	getRCornerMask((*sendImg1).BorderTFT, maskChangeGradietRCorner, canshumask._keneralsize[0][0]);
	//��ȡ��Ҫ�����ݶȵ���������ģ�� �� ��Ҫ�����ĵط�Ϊ255�����������Ϊ0
	getDrawInAreaMask((*sendImg1).GradientResult_point[0][0], maskChangeGradientDrawIn, canshumask._size_x_left[0][0], canshumask._size_x_right[0][0], canshumask._size_y_up[0][0], canshumask._size_y_down[0][0]);
	//�ı�Բ�������ݶ�ֵ    A=  A*f1
}

void setEachMethodMask()
{
	

	imwrite((*sendImg1).sNowTimeFileDir + "BorderTFTXXX"+ ".jpg", (*sendImg1).BorderTFT);
	
	//!!!!!!!!��ͬ�������Ҫ��ͬ��������
	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_point[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);   //20 10 20 50 60
																												   //�����ŵ�Maskģ��																										   //************  ͼƬ1��ͼƬ4�Ĵ�Сһ����ͼƬ2��ͼƬ3�Ĵ�Сһ��

	initChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_muras[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);
	//0219 add sub������mask����

	initChangePartOfTheRegionalGradient((*sendImg1).subtractTemplateTftImgs[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, *SubCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_louguang[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).LightMaskChangeGradientDrawIn, *lightCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).gradientResultThreshold_bgr_sub[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubBGRMaskChangeGradietRCorner, (*sendImg1).SubBGRMaskChangeGradientDrawIn, *SubBGRCanShuMask);



}



//�ı��������ĵ���������ĿǰĬ��Ϊ0.0 
// �����ݶ�ͼ��ͨ��init�õ��ı߽�mask������mask���ݶ�ͼ��mask�������ϵ��
void ChangePartOfTheRegionalGradient(Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, Mat &imgIn, Mat &imgOut, float f1, float f2)
{

	//R���������ϵ��

	changeIouGradirnt(imgIn, maskChangeGradietRCorner, imgOut, f1);
	//changeIouGradirnt((*sendImg1).gradientTFT, (*sendImg1).maskChangeGradietRCorner, (*sendImg1).plusResult, f1);
	(*sendImg1).plusResult = imgOut.clone();
	//namedWindow("plusResult", 0);
	//imshow("plusResult", (*sendImg1).plusResult);

	//�����������ϵ��
	changeIouGradirnt(imgOut, maskChangeGradientDrawIn, imgOut, f2);

	//changeIouGradirnt((*sendImg1).gradientTFT, (*sendImg1).maskChangeGradientDrawIn, (*sendImg1).OuterBorderTFTImg, f2);
	(*sendImg1).OuterBorderTFTImg = imgOut.clone();
	//namedWindow("OuterBorderResult", 0);
	//imshow("OuterBorderResult",imgOut );

	//���ѡ���������ϵ��
	//maskChangeGradientMouseArea
	//12.19  ���Ȧ����������δʹ��

}