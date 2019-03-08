#include "stdafx.h"
#include "onedll.h"
#include "writeAndReadIni.h"
/***************************************************文件读取配置****************************************************************/
void WriteConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill)
{
	char name[128] = "logo";
	//将参数写入config文件中
	WritePrivateProfileString(name, ("_TFTHOLD"), to_string(_TFTHOLD).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_DUSTHOLD"), to_string(_DUSTHOLD).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_DUSTSHAPE"), to_string(_DUSTSHAPE).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_num_gradient"), to_string(_num_gradient).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_num_grayScale"), to_string(_num_grayScale).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_dst_m_gradient"), to_string(_dst_m_gradient).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_dst_m_grayScale"), to_string(_dst_m_grayScale).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_dst_s_grayScale"), to_string(_dst_s_grayScale).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_meanValueGradient"), to_string(_meanValueGradient).c_str(), configDir.c_str());
	WritePrivateProfileString(name, ("_fill"), to_string(_fill).c_str(), configDir.c_str());

}

void ReadConfig(string configDir, int _TFTHOLD, int _DUSTHOLD, int _DUSTSHAPE, int _num_gradient, int _num_grayScale, double _dst_m_gradient, double _dst_m_grayScale, double _dst_s_grayScale, double _meanValueGradient, int _fill)
{
	char name[128] = "logo";
	//读取config文件
	_TFTHOLD = GetPrivateProfileInt(name, ("_TFTHOLD"), 100, configDir.c_str());
	_DUSTHOLD = GetPrivateProfileInt(name, ("_DUSTHOLD"), 60, configDir.c_str());
	_DUSTSHAPE = GetPrivateProfileInt(name, ("_DUSTSHAPE"), 90, configDir.c_str());
	_num_gradient = GetPrivateProfileInt(name, ("_num_gradient"), 1, configDir.c_str());
	_num_grayScale = GetPrivateProfileInt(name, ("_num_grayScale"), 0, configDir.c_str());
	_fill = GetPrivateProfileInt(name, ("_fill"), 0, configDir.c_str());
	cout << _fill << endl;
	cout << _num_grayScale << endl;


	char _dst_m_gradient_get[128];
	GetPrivateProfileString(name, "_dst_m_gradient", "0.000000", _dst_m_gradient_get, 128, configDir.c_str());
	_dst_m_gradient = atof(_dst_m_gradient_get); //atof()作用为char转为double

	char _dst_m_grayScale_get[128];
	GetPrivateProfileString(name, "_dst_m_grayScale", "0.000000", _dst_m_grayScale_get, 128, configDir.c_str());
	_dst_m_grayScale = atof(_dst_m_grayScale_get); //atof()作用为char转为double

	char _dst_s_grayScale_get[128];
	GetPrivateProfileString(name, "_dst_s_grayScale", "0.000000", _dst_s_grayScale_get, 128, configDir.c_str());
	_dst_s_grayScale = atof(_dst_s_grayScale_get); //atof()作用为char转为double

	char _meanValueGradient_get[128];
	GetPrivateProfileString(name, "_meanValueGradient", "0.000000", _meanValueGradient_get, 128, configDir.c_str());
	_meanValueGradient = atof(_meanValueGradient_get); //atof()作用为char转为double



}

//测试读写性能
void testReadAndWrite()
{

	//测试读写

	string configDir = "D:\\lcmConfig.ini";
	WriteConfig(configDir, 10, 20, 30, 40, 50, 60.2, 70.5, 80.6, 90.2, 80);
	int _TFTHOLD = 10;
	int _DUSTHOLD = 0;
	int _DUSTSHAPE = 0;
	int _num_gradient = 0;
	int _num_grayScale = 0;
	double _dst_m_gradient = 0;
	double _dst_m_grayScale = 0;
	double _dst_s_grayScale = 0;
	double _meanValueGradient = 0;
	int _fill = 0;
	ReadConfig(configDir, _TFTHOLD, _DUSTHOLD, _DUSTSHAPE, _num_gradient, _num_grayScale, _dst_m_gradient, _dst_m_grayScale, _dst_s_grayScale, _meanValueGradient, _fill);
}
