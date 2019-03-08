/*
���ڣ�2019-9-15
�޸��ߣ�lqy
�汾�ţ�v2019-9-15-1
*/

#include "stdafx.h"
#include "onedll.h"
#include "removeFalseDetect.h"
#include "CalculatingDefectInformation.h"




/*
���ڣ�2019-9-15
�޸��ߣ�lqy
�汾�ţ�v2019-9-15-1



*/
void StatisticsDefectArea()
{



}






//1.4 lyl update ���flag ���flag = true����ʾ覴���Ϣ��false������覴õľ�����Ϣ

//removePointFalse((*sendImg1).ResultMouse, (*sendImg1).contours_points[0][0], (*sendImg1).pointResults[0][0], PointCanShureMoveFalse, 0, false, defect);
void removePointFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	(*defect).clear();
	defectInformation Xiaci;

	//Xiaci.h = 52;
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);

	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	contoursOut.clear();
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();

	int s_cx;
	int s_cy;

	Mat tftResultShow = imgIn.clone();
	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
	cout << "��һ��Pointȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	//int N = contoursIn.size();
	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{
			min_rect = minAreaRect(contoursIn[i]);
			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�

			s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			s_cy = int(s_N.m01 / s_N.m00);




			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//��Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);
			//if (((*canshuX)._dis_l < s_cx < (*sendImg1).new_x) && ((*canshuX)._dis_u < s_cx < (*sendImg1).new_y))
			//{
			//	result = count_gradient(min_rect);
			//}
			//ͳ���ݶȺͻҶ��쳣


			//defectInformation *Xiaci_Info;
			result = count_gradient1((*sendImg1).GradientResult_point[cameraFlag][imgFlag], (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);
			//Xiaci.TrueOrFalse = result;


			if (result) {
				//д�����
				rectangle(tftResultShow, P[1], P[3], cvScalar(0, 0, 255), 3, 4, 0);
				contoursOut.insert(contoursOut.end(), contoursIn[i]);

				//(*defect).push_back(*Xiaci_Info);


			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}

			}
		}
	}


	if (flag)
	{
		cout << "ɸѡ���pointȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;

	}
	//Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_point = tftResultShow;
	cout << imgFlag << endl;
	imwrite((*sendImg1).sNowTimeFileDir + "PointResult" + to_string(imgFlag) + ".jpg", (*sendImg1).tftResultShow_point);


	//namedWindow("PointResult" + to_string(imgFlag), 0);
	//imshow("PointResult"+ to_string(imgFlag), imgOut);
	//waitKey(0);
}


//1.4 update
void removeMuraFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{

	(*defect).clear();
	defectInformation Xiaci;


	contoursOut.clear();
	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();
	Mat tftResultShow = imgIn.clone();


	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;

	if (flag)
	{
		cout << "muraȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	}

	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{

			min_rect = minAreaRect(contoursIn[i]);

			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�
			int s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			int s_cy = int(s_N.m01 / s_N.m00);
			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//mura�Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);
			//if (((*canshuX)._dis_l < s_cx < (*sendImg1).new_x) && ((*canshuX)._dis_u < s_cx < (*sendImg1).new_y))
			//{
			//	result = count_gradient(min_rect);
			//}
			//ͳ���ݶȺͻҶ��쳣
			result = count_gradient1((*sendImg1).gradientTFT_muras[cameraFlag][imgFlag], (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);
			if (result) {
				//д�����
				////��ɫ�Ŀ�����ʾmura
				rectangle(tftResultShow, P[1], P[3], cvScalar(0, 255, 255), 3, 4, 0);
				//rectangle(tftResultShow, P[1], P[3], cvScalar(0, 255, 0), 3, 4, 0);
				contoursOut.insert(contoursOut.end(), contoursIn[i]);
			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}

			}
		}
	}
	//saveImg();
	//memcpy(dataResult, (*sendImg1).tftResultShow.data, rows*cols * 4);
	if (flag)
	{
		cout << "ɸѡ���muraȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;
	}
	//Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_mura = tftResultShow;

	imwrite((*sendImg1).sNowTimeFileDir + "MuraResult" + to_string(imgFlag) + ".jpg", tftResultShow);
	//namedWindow("MuraResult" + to_string(imgFlag), 0);
	//imshow("MuraResult"+to_string(imgFlag), imgOut);   
	//waitKey(0);
}

//Mat &imgIn,,vector<vector<Point>>contours,vector<vector<Point>> realDefectCouhter;

void removeLouguangFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	contoursOut.clear();
	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();
	Mat tftResultShow = imgIn.clone();


	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;

	if (flag)
	{
		cout << "©���ȷ��ȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	}

	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{

			min_rect = minAreaRect(contoursIn[i]);
			Point2f rect_points[4]; min_rect.points(rect_points);

			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�
			int s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			int s_cy = int(s_N.m01 / s_N.m00);
			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//mura�Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);

			result = count_gradient1((*sendImg1).GradientResult_louguang[cameraFlag][imgFlag], (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);
			if (result) {
				//д�����
				////ziɫ�Ŀ�����ʾmura
				//rectangle(tftResultShow, P[1], P[3], cvScalar(240, 32, 160), 3, 4, 0);
				for (int j = 0; j < 4; j++)
				{
					line(tftResultShow, rect_points[j], rect_points[(j + 1) % 4], Scalar(240, 32, 16), 2);
				}
				contoursOut.insert(contoursOut.end(), contoursIn[i]);
			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}
			}
		}
	}


	if (flag)
	{
		cout << "ɸѡ���©��ȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;
	}


	Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_louguang = imgOut.clone();
	//namedWindow("LouguangResult", 0);
	//imshow("LouguangResult", imgOut);

}

//�޸���sub�Ļ��򲿷ֵĴ��룬�ĳ������ĸ���������
void removeSubTmpFalse(Mat &imgIn, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{


	(*defect).clear();
	defectInformation Xiaci;



	contoursOut.clear();
	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();
	Mat tftResultShow = imgIn.clone();


	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;

	if (flag)
	{
		cout << "subȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	}

	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{

			min_rect = minAreaRect(contoursIn[i]);
			Point2f rect_points[4]; min_rect.points(rect_points);

			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�
			int s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			int s_cy = int(s_N.m01 / s_N.m00);
			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//mura�Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);
			//if (((*canshuX)._dis_l < s_cx < (*sendImg1).new_x) && ((*canshuX)._dis_u < s_cx < (*sendImg1).new_y))
			//{
			//	result = count_gradient(min_rect);
			//}
			//ͳ���ݶȺͻҶ��쳣
			result = count_gradient1((*sendImg1).subtractTemplateTftImgs[cameraFlag][imgFlag], (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);

			if (result) {
				//д�����
				////ziɫ�Ŀ�����ʾmura
				//rectangle(tftResultShow, P[1], P[3], cvScalar(240, 32, 160), 3, 4, 0);
				for (int j = 0; j < 4; j++)
				{
					line(tftResultShow, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 2550), 2);
				}

				contoursOut.insert(contoursOut.end(), contoursIn[i]);
			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}

			}
		}
	}
	//saveImg();
	//memcpy(dataResult, (*sendImg1).tftResultShow.data, rows*cols * 4);
	//cout << "6666666" << endl;
	if (flag)
	{
		cout << "ɸѡ���subTmpȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;
	}

	//Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_subTmp = tftResultShow;
	imwrite((*sendImg1).sNowTimeFileDir + "subTmp" + to_string(imgFlag) + ".jpg", tftResultShow);
	//namedWindow("subTmp" + to_string(imgFlag), 0);
	//imshow("subTmp"+to_string(imgFlag), imgOut);
	//waitKey(0);

}


//0118 add ֻ�����ݶ�ͳ��ֵȥɸѡ覴õĺ��� lyl
void removeGradientFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect, int DefectType)
{





	//(*defect).clear();
	//defectInformation Xiaci;

	//Xiaci.h = 52;
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);

	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	contoursOut.clear();
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	//bool result1;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();

	int s_cx;
	int s_cy;

	Mat tftResultShow = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
	cout << "δ�����ݶ�ɸѡǰ��ȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	//int N = contoursIn.size();
	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{
			min_rect = minAreaRect(contoursIn[i]);
			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�

			s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			s_cy = int(s_N.m01 / s_N.m00);




			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//��Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(0, 70, 70), 3, 4, 0);
			//cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);
			//if (((*canshuX)._dis_l < s_cx < (*sendImg1).new_x) && ((*canshuX)._dis_u < s_cx < (*sendImg1).new_y))
			//{
			//	result = count_gradient(min_rect);
			//}
			//ͳ���ݶȺͻҶ��쳣


			//defectInformation *Xiaci_Info;
			//result = count_gradient1((*sendImg1).GradientResult_point[imgFlag], (*sendImg1).imgTftColorCuts[imgFlag], min_rect, canshuMoveFalse, imgFlag, flag, defect);
			result = count_gradient_only(imgIn, imgThresholdIn, (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag], imgOut, min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);
			cout << "lalalal ";
			//Xiaci.TrueOrFalse = result;


			if (result) {
				//д�����
				rectangle(tftResultShow, P[1], P[3], cvScalar(0, 0, 255), 3, 4, 0);
				contoursOut.insert(contoursOut.end(), contoursIn[i]);

				//(*defect).push_back(*Xiaci_Info);


			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}

			}
		}
	}


	if (flag)
	{
		cout << "ɸѡ����ݶ�ȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;

	}
	//Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_gradient[cameraFlag][imgFlag][DefectType] = tftResultShow;
	cout << imgFlag << endl;
	imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_gradient" + to_string(imgFlag) + to_string(DefectType) + ".jpg", (*sendImg1).tftResultShow_gradient[cameraFlag][imgFlag][DefectType]);


	//namedWindow("PointResult" + to_string(imgFlag), 0);
	//imshow("PointResult"+ to_string(imgFlag), imgOut);
	//waitKey(0);
}




