#include "Main.h"
#include "util.h"
#include <math.h>
#include <Windows.h>
#include <math.h>

// OpenCV includes
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;

double g_Width, g_Height;

cv::VideoCapture cap;
Mat frame;

// Para detectar as bordas
Mat src_gray, dst, detected_edges; 
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

// Para impressão do resultado
int pares = 0;
int impares = 0;

void blueFilter(const Mat& src)
{

	Mat aux;
	src.copyTo(aux);

	// Filtro azul
	Mat imgHSV;
	cvtColor(src, imgHSV, COLOR_BGR2HSV); //Converte de BGR para HSV
	Mat blueOnly;
	inRange(imgHSV, Scalar(90, 140, 0), Scalar(179, 255, 255), blueOnly);

	// Retira objetos pequenos do fundo
	erode(blueOnly, blueOnly, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(blueOnly, blueOnly, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	// Preenche gaps na imagem
	dilate(blueOnly, blueOnly, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(blueOnly, blueOnly, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	/// Reduz os ruídos
	blur(blueOnly, detected_edges, Size(3, 3));

	/// Detector de bordas Canny
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	dst = Scalar::all(0);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;


	cv::findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	int quantidade = hierarchy.size()/2; // quantidade de quadrados na tela

	// contadores para minimizar erros
	if (quantidade > 0){
		if (quantidade % 2 == 0){
			pares++;
			if (pares > 10){
				putText(detected_edges, "Par venceu!", Point(g_Width + 50, g_Height + 100), FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(90, 140, 0));
			}
		}
		else {
			impares++;
			if (impares > 10){
				putText(detected_edges, "Impar venceu!", Point(g_Width + 50, g_Height + 100), FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(90, 140, 0));
			}
		}
	} else { // zerou a rodada
		pares = 0;
		impares = 0;
	}

	src.copyTo(dst, detected_edges);

	src.copyTo(src, dst);

	imshow("Bordas", dst);

}

int main(int argc, char **argv)
{

	cap.open("Resources\\InputData\\video.mp4");

	// Webcam
	// cap.open(0);

	// Como funciona com a webcam

	if (!cap.isOpened())
	{
		cout << "error, could not open the capture" << endl;
		system("pause");
		exit(1);
	}
	
	
	while (1)
	{
		cap >> frame;

		if (frame.empty()){
			break;
		}
		blueFilter(frame);

		cv::waitKey(10);

		imshow("video", frame);
	}

	

	return 0;
}




