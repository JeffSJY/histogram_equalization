/*
ֱ��ͼ���⻯
���裺
1����ȡͼ�񣬻ҶȻ���
2������ͼ����󣬲�ͳ��0-255֮������ظ�����
3��������ʷֲ�ͼ�Լ��ۼƷֲ�ͼ��
4�������ص����ӳ�䣻
5����ӳ����ֵ����ԭͼ����Ӧ������ֵ��
6����ʾֱ��ͼ���⻯���ͼ����opencv�������õĺ������Ա�

˵�����ó���ֻ�����ֱ��ͼ���⻯��ʵ�֣���ϰ��
	  ����������һ�²���������ҶȻ��ȵȾ�������opencv���ú���

���л�����visual studio 2015
		  opencv3.4.1

���ڣ�2018/07/18
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
	Mat outdoor5_left = imread("outdoor5_left.png");//ͼƬ��Ϣ
	
	Mat out_mypic;
	out_mypic = his_equ(outdoor5_left);
	namedWindow("me");
	imshow("me", out_mypic);
	

	//opencv���õ�ֱ��ͼ���⻯
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
	//��ԭʼͼ��ҶȻ�
	Mat gray_pic;
	cvtColor(src_pic, gray_pic, COLOR_RGB2GRAY);

	//�����Ҷ�ͼ����󣬲�ͳ��0-255���ص����
	vector<int> pixel_num(256,0);//����������ص����Ŀ
	for (int i = 0; i < gray_pic.rows; i++) {
		for (int j = 0; j < gray_pic.cols; j++) {
			pixel_num.at(gray_pic.at<uchar>(i, j))++;
		}
	}

	//��ȡ���ʷֲ�ͼ�Լ��ۻ��ֲ�ͼ
	double num = gray_pic.rows * gray_pic.cols;
	vector<double> pixel_probability(256,0), pro_cumulative(256,0);
	//vector<int>::iterator it;
	for (size_t i = 0; i < pixel_num.size(); i++) {
		pixel_probability.at(i) = pixel_num.at(i) / num;
		pro_cumulative.at(i) = (i > 0) ?  pixel_probability.at(i) + pixel_probability.at(i - 1) : pixel_probability.at(i);
	}

	//��ԭʼ���ؽ���ӳ��
	for (size_t i = 0; i < pixel_num.size(); i++) {
		pro_cumulative.at(i) = (i > 0) ? pixel_num.at(i)*pro_cumulative.at(i) : 0;
	}

	//��ԭʼ�����滻Ϊӳ��������
	for (int i = 0; i < gray_pic.rows; i++) {
		for (int j = 0; j < gray_pic.cols; j++) {
			gray_pic.at<uchar>(i, j) = pro_cumulative.at(gray_pic.at<uchar>(i,j));
		}
	}
	
	return gray_pic;
}

/*
������bug��
1��83��gray_pic.at<uchar>(i,j)�б�ʾλ�õ�(i,j)����л�ˣ�����һ���ڴ����
2��68����vector<double> pixel_probability(256,0), pro_cumulative(256,0),����ֻ��������������û�н��г�ʼ��
	������vector Խ��Ĵ���
3��opencv ��ʾͼ���������waitkey(0)��䣬��Ȼû�б������ǵ���ռ���ڴ�ܴ󣬶��Ҳ���������ʾͼ��
	�ڰ���д��ֱ��ͼ���⻯����ע��֮����Ȼû��������ʾ����������ʶ��������⣻
*/

/*
�ܽ�
	���㣺1��Ӧ�ðѻҶȻ�ͼ�����his_equ(Mat &src_pic)�����棬�����޸��ˣ�

	���⣺1����д��ֱ��ͼ���⻯������opencv���ú����õ���ͼ���кܴ���
			 ��֪��opencv��������Ż��ģ���ʱ���ٿ�һ�����Դ��
*/