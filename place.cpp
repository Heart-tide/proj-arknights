#include "place.h"
#include "ui_place.h"

Place::Place(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Place)
{
    ui->setupUi(this);
}

Place::~Place()
{
    delete ui;
}

void Place::addOperator(Operator* op)
{
    
}

void Place::addReunion(Reunion* reunion)
{
    
}

void Place::removeOperator(Operator* op)
{
    
}

void Place::removeReunion(Reunion* reunion)
{
    
}
