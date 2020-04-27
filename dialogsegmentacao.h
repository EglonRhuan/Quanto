#ifndef DIALOGSEGMENTACAO_H
#define DIALOGSEGMENTACAO_H


#include <QDialog>
#include <QMainWindow>
#include "opencv/cv.h"


namespace Ui {
class DialogSegmentacao;
}

class DialogSegmentacao : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSegmentacao(QWidget *parent = 0);
    ~DialogSegmentacao();
    cv::Mat imagem,imagemAux;
    int tipoOperacao; //indica qual operacao de segmentacao sera realizada: 0-Binaria; 1-Negativo; 3-Limiar.

    void inicializarDialog();

private slots:

    void on_horizontalSliderSegmentacao_valueChanged(int value);

    void on_buttonBox_accepted();

    void limiar(int value1, int value2);


    void on_horizontalSlider2_valueChanged(int value);

private:
    Ui::DialogSegmentacao *ui;
};

#endif // DIALOGSEGMENTACAO_H
