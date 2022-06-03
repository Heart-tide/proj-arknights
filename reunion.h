#ifndef REUNION_H
#define REUNION_H

#include "infected.h"
#include <QVector2D>

class Reunion : public Infected {
    Q_OBJECT

public:
    Reunion(size_t health, int damage, int defense, size_t interval,
        size_t deployment_time, float move_speed,
        size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    virtual void action(size_t time, size_t& hp, Infected* op, Map* map) = 0;
    void move(size_t& hp, Map* map);
    virtual void attack(Infected* op) override;

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual bool isFlying() { return false; }
    QString getName() const override { return "Reunion"; }
    //* 整合运动的攻击范围是一个正方形，其边长为该函数返回值的 2 倍加 1
    virtual int getAttackArea() const = 0;

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

protected:
    Place* adjacentPlace(Map* map);
    void chooseDirection();

    int _move_speed; //* 每帧移动的距离倍数
    QVector<Place*> _route;
    QPoint _direction[2]; //* 存储每帧移动长度的整数部分，0 号位存储值本身，1 号位存储单位化值
    QVector2D _remaining_direction[2]; //* 存储每帧移动长度的小数部分，0 号位累计存储，1 号位存储单位剩余长度
};

//************* 地面单位 *************
class GroundReunion : public Reunion {
    Q_OBJECT

public:
    GroundReunion(size_t health, int damage, int defense, size_t interval,
        size_t deployment_time, float move_speed,
        size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time, size_t& hp, Infected* op, Map* map) override;
    QString getName() const override { return "GroundReunion"; }

    void paintEvent(QPaintEvent*) override;
};

class Yuan : public GroundReunion {
    Q_OBJECT

public:
    Yuan(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent);

    QString getName() const override { return "Yuan"; }
    int getAttackArea() const override { return 0; }
    QString getSkill() const override { return "原石虫，很可爱！"; }
};

class Soldier : public GroundReunion {
    Q_OBJECT

public:
    Soldier(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent);

    QString getName() const override { return "Soldier"; }
    int getAttackArea() const override { return 1; }
    QString getSkill() const override { return "可以进行远程攻击，恋战"; }
};

class Revenger : public GroundReunion {
    Q_OBJECT

public:
    Revenger(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent);

    QString getName() const override { return "Revenger"; }
    int getAttackArea() const override { return 0; }
    QString getSkill() const override { return "血量低于50%时攻击力加倍"; }

    void attack(Infected* op) override;

private:
    bool _low_health;
};

class Defender : public GroundReunion {
    Q_OBJECT

public:
    Defender(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent);

    QString getName() const override { return "Defender"; }
    int getAttackArea() const override { return 0; }
    QString getSkill() const override { return "不进行攻击，极端恋战"; }

    void action(size_t, size_t& hp, Infected* op, Map* map) override;
};

//************* 无人机 *************
class UAV : public Reunion {
    Q_OBJECT

public:
    UAV(size_t health, int damage, int defense, size_t interval, size_t deployment_time,
        float move_speed, size_t id, QVector<Place*> route, QWidget* parent,
        QMovie* idle_movie, QMovie* attack_movie);

    void action(size_t time, size_t& hp, Infected* op, Map* map) override;
    bool isFlying() override { return true; }
    QString getName() const override { return "UAV"; }

    void paintEvent(QPaintEvent*) override;
};

class Monster : public UAV {
    Q_OBJECT

public:
    Monster(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent);

    QString getName() const override { return "Monster"; }
    int getAttackArea() const override { return 1; }
    QString getSkill() const override { return "飞行单位，畏战"; }
};

#endif // REUNION_H
