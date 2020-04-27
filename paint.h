#ifndef PAINT_H
#define PAINT_H

#include <QMainWindow>
//#include <QtWidgets>
#include <QPainter>

class paint
{
public:
    QPainter *painter;
    QPoint posicaoDoClick;
    QImage imgOriginal, imgPainter;
    double escalaPorDezPixels;
    QString unMedidaEscala;
    bool haEscala, mostrarMedida;
    QStringList listaCor = (QStringList() << "BLACK" << "WHITE" << "BLUE" << "YELLOW" << "RED" << "GREEN" << "BLACK");
    int cor;

public slots:
    paint(QString unMedida, double escala, int corPaint, bool checkBoxMostrarMedida);
    QImage mouseMove(const QString acaoSolicitada, const QPoint newPos);
    QImage mouseRelease();
    QImage plotarEscala(double escalaPorDezPixels, QString unMedida);
    double calcTamanho(QString acaoSolicitada, QPoint newPos);
};

#endif // PAINT_H
