#include "estereologia.h"
//#include <QMessageBox>
#include "QMessageBox"
//#include "opencv/cv.h"
#include <QPainter>
#include "imageoperations.h"
#include "QStandardItemModel"
//#include "QTableView"
#include "QListView"
#include <QStringListModel>

using namespace cv;

estereologia::estereologia()
{

}

QImage estereologia::fracaoDePontos(QImage imagem, QString *strResultado){

    QPainter painter(&imagem);
    int x, y;
    double pontosFase1=0, pontosFase2=0, fracaoVolumetrica;
    Mat matImg;

    matImg = qImageToMat(imagem);
    painter.setPen("RED");

    int wPart = imagem.width()/10;
    int hPart = imagem.height()/10;

    for (int i=0;i<10;i++){
        for (int j=0; j<10;j++){
            x = i*wPart + (wPart/2);
            y = j*hPart + (hPart/2);

            if(matImg.at<Vec3b>(y,x)[0] > 0){
                pontosFase1++;
            }else{
                pontosFase2++;
            }

            QRect ponto(x, y , 5, 5);
            painter.fillRect(ponto,"RED");
            painter.drawRect(ponto);
        }
    }


    fracaoVolumetrica = pontosFase2/(pontosFase2 + pontosFase1);

    *strResultado = "Fracao volumetrica: " + QString::number(fracaoVolumetrica) + "\n pontos na fase 1(branco): " + QString::number(pontosFase1) + " pontos na fase 2(preto): " + QString::number(pontosFase2);
    return imagem;
}

QImage estereologia::areaSuperficial(QImage imagem, double escalaPorDezPixels, QString *strResultado){
    QPainter painter(&imagem);
    Mat matImg;
    int linha, cor=0, quantLinhas = 10;

    QVector<int> vetorPontos(quantLinhas);
    QString relatorio;

    matImg = qImageToMat(imagem);
    threshold(matImg, matImg, 127, 255, 0);

    int hPart = imagem.height()/quantLinhas;


    for (int i=0;i<quantLinhas;i++){
        linha = hPart*i + (hPart/2);
        for (int w=0;w<matImg.cols;w++){
            if (matImg.at<Vec3b>(linha,w)[0]==cor){
                vetorPontos[i]++;
                painter.setPen("RED");
                QRect rect(w - 3,linha - 3, 6, 6);
                painter.fillRect(rect, "RED");
                painter.drawEllipse(rect);
                if (cor==0){
                    cor=255;
                }else{
                    cor=0;
                }
            }
        }
        painter.setPen("RED");
        painter.drawLine(1, linha ,imagem.width(),linha);
    }

    //valores necessários para o cálculo de Sv
    double somatorioPontos = 0;
    for (int i=0;i<quantLinhas;i++){
        somatorioPontos+=vetorPontos[i];
    }
    double comprimentoDasLinhasEmPixels = imagem.width() * quantLinhas;
    double comprimentoDasLinhasEmUm = (comprimentoDasLinhasEmPixels/10) * escalaPorDezPixels;
    double sv = (somatorioPontos/comprimentoDasLinhasEmUm) * 2;


    //cria a lista com os itens para mostrar na tela
    *strResultado = "";
    for (int i=0;i<quantLinhas;i++){
        *strResultado += "linha " + QString::number(i) + ":" + QString::number(vetorPontos[i]) + "\n";
    }
    *strResultado += "total de pontos:" + QString::number(somatorioPontos) + "\n";
    *strResultado += "Sv = " + QString::number(sv);
    //mostrarResultado(lista);

    return imagem;
}

bool estereologia::temVizinhoPreto(Mat &matImg, int i, int j){

    //if (i>1100 || j > 560){
    //    QMessageBox::information(NULL,"as","asdasd");
    // }

    if(matImg.at<Vec3b>(i-1,j)[0] == 0 || matImg.at<Vec3b>(i+1,j)[0] == 0 || matImg.at<Vec3b>(i,j-1)[0] == 0 || matImg.at<Vec3b>(i,j+1)[0] == 0 || matImg.at<Vec3b>(i-1,j-1)[0] == 0 || matImg.at<Vec3b>(i+1,j-1)[0] == 0 || matImg.at<Vec3b>(i-1,j+1)[0] == 0 || matImg.at<Vec3b>(i+1,j+1)[0] == 0){

    //if(matImg.at<Vec3b>(i-1,j)[0] == 255 || matImg.at<Vec3b>(i+1,j)[0] == 255 || matImg.at<Vec3b>(i,j-1)[0] == 255 || matImg.at<Vec3b>(i,j+1)[0] == 255 ){
        return true;
    }else{
        return false;
    }


}

void estereologia::contornarObjeto(Mat &matImg, int i, int j){

    //pinta o contorno do pixel
    int linha,coluna;

    matImg.at<Vec3b>(i,j)[0] = 3;
    matImg.at<Vec3b>(i,j)[1] = 0;
    matImg.at<Vec3b>(i,j)[2] = 255;

    if(i==matImg.rows || j==matImg.cols || i==1 || j==1)
        return;


    //verifica os vizinhos e chama a recursividade
    linha = i-1;
    coluna = j;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
        //imshow("asd",matImg);
        //        QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }
    linha = i+1;
    coluna = j;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
       // imshow("asd",matImg);
       // QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i;
    coluna = j+1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
       // imshow("asd",matImg);
       // QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i-1;
    coluna = j+1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
      //  imshow("asd",matImg);
      //  QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i+1;
    coluna = j+1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
      //  imshow("asd",matImg);
      //  QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i;
    coluna = j-1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
      //  imshow("asd",matImg);
      //  QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i-1;
    coluna = j-1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
      //  imshow("asd",matImg);
      //  QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

    linha = i+1;
    coluna = j-1;
    if(matImg.at<Vec3b>(linha,coluna)[0] == 0 ){
      //  imshow("asd",matImg);
      //  QMessageBox::information(NULL, "asd", "8 - " + QString::number(linha) + " - " + QString::number(coluna));
        contornarObjeto(matImg, linha,coluna);
    }

}










