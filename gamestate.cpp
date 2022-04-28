#include "gamestate.h"
#include <QDebug>
#include <iostream>
using std::cout;
using std::endl;

GameState::GameState(size_t total_hp, QWidget* parent)
    : _map(new Map(5, 7))
{
    _map->setParent(this);
}

GameState::~GameState()
{
}

void GameState::deployOperator()
{
}

void GameState::removeOperator()
{
}

void GameState::configure()
{
    _map->loadMap();
    _map->loadRoutes();
    qDebug() << "5*7大小的地图已生成, 下面为您打印:";
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 7; j++) {
            cout << "  "
                 << ((*_map)[i][j]->is_base() ? 'B' : ((*_map)[i][j]->is_entrance() ? 'E' : '*'));
        }
        cout << "\n";
    }
}

void GameState::update()
{
}

void GameState::strategy()
{
}
