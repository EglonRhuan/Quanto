#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogbrilhocontraste.h"
#include "dialogoplogicasaritmeticas.h"
#include "dialogsegmentacao.h"

#include "QFileDialog"
#include "QString"
#include <QLabel>
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"\
//#include <QMessageBox>
#include "QMessageBox"
#include "imageoperations.h"
#include <QtWidgets>
#include "estereologia.h"
#include<QPainter>
#include <dirent.h>
#include "qevent.h"


using namespace std;
using namespace cv;


/*
 * Funcao para mostrar a imagem no label principal
 * Esta funcao coloca a imagem com a devida alteracao feita pelo usuario na tela principal e organiza o snapshot
 *
 */

void MainWindow::abrirImagem(QImage img){

    /*if (img.width() > ui->scrollAreaPrincipal->width() || img.height() > ui->scrollAreaPrincipal->height() ){
        ui->label->resize(ui->scrollAreaPrincipal->size());
    }else{
        ui->label->resize(img.size());

    }*/
    ui->label->setPixmap(QPixmap::fromImage(img));    

}



void MainWindow::showImage(Mat img){

    QPixmap imgOld, imgOld1, imgOld2, imgOld3, imgOld4, imgOld5 ;

    imgOld = QPixmap::fromImage(ui->label->pixmap()->toImage());

    QImage tmp = matToQImage(img);
    ui->label->setPixmap(QPixmap::fromImage(tmp));

    if (ui->labelOld1->pixmap()==0){
        ui->labelOld1->setPixmap(imgOld);
        return;
    }

    if (ui->labelOld2->pixmap()== 0){
        ui->labelOld2->setPixmap(imgOld);
        return;
    }else if(ui->labelOld3->pixmap()== 0){
        ui->labelOld3->setPixmap(imgOld);
        return;
    }else if (ui->labelOld4->pixmap()== 0){
        ui->labelOld4->setPixmap(imgOld);
        return;
    }else if (ui->labelOld5->pixmap()== 0){
        ui->labelOld5->setPixmap(imgOld);
        return;
    }


    imgOld2 = QPixmap::fromImage(ui->labelOld2->pixmap()->toImage());
    ui->labelOld1->setPixmap(imgOld2);

    imgOld3 = QPixmap::fromImage(ui->labelOld3->pixmap()->toImage());
    ui->labelOld2->setPixmap(imgOld3);

    imgOld4 = QPixmap::fromImage(ui->labelOld4->pixmap()->toImage());
    ui->labelOld3->setPixmap(imgOld4);

    imgOld5 = QPixmap::fromImage(ui->labelOld5->pixmap()->toImage());
    ui->labelOld4->setPixmap(imgOld5);
    ui->labelOld5->setPixmap(imgOld);
    return;


}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->scrollAreaPrincipal->setWidget(ui->label);
    connect( ui->action_Sair, SIGNAL(triggered()), this, SLOT(close()));
    ui->labelOld1->installEventFilter(this);
    ui->labelOld2->installEventFilter(this);
    ui->labelOld3->installEventFilter(this);
    ui->labelOld4->installEventFilter(this);
    ui->labelOld5->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Abrir_imagem_triggered()
{
    QString nomeArquivo;
    QStringList listaString;

    //rotina necessária para deixar o label redimensionável (Obs.:o zoom tira essa opção pra funcionar)
    ui->scrollArea->setWidgetResizable(true);

    diretorio = "";

    nomeArquivo = QFileDialog::getOpenFileName(this, tr("Abrir arquivo..."), QString(), tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *pgm *.tiff *.tif *.qto)"));

    if (nomeArquivo != ""){

        carregarProjeto(nomeArquivo);


        //a partir daqui as rotinas são para salvar o caminho do arquivo.
        listaString = nomeArquivo.split("\\"); //divide a string em caso de Windows
        barra = "\\";

        if (listaString.length() == 1){
            listaString = nomeArquivo.split("/"); //divide a string em caso de Linux
            barra = "/";
        }

        for (int i = 0; i < listaString.length()-1; i++){
            diretorio += listaString[i] + barra ;
        }
        diretorio = diretorio.replace(diretorio.size()-1,1,""); // rotina para retirar a ultima barra..

        primeiroArquivo = listaString[listaString.length()-1];

        arquivoCorrente = primeiroArquivo;

        //se o arquivo é carregado em uma pasta qualquer, cria-se a pasta do diretorio padrão, se o arquivo for carregado do diretorio do programa ele não cria nova pasta pra não ficarem pastas infinitas
        QStringList diretorios = diretorio.split(barra);
        if(diretorios.last() != nomeDiretorioPadrao ){
            //cria o diretorio para salvar as imagens do projeto
            QDir dir = QDir::root();
            caminhoCompletoDoProjeto = diretorio + barra + nomeDiretorioPadrao;
            dir.mkdir(caminhoCompletoDoProjeto);
        }else{
            caminhoCompletoDoProjeto = diretorio;
        }

    }

    ativarControlesSegmentacao();
}



void MainWindow::on_action_Salvar_triggered()
{    
    //testa se ha imagem no label
    if (!haImagem())
            return;

    QString caminhoSalvar;

    //se o objeto é nulo deve-se salvar somente a imagem
    if (objContarParticulas == NULL){
        caminhoSalvar = QFileDialog::getSaveFileName(this, tr("Salvar projeto..."), QString(), tr("JPEG(*.jpg);; JPEG *.jpeg;; TIFF *.tiff;; TIFF *.tif;; PNG *.png;; Bitmap *.bmp"));
    }else{
        caminhoSalvar = QFileDialog::getSaveFileName(this, tr("Salvar projeto..."), QString(), tr("Quanto projects(*.qto)"));
    }



    this->salvarProjeto(caminhoSalvar);


}

void MainWindow::on_action_Exportar_imagem_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();

    imagem = qImageToMat(qimage);

    QString salvar = QFileDialog::getSaveFileName(this, tr("Exportar imagem..."), QString(), tr("Jpeg Images (.jpg)"));
    salvar = salvar + ".jpg";

    if( salvar == "" )
    { return;}

    imwrite(salvar.toStdString().c_str(), imagem);
}



/**
 * @brief MainWindow::eventFilter  Funcao para capturar os eventos envolvendo os labels.Esta funcao recupera as imagens do snapshot para o label principal
 * @param obj
 * @param e
 * @return
 *
 *
 */
