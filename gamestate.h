#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "infected.h"
#include "map.h"
#include "reunion.h"
#include <QWidget>

class GameState : public QWidget {
    Q_OBJECT

public:
    GameState(size_t total_hp, QWidget* parent);
    ~GameState();

    void deployOperator();
    void removeOperator();
    void configure();
    void update();
    void strategy();

private:
    Map* _map;
    vector<Operator*> _operators;

    size_t _create_interval;
    size_t _reunion_dp;

    size_t _time;
    size_t _dp; //* dp means Deployment Point
    size_t _hp;

    size_t _speed;
    bool _is_paused;
};

#endif // GAMESTATE_H
