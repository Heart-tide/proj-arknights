#ifndef REUNION_H
#define REUNION_H

#include "infected.h"
#include <QVector>
#include <QWidget>

class Place;

namespace Ui {
class Reunion;
}

class Reunion : public Infected {
    Q_OBJECT

public:
    Reunion(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t move_speed,
        size_t id, QVector<Place*> route, QWidget* parent);
    ~Reunion() = default;

    void action(size_t time, size_t& hp, Infected* op);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual bool is_flying() { return false; }
    QString giveName() const override { return "Reunion"; }

protected:
    int _move_speed; //* 每步移动的距离
    QVector<Place*> _route;
    QPair<int, int> direction;

private:
    Ui::Reunion* ui;
};

class TestReunion : public Reunion {
    Q_OBJECT

public:
    TestReunion(Place* place, size_t deployment_time,
        size_t id, QVector<Place*> route, QWidget* parent);

    QString giveName() const override { return "TestReunion"; }
    void paintEvent(QPaintEvent*) override;
};

#endif // REUNION_H