bool MainWindow::eventFilter(QObject* obj, QEvent* e){
 //esta função não funcionou devido a problemas com o qmouseevent. Verificar depois.





    QMouseEvent *k = static_cast<QMouseEvent *>(e);
    QPoint pos = k->pos();


    //este calculo eh necessario para ajustar a posicao do ponto, pois o mouseEvent eh do label e o ponto vai na imagem. Na maioria das vezes a imagem tem tamanho diferente do label.
    if (ui->label->pixmap()){
        double x =  double(ui->label->pixmap()->toImage().width())/double(ui->label->width());
        double y = double(ui->label->pixmap()->toImage().height())/double(ui->label->height());
        x = pos.x() * x;
        y = pos.y() * y;

        pos.setX(x);
        pos.setY(y);
    }


    if(obj == ui->labelOld1){
        if(e->type() == QEvent::MouseButtonPress){
            if(ui->labelOld1->pixmap()!=0){
                ui->label->setPixmap(QPixmap::fromImage(ui->labelOld1->pixmap()->toImage()));
            }
        }
    }

    if(obj == ui->labelOld2){
        if(e->type() == QEvent::MouseButtonPress){
            if(ui->labelOld2->pixmap()!=0){
                ui->label->setPixmap(QPixmap::fromImage(ui->labelOld2->pixmap()->toImage()));
            }
        }
    }

    if(obj == ui->labelOld3){
        if(e->type() == QEvent::MouseButtonPress){
            if(ui->labelOld3->pixmap()!=0){
                ui->label->setPixmap(QPixmap::fromImage(ui->labelOld3->pixmap()->toImage()));
            }
        }
    }

    if(obj == ui->labelOld4){
        if(e->type() == QEvent::MouseButtonPress){
            if(ui->labelOld4->pixmap()!=0){
                ui->label->setPixmap(QPixmap::fromImage(ui->labelOld4->pixmap()->toImage()));
            }
        }
    }

    if(obj == ui->labelOld5){
        if(e->type() == QEvent::MouseButtonPress){
            if(ui->labelOld5->pixmap()!=0){
                ui->label->setPixmap(QPixmap::fromImage(ui->labelOld5->pixmap()->toImage()));
            }
        }
    }

    if(obj == ui->label){
        if(e->type() == QEvent::MouseButtonPress){
            if (acaoSolicitada == "contagemParticulas"){
                if(k->button() == Qt::LeftButton){
                    realizarContagem(pos, "incrementar");
                }else if(k->button() == Qt::RightButton){
                    realizarContagem(pos, "decrementar");
                }
            }else{
                if(k->button() == Qt::LeftButton){
                    paintObj->posicaoDoClick = pos;

                }
            }
        }

        if (acaoSolicitada != "contagemParticulas"){
            if(e->type() == QEvent::MouseMove){

                QImage imagem = paintObj->mouseMove(acaoSolicitada,pos);
                ui->label->setPixmap(QPixmap::fromImage(imagem));
            }

            if(e->type() == QEvent::MouseButtonRelease){
                if (acaoSolicitada == "selecionarEscala"){
                    pegarDadosEscala(paintObj->posicaoDoClick, pos);
                    ui->label->removeEventFilter(this);
                    delete paintObj;
                }else{
                     //ui->label->setPixmap(QPixmap::fromImage(paintObj->imgOriginal));
                    QImage imagem = paintObj->mouseRelease();
                    Mat matImg = qImageToMat(imagem);
                    showImage(matImg);
                }
            }
        }
    }

return false;

}

