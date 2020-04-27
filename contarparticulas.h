#ifndef CONTARPARTICULAS_H
#define CONTARPARTICULAS_H

#include <QMainWindow>
//#include <QWidget>
#include "QLabel"
#include "qlabel.h"
#include "QLinkedList"




typedef struct ponto{
   int idTipo;
   int idPonto;
   QPoint pos;
   QColor corAux;
   QColor cor;
}ponto;

typedef struct imagemAnalisada{
    QLinkedList<ponto> *listaPontos;
    QString nomeImagem;
    int quantidade[10];
}imagemAnalisada;


class ContarParticulas
{
public:
    ContarParticulas(QImage img);
    QColor cor[10];
    int quantidade[10];
    QLinkedList<ponto> *listaPontos;
    QImage imgOriginal, imgAux;
    QString nomesTipos[10];
    QImage templ[10];
    double limiarParaTemplate[10];
    QLinkedList<imagemAnalisada> listaImagensAnalisadas;

public slots:
    void incrementar(QLabel *labelPrincipal, QPoint pos, int idTipo);
    void incrementar(QPoint pos, int idTipo);
    void decrementar(QLabel *labelPrincipal, QPoint pos);    
    void contagemAutomatica(QLabel *labelPrincipal, int idTipo);
    void selecionarTemplate(int idTipo);
    void mudarImagem(QImage img, QString nomeArquivo);
    void exportarDados(QString diretorio);
    QString getListaPontos();
    QString getTipos();
    void carregarDadosAnalise(QString dadosAnalise, QLabel *labelPrincipal);    
    void plotPontos(QLabel *labelPrincipal);

protected:

};

#endif // CONTARPARTICULAS_H
