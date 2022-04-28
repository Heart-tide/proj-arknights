
#include "reunion.h"
#include <QDebug>

Reunion::Reunion(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t move_speed,
    size_t id, vector<Place*> route, QWidget* parent)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent)
    , _move_speed(move_speed)
    , _route(route)
{
}

void Reunion::action(size_t time, size_t& hp, Infected* op)
{
    //? 220428 因将==误写作=，在此debug半个钟头
    if (_is_active == false || (time - _last_action_time) < _interval)
        return;
    _last_action_time = time;
    //* 攻击与移动共享间隔，不再区分。每轮不是攻击，就是移动 --- 做UI时应修改此处
    if (op) {
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
        }
    } else {
        //* 暂时无视移动距离书写，每次移动一格
        _route.pop_back();
        qDebug() << "\tMOVE" << qPrintable(giveName()) << "#"
                 << _id << qPrintable(_place->giveId()) << "-->"
                 << qPrintable(_route.back()->giveId());
        removeFrom();
        addTo(_route.back());
        if (_route.back()->isBase()) {
            qDebug() << "\tGETINTOBASE" << qPrintable(giveName()) << "#" << giveId();
            _is_active = false;
            hp--;
        }
    }
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

TestReunion::TestReunion(Place* place, size_t deployment_time,
    size_t id, vector<Place*> route, QWidget* parent)
    : Reunion(20, 5, 3, place, deployment_time, 1, id, route, parent)
{
}