void MainWindow::on_actionMedia_triggered()
{

    Mat imagem;
    QImage qimage;
    int kValue;
    bool ok;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    kValue = QInputDialog::getInt(this, tr("Matriz do filtro Media"), tr("Digite o tamanho da matriz para o filtro Média:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario o KValue (tamanho da matriz) do filtro


    if(!ok){
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    blur(imagem,imagem,Size(kValue,kValue),Point(-1,-1));

    this->showImage(imagem);

}

void MainWindow::on_actionMediana_triggered()
{
    Mat imagem;
    QImage qimage;
    int kValue;
    bool ok;


    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    kValue = QInputDialog::getInt(this, tr("Matriz do filtro Mediana"), tr("Digite o tamanho da matriz para o filtro Mediana:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario o KValue (tamanho da matriz) do filtro

    if(!ok){
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    medianBlur(imagem,imagem,kValue);
    this->showImage(imagem);

}

void MainWindow::on_actionGaussiana_triggered()
{
    Mat imagem;
    QImage qimage;
    int kValue;
    bool ok;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    kValue = QInputDialog::getInt(this, tr("Matriz do filtro Gaussiano"), tr("Digite o tamanho da matriz para o filtro Gaussiano:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario o KValue (tamanho da matriz) do filtro

    if(!ok){
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    GaussianBlur(imagem,imagem,Size(kValue,kValue),0,0);

    this->showImage(imagem);

}

void MainWindow::on_actionCanny_triggered()
{
    Mat imagem;
    QImage qimage;
    int abertura;
    bool ok;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    abertura = QInputDialog::getInt(this, tr("Abertura do filtro Canny"), tr("Digite a abertura para o filtro Canny:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario a abertura para o filtro Canny

    if(!ok){
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor( imagem, imagem, CV_BGR2GRAY );

    Canny(imagem,imagem,0,0,abertura,false);

    cvtColor( imagem, imagem, CV_GRAY2BGR );

    this->showImage(imagem);


}

void MainWindow::on_actionLaplaciano_triggered()
{
    Mat src, dst;
    QImage qimage;
    int kValue;
    bool ok;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    kValue = QInputDialog::getInt(this, tr("Matriz do filtro Laplaciano"), tr("Digite o tamanho da matriz para o filtro Laplaciano:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usuario o KValue (tamanho da matriz) do filtro Laplaciano

    if(!ok){
        return;
    }



    qimage = ui->label->pixmap()->toImage();
    src = qImageToMat(qimage);

    cvtColor( src, src, CV_RGB2GRAY );
    GaussianBlur(src,src,Size(kValue,kValue),0,0,BORDER_DEFAULT);

    Laplacian(src,dst,CV_16S,3,1,0,BORDER_DEFAULT);

    convertScaleAbs( dst, dst);


    cvtColor( dst, dst, CV_GRAY2BGR );

    this->showImage(dst);
    //cvReleaseImage(&imagem);

}

void MainWindow::on_actionSobel_triggered()
{
    Mat imagem, img_x, img_y;
    QImage qimage;
    bool ok;
    int kValue;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }



    kValue = QInputDialog::getInt(this, tr("Matriz do filtro sobel"), tr("Digite o tamanho da matriz para o filtro Sobel:"), 3, 1, 9, 2, &ok); //imputDialog pra solicitar do usu'ario o KValue (tamanho da matriz) do filtro sobel

    if(!ok){
        return;
    }


    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor( imagem, imagem, CV_RGB2GRAY );
    Sobel(imagem, img_x, CV_16S, 0, 1, kValue, 1, 0, BORDER_DEFAULT );

    Sobel(imagem, img_y, CV_16S, 1, 0, kValue, 1, 0, BORDER_DEFAULT );

    convertScaleAbs( img_x, img_x );
    convertScaleAbs( img_y, img_y );

    addWeighted( img_x, 0.5, img_y, 0.5, 0, imagem);

    cvtColor( imagem, imagem, CV_GRAY2BGR );

    this->showImage(imagem);


}

void MainWindow::on_actionErosao_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    Mat element = getStructuringElement(MORPH_RECT, Size( 3, 3), Point(2,2) );

    erode(imagem,imagem,element);

    this->showImage(imagem);
}

void MainWindow::on_actionDilatacao_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    Mat element = getStructuringElement(MORPH_RECT, Size( 3, 3), Point(2,2) );

    dilate(imagem,imagem,element);

    this->showImage(imagem);
}

void MainWindow::on_actionEqualizar_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor( imagem, imagem, CV_BGR2GRAY );

    equalizeHist( imagem, imagem );


    cvtColor( imagem, imagem, CV_GRAY2BGR );

    this->showImage(imagem);
}

void MainWindow::on_actionLimiar_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    DialogSegmentacao janela(this);
    janela.imagem = imagem;
    janela.tipoOperacao = 3;
    janela.setModal(true);
    janela.move(430,80);

    janela.inicializarDialog();

    janela.exec();

    this->showImage(janela.imagem);

}

void MainWindow::on_actionNegativo_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor(imagem,imagem,COLOR_BGR2GRAY);

    imagem = 255-imagem;
    cvtColor(imagem,imagem,COLOR_GRAY2BGR);

    /*DialogSegmentacao janela(this);
    janela.imagem = imagem;
    janela.tipoOperacao = 1;
    janela.setModal(true);
    janela.move(430,80);

    janela.inicializarDialog();

    janela.exec();*/


    //imshow("as",imagem);
    this->showImage(imagem);
}

void MainWindow::on_actionBinaria_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    DialogSegmentacao janela(this);
    janela.imagem = imagem;
    janela.tipoOperacao = 0;
    janela.setModal(true);
    janela.move(430,80);

    janela.inicializarDialog();

    janela.exec();

    this->showImage(janela.imagem);

}

void MainWindow::on_actionWatershed_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    QMessageBox::information(NULL, "Programa em implementação", "Esta funcionalidade ainda não foi implementada");

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor(imagem,imagem,COLOR_BGR2GRAY);
    threshold(imagem,imagem,100,255,0);


    //watershed(imagem,imagem);

   // imshow("ksbdfjb",imagem);
    //cvtColor(imagem,imagem,CV_GRAY2BGR);

    //this->showImage(imagem);
    //cvReleaseImage(&imagem);

}

/**
 * @brief MainWindow::on_actionVisualizar_triggered
 *Funcao que mostra o histograma da imagem
 *Codigo retirado do tutorial do opencv: http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
 *
 */
void MainWindow::on_actionVisualizar_triggered()
{
    Mat imagem;
    QImage qimage;

    //testa se ha imagem no label
    if (!haImagem())
            return;

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);    

    imagem = obterHistograma(imagem);

      /// Display
      namedWindow("Histograma", CV_WINDOW_NORMAL);
      moveWindow("Histograma",500,100 );
      imshow("Histograma", imagem );

}

void MainWindow::on_actionBrilhoContraste_triggered()
{
    Mat imagem;
    QImage qimage;

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    DialogBrilhoContraste janela(this);
    janela.setModal(true);
    janela.move(430,80);

    janela.imagem = imagem;

    janela.exec();
    //destroyWindow("Prévia");

    this->showImage(janela.imagem);

}

void MainWindow::on_actionTons_de_cinza_triggered()
{
    Mat imagem;
    QImage qimage;

    //testa se ha imagem no label
    if (!haImagem())
            return;

    qimage = ui->label->pixmap()->toImage();
    imagem = qImageToMat(qimage);

    cvtColor( imagem, imagem, CV_RGB2GRAY );
    cvtColor(imagem, imagem, CV_GRAY2BGR);


    this->showImage(imagem);

}

void MainWindow::on_actionSomar_triggered()
{
    ExecutarOpLogicaAritmetica(1);
}

void MainWindow::on_actionSubtrair_triggered()
{
    ExecutarOpLogicaAritmetica(2);
}

void MainWindow::on_actionE_triggered()
{
    ExecutarOpLogicaAritmetica(3);
}

void MainWindow::on_actionN_o_triggered()
{
    ExecutarOpLogicaAritmetica(4);
}

void MainWindow::on_actionOu_triggered()
{
    ExecutarOpLogicaAritmetica(5);
}

void MainWindow::on_actionOu_exclusivo_triggered()
{
    ExecutarOpLogicaAritmetica(6);
}

/**
 * @brief MainWindow::ExecutarOpLogicaAritmetica(1);
 * @param tipoOperacao 1 = somar; 2 = subtrair; 3 = E; 4 = NÃO;5 = OU; 6 = OU exclusivo.
 *
 *  Funcao que cria o objeto, inicia o dialog de operacoes logicas e aritmeticas com imagens e mostra a imagem restultado da operacao
 */
void MainWindow::ExecutarOpLogicaAritmetica(int tipoOperacao){

    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return;
    }

    DialogOpLogicasAritmeticas janela(this);
    janela.setModal(true);
    janela.move(430,80);

    janela.tipoOperacao = tipoOperacao;

    if (ui->label->pixmap()){
        janela.imgLabel = ui->label->pixmap()->toImage();
    }

    if (ui->labelOld1->pixmap()){
        janela.imgOld1 = ui->labelOld1->pixmap()->toImage();
    }

    if (ui->labelOld2->pixmap()){
        janela.imgOld2 = ui->labelOld2->pixmap()->toImage();
    }

    if (ui->labelOld3->pixmap()){
        janela.imgOld3 = ui->labelOld3->pixmap()->toImage();
    }

    if (ui->labelOld4->pixmap()){
        janela.imgOld4 = ui->labelOld4->pixmap()->toImage();
    }

    if (ui->labelOld5->pixmap()){
        janela.imgOld5 = ui->labelOld5->pixmap()->toImage();
    }

    janela.inicializar();
    janela.exec();

    if (janela.imgResultado.data)
        this->showImage(janela.imgResultado);

}

void MainWindow::on_actionZoomMais_triggered()
{
    ui->scrollArea->setWidgetResizable(false);
    ui->label->resize(ui->label->size() * 1.1);
    ui->scrollAreaLabel->resize(ui->scrollAreaLabel->size() * 1.1);


}

void MainWindow::on_actionZoomMenos_triggered()
{
    ui->scrollArea->setWidgetResizable(false);
    ui->label->resize(ui->label->size() * 0.9);
    ui->scrollAreaLabel->resize(ui->scrollAreaLabel->size() * 0.9);

}  

void MainWindow::on_pushButtonAddTipo_clicked()
{

    QObjectList listaRadioButtons = ui->frameContaParticulas->children();

    if (listaRadioButtons.size()>10){
        QMessageBox::information(NULL, "limite excedido", "O limite máximo de tipos é 10 tipos");
        return;
    }


    QString nomeTipo;
    bool ok;
    nomeTipo = QInputDialog::getText(this, tr("Tipo a ser incluído"),
                                             tr("Tipo:"), QLineEdit::Normal,
                                     "Novo tipo", &ok); //imputDialog pra solicitar do usuario o nome do tipo;

    if (!ok)
        return;



    //esta condicao serve para nomear o primeiro radioButton
    if (listaRadioButtons.size()>1){        
        if(listaRadioButtons.at(1)->objectName() == "tipo1"){
            ui->tipo1->setText("1. " + nomeTipo);
            ui->tipo1->setObjectName("1. " + nomeTipo);
            return;
        }
    }

    //rotina para numerar os nomes, assim fica mais fácil identificar o tipo na contagem da imagem
    nomeTipo = QString::number(listaRadioButtons.length()) + ". " + nomeTipo;
    ui->frameContaParticulas->resize(ui->frameContaParticulas->width(), ui->frameContaParticulas->height() + 20);

    QRadioButton *button = new QRadioButton(nomeTipo, ui->frameContaParticulas);
    button->setObjectName(nomeTipo);
    QFont fonte("MS Shell Dlg 2", 7);
    button->setFont(fonte);


    ui->frameContaParticulas->layout()->addWidget(button);

    button->show();

}

void MainWindow::on_pushButtonDeletarTipo_clicked()
{

    QObjectList listaRadioButtons = ui->frameContaParticulas->children();

    if (listaRadioButtons.size() <= 1){
        return;
    }

    listaRadioButtons[listaRadioButtons.size()-1]->deleteLater();

    ui->frameContaParticulas->resize(ui->frameContaParticulas->width(), ui->frameContaParticulas->height() - 20);

}

void MainWindow::on_pushButtonIniciarContagem_clicked()
{    

    //testa se ha imagem no label
    if (!haImagem())
            return;

    //define o label que contem a imagem para capturar o evento de clique de contagem
    ui->label->installEventFilter(this);

    if (objContarParticulas==NULL){
        objContarParticulas = new ContarParticulas(ui->label->pixmap()->toImage());
    }else{
        objContarParticulas->imgOriginal = ui->label->pixmap()->toImage();
        objContarParticulas->plotPontos(ui->label);
    }    


    QString nomesTipos[10];
    QObjectList listaRadioButtons = ui->frameContaParticulas->children();
    int indiceAux = 0;
    for (int i = 0; i < listaRadioButtons.size(); i++){
            if (isRadioButton(listaRadioButtons[i])){                
                QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
                objContarParticulas->nomesTipos[indiceAux] = radioButton->text();
                indiceAux++;

                //a rotina abaixo serve para retirar a contagem anterior no momento de começar uma nova contagem
                radioButton->setText(radioButton->text().section('#',0,0));

            }
    }


    ativarControlesContagem();
    acaoSolicitada = "contagemParticulas";   

}

void MainWindow::on_pushButtonReiniciarContagem_clicked()
{
    abrirImagem(objContarParticulas->imgOriginal);
    objContarParticulas = NULL;
    objContarParticulas = new ContarParticulas(ui->label->pixmap()->toImage());
}





void MainWindow::on_pushButtonPausarContagem_clicked(){



    /* Até o momento não foi possível mudar o nome do botão cancelar nem criar outro botão
    QFileDialog *janelaSalvar = new QFileDialog();
    janelaSalvar->setLabelText(QFileDialog::DialogLabel(3),"&asd" );
    janelaSalvar->setParent(this);
    janelaSalvar->setNameFilter("Salvar relatório");
    //janelaSalvar->set

    QString salvar = janelaSalvar->exec(); */


    QObjectList listaRadioButtons = ui->frameContaParticulas->children();
    for (int i = 0; i < listaRadioButtons.size(); i++){
            if (isRadioButton(listaRadioButtons[i])){
                QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
                //a rotina abaixo serve para retirar a contagem anterior no momento de começar uma nova contagem
                radioButton->setText(radioButton->text().section('#',0,0));

            }

    }


    abrirImagem(objContarParticulas->imgOriginal);
    ui->label->removeEventFilter(this);
    ativarControlesSegmentacao();

}

void MainWindow::on_pushButtonSalvarRelatorio_clicked()
{
    QMessageBox::information(NULL, "Importante","Selecione uma pasta e será gerado um relatório de todos os projetos Quanto existentes nela. "
                                                "\n  \n Para que o relatório tenha dados consistentes, os projetos Quanto da pasta selecionada deverão possuir o mesmo padrão e os mesmos tipos");

    QString diretorioDosProjetos = QFileDialog::getExistingDirectory(this, "Selecione a pasta e geraremos o relatório de todos os projetos Quanto");

    if (diretorioDosProjetos!=""){
        objContarParticulas->exportarDados(diretorioDosProjetos);
    }
}



/**
 * @brief realizarContagem em um ponto da imagem foi feito o clique e qual tipo (radioButton) foi selecionado
 */

void MainWindow::realizarContagem(QPoint pos, QString operacao){
    QObjectList listaRadioButtons = ui->frameContaParticulas->children();

    for (int i = 0; i < listaRadioButtons.size(); i++){
        if (isRadioButton( listaRadioButtons[i])){
            QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);

            if (radioButton->isChecked()){
                //teste para saber o tipo de operacao realizado pelo usuario, clique com botao esquerdo = incrementar; clique com botao esquerdo do mouse = decrementar
                if (operacao == "incrementar"){
                    objContarParticulas->incrementar(ui->label,pos,i);

                }else if(operacao == "decrementar"){
                    objContarParticulas->decrementar(ui->label, pos);

                }

            }


        }

    }

    for (int i = 0; i < listaRadioButtons.size(); i++){
        if (isRadioButton(listaRadioButtons[i])){
            QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
            radioButton->setText(radioButton->objectName() + " #" + QString::number(objContarParticulas->quantidade[i-1]));
        }
    }
}

void MainWindow::on_pushButtonSelecionarTemplate_clicked()
{
    QObjectList listaRadioButtons = ui->frameContaParticulas->children();
    for (int i = 0; i < listaRadioButtons.size(); i++){
        if (isRadioButton(listaRadioButtons[i])){
            QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
            if (radioButton->isChecked()){
                objContarParticulas->selecionarTemplate(i);
            }


        }

    }
}

void MainWindow::on_pushButtonContagemAutomatica_clicked()
{
    QObjectList listaRadioButtons = ui->frameContaParticulas->children();
    for (int i = 0; i < listaRadioButtons.size(); i++){
        if (isRadioButton(listaRadioButtons[i])){
            QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
            if (radioButton->isChecked()){
                objContarParticulas->contagemAutomatica(ui->label, i);
            }


        }

    }

    for (int i = 0; i < listaRadioButtons.size(); i++){
            if (isRadioButton(listaRadioButtons[i])){
                QRadioButton *radioButton =  qobject_cast<QRadioButton*>(listaRadioButtons[i]);
                radioButton->setText(radioButton->objectName() + " " + QString::number(objContarParticulas->quantidade[i-1]));
            }
        }
}




double MainWindow::incluirEscala()
{
    bool ok;
    double valor = QInputDialog::getDouble(this, tr("Definir escala"), tr("1cm equivale a quantos pixels?:"), 3, 1, 1000, 1, &ok);
    return valor;

}

bool MainWindow::haImagem(){
    if (!ui->label->pixmap()){
        QMessageBox::information(NULL, "Mensagem de erro", "Não foi encontrada a imagem para a operação");
        return false;
    }else{
        return true;
    }
}

void MainWindow::iniciarPaint(QString acao){

    //testa se ha imagem no label
    if (!haImagem())
            return;


    ui->label->installEventFilter(this);
    acaoSolicitada = acao;   
    paintObj = new paint(unMedidaEscala, escalaPorDezPixels, ui->comboBoxCoresFormas->currentIndex(),ui->checkBoxMostrarMedida->isChecked());
    paintObj->imgOriginal = ui->label->pixmap()->toImage();
}

void MainWindow::on_comboBoxCoresFormas_currentIndexChanged(int index)
{
    if (paintObj !=NULL){
        paintObj->cor = index;
    }
}

void MainWindow::on_pushButtonLine_clicked()
{
    iniciarPaint("pushButtonLine");
}

void MainWindow::on_pushButtonRect_clicked()
{
     iniciarPaint("pushButtonRect");
}

void MainWindow::on_pushButtonSelection_clicked()
{
    QMessageBox::information(NULL,"Aplicativo em desenvolvimento", "Esta funcionalidade ainda não foi implementada");
    //iniciarPaint("pushButtonSelection");
}

void MainWindow::on_pushButtonElispse_clicked()
{
    iniciarPaint("pushButtonElispse");
}

void MainWindow::on_pushButtonArc_clicked()
{
    iniciarPaint("pushButtonArc");
}

void MainWindow::on_pushButtonPolygon_clicked()
{
    QMessageBox::information(NULL,"Aplicativo em desenvolvimento", "Esta funcionalidade ainda não foi implementada");
    //iniciarPaint("pushButtonPolygon");
}

void MainWindow::on_pushButtonText_clicked()
{
    QMessageBox::information(NULL,"Aplicativo em desenvolvimento", "Esta funcionalidade ainda não foi implementada");
    //iniciarPaint("pushButtonText");
}

void MainWindow::on_pushButtonPen_clicked()
{
    QMessageBox::information(NULL,"Aplicativo em desenvolvimento", "Esta funcionalidade ainda não foi implementada");
    //iniciarPaint("pushButtonPen");
}

void MainWindow::on_pushButtonBackMouse_clicked()
{
    ui->label->removeEventFilter(this);
}

void MainWindow::on_actionSelecionar_escala_triggered()
{
    QMessageBox::information(NULL,"Selecionar escala", "Para definir a escala, trace uma reta com uma distância conhecida.");
    iniciarPaint("selecionarEscala");
}


void MainWindow::pegarDadosEscala(QPoint posicaoDoClick, QPoint posicaoDoRelease){
    double tamanhoEmPixels = posicaoDoRelease.x() - posicaoDoClick.x();
    double tamanhoReal;

    QDialog dialog(this);
    // layout para organizar os labels e o formulario
    QFormLayout form(&dialog);

    // Adicionar texto ao dialog
    form.addRow(new QLabel("Qual tamanho corresponde à reta criada?"));

    QLineEdit *lineEditReta = new QLineEdit(&dialog);
    QString labelReta = QString("Tamanho da reta em pixels").arg(1);
    form.addRow(labelReta, lineEditReta);
    lineEditReta->setText(QString::number(std::abs(tamanhoEmPixels)));


    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString("Tamanho").arg(2);
    form.addRow(label, lineEdit);

    QComboBox *combobox = new QComboBox(&dialog);
    combobox->addItem("km");
    combobox->addItem("m");
    combobox->addItem("dm");
    combobox->addItem("cm");
    combobox->addItem("mm");
    combobox->addItem("µm");
    combobox->addItem("nm");
    QString label2 = QString("Unidade de medida").arg(3);
    form.addRow(label2, combobox);



    // Adicinando os botoes de ok e cancelar
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Mostrando o dialog
    if (dialog.exec() == QDialog::Accepted) {
        //se o usuario clicar em ok
        ui->checkBoxMostrarMedida->setEnabled(true);
        bool ok, ok2;
        tamanhoReal = lineEdit->text().toDouble(&ok);
        tamanhoEmPixels = lineEditReta->text().toDouble(&ok2);
        unMedidaEscala = combobox->currentText();

        if (!ok || !ok2){
            QMessageBox::information(NULL,"Erro", "O tamanho deve ser um número, não um texto. Selecione a escala novamente");
            ui->label->removeEventFilter(this);
            ui->label->setPixmap(QPixmap::fromImage(paintObj->imgOriginal));
            return;
        }

       escalaPorDezPixels = (tamanhoReal * 10)/tamanhoEmPixels;



      //funcao plotar escala do arquivo Paint.cpp
       ui->label->setPixmap(QPixmap::fromImage(paintObj->plotarEscala(escalaPorDezPixels,unMedidaEscala)));


    }else{
         ui->label->removeEventFilter(this);
         ui->label->setPixmap(QPixmap::fromImage(paintObj->imgOriginal));
         return;

    }

    //plotando a escala

}

bool MainWindow::isRadioButton(QObject* objeto){

    if (objeto->isWidgetType()){
        //este if é necessário para confirmar que os outros widgets que estão dentro do frame não vão ser considerados como radioButtons
        if (objeto->objectName()!= "pushButtonDeletarTipo" & objeto->objectName()!= "pushButtonAddTipo" & objeto->objectName()!= "labelContaParticulas"){
            return true;
        }else {
            return false;
        }
    }else {
        return false;
    }

}

/**
 * @brief MainWindow::on_action_Proxima_imagem_triggered Percorre todo o diretorio e, ao encontrar a imagem corrente, mostra a proxima imagem
 */
void MainWindow::on_action_Proxima_imagem_triggered()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    DIR *dir;
    struct dirent *lsdir;
    char *charNomeArquivo;
    QString nomeArquivo,caminhoArquivo,ArquivoTrocado;

    //rotina necessária para deixar o label redimensionável (Obs.:o zoom tira essa opção pra funcionar)
    ui->scrollArea->setWidgetResizable(true);

    ArquivoTrocado = arquivoCorrente;    

    //se houver instancia do objeto de contagem
    if (objContarParticulas != NULL){
        //é necessário colocar a imagem original no label pq ela é que vai ser salva #gambiarra
        abrirImagem(objContarParticulas->imgOriginal);
        QString nomeProjetoQuanto = arquivoCorrente;
        nomeProjetoQuanto = nomeProjetoQuanto.replace(nomeProjetoQuanto.length()-4,4, "." + extensao);
        salvarProjeto(caminhoCompletoDoProjeto + barra + nomeProjetoQuanto);
        this->on_pushButtonPausarContagem_clicked();
    }else{
        salvarProjeto(caminhoCompletoDoProjeto + barra + arquivoCorrente);
    }

    QDir diret(diretorio);

    //dir = opendir(diretorio.toStdString().c_str());
    if (!diret.exists()){
            QMessageBox::information(NULL, "Erro ao abrir diretório","Não foi possivel abrir diretorio.");
            return;
    }

   QStringList listaArquivos = diret.entryList();

   int indiceImgOriginal = listaArquivos.indexOf(arquivoCorrente);
   int indiceProxImagem = indiceImgOriginal;
   bool loop = true;

   do{
       indiceProxImagem++;
       if(indiceProxImagem >= listaArquivos.size()){
            indiceProxImagem = 0;
       }

       if (listaArquivos.at(indiceProxImagem).contains(".png", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".xpm", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".jpg", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".jpeg", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".bmp", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".pgm", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".tiff", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".qto", Qt::CaseInsensitive)){
            loop = false;
       }
   }while(loop);

   carregarProjeto(diretorio+ "/" + listaArquivos.at(indiceProxImagem));
   arquivoCorrente = listaArquivos.at(indiceProxImagem);

   loop = true;
   do{
       indiceProxImagem++;
       if(indiceProxImagem >= listaArquivos.size()){
            indiceProxImagem = 0;
       }

       if (listaArquivos.at(indiceProxImagem).contains(".png", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".xpm", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".jpg", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".jpeg", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".bmp", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".pgm", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".tiff", Qt::CaseInsensitive) || listaArquivos.at(indiceProxImagem).contains(".qto", Qt::CaseInsensitive)){
            loop = false;
       }
   }while(loop);

   if(listaArquivos.at(indiceProxImagem) == primeiroArquivo){
       ui->action_Proxima_imagem->setEnabled(false);
       ui->action_Imagem_anterior->setEnabled(true);
   }
}






/**
 * @brief MainWindow::on_action_Imagem_anterior_triggered Percorre todo o diretorio e, ao encontrar a imagem corrente, mostra a imagem anterior
 */
void MainWindow::on_action_Imagem_anterior_triggered()
{

    //testa se ha imagem no label
    if (!haImagem())
            return;

    DIR *dir;
    struct dirent *lsdir;
    char *charNomeArquivo;
    QString nomeArquivo, arquivoCorrenteAux, ultimaImagemDoDiretorio, caminhoArquivoParaAbrir, nomeProjetoQuanto;
    bool isPrimeiraImagem = true; //indica se a imagem corrente e a primeira do diretorio.


    //rotina necessária para deixar o label redimensionável (Obs.:o zoom tira essa opção pra funcionar)
    ui->scrollArea->setWidgetResizable(true);



    //se houver instancia do objeto de contagem
    if (objContarParticulas != NULL){
        //é necessário colocar a imagem original no label pq ela é que vai ser salva #gambiarra
        abrirImagem(objContarParticulas->imgOriginal);
        nomeProjetoQuanto = nomeProjetoQuanto.replace(arquivoCorrente.length()-4,4, "." + extensao);
        salvarProjeto(caminhoCompletoDoProjeto + barra + nomeProjetoQuanto);
        this->on_pushButtonPausarContagem_clicked();
    }else{
        salvarProjeto(caminhoCompletoDoProjeto + barra + arquivoCorrente);
    }


    dir = opendir(diretorio.toStdString().c_str());
    if (dir == 0) {
            QMessageBox::information(NULL, "Erro ao abrir diretório","Não foi possivel abrir diretorio.");
            return;
    }

    // Percorre os arquivos do diretorio
    while ( ( lsdir = readdir(dir) ) != NULL ){
        charNomeArquivo = lsdir->d_name;
        nomeArquivo = charNomeArquivo;

        //testa se o arquivo em questao e uma imagem
        if (nomeArquivo.contains(".png", Qt::CaseInsensitive) || nomeArquivo.contains(".xpm", Qt::CaseInsensitive) || nomeArquivo.contains(".jpg", Qt::CaseInsensitive) || nomeArquivo.contains(".jpeg", Qt::CaseInsensitive) || nomeArquivo.contains(".bmp", Qt::CaseInsensitive) || nomeArquivo.contains(".pgm", Qt::CaseInsensitive) || nomeArquivo.contains(".tiff", Qt::CaseInsensitive) || nomeArquivo.contains(".qto", Qt::CaseInsensitive)){
            ultimaImagemDoDiretorio = nomeArquivo;
        }
    }

    arquivoCorrenteAux = ultimaImagemDoDiretorio;
    nomeArquivo = diretorio + barra + ultimaImagemDoDiretorio;
    caminhoArquivoParaAbrir = nomeArquivo;

    closedir(dir);
    dir = opendir(diretorio.toStdString().c_str());

    // Percorre os arquivos do diretorio
    while ( ( lsdir = readdir(dir) ) != NULL ){
        charNomeArquivo = lsdir->d_name;
        nomeArquivo = charNomeArquivo;

        //testa se o arquivo em questao e uma imagem
        if (nomeArquivo.contains(".png", Qt::CaseInsensitive) || nomeArquivo.contains(".xpm", Qt::CaseInsensitive) || nomeArquivo.contains(".jpg", Qt::CaseInsensitive) || nomeArquivo.contains(".jpeg", Qt::CaseInsensitive) || nomeArquivo.contains(".bmp", Qt::CaseInsensitive) || nomeArquivo.contains(".pgm", Qt::CaseInsensitive) || nomeArquivo.contains(".tiff", Qt::CaseInsensitive) || nomeArquivo.contains(extensao, Qt::CaseInsensitive)){
            if (nomeArquivo == arquivoCorrente){
                if(nomeArquivo == primeiroArquivo){
                    QMessageBox::warning(NULL,"Primeira imagem", "Esta é a primeira imagem");
                    closedir(dir);
                    return;
                }
                arquivoCorrente = arquivoCorrenteAux;                
                carregarProjeto(caminhoArquivoParaAbrir);                
                break;
            }else{
                    isPrimeiraImagem = false;
                    arquivoCorrenteAux = nomeArquivo;
                    nomeArquivo = diretorio + barra + nomeArquivo;
                    caminhoArquivoParaAbrir = nomeArquivo;                    
                    carregarProjeto(caminhoArquivoParaAbrir);
                    //this->on_pushButtonIniciarContagem_clicked();
                }
        }
    }

    closedir(dir);

}

void MainWindow::on_checkBoxMostrarMedida_clicked()
{
    if (paintObj != NULL)
        paintObj->mostrarMedida = ui->checkBoxMostrarMedida->isChecked();

}

void MainWindow::carregarProjeto(QString caminho){    
    Mat matImagem;

    if (caminho.right(3) == "qto"){

      projetoQuanto = getDadosProjeto(caminho);

       QStringList listaDadosAnalise = projetoQuanto.dadosAnalise.split("*");
       //se for contagem, ajusta os tipos no radioButton
       if (listaDadosAnalise.at(0) == "contagem"){
           QObjectList listaRadioButtons = ui->frameContaParticulas->children();           

           for (int i=0; i< listaRadioButtons.size();i++){
               if (isRadioButton(listaRadioButtons.at(i)))
                   ui->frameContaParticulas->children().at(i)->deleteLater();
           }

           QStringList listaTipos = listaDadosAnalise.at(2).split("-");
           QStringListIterator iteratorTipos(listaTipos);
           QString nomeTipo;
           while (iteratorTipos.hasNext()){
               nomeTipo = iteratorTipos.next();
               if (nomeTipo!= ""){                   
                   QRadioButton *button = new QRadioButton(nomeTipo, ui->frameContaParticulas);
                   button->setObjectName(nomeTipo);
                   button->setChecked(true);
                   QFont fonte("MS Shell Dlg 2", 7);
                   button->setFont(fonte);
                   ui->frameContaParticulas->layout()->addWidget(button);
                   button->show();

               }
           }
           this->abrirImagem(projetoQuanto.imagem);

       }


    } else {        

        //matImagem = imread(caminho.toStdString().c_str() );

        QImage imagem(caminho);




        //código para corrigir o problema que ocorre ao abrir imagem que tenha caracter especial no caminho.
       /* QFile imfile(caminho);
        imfile.open(QFile::ReadOnly);
        std::string imbuff = imfile.readAll().toStdString();
        imfile.close();
        std::vector<byte> vectordata(imbuff.begin(), imbuff.end());
        matImagem(cv::imdecode(cv::Mat(vectordata, true), CV_LOAD_IMAGE_COLOR));*/



        //ui->label->setPixmap(imagemAux);







        //QImage imagem = matToQImage(matImagem);
        projetoQuanto.dadosAnalise = "";
        this->abrirImagem(imagem);
        if (objContarParticulas != NULL)
            objContarParticulas = NULL;
    }

    //caso haja dados de contagens anteriores, os dados são carregados para o objeto da contagem
    if (projetoQuanto.dadosAnalise != ""){
        //verificar como vai ficar a questão do new contarParticulas
        objContarParticulas = new ContarParticulas(ui->label->pixmap()->toImage());
        objContarParticulas->carregarDadosAnalise(projetoQuanto.dadosAnalise, ui->label);
    }


}

void MainWindow::salvarProjeto(QString caminho){

    if( caminho == "" )
    { return;}    

    QImage imagem = ui->label->pixmap()->toImage();



    if (objContarParticulas != NULL){


        //string do cabeçalho
        QString cabecalho = "Qto~contagem*";
        cabecalho += diretorio + "/" + arquivoCorrente + "*";        
        cabecalho += objContarParticulas->getTipos() + "*\n";

        //String com as informações dos pontos
        QString strListaPontos = objContarParticulas->getListaPontos();

        QString nomeArquivoQuanto = arquivoCorrente;
        nomeArquivoQuanto.replace((nomeArquivoQuanto.length()-3),3,extensao);

        QString dadosAnalise = cabecalho + strListaPontos;


        //transforma a imagem em QByteArray para juntar com o texto, comprimir e salvar
        QByteArray imagemByteArray;
        QBuffer buffer(&imagemByteArray);
        buffer.open(QIODevice::WriteOnly);
        imagem.save(&buffer, "jpg");

        QImage img = QImage::fromData(imagemByteArray, "BMP");

        //junção da imagem com a string das informações do objeto contar particulas
        imagemByteArray.append(dadosAnalise);

        //compacta o arquivo para ficar em tamanho menor
        imagemByteArray = qCompress(imagemByteArray, 9);


        //QMessageBox::information(NULL, "as", caminho + "/" + nomeArquivo);

        QFile arquivoParaSalvar(caminho);
        arquivoParaSalvar.open(QIODevice::WriteOnly);
        arquivoParaSalvar.write(imagemByteArray);

    }else{
        //se o objContarParticulas for nulo entao não há análise, é só salvar
        Mat MatImg = qImageToMat(imagem);        
        imwrite(caminho.toStdString().c_str(), MatImg);
    }
}

void MainWindow::on_pushButtonFracaoDePontos_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;
    QString strResultado;
    estereologia estereObj;
    QImage img;
    img = ui->label->pixmap()->toImage();
    img = estereObj.fracaoDePontos(img, &strResultado);
    this->showImage(qImageToMat(img));
    QMessageBox::information(NULL,"Fração volumétrica por fração de pontos (Pp)",strResultado);

}

void MainWindow::on_pushButtonAreaSuperficial_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    if (escalaPorDezPixels == NULL){
        QMessageBox::warning(NULL," Não foi definida a escala", "Este procedimento necessita de uma escala. Por favor, defina a escala antes de repetir a operação");
        return;
    }

    estereologia estereObj;
    QImage img;
    QString strResultado;
    img = ui->label->pixmap()->toImage();
    img = estereObj.areaSuperficial(img, escalaPorDezPixels, &strResultado);
    this->showImage(qImageToMat(img));
    QMessageBox::information(NULL,"Área superficial por unidade de volume teste (Sv)", strResultado);
}

