#ifndef INFECTED_H
#define INFECTED_H

#include "place.h"
#include <QString>
#include <QWidget>

class GameState;

class Infected : public QWidget {
    Q_OBJECT

public:
    Infected();
    Infected(size_t health, int damage, size_t attack_interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent);
    virtual ~Infected() = default;

    void reduceHealth(int damage);
    virtual void action(size_t time, size_t& hp) = 0;
    virtual void addTo(Place* place) = 0;
    virtual void removeFrom() = 0;

    virtual size_t cost() { return 0; }
    virtual QString giveName() const { return "Infected"; }
    bool isActive() { return _is_active; }
    size_t giveId() const { return _id; }

protected:
    size_t _health;
    const size_t _max_health;
    int _damage;
    size_t _attack_interval; //* 连续两次攻击的间隔tick数
    size_t _last_attack_time;

    Place* _place;
    size_t _deployment_time;
    bool _is_active;

    size_t _id;
};

#endif // INFECTED_H
