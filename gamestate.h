#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "map.h"
#include "infected.h"
#include <QWidget>

class GameState : public QWidget
{
    Q_OBJECT

public:
    GameState(size_t max_hp, QWidget *parent);
    ~GameState();

    void deployOperator();
    void removeOperator();
    void configure();
    void update();

private:
    Map map;
    vector<Operator*> operators;

    size_t time;
    size_t dp; //* dp means Deployment Point
    size_t hp;

    size_t speed;
    bool is_paused;
};

#endif // GAMESTATE_H
