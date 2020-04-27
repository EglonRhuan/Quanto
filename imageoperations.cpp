#include "imageoperations.h"
//#include <QMessageBox>
#include "QMessageBox"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
#include <QBytearray>
#include "qbuffer.h"


using namespace cv;

Mat obterHistograma(Mat &imagem){


    /// Establish the number of bins
    int histSize = 256;
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    Mat b_hist, g_hist, r_hist;
    /// Compute the histograms:


    //Grande gambiarra pra fazer a função funcionar, assim somente o histograma em gray scale funciona
    // forming an array of matrices is a quite efficient operation,
    // because the matrix data is not copied, only the headers
    Mat out[] = {imagem, imagem, imagem};

    int from_to[] = { 0,0, 1,0, 2,0 };
    mixChannels( &imagem, 1, out, 2, from_to, 3 );
    cvtColor(out[0],out[0],CV_BGR2GRAY);
    calcHist( &out[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &out[0], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &out[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    //int from_to03[] = { 0,2, 1,2, 2,2 };
    //mixChannels( &imagem, 1, out, 2, from_to03, 3 );
    //cvtColor(out[2],out[2],CV_BGR2GRAY);



    //esta parte é pra gerar o histograma da imagem colorida, porém o split não funciona em modo debug

   /* /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( imagem, bgr_planes );
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );*/




    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h + 55, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
     }


    //codigo para criar a barra de scala de cinza
    for (int i = histImage.rows -45 ;i < histImage.rows; i++){
        for (int j = 0; j < histImage.cols*3; j++){
            histImage.at<uchar>(i, j) = j/((hist_w*3)/256);
        }
    }

    /*Mat histImage;

    cvtColor(imagem,imagem,CV_BGR2GRAY);
    for (int i = 0;i < imagem.rows; i++){
        for (int j = 0; j < imagem.cols*3; j++){
            histImage.at<uchar>(i, j) = j/((hist_w*3)/256);
        }
    }

    return(histImage);*/
    return (histImage);
}

/*
*  Transforma uma imagem aberta no openCV para uma imagem do tipo QImage. Isto e necessario para trabalhar com a imagem no QT creator
*
*
*
*/
QImage matToQImage(Mat const& src) {
    cv::Mat temp; // make the same cv::Mat
    cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

Mat qImageToMat(const QImage& qimage) {
  cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
  cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
  int from_to[] = { 0,0,  1,1,  2,2 };
  cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
  return mat2;
}

/**
 * @brief encontrarParticulas Funcao que retorna uma lista com os pontos (QPoint) dos locais da imagem passada onde foram encontradas partes parecidas com o template passado, respeitando o limiar da correlacao
 * @param imgOriginal
 * @param templ
 * @param limiar
 */

QList<QPoint> encontrarParticulas(QImage imgOriginal, QImage Qtempl, double limiar){
    Mat result,templ = qImageToMat(Qtempl), matImg = qImageToMat(imgOriginal);
    QPoint posicao;
    QList<QPoint> listaPosicoes;

    matchTemplate(matImg,templ,result,CV_TM_CCORR_NORMED);   

    normalize( result, result, 0, 1, NORM_MINMAX, -1);


    //Aqui e feita uma normalizacao do limiar, ele e dividido por 200 pq este e o numero maximo do horizontalSlider
    limiar = double(limiar/100);
    threshold(result, result, limiar, 1., CV_THRESH_TOZERO);


    while (true)
       {
           double minval, maxval, threshold = 0.8;
           cv::Point minloc, maxloc;
           cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

           if (maxval >= threshold){
               //pontos onde a correlacao encontrou uma parte parecida com o template
               posicao.setX(maxloc.x+(templ.cols/2));
               posicao.setY(maxloc.y+(templ.cols/2));
               listaPosicoes.append(posicao);
               cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));


           }
           else
               break;
       }

    return listaPosicoes;
}


quantoTp getDadosProjeto(QString caminho) {
    quantoTp quantoProjetc;
    QImage imagem;
    QString dadosAnalise;

    QFile infile(caminho);
    infile.open(QIODevice::ReadOnly);
    QByteArray imagemByteArray = infile.readAll();
    imagemByteArray = qUncompress(imagemByteArray);

    QList<QByteArray> listaByteArray = imagemByteArray.split('qto~');

    dadosAnalise = listaByteArray.last();
    imagem.loadFromData(imagemByteArray);
    infile.close();

    quantoProjetc.dadosAnalise = dadosAnalise;
    quantoProjetc.imagem = imagem;

    return quantoProjetc;

}

















