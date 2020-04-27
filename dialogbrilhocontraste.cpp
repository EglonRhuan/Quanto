#include "dialogbrilhocontraste.h"
#include "ui_dialogbrilhocontraste.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "QFileDialog"
#include "QString"
#include <QLabel>
#include "imageoperations.h"


using namespace cv;


DialogBrilhoContraste::DialogBrilhoContraste(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogBrilhoContraste)

{
    ui->setupUi(this);
}

DialogBrilhoContraste::~DialogBrilhoContraste()
{
    delete ui;
}

void DialogBrilhoContraste::on_buttonBox_accepted()
{
    if (imagemAux.data)
        imagem = imagemAux;
}


void DialogBrilhoContraste::on_horizontalSliderBrilho_valueChanged(int value)
{
    imagemAux = Mat::zeros( imagem.size(), imagem.type() );


    int brilho;  /**< Simple brightness control*/
    double contraste;  /**< Simple contrast control*/

        brilho = value;
        contraste = contraste = ((2.0*ui->horizontalSliderContraste->value())/100.0)+1.0; // operacao matematica feita para ajustar o valor do contraste, haja vista que deve estar entre 1.0 e 3.0

    for( int y = 0; y < imagem.rows; y++ ){
        for( int x = 0; x < imagem.cols; x++ ) {
            for( int c = 0; c < 3; c++ ) {
               imagemAux.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(contraste * ( imagem.at<Vec3b>(y,x)[c] ) + brilho);

            }
        }
     }

    ui->labelValorBrilho->setText(QString::number(value));
    ui->labelBrilhoContraste->setPixmap(QPixmap::fromImage(matToQImage(imagemAux)));
}

void DialogBrilhoContraste::on_horizontalSliderContraste_valueChanged(int value)
{
    imagemAux = Mat::zeros( imagem.size(), imagem.type() );


    int brilho;  /**< Simple brightness control*/
    double contraste;  /**< Simple contrast control*/

        brilho = ui->horizontalSliderBrilho->value();
        contraste = ((2.0*value)/100.0)+1.0; // operacao matematica feita para ajustar o valor do contraste, haja vista que deve estar entre 1.0 e 3.0


    for( int y = 0; y < imagem.rows; y++ ){
        for( int x = 0; x < imagem.cols; x++ ) {
            for( int c = 0; c < 3; c++ ) {
               imagemAux.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(contraste * ( imagem.at<Vec3b>(y,x)[c] ) + brilho);

            }
        }
     }

    ui->labelValorContraste->setText(QString::number(value));
    ui->labelBrilhoContraste->setPixmap(QPixmap::fromImage(matToQImage(imagemAux)));


}
