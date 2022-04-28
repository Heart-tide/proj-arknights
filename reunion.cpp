
#include "reunion.h"
#include <QDebug>

Reunion::Reunion(size_t health, int damage, size_t attack_interval,
    Place* place, size_t deployment_time, size_t move_speed,
    size_t id, vector<Place*> route, QWidget* parent)
    : Infected(health, damage, attack_interval, place,
        deployment_time, id, parent)
    , _is_blocked(false)
    , _move_speed(move_speed)
    , _route(route)
{
}

void Reunion::action(size_t time, size_t& hp)
{
    if (_is_active == false) { //? 220428 因将==误写作=，在此debug半个钟头
        return;
    }
    if ((time - _last_attack_time) >= _attack_interval) {
        //! TODO
    }
    auto active_time = time - _deployment_time;
    if (active_time % _move_speed == 0) {
        _route.pop_back();
        qDebug() << "\tMOVE" << qPrintable(giveName()) << "#" << _id << qPrintable(_place->giveId())
                 << "-->" << qPrintable(_route.back()->giveId());
        removeFrom();
        addTo(_route.back());
        if (_route.back()->is_base()) {
            qDebug() << "\tGETINBASE" << qPrintable(giveName()) << "#" << giveId();
            _is_active = false;
            hp--;
        }
    }
}

void Reunion::addTo(Place* place)
{
    if (!place->is_base()) {
        _place = place;
        place->addReunion(this);
    }
}

void Reunion::removeFrom()
{
    _place->removeReunion(this);
    _place = nullptr;
}

TestReunion::TestReunion(Place* place, size_t deployment_time,
    size_t id, vector<Place*> route, QWidget* parent)
    : Reunion(20, 5, 3, place, deployment_time, 3, id, route, parent)
{
}
