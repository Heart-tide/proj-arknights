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
    explicit Operator(QWidget* parent = nullptr);
    explicit Operator(Place* place, size_t deployment_time);
    ~Operator();

    void action() override;
    void deploy();

    static constexpr size_t cost = 12;

protected:
    size_t _block;

private:
    Ui::Operator* ui;
};

#endif // OPERATOR_H
