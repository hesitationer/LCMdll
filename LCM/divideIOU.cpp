#include "stdafx.h"
#include "divideIOU.h"
#include"LCM.h"
#include "CalculatingDefectInformation.h"
#include "removeFalseDetect.h"

bool draw;
Mat roi;
Point cursor=Point(-1,-1);  //鼠标位置
Point peak=Point(-1,-1); //左击顶点
Point mousePosition=Point(-1,-1); //画框鼠标定点
RNG g_rng(12345);
//extern Rect rect;      //我对你无能的细节不感兴趣。




//改变一张图片的部分区域梯度值
//Mat &imgIn,	输入图片 
//Mat &imgMask,	需要改变的额区域为255，无需改变的区域为0
//Mat &imgOut,	输出图片
//float percentum  改变的系数
void changeIouGradirnt(Mat &imgIn, Mat &imgMask, Mat &imgOut, float percentum)
{
	//border取反
	Mat imgMaskinverse;
	bitwise_not(imgMask, imgMaskinverse);

	//(*sendImg1).gradientTFT与
	//Mat tmp_gradientTFT = (*sendImg1).gradientTFT.clone();//TFT 结果图

	bitwise_and(imgMaskinverse, imgIn, imgOut); //保留了中间部分梯度

	Mat tmpImgIn, Origntmp;																							 //boder二值化后与原图
	bitwise_and(imgMask, imgIn, tmpImgIn); //保留了边缘梯度

										   //对图像的梯度进行整体放缩

	Origntmp = tmpImgIn.clone();
	Origntmp = percentum * Origntmp;
	//IplImage* Origntmp = cvCloneImage(&(IplImage)tmpImgIn);   //Mat->cvArr可以通过IplImage+&的方法
	//double f2 = 0.01*f1; //减少trackerbar只能int的影响
	//cvConvertScale(Origntmp, Origntmp, percentum, 0);
	//Mat convImg = cvarrToMat(Origntmp, true);     //cvArr->MAt								  //两图相加
	imgOut = (Origntmp + imgOut);//获得最终结果图
}


//鼠标选定时画框辅助确定
void DrawRectangle(Mat& img, Rect box)
{
	rectangle(img, box.tl(), box.br(), Scalar(0, 255, 0), 2);
}

