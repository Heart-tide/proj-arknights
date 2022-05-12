#ifndef INFECTED_H
#define INFECTED_H

#include "place.h"
#include <QException>
#include <QPainter>
#include <QString>
#include <QWidget>

class GameState;

class Infected : public QWidget {
    Q_OBJECT

public:
    Infected(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent);
    virtual ~Infected() = default;

    void reduceHealth(int damage);

    virtual void addTo(Place* place) = 0;
    virtual void removeFrom() = 0;

    virtual QString giveName() const { return "Infected"; }
    virtual size_t giveCost() { return 0; }

    size_t giveID() const { return _id; }
    Place* givePlace() { return _place; }
    int giveHealth() { return _health; }
    bool isActive() { return _is_active; }

    void paintEvent(QPaintEvent*) = 0;

protected:
    int _health;
    const int _max_health;
    int _damage;
    size_t _interval; //* 连续两次攻击的间隔tick数
    size_t _last_action_time;

    Place* _place;
    size_t _deployment_time;
    bool _is_active;

    size_t _id;
};

Infected* giveRandomInfected(QVector<Infected*>& infecteds);

//**************** 胜利和失败条件 ****************
class GameOverException : public QException {
public:
    void raise() const override { throw *this; }
    GameOverException* clone() const override { return new GameOverException(*this); }
};

class WinException : public GameOverException {
public:
    void raise() const override { throw *this; }
    WinException* clone() const override { return new WinException(*this); }
};

class LoseException : public GameOverException {
public:
    void raise() const override { throw *this; }
    LoseException* clone() const override { return new LoseException(*this); }
};

#endif // INFECTED_H
