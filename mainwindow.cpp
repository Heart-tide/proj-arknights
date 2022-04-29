#include "mainwindow.h"
#include "gamestate.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto game = new GameState(3, 10, 100, 1, this);
    connect(ui->deploy_operator_btn, &QPushButton::clicked,
        game, [=]() {
            game->deployOperator(0, 0, 0);
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}
