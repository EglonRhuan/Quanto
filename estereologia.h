#ifndef ESTEREOLOGIA_H
#define ESTEREOLOGIA_H

#include <QPainter>
#include "opencv/cv.h"


class estereologia
{

public:

public slots:
    estereologia();
    QImage fracaoDePontos(QImage imagem, QString *strResultado);
    QImage areaSuperficial(QImage imagem, double escalaPorDezPixels, QString *strResultado);
    QImage inserirlinhasVerticais(QImage imagem, int quantLinhas);
    QImage inserirGradeLinhas(QImage imagem, int quantLinhas);
    QImage fracaoLinear(QImage imagem);
    QImage comprimentoPorUnidadeDevolume(QImage imagem, double escalaPorDezPixels);
    void contornarObjeto(cv::Mat &matImg, int i, int j);
    bool temVizinhoPreto(cv::Mat &matImg, int i, int j);
    QImage inserirlinhasHorizontais(QImage imagem, int quantLinhas);
    QImage inserirGradeDePontos(QImage imagem, int quantLinhas);

    void mostrarResultado(QStringList lista);
};

#endif // ESTEREOLOGIA_H