//0118 add ֻ���ûҶ�ͳ��ֵȥɸѡ覴õĺ��� lyl
//0119 add ͳ�������� lyl
void removeGrayFalse(Mat &imgIn, Mat &imgThresholdIn, Mat &imgOut, vector<vector<Point>>contoursIn, vector<vector<Point>>contoursOut, CanShureMoveFalse *canshuMoveFalse, int cameraFlag, int imgFlag, bool flag, vector<defectInformation> *defect)
{
	//(*defect).clear();
	defectInformation Xiaci;

	//Xiaci.h = 52;
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);
	//(*defect).push_back(Xiaci);

	//����ȱ�ݵ���С��Ӿ���,��ɫ�ľ��ο�
	//2018-11-7  by hyw
	contoursOut.clear();
	Point2f P[4];
	RotatedRect min_rect;
	bool result;
	//bool result1;
	/*������д������*/
	//(*sendImg1).tftResultShow = (*sendImg1).imgTftColorCut.clone();

	int s_cx;
	int s_cy;

	Mat tftResultShow = (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag].clone();
	int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
	cout << "δ���лҶ�ɸѡǰ��ȱ�ݵ���Ŀ�ǣ�" << contoursIn.size() << endl;
	//int N = contoursIn.size();
	if (contoursIn.size() > 0)
	{
		for (int i = 0; i < contoursIn.size(); i++)
		{
			min_rect = minAreaRect(contoursIn[i]);
			min_rect.points(P);
			if (flag)
			{
				cout << "��" << i + 1 << "��ȱ��" << endl;
			}

			//��ͼ�ϱ������ȱ�ݵ����
			Moments s_N = moments(contoursIn[i]);//�����һ�������ľ�

			s_cx = int(s_N.m10 / s_N.m00);   //����ȱ�����ĵ�
			s_cy = int(s_N.m01 / s_N.m00);




			//д�����
			cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (0, 0, 255), 2);
			//����С��ӿ�
			//rectangle(tftResultShow, P[1], P[3], cvScalar(255, 0, 0), 3, 4, 0);
			//��Ļ���ɫ��ʾ
			rectangle(tftResultShow, P[1], P[3], cvScalar(0, 70, 70), 3, 4, 0);
			//cv::putText(tftResultShow, to_string(i + 1), Point(s_cx, s_cy), font, 1.2, (255, 0, 0), 2);
			//if (((*canshuX)._dis_l < s_cx < (*sendImg1).new_x) && ((*canshuX)._dis_u < s_cx < (*sendImg1).new_y))
			//{
			//	result = count_gradient(min_rect);
			//}
			//ͳ���ݶȺͻҶ��쳣


			//defectInformation *Xiaci_Info;
			//result = count_gradient1((*sendImg1).GradientResult_point[imgFlag], (*sendImg1).imgTftColorCuts[imgFlag], min_rect, canshuMoveFalse, imgFlag, flag, defect);
			//result = count_gradient_only(imgIn, (*sendImg1).imgTftColorCuts[imgFlag], min_rect, canshuMoveFalse, imgFlag, flag, defect);
			result = count_gray_only(imgThresholdIn, imgIn, imgOut, min_rect, canshuMoveFalse, cameraFlag, imgFlag, flag, defect);

			//Xiaci.TrueOrFalse = result;


			if (result) {
				//д�����
				rectangle(tftResultShow, P[1], P[3], cvScalar(0, 70, 70), 3, 4, 0);
				contoursOut.insert(contoursOut.end(), contoursIn[i]);

				//(*defect).push_back(*Xiaci_Info);


			}
			else {
				if (flag)
				{
					cout << "δ��⵽ȱ�ݣ�" << endl;
				}

			}
		}
	}


	if (flag)
	{
		cout << "�Ҷ�ɸѡ���ȱ�ݵ���Ŀ�ǣ�" << contoursOut.size() << endl;

	}
	//Mat imgOut = tftResultShow.clone();
	(*sendImg1).tftResultShow_gray[imgFlag] = tftResultShow;
	cout << imgFlag << endl;
	imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_gray" + to_string(imgFlag) + ".jpg", (*sendImg1).tftResultShow_gray[imgFlag]);


	//namedWindow("PointResult" + to_string(imgFlag), 0);
	//imshow("PointResult"+ to_string(imgFlag), imgOut);
	//waitKey(0);
}