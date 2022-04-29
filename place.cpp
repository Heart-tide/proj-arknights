#include "place.h"
#include "ui_place.h"
#include <QDebug>
#include <QString>

Place::Place()
    : ui(new Ui::Place)
    , _op(nullptr)
    , _id(QPair<size_t, size_t>(0, 0))
{
    ui->setupUi(this);
}

Place::~Place()
{
    delete ui;
}

void Place::addOperator(Infected* op)
{
    if (_op == nullptr)
        _op = op;
}

void Place::removeReunion(Infected* reunion)
{
    for (auto it = _reunions.begin(); it < _reunions.end(); it++) {
        if (*it == reunion) {
            _reunions.erase(it);
            break;
        }
    }
}

QString Place::giveId() const
{
    return "[" + QString::number(_id.first)
        + "," + QString::number(_id.second) + "]";
}
