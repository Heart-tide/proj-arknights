#ifndef OPERATOR_H
#define OPERATOR_H

#include "infected.h"
#include <QWidget>

namespace Ui {
class Operator;
}

class Operator : public Infected {
    Q_OBJECT

public:
    Operator(size_t health, int damage, size_t interval,
        Place* place, size_t deployment_time, size_t id, QWidget* parent,
        size_t block);
    ~Operator() = default;

    void action(size_t time, QVector<Infected*>& reunions);

    void addTo(Place* place) override;
    void removeFrom() override;

    virtual size_t giveCost() = 0;
    QString giveName() const override { return "Operator"; }

protected:
    size_t _block;

private:
    Ui::Operator* ui;
};

class TestOperator : public Operator {
    Q_OBJECT

public:
    TestOperator(Place* place, size_t deployment_time,
        size_t id, QWidget* parent);

    size_t giveCost() override { return 0; }
    QString giveName() const override { return "TestOperator"; }
};

#endif // OPERATOR_H
