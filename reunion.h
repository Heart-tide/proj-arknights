#ifndef REUNION_H
#define REUNION_H

#include "infected.h"
#include <QWidget>
#include <vector>
using std::vector;

class Place;

namespace Ui {
class Reunion;
}

class Reunion : public Infected {
    Q_OBJECT

public:
    Reunion(size_t health, int damage, size_t attack_interval,
        Place* place, size_t deployment_time, size_t move_speed,
        size_t id, vector<Place*> route, QWidget* parent);
    ~Reunion() = default;

    void action(size_t time, size_t& hp) override;
    void addTo(Place* place) override;
    void removeFrom() override;

    //* reunion也有cost，用于随机生成的出怪策略
    virtual bool is_flying() { return false; }
    QString giveName() const override { return "Reunion"; }

protected:
    bool _is_blocked;
    size_t _move_speed; //* 每步移动需要的tick数
    vector<Place*> _route;

private:
    Ui::Reunion* ui;
};

class TestReunion : public Reunion {
    Q_OBJECT

public:
    TestReunion(Place* place, size_t deployment_time,
        size_t id, vector<Place*> route, QWidget* parent);

    QString giveName() const override { return "TestReunion"; }
};

#endif // REUNION_H
