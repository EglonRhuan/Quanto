#include "dialogoplogicasaritmeticas.h"
#include "ui_dialogoplogicasaritmeticas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QMessageBox>
#include "QMessageBox"
#include <QtWidgets>
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"

#include "imageoperations.h"



using namespace cv;


DialogOpLogicasAritmeticas::DialogOpLogicasAritmeticas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpLogicasAritmeticas)
{
    ui->setupUi(this);

    setAcceptDrops(true);
    ui->labelIm_7->installEventFilter(this);
    ui->labelIm_8->installEventFilter(this);

}

DialogOpLogicasAritmeticas::~DialogOpLogicasAritmeticas()
{
    delete ui;
}

bool DialogOpLogicasAritmeticas::eventFilter(QObject* obj, QEvent* e){
    QString nomeArquivo;
    Mat imagem;

    if(e->type() == QEvent::MouseButtonDblClick){
        nomeArquivo = QFileDialog::getOpenFileName(this, tr("Abrir arquivo..."), QString(), tr("Images (*.png *.xpm *.jpg *.bmp *pgm *.tiff)"));
        imagem = imread(nomeArquivo.toStdString().c_str() );
        if( !imagem.data )
            return false;
        QImage tmp = matToQImage(imagem);

        if(obj == ui->labelIm_7){
            ui->labelIm_7->setPixmap(QPixmap::fromImage(tmp));
        }else if(obj == ui->labelIm_8){
            ui->labelIm_8->setPixmap(QPixmap::fromImage(tmp));
        }
    }
    return false;
}

void DialogOpLogicasAritmeticas::inicializar(){
    ui->labelIm_1->setPixmap(QPixmap::fromImage(imgLabel));
    ui->labelIm_2->setPixmap(QPixmap::fromImage(imgOld1));
    ui->labelIm_3->setPixmap(QPixmap::fromImage(imgOld2));
    ui->labelIm_4->setPixmap(QPixmap::fromImage(imgOld3));
    ui->labelIm_5->setPixmap(QPixmap::fromImage(imgOld4));
    ui->labelIm_6->setPixmap(QPixmap::fromImage(imgOld5));


    switch (tipoOperacao) {
    case 1:
        ui->labelOperador->setText("<html><head/><body><p align='center'>+</p></body></html>");
        break;
    case 2:
        ui->labelOperador->setText("<html><head/><body><p align='center'>-</p></body></html>");
        break;
    case 3:
        ui->labelOperador->setText("<html><head/><body><p align='center'>AND</p></body></html>");
        break;
    case 4:
        ui->labelOperador->setText("NOT");
        ui->labelImSelecionada_2->setVisible(false);
        break;
    case 5:
        ui->labelOperador->setText("<html><head/><body><p align='center'>OR</p></body></html>");
        break;
    case 6:
        ui->labelOperador->setText("<html><head/><body><p align='center'>XOR</p></body></html>");
        break;
    default:
        break;
    }
}



void DialogOpLogicasAritmeticas::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DialogOpLogicasAritmeticas::dragMoveEvent(QDragMoveEvent *event)
{

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DialogOpLogicasAritmeticas::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;


        if (event->pos().rx() > ui->labelImSelecionada_1->pos().rx() && event->pos().rx() < ui->labelImSelecionada_1->pos().rx() + 180 && event->pos().ry() > ui->labelImSelecionada_1->pos().ry() && event->pos().ry() < ui->labelImSelecionada_1->pos().ry() + 150){ // este if foi feito para testar se a imagem foi liberada na mesma posicao do labelImSelecionada_1
            ui->labelImSelecionada_1->setPixmap(pixmap);
        }

        if (event->pos().rx() > ui->labelImSelecionada_2->pos().rx() && event->pos().rx() < ui->labelImSelecionada_2->pos().rx() + 180 && event->pos().ry() > ui->labelImSelecionada_2->pos().ry() && event->pos().ry() < ui->labelImSelecionada_2->pos().ry() + 150){ // este if foi feito para testar se a imagem foi liberada na mesma posicao do labelImSelecionada_2
            ui->labelImSelecionada_2->setPixmap(pixmap);
        }

    } else {
        event->ignore();
    }


    if(tipoOperacao == 4){
        if(ui->labelImSelecionada_1->pixmap()){
            executarOperacao(); //rotina necessaria, pois a operacao de negacao usa somente uma imagem
            return;
        }
    }

    if(ui->labelImSelecionada_1->pixmap() && ui->labelImSelecionada_2->pixmap()){
        if (ui->labelImSelecionada_1->pixmap()->size() != ui->labelImSelecionada_2->pixmap()->size()){
            ui->labelResultado->setText("<html><head/><body><p align='center'><span style='font-weight:600;'>Para realizar a operação </span></p><p align='center'><span style='font-weight:600;'>as imagens devem ser do</span></p><p align='center'><span style='font-weight:600;'>mesmo tamanho.</span></p></body></html>");
            ui->buttonBox->setDisabled(true);
        }else{
            ui->buttonBox->setDisabled(false);
            this->executarOperacao();

        }

    }


}

//! [1]
void DialogOpLogicasAritmeticas::mousePressEvent(QMouseEvent *event)
{
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if (!child)
            return;

        if(!child->pixmap())
            return;

        if(child->objectName()=="buttonBox" || child->objectName()=="line") //rotina necessaria para corrigir um bug que acontecia quando se clicava nestes dois objetos.
            return;


        QPixmap pixmap = *child->pixmap();


       QPixmap pixmapMenor = pixmap.scaled(177,152);

       QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap << QPoint(event->pos() - child->pos());
    ///! [1]

    //! [2]
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", itemData);
    //! [2]

    //! [3]
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmapMenor);
        drag->setHotSpot(event->pos() - child->pos());

    //! [3]

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

        child->setPixmap(tempPixmap);

        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            child->close();
        } else {
            child->show();
            child->setPixmap(pixmap);
        }

}


void DialogOpLogicasAritmeticas::executarOperacao(){

    Mat imgSelecionada1, imgSelecionada2;

    if(ui->labelImSelecionada_1->pixmap())
        imgSelecionada1 = qImageToMat(ui->labelImSelecionada_1->pixmap()->toImage());
    if (ui->labelImSelecionada_2->pixmap())
        imgSelecionada2 = qImageToMat(ui->labelImSelecionada_2->pixmap()->toImage());

    if(!ui->labelImSelecionada_1->pixmap() && !ui->labelImSelecionada_2->pixmap())
        return;

    switch (tipoOperacao) {
    case 1:
        add(imgSelecionada1,imgSelecionada2,imgAux);
        break;
    case 2:
        subtract(imgSelecionada1,imgSelecionada2,imgAux);
        break;
    case 3:
        bitwise_and(imgSelecionada1,imgSelecionada2,imgAux);
        break;
    case 4:
        bitwise_not(imgSelecionada1,imgAux);
        break;
    case 5:
        bitwise_or(imgSelecionada1,imgSelecionada2,imgAux);
        break;
    case 6:
        bitwise_xor(imgSelecionada1,imgSelecionada2,imgAux);
        break;
    default:
        break;
    }

    ui->labelResultado->setPixmap(QPixmap::fromImage(matToQImage(imgAux)));
}


void DialogOpLogicasAritmeticas::on_buttonBox_accepted()
{
    imgResultado = imgAux;
}
