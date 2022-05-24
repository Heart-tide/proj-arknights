#include "gamestate.h"
#include "ui_map.h"
#include <QDialog>
#include <QString>
#include <ctime>

using std::function;

GameState::GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_dp, QWidget* parent)
    : QWidget(parent)
    , _map(new Map(this, this))
    , _timer(new QTimer(this))
    , _enemy_stats(reunion_stats)
    , _create_interval(create_intervel)
    , _reunion_dp(0)
    , _time(0)
    , _dp(default_dp)
    , _hp(total_hp)
    , _total_hp(total_hp)
    , _speed(1)
    , _gameover(false)
{
    connectWithWidgets();

    connect(_timer, &QTimer::timeout, this, &GameState::update);
    _timer->start(20 / _speed); //* 20ms 更新一次！

    resize(_map->size()); //* 按载入的 map 大小修改自身大小
}

//* 与 ui 中所有的控件 connect
void GameState::connectWithWidgets()
{
    auto ui = _map->ui;
    connect(ui->push_speed, &QPushButton::clicked, this, [=]() {
        if (_speed == 1) {
            ui->push_speed->setStyleSheet("image: url(:/res/photo/speed_2x.png);");
            printLog("#9999ff", "SPEED", "UP");
        } else {
            ui->push_speed->setStyleSheet("image: url(:/res/photo/speed_1x.png);");
            printLog("#9999ff", "SPEED", "DOWN");
        }
        _speed = 3 - _speed;
        _timer->setInterval(20 / _speed);
    });
    connect(ui->push_pause, &QPushButton::clicked, this, [=]() {
        if (_timer->isActive()) {
            ui->push_pause->setStyleSheet("image: url(:/res/photo/continue.png);");
            printLog("#9999ff", "TIME", "PAUSE");
            _timer->stop();
            //* 停止所有对象的攻击动作
            for (auto it = _active_operators.begin(); it < _active_operators.end(); it++) {
                (*it)->stopAttacking();
            }
            for (auto it = _active_reunions.begin(); it < _active_reunions.end(); it++) {
                (*it)->stopAttacking();
            }
        } else if (!_timer->isActive() && !_gameover) {
            ui->push_pause->setStyleSheet("image: url(:/res/photo/stop.png);");
            printLog("#9999FF", "TIME", "CONTINUE");
            _timer->start();
        }
    });
    //* 将按钮们置顶，避免被地图块所遮盖
    ui->push_speed->raise();
    ui->push_pause->raise();
}

//* update 函数，与 gamestate 计时器 connect
void GameState::update()
{
    _time++;
    _dp += (_time % 50 == 0); //* 每 50 帧增加一费
    try {
        reunionStragegy();
        reunionAction();
        operatorAction();
    } catch (GameOverException& exception) {
        //* 检测游戏结束
        emit _map->ui->push_pause->clicked(); //* 暂停计时器
        _gameover = true;
        printLog("#39C5BB", exception.what(), QString("( HP %1 / %2 )").arg(_hp).arg(_total_hp), nullptr, true);
    }
    _map->ui->label_enemy_stats->setText(QString::number(_enemy_stats)); //* 仅显示未出现的敌人数
    _map->ui->label_hp->setText(QString::number(_hp));
    _map->ui->label_dp->setText(QString::number(_dp));
}

//* 部署一个 Operator 对象
void GameState::deployOperator(size_t choice, Place* place, Orientation orientation)
{
    if (place->giveOperator() != nullptr) //* 一个格子最多只能有一个干员
        return;
    Operator* op = nullptr;
    static size_t id_counter = 0;
    switch (choice) {
    case 0:
        if (Irene::cost <= _dp) {
            op = new Irene(dynamic_cast<LowerPlace*>(place), _time, id_counter++, this, orientation);
        }
        break;
    case 1:
        if (Kroos::cost <= _dp) {
            op = new Kroos(dynamic_cast<HigherPlace*>(place), _time, id_counter++, this, orientation);
        }
        break;
    }
    if (op == nullptr) {
        return;
    }
    // _map->_operatorSelected = -1; //* 永远不清空干员预置栏
    findAttackPlaces(op); //* 设置该干员的攻击范围
    _dp -= op->giveCost();
    _active_operators.push_back(op);
    printLog("#33cccc", "DEPLOY", QString("%1 %2").arg(op->giveName()).arg(op->givePlace()->giveID()));
}

//* 场上所有 active 的 operator 按其生成次序行动
void GameState::operatorAction()
{
    QVector<Operator*> still_active;
    for (auto it = _active_operators.begin(); it < _active_operators.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->action(_time);
            still_active.push_back(*it);
        } else {
            delete (*it);
        }
    }
    _active_operators = still_active;
}

