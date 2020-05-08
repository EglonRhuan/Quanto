//#include "contarparticulas.h"
//#include <QMainWindow>
//#include <QtWidgets>

#include "imageoperations.h"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "dialogcontagemautomatica.h"
#include "mainwindow.h"
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include "QMessageBox"
#include <dirent.h>
#include <QDir>




using namespace cv;


ContarParticulas::ContarParticulas(QImage img)
{
    imgOriginal = img;


    for (int i = 0; i<10; i++){
        quantidade[i] = 0;
    }

    listaPontos = new QLinkedList<ponto>;

    cor[0] = Qt::red;
    cor[1] = Qt::blue;
    cor[2] = Qt::green;
    cor[3] = Qt::cyan;
    cor[4] = Qt::darkRed;
    cor[5] = Qt::magenta;
    cor[6] = Qt::darkBlue;
    cor[7] = Qt::darkMagenta;
    cor[8] = Qt::darkCyan;
    cor[9] = Qt::darkYellow;

}

void ContarParticulas::incrementar(QLabel *labelPrincipal, QPoint pos, int idTipo){

    incrementar(pos, idTipo);

    plotPontos(labelPrincipal);

}

void ContarParticulas::incrementar(QPoint pos, int idTipo){

    ponto pontoAux;


    double x = pos.x();
    double y = pos.y();

    pontoAux.cor = cor[idTipo-1];
    if (listaPontos->isEmpty()){
        pontoAux.idPonto = 0;
    }else{
        pontoAux.idPonto = listaPontos->last().idPonto + 1;
    }

    //Ajustes para setar a corAux como uma cor em hexadecimal do tipo #RRGGBB
    QString corAux = "#000000";
    QString idStr = QString::number(pontoAux.idPonto, 16);
    corAux.replace(7-idStr.size(),idStr.size(),idStr);
    pontoAux.corAux.setNamedColor(corAux);

    pontoAux.idTipo = idTipo;
    pontoAux.pos = QPoint(x,y);

    listaPontos->append(pontoAux);

    quantidade[idTipo-1]++;

}


void ContarParticulas::decrementar(QLabel *labelPrincipal, QPoint pos){

   Mat img = qImageToMat(imgAux);
   double x = pos.x();
   double y = pos.y();


   QColor pixel = imgAux.pixel(x,y);


   QLinkedList<ponto>::iterator i;
      for (i = listaPontos->begin(); i != listaPontos->end(); ++i){

          if ((*i).corAux == pixel.name()){
              //QMessageBox::information(NULL,"as", QString::number(quantidade[(*i).idTipo-1]));
              quantidade[(*i).idTipo-1] = quantidade[(*i).idTipo-1] - 1;
              i = listaPontos->erase(i);

              //QMessageBox::information(NULL,"as", QString::number(quantidade[(*i).idTipo-1]));
          }

      }

    plotPontos(labelPrincipal);

}


void ContarParticulas::plotPontos(QLabel *labelPrincipal){

   QImage imgPlot;
   imgPlot = imgOriginal;
   imgAux = imgOriginal;



    QPainter painter(&imgPlot);
    QPainter painterAux(&imgAux);


    QLinkedList<ponto>::iterator i;
    for (i = listaPontos->begin(); i != listaPontos->end(); ++i){

        QRect marcador((*i).pos.rx()-4, (*i).pos.ry()-4, 8,8);
        //idTipo eh o identificador do radioButton que esta selecionado, eh necessario fazer idTipo-1 porque o array cor comeca em 0
        painter.fillRect(marcador, (*i).cor);
        painter.setPen((*i).cor);
        painter.drawRect(marcador);
        painter.drawText((*i).pos.rx()+4, (*i).pos.ry()-18, 20, 20, 1, QString::number((*i).idTipo), 0);

        painterAux.fillRect(marcador, (*i).corAux);
        painterAux.setPen((*i).corAux);
        painterAux.drawRect(marcador);
        painterAux.drawText((*i).pos.rx()+4, (*i).pos.ry()-18, 20, 20, 1, QString::number((*i).idTipo), 0);


    }

    labelPrincipal->setPixmap(QPixmap::fromImage(imgPlot));

}

