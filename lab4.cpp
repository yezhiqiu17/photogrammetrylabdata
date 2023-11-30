// lab4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "stdio.h"
#include <iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;


int main()
{
	Mat data1 = imread("E:\\摄影测量\\LAB4\\data\\10040084.tif");
	int imw1 = data1.cols;
	int imh1 = data1.rows;

	double x0 = 0, y0 = 0, f = 120, XS1 = 497296.587, YS1 = 5419884.550, ZS1 = 1185.010, u1 = 0.0083296730, v1 = 0.0128017031, k1 = -3.1315833588;
	double XS2 = 497048.699, YS2 = 5419882.847, ZS2 = 1184.616, u2 = 0.0063112400, v2 = 0.0054716030, k2 = -3.1329438244;

	FILE* fp_in1 = fopen("E:\\摄影测量\\LAB4\\data\\left4.txt", "r");
	int num = 38;//
	double* ix1 = new double[num];
	double* iy1 = new double[num];

	for (int i = 0; i < num; i++)
	{
		fscanf(fp_in1, "%lf%lf", &ix1[i], &iy1[i]);
	}
	cout << "左影像读入成功！" << endl;
	fclose(fp_in1);
	fp_in1 = NULL;

	Mat data2 = imread("E:\\摄影测量\\LAB4\\data\\10040083.tif");
	int imw2 = data2.cols;
	int imh2 = data2.rows;

	FILE* fp_in2 = fopen("E:\\摄影测量\\LAB4\\data\\right4.txt", "r");
	double* ix2 = new double[num];
	double* iy2 = new double[num];
	for (int i = 0; i < num; i++)
	{
		fscanf(fp_in2, "%lf%lf", &ix2[i], &iy2[i]);
	}
	cout << "右影像读入成功！" << endl;
	fclose(fp_in2);
	fp_in2 = NULL;

	for (int i = 0; i < num; i++)
	{
		ix1[i] = (ix1[i] - int(imw1 / 2)) * 0.012;
		iy1[i] = (iy1[i] - int(imh1 / 2)) * 0.012;
		ix2[i] = (ix2[i] - int(imw2 / 2)) * 0.012;
		iy2[i] = (iy2[i] - int(imh2 / 2)) * 0.012;
	}

	double Bx, By, Bz;
	Bx = XS2 - XS1;
	By = YS2 - YS1;
	Bz = ZS2 - ZS1;

	double S1a1, S1a2, S1a3, S1b1, S1b2, S1b3, S1c1, S1c2, S1c3, S2a1, S2a2, S2a3, S2b1, S2b2, S2b3, S2c1, S2c2, S2c3;
	S1a1 = cos(u1) * cos(k1) - sin(v1) * sin(u1) * sin(k1);
	S1a2 = -cos(u1) * sin(k1) - sin(u1) * sin(v1) * cos(k1);
	S1a3 = -sin(u1) * cos(v1);
	S1b1 = cos(v1) * sin(k1);
	S1b2 = cos(v1) * cos(k1);
	S1b3 = -sin(v1);
	S1c1 = sin(u1) * cos(k1) + cos(u1) * sin(v1) * sin(k1);
	S1c2 = -sin(u1) * sin(k1) + cos(u1) * sin(v1) * cos(k1);
	S1c3 = cos(u1) * cos(v1);
	S2a1 = cos(u2) * cos(k2) - sin(v2) * sin(u2) * sin(k2);
	S2a2 = -cos(u2) * sin(k2) - sin(u2) * sin(v2) * cos(k2);
	S2a3 = -sin(u2) * cos(v2);
	S2b1 = cos(v2) * sin(k2);
	S2b2 = cos(v2) * cos(k2);
	S2b3 = -sin(v2);
	S2c1 = sin(u2) * cos(k2) + cos(u2) * sin(v2) * sin(k2);
	S2c2 = -sin(u2) * sin(k2) + cos(u2) * sin(v2) * cos(k2);
	S2c3 = cos(u2) * cos(v2);

	double* X1 = new double[num];
	double* Y1 = new double[num];
	double* Z1 = new double[num];
	double* X2 = new double[num];
	double* Y2 = new double[num];
	double* Z2 = new double[num];
	for (int i = 0; i < num; i++)
	{
		X1[i] = S1a1 * ix1[i] + S1a2 * iy1[i] - S1a3 * f;
		Y1[i] = S1b1 * ix1[i] + S1b2 * iy1[i] - S1b3 * f;
		Z1[i] = S1c1 * ix1[i] + S1c2 * iy1[i] - S1c3 * f;
		X2[i] = S2a1 * ix2[i] + S2a2 * iy2[i] - S2a3 * f;
		Y2[i] = S2b1 * ix2[i] + S2b2 * iy2[i] - S2b3 * f;
		Z2[i] = S2c1 * ix2[i] + S2c2 * iy2[i] - S2c3 * f;
	}

	double* N1 = new double[num];
	double* N2 = new double[num];
	double* XA = new double[num];
	double* YA = new double[num];
	double* ZA = new double[num];
	for (int i = 0; i < num; i++)
	{
		N1[i] = (Bx * Z2[i] - Bz * X2[i]) / (X1[i] * Z2[i] - X2[i] * Z1[i]);
		N2[i] = (Bx * Z1[i] - Bz * X1[i]) / (X1[i] * Z2[i] - X2[i] * Z1[i]);
		XA[i] = XS1 + N1[i] * X1[i];
		ZA[i] = ZS1 + N1[i] * Z1[i];
		YA[i] = 0.5 * (YS1 + N1[i] * Y1[i] + YS2 + N2[i] * Y2[i]);
	}
	cout << "坐标成功计算！" << endl;
	FILE* fp_out = fopen("E:\\摄影测量\\LAB4\\data\\res.txt", "w");
	for (int i = 0; i < num; i++)
	{
		fprintf(fp_out, "%lf %lf %lf\n", XA[i], YA[i], ZA[i]);
	}
	fclose(fp_out);
	fp_out = NULL;

	double* ixl = new double[num];
	double* iyl = new double[num];
	double* ixr = new double[num];
	double* iyr = new double[num];
	double errorx1 = 0, errory1 = 0, errorx2 = 0, errory2 = 0;

	for (int i = 0; i < num; i++)
	{
		ixl[i] = -f * (S1a1 * (XA[i] - XS1) + S1b1 * (YA[i] - YS1) + S1c1 * (ZA[i] - ZS1)) / (S1a3 * (XA[i] - XS1) + S1b3 * (YA[i] - YS1) + S1c3 * (ZA[i] - ZS1));
		iyl[i] = -f * (S1a2 * (XA[i] - XS1) + S1b2 * (YA[i] - YS1) + S1c2 * (ZA[i] - ZS1)) / (S1a3 * (XA[i] - XS1) + S1b3 * (YA[i] - YS1) + S1c3 * (ZA[i] - ZS2));
		ixr[i] = -f * (S2a1 * (XA[i] - XS2) + S2b1 * (YA[i] - YS2) + S2c1 * (ZA[i] - ZS2)) / (S2a3 * (XA[i] - XS2) + S2b3 * (YA[i] - YS2) + S2c3 * (ZA[i] - ZS2));
		iyr[i] = -f * (S2a2 * (XA[i] - XS2) + S2b2 * (YA[i] - YS2) + S2c2 * (ZA[i] - ZS2)) / (S2a3 * (XA[i] - XS2) + S2b3 * (YA[i] - YS2) + S2c3 * (ZA[i] - ZS2));

		ixl[i] = fabs(ixl[i] - ix1[i]);
		iyl[i] = fabs(iyl[i] - iy1[i]);
		ixr[i] = fabs(ixr[i] - ix2[i]);
		iyr[i] = fabs(iyr[i] - iy2[i]);



		errorx1 += ixl[i];
		errory1 += iyl[i];
		errorx2 += ixr[i];
		errory2 += iyr[i];
	}
	errorx1 = errorx1 / num / 0.012;
	errory1 = errory1 / num / 0.012;
	errorx2 = errorx2 / num / 0.012;
	errory2 = errory2 / num / 0.012;


	FILE* fp_out2 = fopen("E:\\摄影测量\\LAB4\\data\\error.txt", "w");
	fprintf(fp_out2, "%lf %lf %lf %lf\n", errorx1, errory1, errorx2, errory2);
	fclose(fp_out2);
	fp_out2 = NULL;
	delete[]ix1; ix1 = NULL;
	delete[]iy1; iy1 = NULL;
	delete[]ix2; ix2 = NULL;
	delete[]iy2; iy2 = NULL;
	delete[]X1; X1 = NULL;
	delete[]Y1; Y1 = NULL;
	delete[]Z1; Z1 = NULL;
	delete[]X2; X2 = NULL;
	delete[]Y2; Y2 = NULL;
	delete[]Z2; Z2 = NULL;
	delete[]N1; N1 = NULL;
	delete[]N2; N2 = NULL;
	delete[]XA; XA = NULL;
	delete[]YA; YA = NULL;
	delete[]ZA; ZA = NULL;
	delete[]ixl; ixl = NULL;
	delete[]iyl; iyl = NULL;
	delete[]ixr; ixr = NULL;
	delete[]iyr; iyr = NULL;


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
