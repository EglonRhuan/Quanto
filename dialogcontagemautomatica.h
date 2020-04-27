#ifndef DIALOGCONTAGEMAUTOMATICA_H
#define DIALOGCONTAGEMAUTOMATICA_H

#include <QDialog>

namespace Ui {
class DialogContagemAutomatica;
}

class DialogContagemAutomatica : public QDialog
{
    Q_OBJECT

public:
    QImage imgOriginal, templEscolhido;
    QPoint posicaoDoClick;
    double limiarEscolhido;

    explicit DialogContagemAutomatica(QWidget *parent, QImage img);
    ~DialogContagemAutomatica();

private:
    Ui::DialogContagemAutomatica *ui;

protected:
    bool eventFilter(QObject* obj, QEvent* e);
    void marcarParticulas(double limiar);
private slots:
    void on_pushButtonElecionarTemplate_clicked();
    void on_horizontalSliderContagemAutomatica_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonZoomMais_clicked();
    void on_pushButtonZoomMenos_clicked();    
};

#endif // DIALOGCONTAGEMAUTOMATICA_H
