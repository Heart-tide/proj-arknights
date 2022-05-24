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
        QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual size_t giveCost() const = 0;
    QString giveName() const override { return "Operator"; }
    Orientation giveOrientation() const { return _orientation; }
    //* 假设向右部署，干员的矩形攻击范围的 height, width 大小
    virtual QPair<int, int> giveAttackArea() const { return QPair<int, int>(1, 1); }

    virtual bool isGroundToAir() const { return false; }

    void setAttackPlaces(const QVector<Place*>& attack_places) { _attack_places = attack_places; }
    virtual Infected* findAttacked() const;

    void paintEvent(QPaintEvent*) override;

protected:
    Orientation _orientation;
    QVector<Place*> _attack_places;
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
    bool isGroundToAir() const override { return true; }
    QPair<int, int> giveAttackArea() const override { return QPair<int, int>(3, 3); }
};

class Kroos : public Sniper {
    Q_OBJECT

public:
    Kroos(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    size_t giveCost() const override { return cost; }
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
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString giveName() const override { return "Guard"; }
    QPair<int, int> giveAttackArea() const override { return QPair<int, int>(1, 2); }
};

class Irene : public Guard {
    Q_OBJECT

public:
    Irene(LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    size_t giveCost() const override { return cost; }
    QString giveName() const override { return "Irene"; }
    size_t giveBlock() const override { return block; }

    static constexpr size_t cost = 4;
    static constexpr size_t block = 2;
};

//************* 医疗 *************
class Doctor : public Operator {
    Q_OBJECT

public:
    Doctor(size_t health, int damage, size_t interval,
        HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString giveName() const override { return "Doctor"; }
    QPair<int, int> giveAttackArea() const override { return QPair<int, int>(3, 4); }

    Infected* findAttacked() const override;
};

class HoneyBerry : public Doctor {
    Q_OBJECT

public:
    HoneyBerry(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    size_t giveCost() const override { return cost; }
    QString giveName() const override { return "HoneyBerry"; }

    static constexpr size_t cost = 5;
};

#endif // OPERATOR_H
