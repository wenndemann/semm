#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( this->ui->actionExit, SIGNAL( triggered( ) ), this, SLOT( close( ) ) );
    connect( this->ui->actionFullscreen, SIGNAL( triggered( ) ), this->ui->playboardView, SLOT( fullscreen( ) ) );
}

void MainWindow::initializeVisualization( uint32_t playboardWidth, uint32_t playboardHeight,
                                          uint32_t meepleWidth, uint32_t meepleHeight )
{
    this->ui->playboardView->initializePlayboard( playboardWidth, playboardHeight,
                                                  meepleWidth, meepleHeight );
}

MainWindow::~MainWindow()
{
    delete ui;
}

