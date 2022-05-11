#include "place.h"
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

extern void deployOperator(GameState* gamestate, Place* place);

Place::Place(GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , _gamestate(gamestate)
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

Base::Base(GameState* gamestate, QWidget* parent)
    : UnreachablePlace(gamestate, parent)
{
}

Entrance::Entrance(GameState* gamestate, QWidget* parent)
    : UnreachablePlace(gamestate, parent)
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
    deployOperator(_gamestate, this);
}

UnreachablePlace::UnreachablePlace(GameState* gamestate, QWidget* parent)
    : Place(gamestate, parent)
{
}
