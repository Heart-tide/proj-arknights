#include "reunion.h"
#include <QDebug>

Reunion::Reunion(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t move_speed,
    size_t id, QVector<Place*> route, QWidget* parent)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent)
    , _move_speed(move_speed)
    , _route(route)
{
    _route.pop_back();
    //? _move_speed 原为无符号整数，运算时出大问题！以后运算前先检查各操作数类型！！！
    direction.first = (_route.back()->x() - _place->x()) / 100 * _move_speed;
    direction.second = (_route.back()->y() - _place->y()) / 100 * _move_speed;
    setGeometry(_place->x() + 10, _place->y(), 90, 90);
    //? 要show才能调用绘图事件！！！
    show();
}

void Reunion::action(size_t time, size_t& hp, Infected* op)
{
    //? 220428 因将==误写作=，在此debug半个钟头
    if (_is_active == false)
        return;
    if (op) {
        //* 攻击！
        if (time - _last_action_time < _interval)
            return;
        _last_action_time = time;
        op->reduceHealth(_damage);
        qDebug() << "\tATTACK" << qPrintable(giveName())
                 << "#" << giveId()
                 << "***" << qPrintable(op->giveName())
                 << "#" << op->giveId()
                 << "HHH -" << _damage
                 << ">>" << op->giveHealth();
        if (!op->isActive()) {
            qDebug() << "\tKILLED" << qPrintable(op->giveName())
                     << "#" << op->giveId();
            op->hide();
        }
    } else {
        //* 移动！
        setGeometry(x() + direction.first, y() + direction.second, 90, 90);
        if (x() == _route.back()->x() + 10 && y() == _route.back()->y()) {
            removeFrom();
            addTo(_route.back());
            _route.pop_back();
            direction.first = (_route.back()->x() - _place->x()) / 100 * _move_speed;
            direction.second = (_route.back()->y() - _place->y()) / 100 * _move_speed;
            if (_place->isBase()) {
                qDebug() << "\tGETINTOBASE" << qPrintable(giveName()) << "#" << giveId();
                _is_active = false;
                hp--;
            }
        }
    }
    update();
}

void Reunion::addTo(Place* place)
{
    _place = place;
    place->addReunion(this);
}

void Reunion::removeFrom()
{
    _place->removeReunion(this);
    _place = nullptr;
}

//* 把血条画出来
void Reunion::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QBrush red_brush(QColor("#66CCFF"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(10, 10, rate * 80, 5);
}

TestReunion::TestReunion(Place* place, size_t deployment_time,
    size_t id, QVector<Place*> route, QWidget* parent)
    : Reunion(50, 34, 50, place, deployment_time, 2, id, route, parent) //* 原石虫攻击力限时 UP !
{
}

void TestReunion::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap yuan("://res/reunion/yuan.png");
    painter.drawPixmap(0, 0, 100, 100, yuan);
    Reunion::paintEvent(event);
}
