#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv/cv.h"
//#include "opencv2/imgproc/imgproc.hpp"
#include "dialogoplogicasaritmeticas.h"
#include <QLabel>
#include "contarparticulas.h"
#include "paint.h"


typedef struct quantoTp{
   QImage imagem;
   QString dadosAnalise;
}quantoTp;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString nomeDiretorioPadrao = "Quanto projects";
    QString caminhoCompletoDoProjeto;
    QString primeiroArquivo, arquivoCorrente, diretorio, barra;
    ContarParticulas* objContarParticulas = NULL;
    QString acaoSolicitada = "";
    paint* paintObj = NULL;
    double escalaPorDezPixels = NULL;
    QString unMedidaEscala;    
    QString extensao = "qto";
    quantoTp projetoQuanto;




private slots:
    void showImage(cv::Mat img);

    void on_action_Abrir_imagem_triggered();

    void on_action_Salvar_triggered();

    void on_actionMedia_triggered();

    void on_actionMediana_triggered();

    void on_actionGaussiana_triggered();

    void on_actionCanny_triggered();

    void on_actionLaplaciano_triggered();

    void on_actionSobel_triggered();

    void on_actionErosao_triggered();

    void on_actionDilatacao_triggered();

    void on_actionEqualizar_triggered();

    void on_actionLimiar_triggered();

    void on_actionNegativo_triggered();

    void on_actionBinaria_triggered();

    void on_actionWatershed_triggered();

    void on_actionVisualizar_triggered();

    void on_actionBrilhoContraste_triggered();

    void on_actionTons_de_cinza_triggered();

    void on_actionSomar_triggered();

    void on_actionSubtrair_triggered();

    void ExecutarOpLogicaAritmetica(int tipoOperacao);

    void on_actionE_triggered();

    void on_actionN_o_triggered();

    void on_actionOu_triggered();

    void on_actionOu_exclusivo_triggered();

    void on_actionZoomMais_triggered();

    void on_actionZoomMenos_triggered();

    void abrirImagem(QImage img);        

    void on_pushButtonAddTipo_clicked();

    void on_pushButtonDeletarTipo_clicked();

    void on_pushButtonIniciarContagem_clicked();

    void on_pushButtonPausarContagem_clicked();

    void realizarContagem(QPoint pos, QString operacao);

    void on_pushButtonInserirLinhasHorizontais_clicked();    

    void on_pushButtonInserirPontos_clicked();    

    bool haImagem();

    void on_pushButtonCalcularPorosidade_clicked();

    void on_pushButtonFracaoLinear_clicked();

    double incluirEscala();

    void on_pushButtonLine_clicked();

    void iniciarPaint(QString acao);

    void on_pushButtonRect_clicked();

    void on_pushButtonSelection_clicked();

    void on_pushButtonElispse_clicked();

    void on_pushButtonArc_clicked();

    void on_pushButtonPolygon_clicked();

    void on_pushButtonText_clicked();

    void on_pushButtonPen_clicked();

    void on_pushButtonBackMouse_clicked();

    void on_pushButtonContagemAutomatica_clicked();

    void on_pushButtonSelecionarTemplate_clicked();    

    void pegarDadosEscala(QPoint posicaoDoClick, QPoint posicaoDoRelease);

    void on_actionSelecionar_escala_triggered();

    bool isRadioButton(QObject *objeto);

    void on_action_Proxima_imagem_triggered();

    void on_action_Imagem_anterior_triggered();

    void on_comboBoxCoresFormas_currentIndexChanged(int index);

    void on_pushButtonSalvarRelatorio_clicked();

    void on_checkBoxMostrarMedida_clicked();

    void on_action_Exportar_imagem_triggered();

    void on_pushButtonReiniciarContagem_clicked();

    void carregarProjeto(QString caminho);

    void salvarProjeto(QString caminho);

    void ativarControlesContagem();

    void ativarControlesSegmentacao();

    void on_pushButtonFracaoDePontos_clicked();

    void on_pushButtonAreaSuperficial_clicked();

    void on_pushButtonComprimentoPorUnidadeDeVolume_clicked();    

    void on_pushButtonInserirGradeDeLinhas_clicked();

    void on_pushButtonInserirGradeDePontos_clicked();

private:
    Ui::MainWindow *ui;


protected:
    bool eventFilter(QObject* obj, QEvent *event);

};




#endif // MAINWINDOW_H
