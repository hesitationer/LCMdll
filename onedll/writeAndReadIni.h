#pragma once
#include "onedll.h"

/***************************************************�ļ���ȡ����****************************************************************/
void WriteConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

void ReadConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

//���Զ�д����
void testReadAndWrite();
