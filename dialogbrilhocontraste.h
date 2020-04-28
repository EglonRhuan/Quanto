#ifndef DIALOGBRILHOCONTRASTE_H
#define DIALOGBRILHOCONTRASTE_H

#include <QDialog>
#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Ui {
class DialogBrilhoContraste;
}

class DialogBrilhoContraste : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBrilhoContraste(QWidget *parent = 0);
    ~DialogBrilhoContraste();
    cv::Mat imagem, imagemAux; // imagem que sera passada para a classe trabalhar


public slots:

    void on_buttonBox_accepted();

    void on_horizontalSliderBrilho_valueChanged(int value);

    void on_horizontalSliderContraste_valueChanged(int value);

private:
    Ui::DialogBrilhoContraste *ui;

};

#endif // DIALOGBRILHOCONTRASTE_H
