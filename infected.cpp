#include "infected.h"
#include <ctime>

Infected::Infected(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t id, QWidget* parent,
    QMovie* idle_movie, QMovie* attack_movie)
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
    , _idle_movie(idle_movie)
    , _attack_movie(attack_movie)
    , _is_attacking(false)
{
    //* 移动时的 gif 抽 1/3 帧，攻击时的 gif 1.5 倍速播放
    //* 一切都为了更少的卡顿
    _attack_movie->setSpeed(150);
    startTimer(20);
}

void Infected::reduceHealth(int damage)
{
    _health -= damage;
    if (_health > _max_health) {
        _health = _max_health;
    } else if (_health <= 0) {
        removeFrom();
        _is_active = false;
        //* 若死亡，直接释放资源，减少卡顿
        delete _idle_movie;
        delete _attack_movie;
    }
}

void Infected::timerEvent(QTimerEvent*)
{
    update();
}