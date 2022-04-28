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
    Operator(Place* place, size_t deployment_time);
    ~Operator();

    void action(size_t time, size_t& hp) override;
    void deploy();

    static constexpr size_t cost = 12;
    QString giveName() const override { return "Operator"; }

protected:
    size_t _block;

private:
    Ui::Operator* ui;
};

#endif // OPERATOR_H