void MainWindow::on_pushButtonComprimentoPorUnidadeDeVolume_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    if (escalaPorDezPixels == NULL){
        QMessageBox::warning(NULL," Não foi definida a escala", "Este procedimento necessita de uma escala. Por favor, defina a escala antes de repetir a operação");
        return;
    }

    estereologia estereObj;
    QImage img;
    img = ui->label->pixmap()->toImage();
    img = estereObj.comprimentoPorUnidadeDevolume(img, escalaPorDezPixels);
    this->showImage(qImageToMat(img));

}

void MainWindow::on_pushButtonInserirLinhasHorizontais_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    bool ok;
    int quantLinhas = QInputDialog::getInt(this, tr("Quantidade de linhas"), tr("Digite a quantidade de linhas"), 10, 2, 20, 1, &ok);

    if(ok){
        estereologia aa;
        QImage img;
        img = ui->label->pixmap()->toImage();
        img = aa.inserirlinhasHorizontais(img, quantLinhas);
        this->showImage(qImageToMat(img));
    }

}

void MainWindow::on_pushButtonInserirGradeDeLinhas_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    bool ok;
    int quantLinhas = QInputDialog::getInt(this, tr("Quantidade de linhas"), tr("Digite a quantidade de linhas e colunas"), 10, 2, 20, 1, &ok);


    if(ok){
        estereologia aa;
        QImage img;
        img = ui->label->pixmap()->toImage();
        img = aa.inserirGradeLinhas(img, quantLinhas);
        this->showImage(qImageToMat(img));
    }

}

