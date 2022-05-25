#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "operator.h"
#include "reunion.h"
#include <QList>
#include <QTimer>

//* GameState 类，管理整个游戏系统
class GameState : public QWidget {
    Q_OBJECT

public:
    friend void deployOperator(GameState* gamestate, Place* place, int orientation);

    GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_dp, QWidget* parent);
    void connectWithWidgets();

    void update();

    void deployOperator(size_t choice, Place* place, Orientation orientation);
    void operatorAction();
    void findAttackPlaces(Operator* op) const;

    void reunionStragegy();
    Reunion* createRandomReunion();
    void reunionAction();
    Infected* properAttackedOperator(Reunion* reunion) const;

private:
    Map* _map;
    QTimer* _timer;
    QList<Operator*> _active_operators;
    QList<Reunion*> _active_reunions;

    size_t _enemy_stats; //* 剩余敌人个数
    size_t _create_interval;
    size_t _reunion_dp;

    size_t _time;
    size_t _dp; //* dp means Deployment Point
    size_t _hp;
    size_t _total_hp;

    size_t _speed;
    bool _gameover;
};

void deployOperator(GameState* gamestate, Place* place, int orientation);

#endif // GAMESTATE_H
