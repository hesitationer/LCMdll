#pragma once
#include "LCM.h"
#include "divideIOU.h"
/*��������debugģʽ�Ĵ���
�趨����
void setTrackbar();
/
/����ȥ����ȱ�ݵ��Ҳ���
void setremoveFalseTrackbar();

//���ڵ�����������ֵ������
void setMaskTrackbar();

���ڱ���ͼƬ�ĺ���
void saveImg();

������ʾ�������й��̵��м���
void showProcessImg();
void showProcessImg(sendImg * sendImg2);

*/


//�趨����
void setTrackbar();
void setremoveFalseTrackbar();
void setMaskTrackbar();


//��ȡ����
void setCanshuUseTrackbar();
void setCanshuMaskUseTrackbar();
void UseremoveFalseTrackbar();

//��ʾ������̵�һЩ�м���
void showProcessImg();
void showProcessImg(sendImg * sendImg2);



void setWhiteSlicerTrackbar( );
void setmethodTrackbar(Mat imgtest, int camera_flag, int imgFlag);
void setsecondParameterTrackbar(Mat imgtest, int camera_flag, int imgFlag, int method);
void setDustTrackbar(Mat imgtest, int camera_flag, int imgFlag, int method);


//void saveImg();