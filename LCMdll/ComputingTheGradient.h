#pragma once
#pragma once
#include "LCMdll.h"
#include "padding.h"

void spiltImgTFT(sendImg * sendImg2);

Mat gradiecnt_channel_1(Mat image);

Mat gradient_sobel_channel1(Mat image);

void gradient_sobel_channel3_or(sendImg * sendImg2);

void getLightLeakArea(Mat &imgIn, Mat &imgOut1, Mat &imgOut2, int cameraflag, int imgflag);

void drawIn(Mat &imgIn, Mat &imgOut, int shrink);
void pointDetect(Mat & imgIn, Mat & imgOut);
void muraDetect(Mat & imgIn, Mat & imgOut);
void louguangDetect(Mat & imgIn, Mat & imgOut);
void subTemplate(Mat &imgIn, Mat &imgOut);
//2.����ʱ�̶���С
//����
//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void pointDetect(Mat & ingIn ,Mat & imgOut,canshu )       //�����ݶ�ͼ
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterPoint


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void louguangDetect(Mat & ingIn ,Mat & imgOut,canshu)   //�����ݶ�ͼ
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterlouguang


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void muraDetect(MMat & ingIn ,Mat & imgOut,canshu)
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//COUTREmura




//cut    show   countrePoint   blue   show   countrelougunag    yellow  







//��


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void pointDetect(Mat & ingIn ,Mat & imgOut,canshu )       //�����ݶ�ͼ
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterPoint


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void louguangDetect(Mat & ingIn ,Mat & imgOut,canshu)   //�����ݶ�ͼ
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//couterlouguang


//void  drawIn((Mat & ingIn ,Mat & imgOut,x,y,z,w);   �㣬�ţ�©��
//void muraDetect(MMat & ingIn ,Mat & imgOut,canshu)
//void repadding(Mat & ingIn ,Mat & imgOut,x,y,z,w)
//����
//
//�ҵ�ȱ��   
//ȥ���ٵ�ȱ��(canshu)
//COUTREmura




//cut    show   countrePoint   blue   show   countrelougunag    yellow  
//









//couterWhite
//+
//couterBule
//+
//countergreen 