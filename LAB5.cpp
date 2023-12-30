// LAB5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;
Mat Moravec(Mat Img, int WinSize, int threshold, int restrainWinmsize, vector<Point3f>& featurePT);
int main()
{
	string image_path = "F:\\摄影测量\\LAB5\\R-C.jpg";
	//读入灰度图像
	Mat data = imread(image_path, IMREAD_GRAYSCALE);
    int winsize = 9;
    int threshold = 2000;
    int restrainWinSize = 35;
    vector<Point3f> featurePT1;
    Mat res = Moravec(data, winsize, threshold, restrainWinSize, featurePT1);
	imwrite("F:\\摄影测量\\LAB5\\res.png", res);

}
Mat Moravec(Mat Img, int WinSize, int threshold, int restrainWinSize, vector<Point3f>& featurePT)
{
    int rows = Img.rows;
    int cols = Img.cols;
    int k = WinSize / 2;
    Mat resData = Mat::zeros(Img.rows, Img.cols, CV_32FC1);
    for (int i = k; i < Img.rows - k; i++)
    {
        
        for (int j = k; j < Img.cols - k; j++)
        {
            int V1 = 0, V2 = 0, V3 = 0, V4 = 0;
            for (int m = -k; m <= k - 1; m++)
            {
                V1 += (Img.at<uchar>(m + i, j) - Img.at<uchar>(m + i + 1, j)) * (Img.at<uchar>(m + i, j) - Img.at<uchar>(m + i + 1, j));
                V2 += (Img.at<uchar>(m + i, j+ m) - Img.at<uchar>(m+ i + 1, j+ m+ 1)) * (Img.at<uchar>(m + i, j + m) - Img.at<uchar>(m+ i + 1, j + m + 1));
                V3 += (Img.at<uchar>(i, j + m) - Img.at<uchar>(i, j + m + 1)) * (Img.at<uchar>(i, j + m) - Img.at<uchar>(i, j + m + 1));
                V4 += (Img.at<uchar>(m + i, j - m) - Img.at<uchar>(m + i + 1, j - m - 1)) * (Img.at<uchar>(m + i, j - m) - Img.at<uchar>(m + i + 1, j - m - 1));

            }
            float IV = min(min(V1, V2), min(V3, V4));
            resData.at<float>(i, j) = IV;
            
        }
    }
    int windowSize = restrainWinSize;
    int halfWindow = windowSize / 2;
	for (int y = halfWindow; y < resData.rows - halfWindow; y += windowSize)
	{
		for (int x = halfWindow; x < resData.cols - halfWindow; x += windowSize)
		{
			float max = 0;
			bool Flag = 0;
			Point3f pt;
			pt.x = -1;
			pt.y = -1;
			pt.z = 0;
			for (int i = -halfWindow; i <= halfWindow; i++)
			{
				for (int j = -halfWindow; j <= halfWindow; j++)
				{
					float value;
					value = resData.at<float>(y + i, x + j);
					if (value > max)
					{
						max = value;
						pt.x = x + j;
						pt.y = y + i;
						pt.z = value;
						Flag = 1;
					}
				}
			}
			
			if (Flag == 1 && max > threshold)
			{
				featurePT.push_back(pt);
			}
		}
	}
	//绘制彩色特征点
	string image_path = "F:\\摄影测量\\LAB5\\R-C.jpg";
	//以RGB方式加载影像
	Mat img_source = imread(image_path, IMREAD_COLOR);
	//用圆和十字丝标出特征点
	int radius = 4;//设置圆的半径
	for (int i = 0; i < featurePT.size(); i++)
	{//循环读特征点坐标
		int xx = featurePT.at(i).x;
		int yy = featurePT.at(i).y;
		//画圆。参数CV_AA代表平滑，可以修改大小、粗细、颜色等属性。
		circle(img_source, Point(xx, yy), radius, Scalar(0, 255, 0), 1,LINE_AA)	;
		//特征点标号
		//putText(img_source, to_string(i), Point(xx + 2, yy - 2), FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(255, 0, 0), 1.2, LINE_AA);
		//画十字丝
		line(img_source, Point(xx - radius - 1, yy), Point(xx + radius + 1, yy), Scalar(0, 255, 0), 1, LINE_AA);
		line(img_source, Point(xx, yy - radius - 1), Point(xx, yy + radius + 1), Scalar(0, 255, 0), 1, LINE_AA);
	}
	//返回标记好的图像
	return img_source;
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