void ContarParticulas::selecionarTemplate(int idTipo){
    DialogContagemAutomatica janela(0, imgOriginal);
    janela.setModal(true);
    janela.exec();

    templ[idTipo] = janela.templEscolhido;
    limiarParaTemplate[idTipo] = janela.limiarEscolhido;

}

void ContarParticulas::contagemAutomatica(QLabel *labelPrincipal,int idTipo){

    //if(templ[idTipo].isNull()){
        //QMessageBox::information(NULL,"Template Não encontrado","Não há template selecionado para este tipo, você será redirecionado para a seleção de template.");
        selecionarTemplate(idTipo);
        //return;
   // }

   if(templ[idTipo].isNull()){
        return;
   }

    QList<QPoint> listaPosicoes = encontrarParticulas(imgOriginal,templ[idTipo],limiarParaTemplate[idTipo]);


    for (int i = 0; i < listaPosicoes.size(); ++i) {        
        QPoint ponto;
        ponto.setX(listaPosicoes.at(i).x());
        ponto.setY(listaPosicoes.at(i).y());
        incrementar(ponto,idTipo);

    }

    plotPontos(labelPrincipal);
}

void ContarParticulas::mudarImagem(QImage img, QString nomeArquivo){
    imagemAnalisada imagemAtual;

    imgOriginal = img;

    imagemAtual.listaPontos = listaPontos;
    imagemAtual.nomeImagem = nomeArquivo;

    for (int i=0;i<10;i++){
        imagemAtual.quantidade[i] = quantidade[i];
        quantidade[i] = 0;
    }

    listaImagensAnalisadas.append(imagemAtual);
    listaPontos->clear();
}

void ContarParticulas::exportarDados(QString caminho){


    quantoTp projetoCorrente;
    QDir dir(caminho);
    QString ColunaNoRelatorio = "BCDEFGHIJK";
    QFile f( caminho + "/relatorio.csv");
    QString nomeArquivo, nomesTiposAux[10], nomesTiposAnterior[10], pularLinha;
    QStringList listaDadosAnalise, listaTipos, listaCabecalho, dadosCadaPonto;
    int idTipo, pontosPorTipo[10], quantArquivos = 0;


    QStringList listaArquivos = dir.entryList();



    if (!dir.exists()){
        QMessageBox::information(NULL, "Erro ao abrir a pasta","Não foi possivel abrir a pasta.");
        exit (1);
    }

    if (listaArquivos.isEmpty()){
        QMessageBox::information(NULL, "Falha ao encontrar arquivos","Não foi encontrado nenhum arquivo .qto na pasta selecionada");
        exit (1);
    }

    if( f.open( QIODevice::WriteOnly ) ){
        QTextStream ts( &f );
        ts << "Imagem";
    // Percorre os arquivos do caminho
        for(int i=0; i<listaArquivos.length();i++ ){

            nomeArquivo = listaArquivos.at(i);
            if (nomeArquivo.right(3) == "qto"){

                projetoCorrente = getDadosProjeto(caminho + "/" + nomeArquivo);
                listaDadosAnalise = projetoCorrente.dadosAnalise.split(";");
                listaCabecalho = listaDadosAnalise.at(0).split("*");

                listaTipos = listaCabecalho.at(2).split("-");


                for (int i = 0; i<10;i++){
                    pontosPorTipo[i] = 0;
                }


                QStringListIterator iteratorPontos(listaDadosAnalise);
                //este iterator é necessário pra pular a primeira posição da lista que contem informações sobre funcionalidade e outros
                iteratorPontos.next();
                while (iteratorPontos.hasNext()){
                      dadosCadaPonto = iteratorPontos.next().split("-");
                      idTipo = dadosCadaPonto.at(0).toInt();
                      pontosPorTipo[idTipo]++;
                }


                    //Este for foi feito para o caso de existirem arquivos com tipos diferentes dentro da mesma pasta, porém, por enquanto esta questão não será considerada
                    for (int i = 0; i < listaTipos.length(); i++){
                        nomesTiposAux[i] = listaTipos.at(i);
                    }
                    for (int i=0; i<10;i++){
                        //este if impede que o relatório fique cheio de tipos duplicados, só se escreve o tipo quando ele é diferente.
                        //if (nomesTiposAux[i].replace(" ","") != nomesTiposAnterior[i].replace(" ","")){
                        if (nomesTiposAnterior[i] == "" && nomesTiposAux[i] !=""){
                            ts << ";" + nomesTiposAux[i];
                            pularLinha = "\n";
                        }
                        nomesTiposAnterior[i] = nomesTiposAux[i];
                    }
                    //a variável pular linha é uma gambiarra pra pular a linha só quando se escreve um tipo diferente
                    ts << pularLinha;
                    pularLinha = "";

                    ts << nomeArquivo + ";" ;
                    for (int i=0; i<10;i++){
                        if (pontosPorTipo[i] !=0)
                            ts << QString::number(pontosPorTipo[i]) + ";";
                    }
                    ts << "\n";
                    quantArquivos++;

            }
        }
        //inclusÃ£o da soma das contagens em cada coluna do relatório
        ts << "Total;";
        for (int i=0; i<10 ;i++){
            if(nomesTiposAux[i]!=""){
                 ts << "=soma(" + QString(ColunaNoRelatorio.at(i)) +"2:" + QString(ColunaNoRelatorio.at(i)) + QString::number(quantArquivos+1) + ");" ;
            }
        }
        //inclusÃ£o do desvio padrão das contagens em cada coluna do relatório
        ts << "\nDesvio Padrão;";
        for (int i=0; i<10 ;i++){
            if(nomesTiposAux[i]!=""){
                 ts << "=DESVPAD(" + QString(ColunaNoRelatorio.at(i)) +"2:" + QString(ColunaNoRelatorio.at(i)) + QString::number(quantArquivos+1) + ");" ;
            }

        }

    QMessageBox::information(NULL,"Relatório criado com sucesso","O relatório foi criado com sucesso e está no caminho: " + caminho + "/" + nomeArquivo);

    }else{
        QMessageBox::warning(NULL,"falha ao salvar arquivo","Não foi possível salvar o arquivo. Se o relatório estiver aberto não é possível sobrescrevê-lo");
        return;
    }


    //caminhoArquivo = QFileDialog::getSaveFileName(this, tr("Salvar relatório..."), QString(), tr("Arquivos CSV(*.csv)"));


           f.close();


}

