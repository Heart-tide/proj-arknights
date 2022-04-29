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
    Infected(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent);
    virtual ~Infected() = default;

    void reduceHealth(int damage);

    virtual void addTo(Place* place) = 0;
    virtual void removeFrom() = 0;

    virtual QString giveName() const { return "Infected"; }
    virtual size_t giveCost() { return 0; }

    size_t giveId() const { return _id; }
    Place* givePlace() { return _place; }
    int giveHealth() { return _health; }
    bool isActive() { return _is_active; }

protected:
    size_t _health;
    const size_t _max_health;
    int _damage;
    size_t _interval; //* 连续两次攻击的间隔tick数
    size_t _last_action_time;

    Place* _place;
    size_t _deployment_time;
    bool _is_active;

    size_t _id;
};

Infected* giveRandomInfected(QVector<Infected*>& infecteds);

#endif // INFECTED_H
