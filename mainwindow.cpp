#include "mainwindow.h"
#include "gamestate.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto game = new GameState(100, this);
    game->configure();
}

MainWindow::~MainWindow()
{
    delete ui;
}
