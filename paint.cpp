#include "paint.h"
#include <QMainWindow>
#include "mainwindow.h"
//#include <QtWidgets>
#include <QPainter>
#include <QString>
#include <cmath>
#include "QMessageBox"

paint::paint(QString unMedida, double escala, int corPaint, bool checkBoxMostrarMedida)
{
    mostrarMedida = checkBoxMostrarMedida;
    unMedidaEscala = unMedida;
    escalaPorDezPixels = escala;
    cor = corPaint;


    if (unMedida==""){
        haEscala = false;
         unMedidaEscala = "px";
    }else{
        haEscala = true;
    }

}

QImage paint::mouseMove(const QString acaoSolicitada, const QPoint newPos){

    imgPainter = imgOriginal;
    QImage imagem = imgOriginal;
    QRect rect(posicaoDoClick,newPos);
    double tamanho;



    QRect textoTamanhoForma(newPos.x(),newPos.y()-20,300,300);

    painter = new QPainter(&imagem);

    QFont fonte = painter->font();
    fonte.setPointSize(15);
    painter->setFont(fonte);
    painter->setPen(listaCor[cor]);




    if (acaoSolicitada == "pushButtonLine"){
        painter->drawLine(posicaoDoClick,newPos);
        if(haEscala && mostrarMedida){
            tamanho = calcTamanho("pushButtonLine",newPos);
            painter->drawText(textoTamanhoForma, QString::number(tamanho) + unMedidaEscala);
        }


    }else if(acaoSolicitada == "pushButtonRect"){
        painter->drawRect(rect);
        if (haEscala && mostrarMedida){
            tamanho = calcTamanho("pushButtonRect",newPos);
            painter->drawText(textoTamanhoForma, QString::number(tamanho) + unMedidaEscala + "2");
        }

    }else if(acaoSolicitada == "pushButtonSelection"){


    }else if(acaoSolicitada == "pushButtonElispse"){
        painter->drawEllipse(rect);
        if (haEscala && mostrarMedida){
            tamanho = calcTamanho("pushButtonElispse",newPos);
            painter->drawText(textoTamanhoForma, QString::number(tamanho) + unMedidaEscala );
        }

    }else if(acaoSolicitada == "pushButtonArc"){
        painter->drawArc(rect,2,2);

    }else if(acaoSolicitada == "pushButtonPolygon"){
        //painter->drawPolygon();

    }else if(acaoSolicitada == "pushButtonText"){
        QString texto;
        //bool ok;
        //texto = QInputDialog::getText(MainWindow, "Digite o texto", "Digite o texto",  QLineEdit::Normal, QDir::home().dirName(), &ok);

        //bool ok;
           // QString text = QInputDialog::getText(MainWindow, "QInputDialog::getText()", "User name:", QLineEdit::Normal, QDir::home().dirName(), &ok);
           // int kValue = QInputDialog::getInt(MainWindow, "Matriz do filtro Media", "Digite o tamanho da matriz para o filtro Média:", 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario o KValue (tamanho da matriz) do filtro


    }else if(acaoSolicitada == "pushButtonPen"){
       // painter->drawPoint(newPos);
       // imgOriginal = imagem;

    }else if (acaoSolicitada == "selecionarEscala"){
        painter->drawLine(posicaoDoClick.x(),posicaoDoClick.y(),newPos.x(),posicaoDoClick.y());
    }

    imgPainter = imagem;
    return imgPainter;
}


QImage paint::mouseRelease(){

    imgOriginal = imgPainter;

    return imgPainter;

}

QImage paint::plotarEscala(double escalaPorDezPixels, QString unMedida){

    QImage imagem = imgOriginal;
    QPoint posicaoInicial(imagem.width() - 200, imagem.height()-100);
    QPoint posicaoFinal(imagem.width(), imagem.height());

    painter = new QPainter(&imagem);

    painter->setPen("Green");
    QRect rect(posicaoInicial,posicaoFinal);
    painter->fillRect(rect,"Green");
    painter->drawRect(rect);

    painter->setPen("yellow");
    painter->drawLine(posicaoInicial.x()+50,posicaoInicial.y()+50,posicaoInicial.x()+150,posicaoInicial.y()+50);

    painter->drawText(posicaoInicial.x()+65,posicaoInicial.y()+30,QString::number(escalaPorDezPixels)+" "+unMedida);    


    return imagem;

}

double paint::calcTamanho(QString acaoSolicitada, QPoint newPos){

    //variáveis para guardar a base e a altura do retângulo selecionado para fazer a forma
    double base, altura, tamanhoEmPx, tamanhoEmUM;
    base = std::abs(posicaoDoClick.y() - newPos.y());
    altura = std::abs(posicaoDoClick.x() - newPos.x());

    //passando base e altura para a unidade de medida da escala
    base = (escalaPorDezPixels * base)/10;
    altura = (escalaPorDezPixels * altura)/10;


    if (acaoSolicitada == "pushButtonLine"){
        tamanhoEmUM = sqrt((base*base) + (altura*altura));
    }else if(acaoSolicitada == "pushButtonRect"){
        tamanhoEmUM = base * altura;

    }else if(acaoSolicitada == "pushButtonElispse"){
        double raio1 = base/2;
        double raio2 = altura/2;
        tamanhoEmUM = (raio1 * raio2) * 3.14159;

    }

    //tamanhoEmUM = (escalaPorDezPixels * tamanhoEmPx)/10;


        return tamanhoEmUM;




}









