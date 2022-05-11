#include "mainwindow.h"
#include "gamestate.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(700, 605);
    setWindowTitle("Arknights");
    new GameState(20, 100, 100, 1, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
