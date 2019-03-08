#include "stdafx.h"
#include "LCMdll.h"
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

////文件夹读取函数  0127 add lyl
void read_files(std::vector<string> &filepaths, std::vector<string> &filenames, const string &directory)

{
	HANDLE dir;

	WIN32_FIND_DATA file_data;


	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)

		return; /* No files found */


	do {

		const string file_name = file_data.cFileName;

		const string file_path = directory + "/" + file_name;

		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;


		if (file_name[0] == '.')

			continue;


		if (is_directory)

			continue;


		filepaths.push_back(file_path);

		filenames.push_back(file_name);

	} while (FindNextFile(dir, &file_data));


	FindClose(dir);

}

//修改保存的文件夹路径
void saveImg(int cameraFlag, int imgFlag)
{

	//imwrite((*sendImg1).FileDir + "imgTFT.bmp", (*sendImg1).imgTFT);
	imwrite((*sendImg1).FileDir + "imgDust.bmp", (*sendImg1).imgDust);
	imwrite((*sendImg1).sNowTimeFileDir + "gradientDustThreshold.png", (*sendImg1).gradientDustThreshold);
	//imwrite((*sendImg1).FileDir + " BorderTFT.bmp", (*sendImg1).BorderTFT);
	//imwrite((*sendImg1).FileDir + " gradientTFT.bmp", (*sendImg1).gradientTFT);
	//imwrite((*sendImg1).FileDir + " gradientTFT_point.bmp", (*sendImg1).gradientTFT_point);

	//imwrite((*sendImg1).sNowTimeFileDir + " gradientResultThreshold.bmp", (*sendImg1).gradientResultThreshold);

	imwrite((*sendImg1).sNowTimeFileDir + "tftResultShow_final" + "_" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".png", (*sendImg1).imgTftColorCuts[cameraFlag][imgFlag]);
	imwrite((*sendImg1).FileDir + " back.bmp", (*sendImg1).back);

	imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras" + to_string(cameraFlag) + "_" + to_string(imgFlag) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][imgFlag]);
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras" + to_string(cameraFlag) + "_" + to_string(1) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][1]);
	//imwrite((*sendImg1).sNowTimeFileDir + "gradientTFTbgr_muras" + to_string(cameraFlag) + "_" + to_string(2) + ".jpg", (*sendImg1).gradientTFTbgr_muras[cameraFlag][2]);



	imwrite((*sendImg1).sNowTimeFileDir + "gradientResultThreshold_points" + to_string(cameraFlag) + to_string(imgFlag) + ".jpg", (*sendImg1).gradientResultThreshold_points[cameraFlag][imgFlag]);

	//imwrite((*sendImg1).FileDir + " tftResultShow_point.bmp", (*sendImg1).tftResultShow_point);
	//imwrite((*sendImg1).FileDir + " tftResultShow_mura.bmp", (*sendImg1).tftResultShow_mura);
	//imwrite((*sendImg1).FileDir + " imgTftColorCutFill.bmp", (*sendImg1).imgTftColorCutFill);

	//imwrite((*sendImg1).sNowTimeFileDir + " gradientTftThreshold.bmp", (*sendImg1).gradientTftThreshold);
	//imwrite((*sendImg1).FileDir + " gradientResultThreshold_point.bmp", (*sendImg1).gradientResultThreshold_point);
	//imwrite((*sendImg1).FileDir + " gradientResultThreshold_mura.bmp", (*sendImg1).gradientResultThreshold_mura);
	//imwrite((*sendImg1).sNowTimeFileDir + " gradientDustThresholDilate.bmp", (*sendImg1).gradientDustThresholDilate);

	//imwrite((*sendImg1).sNowTimeFileDir + " BorderTFT.bmp", (*sendImg1).BorderTFT);
	//imwrite((*sendImg1).sNowTimeFileDir + " gradientResult.bmp", (*sendImg1).gradientResult);

}

//12.28 跑多张图片的时候会出现内存不足的错误
//添加释放内存的函数
void ReleaseImg(int cameraFlag)
{
	//(*sendImg1).imgTFT.release();
	(*sendImg1).BorderTFT.release();
	(*sendImg1).imgTftColorCuts[cameraFlag][0].release();
	(*sendImg1).gradientDUST.release();
	//(*sendImg1).imgTftColorCutFill.release();
	//(*sendImg1).tftResultShow_final.release();
	(*sendImg1).gradientResultThreshold_points[cameraFlag][0].release();
	(*sendImg1).gradientResultThreshold_points[cameraFlag][0].release();
	(*sendImg1).gradientResultThreshold_muras[cameraFlag][0].release();
	(*sendImg1).gradientResultThreshold_subTeps[cameraFlag][0].release();


}
