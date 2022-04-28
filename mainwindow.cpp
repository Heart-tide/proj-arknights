#include "mainwindow.h"
#include "gamestate.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto game = new GameState(5, 100, 1, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
