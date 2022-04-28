#include "gamestate.h"
#include <QDebug>
#include <QString>
#include <ctime>

GameState::GameState(size_t create_intervel, size_t total_hp,
    size_t default_speed, QWidget* parent)
    : _map(new Map(3, 4))
    , _timer(new QTimer(this))
    , _create_interval(create_intervel)
    , _reunion_dp(0)
    , _time(0)
    , _dp(0)
    , _hp(total_hp)
    , _speed(1)
    , _is_paused(false)
{
    _map->setParent(this);
    configure();
    connect(_timer, &QTimer::timeout, this, &GameState::update);
    _timer->start(1000 / _speed);
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
    qDebug() << _map->giveWidth() << "*" << _map->giveHeight() << "大小的地图已生成:\n";
    for (size_t i = 0; i < _map->giveHeight(); i++) {
        QString line("\t");
        for (size_t j = 0; j < _map->giveWidth(); j++) {
            if ((*_map)[i][j]->is_base())
                line += "B  ";
            else if ((*_map)[i][j]->is_entrance())
                line += "E  ";
            else
                line += "*  ";
        }
        qDebug() << qPrintable(line) << "\n";
    }
    qDebug() << "游戏开始了哦!\n";
}

void GameState::update()
{
    if (_time == 31) { //* 先跑 30 ticks
        _timer->stop();
        return;
    }
    qDebug() << "[" << _time << "]";
    _time++;
    strategy();
    vector<Reunion*> still_active;
    for (auto it = _active_reunions.begin();
         it < _active_reunions.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->action(_time, _hp);
            still_active.push_back(*it);
        }
    }
    _active_reunions = still_active;
}

void GameState::strategy()
{
    if (_time % _create_interval != 0)
        return;
    static size_t count = 0;
    if (count >= 2) //* 出俩得了!!!
        return;
    count++;
    auto reunion = giveRandomReunion();
    reunion->addTo(_map->_entrance);
    _active_reunions.push_back(reunion);
    qDebug() << "\tCREATE" << qPrintable(reunion->giveName()) << "#" << reunion->giveId();
}

Reunion* GameState::giveRandomReunion()
{
    static size_t id_counter = 0;
    srand(static_cast<unsigned>(clock()));
    switch (rand() % 1) {
    case 0:
        return new TestReunion(_map->_entrance, _time, id_counter++,
            _map->giveRandomRoute(), this);
    }
}
