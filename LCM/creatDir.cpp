#include "stdafx.h"
#include "creatDir.h"
void newmkdir(string dir)
{
	int m = 0, n;
	string str1, str2;
	str1 = dir;
	str2 = str1.substr(0, 2);   //substr是C++语言函数，主要功能是复制子字符串，要求从指定位置开始，并具有指定的长度
	str1 = str1.substr(3, str1.size()); //  '//'算一个字符，转义字符
	while (m >= 0)
	{
		m = str1.find('\\');     //.find()是寻找该字符串下标位置
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //判断该目录是否存在   //c_str()：生成一个const char*指针，指向str2
									  //_access,确定文件或文件夹的访问权限。如果指定的存取方式有效，则函数返回0，否则函数返回-1。
		if (n == -1)
		{
			_mkdir(str2.c_str());     //创建目录
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}

string mkdirUseTime(string sProjectDir)
{
	//获取当前时间保持到字符串中
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char cNowTime[64];
	strftime(cNowTime, sizeof(cNowTime), "%Y%m%d-%H%M%S", &t);
	//strftime(cNowTime, sizeof(cNowTime), "%Y-%m-%d %H:%M:%S", &t);
	//格式化输出本地时间

	//判断文件夹是否存在并且命名
	cout << cNowTime;

	//string sProjectDir = "D://lcmProject//";
	newmkdir(sProjectDir);
	string sNowTimeFileDir = sProjectDir + cNowTime;
	newmkdir(sNowTimeFileDir);
	sNowTimeFileDir = sNowTimeFileDir + "//";
	return sNowTimeFileDir;
}