void MainWindow::on_pushButtonInserirGradeDePontos_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    bool ok;
    int quantLinhas = QInputDialog::getInt(this, tr("Quantidade de linhas"), tr("Digite a quantidade de linhas e colunas"), 10, 2, 20, 1, &ok);


    if(ok){
        estereologia aa;
        QImage img;
        img = ui->label->pixmap()->toImage();
        img = aa.inserirGradeDePontos(img, quantLinhas);
        this->showImage(qImageToMat(img));
    }
}

void MainWindow::on_pushButtonInserirPontos_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    estereologia aa;
    QImage img;
    img = ui->label->pixmap()->toImage();
    //img = aa.inserirPontos(img);
    this->showImage(qImageToMat(img));
}


void MainWindow::on_pushButtonFracaoLinear_clicked()
{
    //testa se ha imagem no label
    if (!haImagem())
            return;

    estereologia estereObj;
    //estereObj.pixelsporCm = incluirEscala();
    QImage img;
    img = ui->label->pixmap()->toImage();
    img = estereObj.fracaoLinear(img);
    this->showImage(qImageToMat(img));

}




void MainWindow::on_pushButtonCalcularPorosidade_clicked()
{
    /*QImage imagem;
    Mat matImg;
    int faixa[10]

    imagem = ui->label->pixmap()->toImage();

    matImg = qImageToMat(imagem);

    cvtColor( matImg, matImg, CV_RGB2GRAY );

    for (int i=0;i<matImg.rows;i++){
        for (int j; j< matImg.cols;j++){

        }
    }*/


}

