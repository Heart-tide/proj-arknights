#ifndef OPERATOR_H
#define OPERATOR_H

#include "infected.h"

enum Orientation { UP,
    DOWN,
    LEFT,
    RIGHT };

class Operator : public Infected {
    Q_OBJECT

public:
    Operator(size_t health, int damage, int defense, size_t interval,
        Place* place, size_t deployment_time, size_t id,
        QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    virtual void action(size_t time);
    void attack(Infected* attacked) override;

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual size_t getCost() const = 0;
    QString getName() const override { return "Operator"; }
    Orientation getOrientation() const { return _orientation; }
    //* 假设向右部署，干员的矩形攻击范围的 height, width 大小
    virtual QPair<int, int> getAttackArea() const { return QPair<int, int>(1, 1); }

    virtual bool isGroundToAir() const { return false; }

    void setAttackPlaces(const QVector<Place*>& attack_places) { _attack_places = attack_places; }
    virtual Infected* findAttacked() const;

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

protected:
    Orientation _orientation;
    QVector<Place*> _attack_places;
    GameState* _gamestate; //* 让每个组件都保存一份 gamestate 的指针，确实是明智的选择
    bool _is_skill_used;
};

//************* 狙击手 *************
class Sniper : public Operator {
    Q_OBJECT

public:
    Sniper(size_t health, int damage, int defense, size_t interval,
        HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString getName() const override { return "Sniper"; }
    bool isGroundToAir() const override { return true; }
    QPair<int, int> getAttackArea() const override { return QPair<int, int>(3, 4); }
};

class Kroos : public Sniper {
    Q_OBJECT

public:
    Kroos(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    void attack(Infected* attacked) override;

    size_t getCost() const override { return cost; }
    QString getName() const override { return "Kroos"; }
    QString getSkill() const override { return "每次攻击有 1/3 的概率连击"; }

    //* 将 cost 设计成静态成员，以便在不创建对象的前提下，直接用类名进行引用
    static constexpr size_t cost = 14;
};

//************* 近卫 *************
class Guard : public Operator {
    Q_OBJECT

public:
    Guard(size_t health, int damage, int defense, size_t interval,
        LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString getName() const override { return "Guard"; }
    QPair<int, int> getAttackArea() const override { return QPair<int, int>(1, 2); }
};

class Irene : public Guard {
    Q_OBJECT

public:
    Irene(LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    void attack(Infected* attacked) override;

    size_t getCost() const override { return cost; }
    QString getName() const override { return "Irene"; }
    int getBlock() const override { return 2; }
    QString getSkill() const override { return "连击，50%概率无视50%防御"; }

    static constexpr size_t cost = 23;
};

class Skadi : public Guard {
    Q_OBJECT

public:
    Skadi(LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    void action(size_t time) override;

    size_t getCost() const override { return cost; }
    QString getName() const override { return "Skadi"; }
    int getBlock() const override { return 0; }
    QPair<int, int> getAttackArea() const override { return QPair<int, int>(2, 2); }
    QString getSkill() const override { return "部署后20s内攻击力增加100%"; }

    static constexpr size_t cost = 18;

private:
    bool _attack_doubled;
};

//************* 医疗 *************
class Doctor : public Operator {
    Q_OBJECT

public:
    Doctor(size_t health, int damage, int defense, size_t interval,
        HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation,
        QMovie* idle_movie, QMovie* attack_movie);

    QString getName() const override { return "Doctor"; }
    QPair<int, int> getAttackArea() const override { return QPair<int, int>(3, 4); }

    Infected* findAttacked() const override;
    void attack(Infected* attacked) override;
};

class HoneyBerry : public Doctor {
    Q_OBJECT

public:
    HoneyBerry(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent, Orientation orientation);

    void action(size_t time) override;

    size_t getCost() const override { return cost; }
    QString getName() const override { return "HoneyBerry"; }
    QPair<int, int> getAttackArea() const override { return QPair<int, int>(5, 4); }
    QString getSkill() const override { return "有 20% 的概率攻击目标 +1"; }

    static constexpr size_t cost = 15;
};

#endif // OPERATOR_H
