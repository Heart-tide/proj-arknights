#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "infected.h"
#include "map.h"
#include "reunion.h"
#include <QWidget>

class GameState : public QWidget {
    Q_OBJECT

public:
    GameState(size_t max_hp, QWidget* parent);
    ~GameState();

    void deployOperator();
    void removeOperator();
    void configure();
    void update();
    void strategy();

private:
    Map map;
    vector<Operator*> operators;

    size_t _create_interval;
    size_t _reunion_dp;

    size_t time;
    size_t dp; //* dp means Deployment Point
    size_t hp;

    size_t speed;
    bool is_paused;
};

#endif // GAMESTATE_H
