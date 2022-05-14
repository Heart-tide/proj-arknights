#ifndef OPERATOR_H
#define OPERATOR_H

#include "infected.h"
#include <QPair>
#include <QWidget>

enum Orientation { UP,
    DOWN,
    LEFT,
    RIGHT };

class Operator : public Infected {
    Q_OBJECT

public:
    Operator(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id,
        QWidget* parent, size_t block, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time, Infected* attacked);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual size_t giveCost() = 0;
    QString giveName() const override { return "Operator"; }
    Orientation giveOrientation() const { return _orientation; }
    virtual bool isGroundToAir() { return false; }
    //* 假设向右部署，干员的矩形攻击范围的 height, width 大小
    virtual QPair<int, int> giveAttackArea() const { return QPair<int, int>(1, 1); }

    void paintEvent(QPaintEvent*) override;

protected:
    size_t _block;
    Orientation _orientation;
};

//************* 狙击手 *************
class Sniper : public Operator {
    Q_OBJECT

public:
    Sniper(size_t health, int damage, size_t interval,
        HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString giveName() const override { return "Sniper"; }
    bool isGroundToAir() override { return true; }
    QPair<int, int> giveAttackArea() const override { return QPair<int, int>(3, 3); }
};

class Kroos : public Sniper {
    Q_OBJECT

public:
    Kroos(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    size_t giveCost() override { return cost; }
    QString giveName() const override { return "Kroos"; }

    //* 将 cost 设计成静态成员，以便在不创建对象的前提下，直接用类名进行引用
    static constexpr size_t cost = 5;
};

//************* 近卫 *************
class Guard : public Operator {
    Q_OBJECT

public:
    Guard(size_t health, int damage, size_t interval,
        LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, size_t block, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString giveName() const override { return "Guard"; }
    QPair<int, int> giveAttackArea() const override { return QPair<int, int>(1, 2); }
};

class Irene : public Guard {
    Q_OBJECT

public:
    Irene(LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    size_t giveCost() override { return cost; }
    QString giveName() const override { return "Irene"; }

    static constexpr size_t cost = 3;
};

#endif // OPERATOR_H
