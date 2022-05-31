#include "place.h"
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

extern void deployOperator(GameState* gamestate, Place* place, int orientation);

Place::Place(GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , _gamestate(gamestate)
    , _op(nullptr)
    , _id(QPair<size_t, size_t>(0, 0))
    , _isPainted(false)
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
    : SpecialPlace(gamestate, parent)
{
}

Entrance::Entrance(GameState* gamestate, QWidget* parent)
    : SpecialPlace(gamestate, parent)
{
}

LowerPlace::LowerPlace(GameState* gamestate, QWidget* parent)
    : Place(gamestate, parent)
{
}

HigherPlace::HigherPlace(GameState* gamestate, QWidget* parent)
    : Place(gamestate, parent)
{
}

QString Place::getID() const
{
    return "[" + QString::number(_id.second)
        + "," + QString::number(_id.first) + "]";
}

void Place::paintAttackArea()
{
    _isPainted = !_isPainted;
    update();
}

void Place::mousePressEvent(QMouseEvent* event)
{
    //? 小心生成了但没进 map 的格子，它们也占空间！！！
    //? 减少子父对象的层数，否则对象的覆盖关系会令人迷惑，并使得鼠标点击事件被父对象截获
    // printf("%d %d ", _id.first, _id.second); //* 该行用于快速生成路径
    if (event->button() == Qt::LeftButton) {
        _prees_point = event->pos();
    }
}

void Place::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    int delta_x = event->x() - _prees_point.x();
    int delta_y = event->y() - _prees_point.y();
    if (abs(delta_x) > abs(delta_y)) {
        if (delta_x > 0) {
            deployOperator(_gamestate, this, 3);
        } else {
            deployOperator(_gamestate, this, 2);
        }
    }
    if (abs(delta_x) < abs(delta_y)) {
        if (delta_y > 0) {
            deployOperator(_gamestate, this, 1);
        } else {
            deployOperator(_gamestate, this, 0);
        }
    }
}

void Place::paintEvent(QPaintEvent*)
{
    if (_isPainted) {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::blue, Qt::FDiagPattern));
        painter.drawRect(0, 0, 100, 100);
    }
}

SpecialPlace::SpecialPlace(GameState* gamestate, QWidget* parent)
    : Place(gamestate, parent)
{
}
