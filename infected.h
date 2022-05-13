#ifndef INFECTED_H
#define INFECTED_H

#include "map.h"
#include <QException>
#include <QMovie>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QWidget>

class GameState;

class Infected : public QWidget {
    Q_OBJECT

public:
    Infected(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void reduceHealth(int damage);
    void stopAttacking() { _is_attacking = false; }

    virtual void addTo(Place* place) = 0;
    virtual void removeFrom() = 0;

    virtual QString giveName() const { return "Infected"; }
    virtual size_t giveCost() { return 0; }

    size_t giveID() const { return _id; }
    Place* givePlace() { return _place; }
    int giveHealth() { return _health; }
    bool isActive() { return _is_active; }

    void paintEvent(QPaintEvent*) = 0;
    void timerEvent(QTimerEvent*) override;

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

    QMovie* _idle_movie;
    QMovie* _attack_movie;
    bool _is_attacking;
};

Infected* giveRandomInfected(QVector<Infected*>& infecteds);

//**************** 胜利和失败条件 ****************
class GameOverException : public QException {
public:
    virtual QString what() noexcept = 0;
};

class WinGameException : public GameOverException {
public:
    QString what() noexcept override { return "You Win"; }
};

class LoseGameException : public GameOverException {
public:
    QString what() noexcept override { return "You Lose"; }
};

#endif // INFECTED_H
