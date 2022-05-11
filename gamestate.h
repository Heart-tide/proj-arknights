#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "infected.h"
#include "map.h"
#include "operator.h"
#include "reunion.h"
#include <QTimer>
#include <QWidget>

//* GameState 类，管理整个游戏系统
class GameState : public QWidget {
    Q_OBJECT

public:
    friend void deployOperator(GameState* gamestate, Place* place);

    GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_speed, QWidget* parent);
    ~GameState() = default;

    void update();

    void deployOperator(size_t choice, Place* place);
    void operatorAction();
    Infected* properAttackedReunion(Operator* op) const;

    void reunionStragegy();
    void reunionAction();
    Reunion* createRandomReunion();

    //! TODO win && lose

private:
    Map* _map;
    QTimer* _timer;
    QVector<Operator*> _active_operators;
    QVector<Reunion*> _active_reunions;

    size_t _reunion_stats; //* 剩余敌人个数
    size_t _create_interval;
    size_t _reunion_dp;

    size_t _time;
    size_t _dp; //* dp means Deployment Point
    size_t _hp;

    size_t _speed;
    bool _is_paused;
};

void deployOperator(GameState* gamestate, Place* place);

#endif // GAMESTATE_H
