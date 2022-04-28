#ifndef INFECTED_H
#define INFECTED_H

#include <QWidget>

class Place;

class Infected : public QWidget
{
    Q_OBJECT

public:
    explicit Infected(QWidget *parent = nullptr);
    explicit Infected(Place* place, size_t deployment_time);
    virtual ~Infected();

    void reduceHealth();
    virtual void action() = 0;
    void addTo(Place* place);
    void removeFrom();

    static constexpr size_t cost = 0;

protected:
    size_t health;
    int damage;
    size_t attack_interval; //* 连续两次攻击的间隔tick数
    Place* place;
    size_t deployment_time;
};

#endif // INFECTED_H
