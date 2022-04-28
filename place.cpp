#include "place.h"
#include "ui_place.h"
#include <QDebug>

Place::Place()
    : ui(new Ui::Place)
    , _op(nullptr)
{
    ui->setupUi(this);
}

Place::~Place()
{
    delete ui;
}

void Place::removeReunion(Reunion* reunion)
{
    for (auto it = _reunions.begin(); it < _reunions.end(); it++) {
        if (*it == reunion) {
            _reunions.erase(it);
            break;
        }
    }
}

void UnreachablePlace::addOperator(Operator* op)
{
    qDebug() << "无法向不可到达地块添加干员";
    exit(-1);
}

void UnreachablePlace::addReunion(Reunion* reunion)
{
    qDebug() << "无法向不可到达地块添加整合运动";
    exit(-1);
}

void UnreachablePlace::removeOperator(Operator* op)
{
    qDebug() << "无法从不可到达地块移除干员";
    exit(-1);
}

void UnreachablePlace::removeReunion(Reunion* reunion)
{
    qDebug() << "无法从不可到达地块移除整合运动";
    exit(-1);
}

void Entrance::addReunion(Reunion* reunion)
{
    _reunions.push_back(reunion);
}