QString ContarParticulas::getListaPontos(){

    QString strListaPontos="";

    QLinkedList<ponto>::iterator i;
    for(i = listaPontos->begin(); i != listaPontos->end(); i++){
        strListaPontos += ";";
        strListaPontos += QString::number((*i).idTipo) + "-";
        strListaPontos += QString::number((*i).pos.rx()) + ",";
        strListaPontos += QString::number((*i).pos.ry());
    }

    return strListaPontos;
}

QString ContarParticulas::getTipos(){

    QString strTipos;            
    for (int i=0; i < 10;i++){
        if (nomesTipos[i] != ""){
            strTipos += nomesTipos[i] + "-";
        }
    }

    return strTipos;
}

void ContarParticulas::carregarDadosAnalise(QString dadosAnalise, QLabel *labelPrincipal){
    QPoint pos(100,100);
    int idTipo;


    QStringList listaDadosAnalise = dadosAnalise.split(";");
    QStringList listaCabecalho = listaDadosAnalise.at(0).split("*");
    QStringList listaTipos = listaCabecalho.at(2).split("-");


    for (int i = 0; i < listaTipos.length(); i++){
        nomesTipos[i] = listaTipos.at(i);
    }


    QStringListIterator iteratorPontos(listaDadosAnalise);
    //este iterator é necessário pra pular a primeira posição da lista que contem informações sobre funcionalidade e outros
    iteratorPontos.next();
    while (iteratorPontos.hasNext()){
          QStringList dadosCadaPonto = iteratorPontos.next().split("-");

          pos.setX(dadosCadaPonto.at(1).section(',',-2,-2).toInt());
          pos.setY(dadosCadaPonto.at(1).section(',',1,1).toInt());
          idTipo = dadosCadaPonto.at(0).toInt();

          this->incrementar(pos, idTipo);
    }
}



















