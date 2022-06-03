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
    //* 每 300 * 0.02s = 6s 出一个怪
    auto gamestate = new GameState(50, 300, 5, 13, this);
    setFixedSize(gamestate->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
