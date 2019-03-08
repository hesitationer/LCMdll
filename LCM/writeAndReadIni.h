#pragma once
#include "LCM.h"

/***************************************************文件读取配置****************************************************************/
void WriteConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

void ReadConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill);

//测试读写性能
void testReadAndWrite();

//0127 add 文件夹读写函数lyl
void read_files(std::vector<string> &filepaths, std::vector<string> &filenames, const string &directory);
void saveImg(int cameraFlag , int imgFlag);
void ReleaseImg(int cameraFlag); 