void MainWindow::ativarControlesSegmentacao(){


    //menus
    ui->menu_Arquivo->setEnabled(true);
    ui->menu_Editar->setEnabled(true);
    ui->menuFerramentas->setEnabled(true);
    ui->menu_Filtros->setEnabled(true);
    ui->menu_Zoom->setEnabled(true);
    ui->action_Exportar_imagem->setEnabled(true);


    //parte da contagem de imagens
    ui->pushButtonIniciarContagem->setEnabled(true);    
    ui->pushButtonPausarContagem->setEnabled(false);
    ui->pushButtonContagemAutomatica->setEnabled(false);
    ui->pushButtonAddTipo->setEnabled(true);
    ui->pushButtonDeletarTipo->setEnabled(true);
    ui->pushButtonReiniciarContagem->setEnabled(false);

    //parte que cria as formas
    ui->pushButtonBackMouse->setEnabled(true);
    ui->pushButtonElispse->setEnabled(true);
    ui->pushButtonLine->setEnabled(true);
    //ui->pushButtonPen->setEnabled(true);
    //ui->pushButtonPolygon->setEnabled(true);
    ui->pushButtonRect->setEnabled(true);
    //ui->pushButtonText->setEnabled(true);
    ui->comboBoxCoresFormas->setEnabled(true);    


    //parte da segmentação de imagens
    ui->actionBinaria->setEnabled(true);
    ui->actionBrilhoContraste->setEnabled(true);
    ui->actionCanny->setEnabled(true);
    ui->actionDilatacao->setEnabled(true);
    ui->actionE->setEnabled(true);
    ui->actionEqualizar->setEnabled(true);
    ui->actionErosao->setEnabled(true);
    ui->actionFindContourns->setEnabled(true);
    ui->actionGaussiana->setEnabled(true);
    ui->actionLaplaciano->setEnabled(true);
    ui->actionLimiar->setEnabled(true);
    ui->actionMedia->setEnabled(true);
    ui->actionMediana->setEnabled(true);
    ui->actionNegativo->setEnabled(true);
    ui->actionNormalizar->setEnabled(true);
    ui->actionN_o->setEnabled(true);
    ui->actionOu->setEnabled(true);
    ui->actionOu_exclusivo->setEnabled(true);
    ui->actionSobel->setEnabled(true);
    ui->actionSomar->setEnabled(true);
    ui->actionSubtrair->setEnabled(true);
    ui->actionTons_de_cinza->setEnabled(true);
    ui->actionVisualizar->setEnabled(true);
    ui->actionWatershed->setEnabled(true);
    ui->action_Abrir_imagem->setEnabled(true);
    ui->action_Colar->setEnabled(true);
    ui->action_Copiar->setEnabled(true);
    ui->action_Recortar->setEnabled(true);
    ui->action_Sair->setEnabled(true);
    ui->action_Salvar->setEnabled(true);

    ui->actionSelecionar_escala->setEnabled(true);
    ui->action_Proxima_imagem->setEnabled(true);
    ui->action_Imagem_anterior->setEnabled(true);
    ui->actionZoomMais->setEnabled(true);
    ui->actionZoomMenos->setEnabled(true);

}

