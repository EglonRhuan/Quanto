#ifndef DIALOGOPLOGICASARITMETICAS_H
#define DIALOGOPLOGICASARITMETICAS_H

#include <QDialog>
#include "QImage"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Ui {
class DialogOpLogicasAritmeticas;
}

class DialogOpLogicasAritmeticas : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpLogicasAritmeticas(QWidget *parent = 0);
    ~DialogOpLogicasAritmeticas();

    void inicializar();
    int tipoOperacao; //indica o tipo de operacao a ser executada: 1-somar; 2-subtrair; 3 E; 4-Não; 5-OU; 6-XOR.
    cv::Mat imgAux, imgResultado;
    QImage imgLabel;
    QImage imgOld1;
    QImage imgOld2;
    QImage imgOld3;
    QImage imgOld4;
    QImage imgOld5;
    QImage imgOld6;


    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void executarOperacao();
    bool eventFilter(QObject* obj, QEvent* e);

public slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogOpLogicasAritmeticas *ui;

};

#endif // DIALOGOPLOGICASARITMETICAS_H