//****************鼠标事件选定缩小梯度值ROI区域************
void onMouse(int event, int x, int y, int flags, void *param)
{
	Mat img = (*sendImg1).getMouseChooseimg.clone();   //此处源图像为可更改的
	switch (event)
	{
		//按下鼠标左键
	case CV_EVENT_LBUTTONDOWN:
		//****点击鼠标图象时的操作
		//清除之前ROI图像的显示窗口
		cvDestroyWindow("ROI");
		//初始化
		cursor = Point(x, y);
		//Rect rect = (*sendImg1).rect_getIOU;
		(*sendImg1).rect_getIOU = Rect(x, y, 0, 0);
		draw = true;



		break;

		//松开鼠标左键
	case CV_EVENT_LBUTTONUP:
		if ((*sendImg1).rect_getIOU.height > 0 && (*sendImg1).rect_getIOU.width > 0)
		{
			draw = false;


			//将img中的矩形复制给ROI
			roi = img(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));
			rectangle(img, (*sendImg1).rect_getIOU, Scalar(0, 255, 0), 2);
			namedWindow("SignROI");
			imshow("SignROI", img);

			//调用函数进行绘制




			DrawRectangle(img, (*sendImg1).rect_getIOU);

			//changeIOUthreshod((*sendImg1).rect_getIOU);

			//removeFalseDefects2();

		}

		break;

		//移动光标
	case CV_EVENT_MOUSEMOVE:
		if (draw)
		{
			//用MIN得到左上点作为矩形框的起始坐标，否则画矩形时只能往一个方向
			(*sendImg1).rect_getIOU.x = MIN(x, cursor.x);
			(*sendImg1).rect_getIOU.y = MIN(y, cursor.y);
			(*sendImg1).rect_getIOU.width = abs(cursor.x - x);
			(*sendImg1).rect_getIOU.height = abs(cursor.y - y);
			//防止矩形区域超出图像的范围
			(*sendImg1).rect_getIOU &= Rect(0, 0, img.cols, img.rows);


		}
		break;

	}
}
// 加入实时画框
void onMouse1(int event, int x, int y, int flags, void *param)
{
	//Mat img = (*sendImg1).getMouseChooseimg.clone();   //此处源图像为可更改的
	Mat & img = *(cv::Mat*) param;
	char temp[16];
	switch (event)
	{
		//按下鼠标左键
	case CV_EVENT_LBUTTONDOWN:
	{	//****点击鼠标图象时的操作
		//清除之前ROI图像的显示窗口
		cvDestroyWindow("ROI");
		//初始化
		cursor = Point(x, y);
		//Rect rect = (*sendImg1).rect_getIOU;
		(*sendImg1).rect_getIOU = Rect(x, y, 0, 0);
		//实时画框
		sprintf_s(temp, "(%d,%d)", x, y);
		putText(img, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		draw = true;
		peak = Point(x, y);


		break;
	}
	//松开鼠标左键
	case CV_EVENT_LBUTTONUP:
	{
		

		
		
		
		if ((*sendImg1).rect_getIOU.height > 0 && (*sendImg1).rect_getIOU.width > 0)
		{
			draw = false;
			//将img中的矩形复制给ROI
			roi = img(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));

			//实时画框
			sprintf_s(temp, "(%d,%d)", x, y);
			putText(img, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			rectangle(img, peak, mousePosition, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));

			// 调用函数
			//changeIOUthreshod((*sendImg1).rect_getIOU);
			vector<defectInformation> defect[10];


			
			//将img中的矩形复制给ROI
			roi = (*sendImg1).RemovefixedDefect(Rect((*sendImg1).rect_getIOU.x, (*sendImg1).rect_getIOU.y, (*sendImg1).rect_getIOU.width, (*sendImg1).rect_getIOU.height));
			roi.setTo(0);
			namedWindow("SrcImage1", 0);
			imshow("SrcImage1", (*sendImg1).RemovefixedDefect);



		
			//removePointFalse((*sendImg1).ResultMouse, (*sendImg1).contours_points[0][0], (*sendImg1).pointResults[0][0],PointCanShureMoveFalse ,0,0,false , defect);
		}

	}	break;

	//移动光标
	case CV_EVENT_MOUSEMOVE:
	{
		mousePosition = Point(x, y);
		if (draw)
		{
			//用MIN得到左上点作为矩形框的起始坐标，否则画矩形时只能往一个方向
			(*sendImg1).rect_getIOU.x = MIN(x, cursor.x);   //问题出在这里，不知道为何函数结果为0
			(*sendImg1).rect_getIOU.y = MIN(y, cursor.y);
			(*sendImg1).rect_getIOU.width = abs(cursor.x - x);
			(*sendImg1).rect_getIOU.height = abs(cursor.y - y);
			//防止矩形区域超出图像的范围
			(*sendImg1).rect_getIOU &= Rect(0, 0, img.cols, img.rows);


		}
	}	break;

	}
}

//**************鼠标选定后改变阈值******************
//bug记录：之前对src设置有问题，之前的版本中src输入为最后的显示结果图，实际上这张图只是用来辅助确定假缺陷的位置，我们要修改的实际为梯度图
//对假缺陷做排除工作的也是梯度图，因此要更改以下函数中src引入的源图和count_gradient3中的计算图
//
void changeIOUthreshod(Rect rect)
{
	double f4 = 0.0;
	Mat src = (*sendImg1).gradientTtfMaskValue.clone();
	imshow("before mask zero", src);

	//对输入原图像整体乘系数f4
	Mat Originimg = src.clone();
	IplImage* Origntmp = cvCloneImage(&(IplImage)Originimg);   //Mat->cvArr可以通过IplImage+&的方法
	cvConvertScale(Origntmp, Origntmp, f4, 0);
	Mat convImg = cvarrToMat(Origntmp, true);     //cvArr->MAt

												  //在convImg上取得rect处的梯度
	Mat roi_changeThreshold = convImg(Rect(rect.x, rect.y, rect.width, rect.height));

	//被粘贴的图片上要获得一个粘贴图片大小的位置，必须为Mat属性
	Mat roi = src(Rect(rect.x, rect.y, rect.width, rect.height));
	roi_changeThreshold.copyTo(roi);

	imshow("after mask zero", src);
	(*sendImg1).ResultMouse = src;     //从这张图中可以明显看到函数去除了之前的梯度


}

//对 调整梯度的R角区域模板  进行处理    
void getBorderThreshold(float f1,Mat &maskChangeGradietRCorner)
{
	changeIouGradirnt((*sendImg1).gradientTFT ,maskChangeGradietRCorner, (*sendImg1).plusResult, f1);
	//namedWindow("plusResult", 0);
	//imshow("plusResult", (*sendImg1).plusResult);
	//imwrite("C:\\cppProjects\\Application\\lcmProject\\Data\\plusResult.png", (*sendImg1).plusResult);
}
//对 制作调整梯度的缩进区域模板 进行处理
void getShrinkThreshold(float f3,Mat &maskChangeGradientDrawIn)
{

	changeIouGradirnt((*sendImg1).gradientTFT, maskChangeGradientDrawIn, (*sendImg1).OuterBorderTFTImg, f3);
	//namedWindow("OuterBorderResult", 0);
	//imshow("OuterBorderResult", (*sendImg1).OuterBorderTFTImg);

}
//制作调整梯度的R角区域模板  
void getRCornerMask(Mat &imgIn, Mat &imgOut, int kernelSize)

