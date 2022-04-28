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
    explicit Reunion(QWidget* parent = nullptr);
    explicit Reunion(Place* place, size_t deployment_time);
    ~Reunion();

    void action() override;

    static constexpr size_t cost = 3; //* reunion也有cost，用于随机生成的出怪策略
    static constexpr bool is_flying = false;

protected:
    bool _is_blocked;
    size_t _move_speed; //* 每tick移动的长度
    vector<Place*> _route;

private:
    Ui::Reunion* ui;
};

#endif // REUNION_H
