#include "infected.h"

Infected::Infected(size_t health, int damage, int defense, size_t interval,
    Place* place, size_t deployment_time, size_t id, QWidget* parent,
    QMovie* idle_movie, QMovie* attack_movie)
    : QWidget(parent)
    , _health(health)
    , _max_health(health)
    , _damage(damage)
    , _defense(defense)
    , _interval(interval)
    , _last_action_time(deployment_time)
    , _place(place)
    , _deployment_time(deployment_time)
    , _is_active(true)
    , _id(id)
    , _idle_movie(idle_movie)
    , _attack_movie(attack_movie)
    , _is_attacking(false)
    , _is_attacked_count(0)
{
    //* 移动时的 gif 抽 1/3 帧，攻击时的 gif 1.5 倍速播放
    //* 一切都为了更少的卡顿
    _attack_movie->setSpeed(150);
    startTimer(20);
}

Infected::~Infected()
{
    delete _idle_movie;
    delete _attack_movie;
}

void Infected::reduceHealth(int damage)
{
    int true_damage = max<double>(0.1 * damage, damage - _defense); //* 实际受到的伤害
    _health -= true_damage;
    if (_health <= 0) {
        _is_active = false;
        hide();
    }
    _is_attacked_count = 10; //* 持续 200ms
}

void Infected::increaseHealth(int damage)
{
    _health += damage;
    if (_health > _max_health) {
        _health = _max_health;
    }
}

void Infected::timerEvent(QTimerEvent*)
{
    update();
}

//* 红色图片捏
void Infected::convertToRedImage(QImage& img, int degree)
{
    //* 四通道 8bit-8bit-8bit-8bit 的 rgba 色彩模式，前三位分别表示 rgb，最后一位表示透明度
    //* 8bit 为每位的值域，每个像素点的四位在色彩空间数组中临近存储
    img = QImage(img).convertToFormat(QImage::Format_RGBA8888);
    uint8_t* rgb = img.bits();
    for (int i = 0; i < img.width() * img.height(); i++) {
        int new_red_degree = rgb[i * 4] + (100 - 20 * abs(degree - 5)); //* 线性地增加至 100 个点，然后线性地衰退
        rgb[i * 4] = new_red_degree > 255 ? 255 : new_red_degree;
    }
}