//* 计算干员可攻击到的敌人
void GameState::findAttackPlaces(Operator* op) const
{
    int x = 0, y = 0, height = 0, width = 0;
    switch (op->giveOrientation()) {
    case UP:
        height = op->giveAttackArea().second;
        width = op->giveAttackArea().first;
        x = op->givePlace()->showID().second - width / 2;
        y = op->givePlace()->showID().first - (height - 1);
        break;
    case DOWN:
        height = op->giveAttackArea().second;
        width = op->giveAttackArea().first;
        x = op->givePlace()->showID().second - width / 2;
        y = op->givePlace()->showID().first;
        break;
    case LEFT:
        height = op->giveAttackArea().first;
        width = op->giveAttackArea().second;
        x = op->givePlace()->showID().second - (width - 1);
        y = op->givePlace()->showID().first - height / 2;
        break;
    case RIGHT:
        height = op->giveAttackArea().first;
        width = op->giveAttackArea().second;
        x = op->givePlace()->showID().second;
        y = op->givePlace()->showID().first - height / 2;
        break;
    }
    QVector<Place*> attack_area;
    for (int i = max(x, 0); i < min<int>(x + width, _map->giveWidth()); i++) {
        for (int j = max(y, 0); j < min<int>(y + height, _map->giveHeight()); j++) {
            attack_area.push_back((*_map)[j][i]);
        }
    }
    op->setAttackPlaces(attack_area);
}

//* 生成 Reunion
void GameState::reunionStragegy()
{
    //* 若无任何路径，则进入地图编辑器模式，不出怪
    if ((_map->_routes[false].empty()) && (_map->_routes[true].empty())) {
        return;
    }
    if (_time % _create_interval != 1 || _enemy_stats == 0)
        return;
    _enemy_stats--;
    auto reunion = createRandomReunion();
    _active_reunions.push_back(reunion);
    // printLog(QString("CREATE %1 # %2").arg(reunion->giveName()).arg(reunion->giveID()));
}

//* 随机生成一个 Reunion 的对象，可以给不同类型以不同的生成概率
Reunion* GameState::createRandomReunion()
{
    static size_t id_counter = 0;
    srand(static_cast<unsigned>(clock()));
    size_t choice = rand() % 10;
    //* 若无空中路径，仅生成地面敌人
    if (choice >= 7 && _map->_routes[true].empty()) {
        choice %= 7;
    }
    //* 地面敌人在前，空中敌人在后
    switch (choice) {
    case 0:
    case 1:
    case 2:
        return new Yuan(_time, id_counter++, giveRandomUnit(_map->_routes[false]), this);
    case 3:
    case 4:
        return new Soldier(_time, id_counter++, giveRandomUnit(_map->_routes[false]), this);
    case 5:
    case 6:
        return new Revenger(_time, id_counter++, giveRandomUnit(_map->_routes[false]), this);
    case 7:
    case 8:
    case 9:
        return new Monster(_time, id_counter++, giveRandomUnit(_map->_routes[true]), this);
    default:
        return nullptr;
    }
}

//* 场上所有 active 的 Reunion 按其生成次序行动
void GameState::reunionAction()
{
    QVector<Reunion*> still_active;
    for (auto it = _active_reunions.begin(); it < _active_reunions.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->action(_time, _hp, properAttackedOperator(*it), _map);
            still_active.push_back(*it);
        } else {
            delete (*it);
        }
    }
    _active_reunions = still_active;
    if (still_active.empty() && _enemy_stats == 0) {
        throw WinGameException();
    }
}

//* 计算整合运动可攻击到的干员
Infected* GameState::properAttackedOperator(Reunion* reunion) const
{
    bool is_flying = reunion->isFlying();
    int x = reunion->givePlace()->showID().second - reunion->giveAttackArea();
    int y = reunion->givePlace()->showID().first - reunion->giveAttackArea();
    int a = 2 * reunion->giveAttackArea() + 1;
    for (int i = max(x, 0); i < min<int>(x + a, _map->giveWidth()); i++) {
        for (int j = max(y, 0); j < min<int>(y + a, _map->giveHeight()); j++) {
            //* 暂时不做敌人的远程单位近战单位的区分，仅按攻击范围来看
            if ((*_map)[j][i]->giveOperator()) {
                return (*_map)[j][i]->giveOperator();
            }
        }
    }
    return nullptr;
}

//* 当点击一个格子时调用，若符合条件，则放置一个干员
void deployOperator(GameState* gamestate, Place* place, int orientation)
{
    if (gamestate->_gameover) {
        return;
    }
    int choice = gamestate->_map->_operatorSelected;
    switch (choice) {
    case -1:
        return;
    case 0:
        if (place->isLower()) {
            gamestate->deployOperator(choice, place, static_cast<Orientation>(orientation));
        }
        break;
    case 1:
        if (place->isHigher()) {
            gamestate->deployOperator(choice, place, static_cast<Orientation>(orientation));
        }
        break;
    }
}
