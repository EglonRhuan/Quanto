#include "dialogsegmentacao.h"
#include "ui_dialogsegmentacao.h"
#include <QDialog>
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "imageoperations.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QMessageBox>
#include "QMessageBox"




using namespace cv;


DialogSegmentacao::DialogSegmentacao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSegmentacao)
{
    ui->setupUi(this);

}


DialogSegmentacao::~DialogSegmentacao()
{
   delete ui;
    QImage qImage;

}

void DialogSegmentacao::inicializarDialog(){

    Mat histograma;
    QImage qHistograma;

    imagemAux = imagem;

    cvtColor(imagemAux,imagemAux,CV_BGR2GRAY);
    cvtColor(imagemAux,imagemAux,CV_GRAY2BGR);

    histograma = obterHistograma(imagemAux);

    qHistograma = matToQImage(histograma);

    ui->labelHistograma->setPixmap(QPixmap::fromImage(qHistograma));

    ui->horizontalSliderSegmentacao->setValue(127);

}

void DialogSegmentacao::on_horizontalSliderSegmentacao_valueChanged(int value)
{
    QImage qImage;
    imagemAux = imagem.clone();

    if (tipoOperacao == 3){

        if (value > ui->horizontalSlider2->value()){
            ui->horizontalSlider2->setValue(value);
        }

        int value2 = ui->horizontalSlider2->value();
        limiar(value, value2);
        return;
    }

    cvtColor(imagemAux,imagemAux,CV_BGR2GRAY);
    threshold(imagemAux, imagemAux, value, 255, tipoOperacao );
    cvtColor(imagemAux,imagemAux,CV_GRAY2BGR);

    qImage = matToQImage(imagemAux);
    ui->labelPrevia->setPixmap(QPixmap::fromImage(qImage));

    ui->labelValorSelecionado->setText(QString::number(value));

}

void DialogSegmentacao::on_horizontalSlider2_valueChanged(int value)
{
    QImage qImage;
    imagemAux = imagem.clone();

    ui->labelValorSelecionado_2->setText( QString::number(ui->horizontalSlider2->value()));

    if (value < ui->horizontalSliderSegmentacao->value()){
        ui->horizontalSliderSegmentacao->setValue(value);
    }



    if (tipoOperacao == 3){
        int value1 = ui->horizontalSliderSegmentacao->value();
        limiar(value1, value);
        return;
    }
}



void DialogSegmentacao::on_buttonBox_accepted()
{
    imagem = imagemAux;
}

void DialogSegmentacao::limiar(int value1, int value2){

    QImage qImage;
    //Mat img = imagemAux;

    //QMessageBox::information(NULL, "Mensagem de erro", QString::number(value2));


    for (int i=0; i< imagemAux.rows; i++){
        for (int j=0; j< imagemAux.cols; j++){
            if (imagemAux.at<Vec3b>(i,j)[0] < value1){
                imagemAux.at<Vec3b>(i,j) = 0;
            }else if(imagemAux.at<Vec3b>(i,j)[0] > value2) {
                imagemAux.at<Vec3b>(i,j) = 255;
            }
        }
    }



    qImage = matToQImage(imagemAux);
    ui->labelPrevia->setPixmap(QPixmap::fromImage(qImage));

    ui->labelValorSelecionado->setText(QString::number(value1));

}





