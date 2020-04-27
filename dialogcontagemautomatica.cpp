#include "dialogcontagemautomatica.h"
#include "ui_dialogcontagemautomatica.h"
#include <QMessageBox>
#include "QMessageBox"
#include "contarparticulas.h"
#include "imageoperations.h"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
#include "QPainter"
#include "qevent.h"


DialogContagemAutomatica::DialogContagemAutomatica(QWidget *parent, QImage img) :
    QDialog(parent),
    ui(new Ui::DialogContagemAutomatica)
{
    imgOriginal = img;
    ui->setupUi(this);

    ui->labelContagemAutomatica->setPixmap(QPixmap::fromImage(imgOriginal));
    ui->labelContagemAutomatica->installEventFilter(this);

}

DialogContagemAutomatica::~DialogContagemAutomatica()
{
    delete ui;
}

bool DialogContagemAutomatica::eventFilter(QObject *obj, QEvent *e){




     QMouseEvent *k = (QMouseEvent *)e;
    QPoint newPos = k->pos();
    //este calculo eh necessario para ajustar a posicao do ponto, pois o mouseEvent eh do label e o ponto vai na imagem. Na maioria das vezes a imagem tem tamanho diferente do label.
    if (ui->labelContagemAutomatica->pixmap()){
        double x =  double(imgOriginal.width())/double(ui->labelContagemAutomatica->width());
        double y = double(imgOriginal.height())/double(ui->labelContagemAutomatica->height());
        x = newPos.x() * x;
        y = newPos.y() * y;

        newPos.setX(x);
        newPos.setY(y);
    }


    if (obj == ui->labelContagemAutomatica){
        if(e->type() == QEvent::MouseButtonPress){
            posicaoDoClick = newPos;
        }

        if(e->type() == QEvent::MouseMove){
            QImage imgPainter = imgOriginal;
            QPainter painter(&imgPainter);
            QRect rect(posicaoDoClick,newPos);
            painter.drawRect(rect);
            ui->labelContagemAutomatica->setPixmap(QPixmap::fromImage(imgPainter));
        }
        if (e->type() == QEvent::MouseButtonRelease){
            QImage imgTemplate;
            QRect rect(posicaoDoClick,newPos);
            imgTemplate = imgOriginal.copy(rect);
            ui->labelTemplate1->setPixmap(QPixmap::fromImage(imgTemplate));
            ui->labelContagemAutomatica->removeEventFilter(this);
            ui->labelContagemAutomatica->setPixmap(QPixmap::fromImage(imgOriginal));
            marcarParticulas(ui->horizontalSliderContagemAutomatica->value());
            ui->labelCorrelacao->setEnabled(true);
            ui->horizontalSliderContagemAutomatica->setEnabled(true);

        }
    }



    return false;
}

void DialogContagemAutomatica::on_pushButtonElecionarTemplate_clicked()
{
    ui->labelContagemAutomatica->installEventFilter(this);
     ui->labelContagemAutomatica->setPixmap(QPixmap::fromImage(imgOriginal));
}

void DialogContagemAutomatica::on_horizontalSliderContagemAutomatica_valueChanged(int value)
{
    marcarParticulas(value);
}

void DialogContagemAutomatica::marcarParticulas(double limiar){
    QImage imgPontos = imgOriginal, templ = ui->labelTemplate1->pixmap()->toImage();
    int x,y;
    QRect rect;

    QPainter painter(&imgPontos);
    painter.setPen("RED");

    QList<QPoint> listaPosicoes = encontrarParticulas(imgOriginal, templ, limiar);

    //esta desenha um retangulo ao redor dos pontos contidos na lista retornada pela funcao encontrarParticulas
    for (int i = 0; i < listaPosicoes.size(); ++i) {
       x = listaPosicoes.at(i).x();
       y = listaPosicoes.at(i).y();
       rect.setCoords(x-(templ.width()/2),y-(templ.height()/2),x+(templ.width()/2),y+(templ.height()/2));
       painter.drawRect(rect);
    }

    ui->labelContagemAutomatica->setPixmap(QPixmap::fromImage(imgPontos));


}


void DialogContagemAutomatica::on_buttonBox_accepted()
{
    if (ui->labelTemplate1->pixmap()){
        templEscolhido = ui->labelTemplate1->pixmap()->toImage();
        limiarEscolhido = ui->horizontalSliderContagemAutomatica->value();
    }else{
        QImage imgVazia;
        templEscolhido = imgVazia;
    }
}

void DialogContagemAutomatica::on_buttonBox_rejected()
{
    QImage imgVazia;
    templEscolhido = imgVazia;
}

void DialogContagemAutomatica::on_pushButtonZoomMais_clicked()
{
    ui->labelContagemAutomatica->resize(ui->labelContagemAutomatica->size() * 1.1);
    ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->size() * 1.1);

}

void DialogContagemAutomatica::on_pushButtonZoomMenos_clicked()
{
    ui->labelContagemAutomatica->resize(ui->labelContagemAutomatica->size() * 0.9);
    ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->size() * 0.9);
}





























