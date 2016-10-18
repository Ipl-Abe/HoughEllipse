#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<vector>

using namespace cv;
#define THRESH_MAX 255


char trackbarNameThreshold1[] = "Threshold1";//トラックバーの名前

int main(){
	//int a;
	VideoCapture video("../test.MP4");
	Mat gray_img, bin_img, dst_img, dst_img2,frame;

	Mat src_img = imread("../snap3.png", 1); //3チャンネルカラー画像
	Mat srchist_img = imread("../snap3.png", 0);//グレースケール画像
	if (!src_img.data)return-1;
	if (!srchist_img.data)return-1;

	//resize(frame,frame,Size(),0.3,0.3);
	resize(src_img, src_img, Size(), 0.3, 0.3);//元画像のリサイズ
	//bilateralFilter(src_img,dst_img2,1,100,200);
	//medianBlur(src_img,src_img,13);
	//dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 1);
	//erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 3);
	GaussianBlur(src_img, src_img, Size(5,5), 2, 2);

	cvtColor(src_img, gray_img, CV_BGR2GRAY);//グレースケール変換

	//equalizeHist(gray_img, gray_img);

	//ヒストグラムを描画する画像割り当て
	const int ch_width = 400, ch_height = 300;
	Mat hist_img(Size(ch_width, ch_height), CV_8UC3, Scalar::all(255));

	Mat hist;
	const int hdims[] = { 256 };
	const float hranges[] = { 0, 256 };
	const float* ranges[] = { hranges };
	double max_val = .0;
	namedWindow("bin_image", 1);
	namedWindow("mov",1);
	int levels1 = 135;
	createTrackbar(trackbarNameThreshold1, "bin_image", &levels1, THRESH_MAX);

	//namedWindow("bin image",1);
	//シングルチャンネルのヒストグラム計算
	//画像、画像枚数、計算するチャンネル、マスク、ヒストグラム（出力）、
	//ヒストグラムの次元、ヒストグラムビンの下限
	calcHist(&srchist_img, 1, 0, Mat(), hist, 1, hdims, ranges);

	//最大値の計算
	minMaxLoc(hist, 0, &max_val);

	while (1){
		calcHist(&srchist_img, 1, 0, Mat(), hist, 1, hdims, ranges);

		//最大値の計算
		minMaxLoc(hist, 0, &max_val);

		//ヒストグラムのスケーリングと描画
		Scalar color = Scalar::all(100);
		//スケーリング
		hist = hist * (max_val ? ch_height / max_val : 0.);
		for (int j = 0; j < hdims[0]; ++j){
			int bin_w = saturate_cast<int>((double)ch_width / hdims[0]);
			rectangle(hist_img,
				Point(j*bin_w, hist_img.rows),
				Point((j + 1)*bin_w,
				hist_img.rows - saturate_cast<int>(hist.at<float>(j))),
				color, -1);

		}
		std::vector<std::vector<Point>> contors;
		threshold(gray_img, bin_img, levels1, 255, THRESH_TOZERO);
		//adaptiveThreshold(gray_img,bin_img,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,7,8);
		//dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 3);
		//(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 5);
		//dilate(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 5);
		findContours(bin_img, contors, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		imshow("bin image", bin_img);
		for (int i = 0; i < contors.size(); ++i){
			size_t count = contors[i].size();
			//std::cout <<"ellipse"<< count << std::endl;
			if (count < 360 || count>1000)continue; //(小さすぎるor大きすぎる)輪郭を除外
			std::cout << "ellipse" << count << std::endl;
			Mat pointsf;
			Mat(contors[i]).convertTo(pointsf, CV_32F);
			//楕円フィッティング
			RotatedRect box = fitEllipse(pointsf);
			//楕円の描画
			//ellipse(src_img, box, Scalar(0, 0, 255), 2, CV_AA);
			ellipse(src_img, box, Scalar(0, 0, 255), 2, CV_AA);
		}
		levels1++;
		//std::cout << levels1 << std::endl;
		if (levels1 == 170) {
			levels1 = 135;
			//break;
		}
		//namedWindow("Image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		namedWindow("Histogram", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		namedWindow("fit ellipse", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		namedWindow("bin image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		imshow("fit ellipse", src_img);
		//imshow("fit ellipse", dst_img2);
		imshow("bin image", bin_img);
		//resize(srchist_img, srchist_img, Size(), 0.3, 0.3);
		//imshow("Image", srchist_img);
		imshow("Histogram", hist_img);
		waitKey(100);
	}
}


