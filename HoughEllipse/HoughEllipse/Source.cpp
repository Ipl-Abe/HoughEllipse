#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int
main(int argc, char *argv[])
{
	cv::Mat src_img = cv::imread("../snap1.png", 1);
	//cv::Mat src_img = cv::imread(argv[1], 1);
	if (src_img.empty()) return -1;

	cv::resize(src_img, src_img, cv::Size(), 0.3, 0.3);
	cv::Mat d1_img, d3_img, e1_img, e3_img;
	cv::dilate(src_img, d1_img, cv::Mat(), cv::Point(-1, -1), 1);
	cv::dilate(src_img, d3_img, cv::Mat(), cv::Point(-1, -1), 3);
	cv::erode(src_img, e1_img, cv::Mat(), cv::Point(-1, -1), 1);
	cv::erode(src_img, e3_img, cv::Mat(), cv::Point(-1, -1), 3);

	cv::namedWindow("Dilated image1", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("Dilated image2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("Eroded image1", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("Eroded image2", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

	cv::imshow("Dilated image1", d1_img);
	cv::imshow("Dilated image2", d3_img);
	cv::imshow("Eroded image1", e1_img);
	cv::imshow("Eroded image2", e3_img);


	/*cv::imwrite(__FILE__".Dilated image1.png", d1_img);//__//
	std::cout << __FILE__".Dilated image1.png" << std::endl;//__//
	cv::imwrite(__FILE__".Dilated image2.png", d3_img);//__//
	std::cout << __FILE__".Dilated image2.png" << std::endl;//__//
	cv::imwrite(__FILE__".Eroded image1.png", e1_img);//__//
	std::cout << __FILE__".Eroded image1.png" << std::endl;//__//
	cv::imwrite(__FILE__".Eroded image2.png", e3_img);//__//
	std::cout << __FILE__".Eroded image2.png" << std::endl;*///__//
	cv::waitKey(0);
}