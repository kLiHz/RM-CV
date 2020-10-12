#include <iostream>
#include <opencv2/opencv.hpp>
int main(int argc, char* argv[])
{
	std::string filename;
	if (argc == 2) {
		filename = argv[1];
	} else {
		std::cout << "Input Filename: ";
		std::cin >> filename;
	}
	cv::Mat img = cv::imread(filename);
	if (!img.empty()) {
		std::cout << "Image loaded successfully.\n";
		cv::namedWindow(filename, cv::WINDOW_NORMAL);
		cv::imshow(filename,img);
		cv::waitKey(0);
	} else {
		std::cout << "Unable to open '" << filename << "'.\n";
	}
	return 0;
}
