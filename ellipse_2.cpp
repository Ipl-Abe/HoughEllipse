#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

int main(){
	Mat src_img = imread("../snap1.png",1);
	if (!src_img.data)return-1;

	Mat gray_img, bin_img;
	resize(src_img,src_img,Size(),0.3,0.3);
	cvtColor(src_img, gray_img, CV_BGR2GRAY);

	std::vector<std::vector<Point>> contors;
	//画像の２値化
	threshold(gray_img, bin_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//輪郭の検出
	findContours(bin_img,contors,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);

	for (int i = 0; i < contors.size(); ++i){
		size_t count = contors[i].size();
		if (count < 40 || count > 1000) continue; //(小さすぎるor大きすぎる)輪郭を除外
		Mat pointsf;
		Mat(contors[i]).convertTo(pointsf,CV_32F);
		//楕円フィッティング
		RotatedRect box = fitEllipse(pointsf);
		//楕円の描画
		ellipse(src_img,box,Scalar(0,0,255),2,CV_AA);

	}
	namedWindow("fit ellipse", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	namedWindow("bin image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	imshow("fit ellipse",src_img);
	imshow("bin image", bin_img);
	waitKey(0);
}