void MainWindow::ativarControlesContagem(){
    ui->pushButtonIniciarContagem->setEnabled(false);
    ui->pushButtonAddTipo->setEnabled(false);
    ui->pushButtonDeletarTipo->setEnabled(false);
    ui->menu_Arquivo->setEnabled(false);

    ui->menu_Editar->setEnabled(false);
    ui->menuFerramentas->setEnabled(false);
    ui->menu_Filtros->setEnabled(false);

    ui->actionBinaria->setEnabled(false);
    ui->actionBrilhoContraste->setEnabled(false);
    ui->actionCanny->setEnabled(false);
    ui->actionDilatacao->setEnabled(false);
    ui->actionE->setEnabled(false);
    ui->actionEqualizar->setEnabled(false);
    ui->actionErosao->setEnabled(false);
    ui->actionFindContourns->setEnabled(false);
    ui->actionGaussiana->setEnabled(false);
    ui->actionLaplaciano->setEnabled(false);
    ui->actionLimiar->setEnabled(false);
    ui->actionMedia->setEnabled(false);
    ui->actionMediana->setEnabled(false);
    ui->actionNegativo->setEnabled(false);
    ui->actionNormalizar->setEnabled(false);
    ui->actionN_o->setEnabled(false);
    ui->actionOu->setEnabled(false);
    ui->actionOu_exclusivo->setEnabled(false);
    ui->actionSobel->setEnabled(false);
    ui->actionSomar->setEnabled(false);
    ui->actionSubtrair->setEnabled(false);
    ui->actionTons_de_cinza->setEnabled(false);
    ui->actionVisualizar->setEnabled(false);
    ui->actionWatershed->setEnabled(false);
    ui->action_Abrir_imagem->setEnabled(false);
    ui->action_Colar->setEnabled(false);
    ui->action_Copiar->setEnabled(false);
    ui->action_Recortar->setEnabled(false);
    ui->action_Sair->setEnabled(false);
    ui->action_Salvar->setEnabled(false);

    //parte que cria as formas
    ui->pushButtonBackMouse->setEnabled(false);
    ui->pushButtonElispse->setEnabled(false);
    ui->pushButtonLine->setEnabled(false);
    ui->pushButtonPen->setEnabled(false);
    ui->pushButtonPolygon->setEnabled(false);
    ui->pushButtonRect->setEnabled(false);
    ui->pushButtonText->setEnabled(false);
    ui->comboBoxCoresFormas->setEnabled(false);


    //ui->pushButtonSelecionarTemplate->setEnabled(true);
    ui->pushButtonPausarContagem->setEnabled(true);
    ui->pushButtonContagemAutomatica->setEnabled(true);
    ui->pushButtonSalvarRelatorio->setEnabled(true);
    ui->pushButtonReiniciarContagem->setEnabled(true);

}




// é ã ó ç á í ê ú
















































