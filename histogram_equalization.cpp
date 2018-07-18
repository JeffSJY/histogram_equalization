/*
直方图均衡化
步骤：
1、读取图像，灰度化；
2、遍历图像矩阵，并统计0-255之间的像素个数；
3、求出概率分布图以及累计分布图；
4、将像素点进行映射；
5、将映射后的值代替原图中相应的像素值；
6、显示直方图均衡化后的图像并与opencv里面内置的函数作对比

说明：该程序只是针对直方图均衡化来实现，练习；
	  至于其他的一下操作，比如灰度化等等均是利用opencv内置函数

运行环境：visual studio 2015
		  opencv3.4.1

日期：2018/07/18
Email:chengpengfei12138@163.com
*/

//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;

Mat his_equ(Mat &src_pic);

int main()
{
	Mat outdoor5_left = imread("outdoor5_left.png");//图片信息
	
	Mat out_mypic;
	out_mypic = his_equ(outdoor5_left);
	namedWindow("me");
	imshow("me", out_mypic);
	

	//opencv内置的直方图均衡化
	Mat gray_pic;
	cvtColor(outdoor5_left, gray_pic, COLOR_RGB2GRAY);
	Mat out_opencvpic;
	equalizeHist(gray_pic, out_opencvpic);
	namedWindow("opencv");
	imshow("opencv", out_opencvpic);

	waitKey(0);
	system("pause");
	return 0;
}

Mat his_equ(Mat &src_pic) {
	//对原始图像灰度化
	Mat gray_pic;
	cvtColor(src_pic, gray_pic, COLOR_RGB2GRAY);

	//遍历灰度图像矩阵，并统计0-255像素点个数
	vector<int> pixel_num(256,0);//用来存放像素点的数目
	for (int i = 0; i < gray_pic.rows; i++) {
		for (int j = 0; j < gray_pic.cols; j++) {
			pixel_num.at(gray_pic.at<uchar>(i, j))++;
		}
	}

	//求取概率分布图以及累积分布图
	double num = gray_pic.rows * gray_pic.cols;
	vector<double> pixel_probability(256,0), pro_cumulative(256,0);
	//vector<int>::iterator it;
	for (size_t i = 0; i < pixel_num.size(); i++) {
		pixel_probability.at(i) = pixel_num.at(i) / num;
		pro_cumulative.at(i) = (i > 0) ?  pixel_probability.at(i) + pixel_probability.at(i - 1) : pixel_probability.at(i);
	}

	//对原始像素进行映射
	for (size_t i = 0; i < pixel_num.size(); i++) {
		pro_cumulative.at(i) = (i > 0) ? pixel_num.at(i)*pro_cumulative.at(i) : 0;
	}

	//将原始像素替换为映射后的像素
	for (int i = 0; i < gray_pic.rows; i++) {
		for (int j = 0; j < gray_pic.cols; j++) {
			gray_pic.at<uchar>(i, j) = pro_cumulative.at(gray_pic.at<uchar>(i,j));
		}
	}
	
	return gray_pic;
}

/*
遇到的bug：
1、83行gray_pic.at<uchar>(i,j)中表示位置的(i,j)忘了谢了，报了一个内存错误；
2、68行中vector<double> pixel_probability(256,0), pro_cumulative(256,0),本来只进行了声明，而没有进行初始化
	报出了vector 越界的错误；
3、opencv 显示图像，忘了添加waitkey(0)语句，虽然没有报错，但是导致占用内存很大，而且不能正常显示图像；
	在把我写的直方图均衡化函数注释之后，仍然没有正常显示出来，才意识到这个问题；
*/

/*
总结
	不足：1、应该把灰度化图像放在his_equ(Mat &src_pic)的外面，懒得修改了；

	不解：1、我写的直方图均衡化函数和opencv内置函数得到的图像有很大差别；
			 不知道opencv里是如何优化的，有时间再看一下相关源码
*/