{
	Mat kerne20 = cv::getStructuringElement(cv::MORPH_RECT, Size(kernelSize, kernelSize));
	Mat dilateBorderTFT;
	dilate(imgIn, imgOut, kerne20);  //膨胀后的梯度图
}
//制作调整梯度的缩进区域模板 
void getDrawInAreaMask(Mat &imgIn, Mat &imgOut, int size_x_left, int size_x_right, int size_y_up, int size_y_down)
{
	
	Mat tmp_gradientTFT = Mat::zeros((*sendImg1).PersPectiveTransformation_hight, (*sendImg1).PersPectiveTransformation_weight, CV_8UC1);

	//********获得内缩区域*********
	// roi区域边界长内缩6个像素，宽内缩3个像素
	vector<Point2f>new_poly;
	Point2f new_p1(0 + size_x_left, tmp_gradientTFT.rows - size_y_down);
	Point2f new_p2(0 + size_x_left, 0 + size_y_up);
	Point2f new_p3(tmp_gradientTFT.cols - size_x_right, 0 + size_y_up);
	Point2f new_p4(tmp_gradientTFT.cols - size_x_right, tmp_gradientTFT.rows - size_y_down);

	new_poly.push_back(new_p1);
	new_poly.push_back(new_p2);
	new_poly.push_back(new_p3);
	new_poly.push_back(new_p4);

	int h = new_p1.y - new_p2.y;
	int w = new_p4.x - new_p1.x;

	//*****获得内缩后的图****
	Mat tmp_gradienttft = tmp_gradientTFT(Rect(new_p2.x, new_p2.y, w, h));

	//构建与内缩图同样大小的0图
	Mat padImg = Mat::zeros(Size(tmp_gradienttft.cols, tmp_gradienttft.rows), CV_8UC1); //改为c3时会在与时内存异常

																						//构建与原图同样大小的255图
	Mat templateImg = Mat::zeros(Size(tmp_gradientTFT.cols, tmp_gradientTFT.rows), CV_8UC1);

	Mat templateImg_inverse;
	bitwise_not(templateImg, templateImg_inverse);

	//padImg叠加到templateImg_inverse
	Mat roi = templateImg_inverse(Rect(new_p2.x, new_p2.y, w, h));
	padImg.copyTo(roi);

	imgOut = templateImg_inverse;
	//changeIouGradirnt((*sendImg1).gradientTFT, templateImg_inverse, (*sendImg1).OuterBorderTFTImg, f3);
}
//制作调整梯度的鼠标选定区域模板 
//
//maskChangeGradientMouseArea


//需要用到全局变量  保存一张Mask图片 (*sendImg1).RemovefixedDefect    
void setMouseChoseArea(Mat &imgIn)
{
	(*sendImg1).RemovefixedDefect = Mat(imgIn.rows, imgIn.cols, CV_8UC1, Scalar(255, 255, 255));
	//namedWindow("SrcImage", 0);
	//imshow("SrcImage",(*sendImg1).RemovefixedDefect);
	//waitKey(0);
	//鼠标圈定
	(*sendImg1).getMouseChooseimg = imgIn.clone();  //为鼠标圈定的图赋值
	waitKey(0);
	system("color 9F");
	Mat temp1;
	while (waitKey(30) != 27)
	{
		//long t1 = GetTickCount();
		(*sendImg1).getMouseChooseimg.copyTo(temp1);
		namedWindow("SrcImage",0);
		imshow("SrcImage", (*sendImg1).getMouseChooseimg);
		setMouseCallback("SrcImage", onMouse1, (void*)&(*sendImg1).getMouseChooseimg);
		//setMouseCallback("SrcImage", onMouse, NULL);
		if (draw)
		{
			rectangle(temp1, peak, mousePosition, cv::Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)));
			putText(temp1, "(" + std::to_string(mousePosition.x) + "," + std::to_string(mousePosition.y) + ")", mousePosition, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			imshow("SrcImage", temp1);
		}
	}
}
//保存需要用到的三张图片
//int size_x_left, int size_x_right, int size_y_up, int size_y_down, double f3,if useMuoseChose,MouseChose float


