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
    //* 每 150 * 0.02s = 3s 出一个怪
    auto gamestate = new GameState(50, 150, 5, 13, this);
    setFixedSize(gamestate->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
