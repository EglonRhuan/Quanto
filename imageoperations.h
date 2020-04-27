#ifndef IMAGEOPERATIONS_H
#define IMAGEOPERATIONS_H


//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
#include "QFileDialog"
#include "QString"
#include <QLabel>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "mainwindow.h"


using namespace std;

cv::Mat obterHistograma(cv::Mat &imagem);

QImage matToQImage(const cv::Mat& src);

cv::Mat qImageToMat(const QImage& qimage);

QList<QPoint> encontrarParticulas(QImage imgOriginal, QImage Qtempl, double limiar);

void zip(QString filename , QString zip_filename);

quantoTp getDadosProjeto(QString caminho);




#endif // IMAGEOPERATIONS_H
