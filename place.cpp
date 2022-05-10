#include "place.h"
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

Place::Place(QWidget* parent)
    : QWidget(parent)
    , _op(nullptr)
    , _id(QPair<size_t, size_t>(0, 0))
{
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

Base::Base(QWidget* parent)
    : UnreachablePlace(parent)
{
}

Entrance::Entrance(QWidget* parent)
    : UnreachablePlace(parent)
{
}

QString Place::giveId() const
{
    return "[" + QString::number(_id.first)
        + "," + QString::number(_id.second) + "]";
}

void Place::mousePressEvent(QMouseEvent* event)
{
    //? 小心生成了但没进 map 的格子，它们也占空间！！！
    //? 减少子父对象的层数，否则对象的覆盖关系会令人迷惑，并使得鼠标点击事件被父对象截获
    qDebug() << giveId() << "监测到鼠标点击事件";
}

UnreachablePlace::UnreachablePlace(QWidget* parent)
    : Place(parent)
{
}