QImage estereologia::comprimentoPorUnidadeDevolume(QImage imagem, double escalaPorDezPixels){

    Mat matImg;
    int contador = 0;

    matImg = qImageToMat(imagem);

    threshold(matImg, matImg, 127, 255, 0);

    for (int i=2;i<matImg.rows-2;i++){
        for (int j=2;j<matImg.cols-2;j++){
            if (matImg.at<Vec3b>(i,j)[0]==0){
                if(matImg.at<Vec3b>(i-1,j)[0] == 255 || matImg.at<Vec3b>(i+1,j)[0] == 255 || matImg.at<Vec3b>(i,j-1)[0] == 255 || matImg.at<Vec3b>(i,j+1)[0] == 255){
                    contornarObjeto(matImg,i,j);
                    contador+=1;

                }
            }
        }
    }

    double area = ((matImg.cols * matImg.rows)/10) * escalaPorDezPixels;

    double lv = (contador/area) * 2;

    QMessageBox::information(NULL, "Comprimento por unidade de volume", "Foram encontrados " + QString::number(contador) + " pontos e Lv = " + QString::number(lv));

    //imagem = matToQImage(matImg);
    //imshow("asd",matImg);


    return imagem;

}



QImage estereologia::inserirlinhasVerticais(QImage imagem, int quantLinhas){
    QPainter painter(&imagem);

    painter.setPen("RED");

    int wPart = imagem.width()/quantLinhas;

    for (int i=0;i<quantLinhas;i++){
        painter.drawLine(wPart*i + (wPart/2),1 ,wPart*i + (wPart/2), imagem.height());
    }

    return imagem;
}

QImage estereologia::inserirlinhasHorizontais(QImage imagem, int quantLinhas){
    QPainter painter(&imagem);

    painter.setPen("RED");

    int hPart = imagem.height()/quantLinhas;

    for (int i=0;i<quantLinhas;i++){
        painter.drawLine(1, hPart*i + (hPart/2), imagem.width(), hPart*i + (hPart/2));
    }

    return imagem;
}


QImage estereologia::inserirGradeLinhas(QImage imagem, int quantLinhas){

    imagem = inserirlinhasHorizontais(imagem, quantLinhas);
    imagem = inserirlinhasVerticais(imagem, quantLinhas);

    return imagem;
}

QImage estereologia::inserirGradeDePontos(QImage imagem, int quantLinhas){
    QPainter painter(&imagem);
    int x, y;
    Mat matImg;

    matImg = qImageToMat(imagem);
    painter.setPen("RED");

    int wPart = imagem.width()/quantLinhas;
    int hPart = imagem.height()/quantLinhas;

    for (int i=0;i<quantLinhas;i++){
        for (int j=0; j<quantLinhas;j++){
            x = i*wPart + (wPart/2);
            y = j*hPart + (hPart/2);

            QRect ponto(x, y , 5, 5);
            painter.fillRect(ponto,"RED");
            painter.drawRect(ponto);
        }
    }

    return imagem;
}

QImage estereologia::fracaoLinear(QImage imagem){

    /*QPainter painter(&imagem);
    Mat matImg;
    int linha, cor=0, quantLinhas = 10;
    int contagemPixels = 0;
    QString relatorio;
    QList<double> listaTamanhoParticulas;
    double ResultFracaoLinear, La=0;

    matImg = qImageToMat(imagem);
    threshold(matImg, matImg, 127, 255, 0);

    int hPart = imagem.height()/quantLinhas;
    painter.setPen("RED");

    for (int i=0;i<quantLinhas;i++){
        linha = hPart*i + (hPart/2);
        //rotina para plotar os tracos e medidas nas particulas
        for (int w=0;w<matImg.cols;w++){
            if (matImg.at<Vec3b>(linha,w)[0]==cor){
                painter.drawLine(w, linha-5,w,linha + 5);
                if (cor==0){
                    cor=255;
                }else{
                    cor=0;
                    listaTamanhoParticulas.append(contagemPixels/pixelsporCm);
                    La+=contagemPixels/pixelsporCm;
                    contagemPixels = 0;
                    //QRect a(w,linha,100,100);
                    //painter.drawText(a,"10");
                }
            }
            //rotina para fazer de fato a medida
            if (matImg.at<Vec3b>(linha,w)[0]<100){
                contagemPixels++;
            }
        }
        painter.drawLine(1, linha ,imagem.width(),linha);
    }

    //calculo da fracao linear
    double compTotaldasRetas = (imagem.width() * quantLinhas)/pixelsporCm;
    ResultFracaoLinear = La/compTotaldasRetas;

    QMessageBox::information(NULL, "Fração Linear", "A Fração Linear é: " + QString::number(ResultFracaoLinear));


    //cria a lista com os itens para mostrar na tela
    QStringList lista;
    for (int i=0;i<listaTamanhoParticulas.size();i++){
        lista << "particula " + QString::number(i) + ":  " + QString::number(listaTamanhoParticulas[i]) + "cm";
    }

    mostrarResultado(lista);*/
    return imagem;

}

void estereologia::mostrarResultado(QStringList lista){
    QWidget *janela = new QWidget();
    QListView *view = new QListView(janela);
    QStringListModel *model = new QStringListModel(view);

    model->setStringList(lista);
    view->setModel(model);
    view->show();
    janela->show();
}












