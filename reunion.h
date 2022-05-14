#ifndef REUNION_H
#define REUNION_H

#include "infected.h"
#include <QVector>
#include <QWidget>

class Place;

class Reunion : public Infected {
    Q_OBJECT

public:
    Reunion(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t move_speed,
        size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    virtual void action(size_t time, size_t& hp, Infected* op) = 0;
    void move(size_t& hp);
    void attack(Infected* op);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual bool isFlying() { return false; }
    QString giveName() const override { return "Reunion"; }
    //* 整合运动的攻击范围是一个正方形，其边长为该函数返回值的 2 倍加 1
    virtual int giveAttackArea() const = 0;

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

protected:
    int _move_speed; //* 每步移动的距离
    QVector<Place*> _route;
    QPair<int, int> direction;
};

//************* 地面单位 *************
class GroundReunion : public Reunion {
    Q_OBJECT

public:
    GroundReunion(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t move_speed,
        size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time, size_t& hp, Infected* op) override;
    QString giveName() const override { return "GroundReunion"; }

    void paintEvent(QPaintEvent*) override;
};

class Yuan : public GroundReunion {
    Q_OBJECT

public:
    Yuan(Place* place, size_t deployment_time,
        size_t id, QVector<Place*> route, QWidget* parent);

    QString giveName() const override { return "Yuan"; }
    int giveAttackArea() const override { return 0; }
};

//************* 无人机 *************
class UAV : public Reunion {
    Q_OBJECT

public:
    UAV(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t move_speed,
        size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time, size_t& hp, Infected* op) override;
    bool isFlying() override { return true; }
    QString giveName() const override { return "UAV"; }

    void paintEvent(QPaintEvent*) override;
};

class Monster : public UAV {
    Q_OBJECT

public:
    Monster(Place* place, size_t deployment_time,
        size_t id, QVector<Place*> route, QWidget* parent);

    QString giveName() const override { return "Monster"; }
    int giveAttackArea() const override { return 1; }
};

#endif // REUNION_H
