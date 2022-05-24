#include "mainwindow.h"
#include "gamestate.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Arknights");
    auto gamestate = new GameState(20, 100, 5, 3, this);
    setFixedSize(gamestate->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
