#ifndef OPERATOR_H
#define OPERATOR_H

#include "infected.h"
#include <QWidget>

class Operator : public Infected {
    Q_OBJECT

public:
    Operator(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id,
        QWidget* parent, size_t block);
    ~Operator() = default;

    void action(size_t time, QVector<Infected*>& reunions);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual size_t giveCost() = 0;
    QString giveName() const override { return "Operator"; }

protected:
    size_t _block;
};

//************* 狙击手 *************
class Sniper : public Operator {
    Q_OBJECT

public:
    Sniper(size_t health, int damage, size_t interval,
        HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent);

    void paintEvent(QPaintEvent*) override;
};

class Kroos : public Sniper {
    Q_OBJECT

public:
    Kroos(HigherPlace* higher_place, size_t deployment_time,
        size_t id, QWidget* parent);

    size_t giveCost() override { return 0; }
    QString giveName() const override { return "Kroos"; }
    void paintEvent(QPaintEvent*) override;
};

//************* 近卫 *************
class Guard : public Operator {
    Q_OBJECT

public:
    Guard(size_t health, int damage, size_t interval,
        LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent, size_t block);

    void paintEvent(QPaintEvent*) override;
};

class Irene : public Guard {
    Q_OBJECT

public:
    Irene(LowerPlace* lower_place, size_t deployment_time,
        size_t id, QWidget* parent);

    size_t giveCost() override { return 0; }
    QString giveName() const override { return "Irene"; }
    void paintEvent(QPaintEvent*) override;
};

#endif // OPERATOR_H
