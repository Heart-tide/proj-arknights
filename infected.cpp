#include "infected.h"
#include <ctime>

Infected::Infected()
    : _max_health(0)
{
}

Infected::Infected(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t id, QWidget* parent)
    : QWidget(parent)
    , _health(health)
    , _max_health(health)
    , _damage(damage)
    , _interval(interval)
    , _last_action_time(deployment_time)
    , _place(place)
    , _deployment_time(deployment_time)
    , _is_active(true)
    , _id(id)
{
}

void Infected::reduceHealth(int damage)
{
    _health -= damage;
    if (_health > _max_health) {
        _health = _max_health;
    } else if (_health <= 0) {
        removeFrom();
        _is_active = false;
    }
}

Infected* giveRandomInfected(QVector<Infected*>& infecteds)
{
    srand(static_cast<unsigned>(clock()));
    return infecteds[rand() % infecteds.size()];
}
