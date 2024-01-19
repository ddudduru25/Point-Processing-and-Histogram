#include <iostream>
#include "opencv2/core/core.hpp" // Mat class�� ���� data structure �� ��� ��ƾ�� �����ϴ� ���
#include "opencv2/highgui/highgui.hpp" // GUI�� ���õ� ��Ҹ� �����ϴ� ���(imshow ��)
#include "opencv2/imgproc/imgproc.hpp" // ���� �̹��� ó�� �Լ��� �����ϴ� ���
using namespace cv;
using namespace std;

void SpreadSalts(Mat&, int, int, int);
void CountSalts(Mat&);
void Darker_Up(Mat&);
void Darker_Down(Mat&);
Mat GetHistogram(Mat&);

int main() {

	//�־��� ����(img1.jpg)�� ����, �Ķ�, �ʷ� ���� ���� ������ ������ŭ �������� ����, ī��Ʈ
	/*
	Mat src_img = imread("img1.jpg", 1); //�̹��� �б�
	
	SpreadSalts(src_img, 40,20,80);
	CountSalts(src_img);

	imshow("Test window", src_img); // �̹��� ���
	waitKey(0); // Ű �Է� ���(0: Ű�� �Էµ� �� ���� ���α׷� ����)
	destroyWindow("Test window"); // �̹��� ���â ����
	*/


	//���� ������ ���� ��ο���, �Ʒ��� ������ ���� ��ο���, ������׷� Ȯ��
	/*
	Mat src_img = imread("img2.jpg", 0); // �̹��� �б�
	
	//Darker_Up(src_img); 
	//Darker_Down(src_img); 
	imshow("Test window", src_img); // �̹��� ���

	Mat histo_img = GetHistogram(src_img);
	imshow("histogram", histo_img);

	waitKey(0); // Ű �Է� ���(0: Ű�� �Էµ� �� ���� ���α׷� ����)
	destroyWindow("Test window"); // �̹��� ���â ����
	destroyWindow("histogram");
	*/

	//�־��� ����(img3.jpg, img4.jpg, img5.jpg)�� �̿��� ������ ������ �ϼ�
	
	Mat img3 = imread("img3.jpg", 1);
	Mat img4 = imread("img4.jpg", 1);
	Mat dst, gray_img, mask;
	resize(img4, img4, Size(img3.cols, img3.rows));	
	subtract(img3, img4, dst); 

	Mat img5 = imread("img5.jpg", 1); 
	cvtColor(img5, gray_img, CV_BGR2GRAY); 
	Mat img6(dst, Rect(270, 350, gray_img.cols, gray_img.rows)); 
	threshold(gray_img, mask, 180, 255, THRESH_BINARY);
	mask = 120 - mask;

	img5.copyTo(img6, mask);
	imshow("Test window", dst);

	waitKey(0); 
	destroyWindow("Test window"); 
	

	return 0;
}void SpreadSalts(Mat& img, int red, int blue, int green) { //����, �Ķ�, �ʷ� �� �� ���� �Լ�

	for (int n = 0; n < blue; n++) {

		int x1 = rand() % img.cols;
		int y1 = rand() % img.rows;

		if (img.channels() == 3) 
			img.at<Vec3b>(y1, x1) = Vec3b(255, 0, 0); // blue�� ����
	}
	for (int n = 0; n < green; n++) {
		int x2 = rand() % img.cols;
		int y2 = rand() % img.rows;

		if (img.channels() == 3)
			img.at<Vec3b>(y2, x2) = Vec3b(0, 255, 0); //green���� ����
	}
	for (int n = 0; n < red; n++) {
		int x3 = rand() % img.cols;
		int y3 = rand() % img.rows;
		if (img.channels() == 3)
			img.at<Vec3b>(y3, x3) = Vec3b(0, 0, 255); //red�� ����
	}
}

void CountSalts(Mat& src) { //����, �Ķ�, �ʷ� ���� ���� ���� count�ϴ� �Լ�

	int red = 0;
	int blue = 0;
	int green = 0;

	for (int x = 0; x < src.rows; x++) {
		for (int y = 0; y < src.cols; y++) {
			if (src.at<Vec3b>(x, y) == Vec3b(255, 0, 0)) blue++;
			else if (src.at<Vec3b>(x, y) == Vec3b(0, 255, 0)) green++;
			else if (src.at<Vec3b>(x, y) == Vec3b(0, 0, 255)) red++;
		}
	}
	cout << "(red, blue, green) = (" << red << ", " << blue << ", " << green << ")";
}void Darker_Up(Mat& img) { //���� ������ ��ο���

	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			float g = float(x + 1) / float(img.rows); 
			if (img.at<uchar>(x, y) - (255 - g * 255) > 0)
				img.at<uchar>(x, y) -= (255 - g * 255);
			else
				img.at<uchar>(x, y) = 0;
		}
	}
}void Darker_Down(Mat& img) { //������ ������ ��ο���

	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			float g = float(x + 1) / float(img.rows);
			if (img.at<uchar>(x, y) - g * 255 > 0)
				img.at<uchar>(x, y) -= g * 255;
			else
				img.at<uchar>(x, y) = 0;
		}
	}
}





Mat GetHistogram(Mat& src) {
	Mat histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	calcHist(&src, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < number_bins; i++) {
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	return histImage;
}
