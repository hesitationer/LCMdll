#pragma once
#include "onedll.h"
/*包含用于debug模式的代码
设定滑竿
void setTrackbar();
/
/用于去除假缺陷的我参数
void setremoveFalseTrackbar();

//用于调整分区域阈值参数的
void setMaskTrackbar();

用于保存图片的函数
void saveImg();

用于显示程序运行过程的中间结果
void showProcessImg();
void showProcessImg(sendImg * sendImg2);

*/


//设定滑竿
void setTrackbar();
void setremoveFalseTrackbar();
void setMaskTrackbar();


//读取滑竿
void setCanshuUseTrackbar();
void setCanshuMaskUseTrackbar();
void UseremoveFalseTrackbar();

//显示处理过程的一些中间结果
void showProcessImg();
void showProcessImg(sendImg * sendImg2);



//void saveImg();