/*
//初始化用于区域调整的mask区域
// 输入梯度图和边界图，得到边界mask和内缩mask
*/
void initChangePartOfTheRegionalGradient(Mat ImgIn, Mat bordertft, Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, CanShuMask canshumask)
{
	//对border进行膨胀，获取mask模板  需要调整的地方为255，无需调整的为0
	getRCornerMask(bordertft, maskChangeGradietRCorner, canshumask._keneralsize[0][0]);
	//获取需要调整梯度的内缩区域模板 ， 需要调整的地方为255，无需调整的为0
	getDrawInAreaMask(ImgIn, maskChangeGradientDrawIn, canshumask._size_x_left[0][0], canshumask._size_x_right[0][0], canshumask._size_y_up[0][0], canshumask._size_y_down[0][0]);
	//改变圆弧区域梯度值    A=  A*f1
}

void initChangePartOfTheRegionalGradient(Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn,CanShuMask canshumask)
{

	//对border进行膨胀，获取mask模板  需要调整的地方为255，无需调整的为0
	getRCornerMask((*sendImg1).BorderTFT, maskChangeGradietRCorner, canshumask._keneralsize[0][0]);
	//获取需要调整梯度的内缩区域模板 ， 需要调整的地方为255，无需调整的为0
	getDrawInAreaMask((*sendImg1).GradientResult_point[0][0], maskChangeGradientDrawIn, canshumask._size_x_left[0][0], canshumask._size_x_right[0][0], canshumask._size_y_up[0][0], canshumask._size_y_down[0][0]);
	//改变圆弧区域梯度值    A=  A*f1
}

void setEachMethodMask()
{
	

	imwrite((*sendImg1).sNowTimeFileDir + "BorderTFTXXX"+ ".jpg", (*sendImg1).BorderTFT);
	
	//!!!!!!!!不同的相机需要不同的缩进吗
	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_point[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).PointMaskChangeGradietRCorner, (*sendImg1).PointMaskChangeGradientDrawIn, *PointCanShuMask);   //20 10 20 50 60
																												   //创建团的Mask模板																										   //************  图片1和图片4的大小一样，图片2和图片3的大小一样

	initChangePartOfTheRegionalGradient((*sendImg1).gradientTFT_muras[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).MuraMaskChangeGradietRCorner, (*sendImg1).MuraMaskChangeGradientDrawIn, *MuraCanShuMask);
	//0219 add sub的缩进mask操作

	initChangePartOfTheRegionalGradient((*sendImg1).subtractTemplateTftImgs[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubMaskChangeGradietRCorner, (*sendImg1).SubMaskChangeGradientDrawIn, *SubCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).GradientResult_louguang[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).LightMaskChangeGradietRCorner, (*sendImg1).LightMaskChangeGradientDrawIn, *lightCanShuMask);

	initChangePartOfTheRegionalGradient((*sendImg1).gradientResultThreshold_bgr_sub[0][0], (*sendImg1).BorderTFT,
		(*sendImg1).SubBGRMaskChangeGradietRCorner, (*sendImg1).SubBGRMaskChangeGradientDrawIn, *SubBGRCanShuMask);



}



//改变调整区域的调整参数，目前默认为0.0 
// 输入梯度图，通过init得到的边界mask和内缩mask对梯度图在mask区域乘上系数
void ChangePartOfTheRegionalGradient(Mat &maskChangeGradietRCorner, Mat &maskChangeGradientDrawIn, Mat &imgIn, Mat &imgOut, float f1, float f2)
{

	//R角区域乘以系数

	changeIouGradirnt(imgIn, maskChangeGradietRCorner, imgOut, f1);
	//changeIouGradirnt((*sendImg1).gradientTFT, (*sendImg1).maskChangeGradietRCorner, (*sendImg1).plusResult, f1);
	(*sendImg1).plusResult = imgOut.clone();
	//namedWindow("plusResult", 0);
	//imshow("plusResult", (*sendImg1).plusResult);

	//缩进区域乘以系数
	changeIouGradirnt(imgOut, maskChangeGradientDrawIn, imgOut, f2);

	//changeIouGradirnt((*sendImg1).gradientTFT, (*sendImg1).maskChangeGradientDrawIn, (*sendImg1).OuterBorderTFTImg, f2);
	(*sendImg1).OuterBorderTFTImg = imgOut.clone();
	//namedWindow("OuterBorderResult", 0);
	//imshow("OuterBorderResult",imgOut );

	//鼠标选定区域乘以系数
	//maskChangeGradientMouseArea
	//12.19  鼠标圈定区域函数暂未使用

}