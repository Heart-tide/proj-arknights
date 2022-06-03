#ifndef INFECTED_H
#define INFECTED_H

#include "map.h"
#include <QMovie>
#include <QPainter>

class GameState;

class Infected : public QWidget {
    Q_OBJECT

public:
    Infected(size_t health, int damage, int defense, size_t interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void reduceHealth(int damage);
    void increaseHealth(int damage);
    virtual void attack(Infected*) = 0;
    void stopAttacking() { _is_attacking = false; }

    virtual void addTo(Place* place) = 0;
    virtual void removeFrom() = 0;

    virtual QString getName() const { return "Infected"; }
    virtual size_t getCost() { return 0; }

    size_t getID() const { return _id; }
    Place* getPlace() const { return _place; }
    bool isHealthReduced() const { return _health < _max_health; }
    virtual int getBlock() const { return 0; }
    int getDefense() const { return _defense; }
    bool isActive() const { return _is_active; }

    void paintEvent(QPaintEvent*) = 0;
    void timerEvent(QTimerEvent*) override;

protected:
    void convertToRedImage(QImage& img, int degree);

    int _health;
    const int _max_health;
    int _damage;
    int _defense;

    size_t _interval; //* 连续两次攻击的间隔tick数
    size_t _last_action_time;

    Place* _place;
    size_t _deployment_time;
    bool _is_active;

    size_t _id;

    QMovie* _idle_movie;
    QMovie* _attack_movie;
    bool _is_attacking;
    int _is_attacked_count; //* 受攻击状态（变红）帧数的倒数
};

#endif // INFECTED_H
