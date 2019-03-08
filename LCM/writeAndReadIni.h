#pragma once
#include "LCM.h"

/***************************************************�ļ���ȡ����****************************************************************/
void WriteConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

void ReadConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

//���Զ�д����
void testReadAndWrite();

//0127 add �ļ��ж�д����lyl
void read_files(std::vector<string> &filepaths, std::vector<string> &filenames, const string &directory);
void saveImg(int cameraFlag , int imgFlag);
void ReleaseImg(int cameraFlag); 



