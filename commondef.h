#ifndef __COMMONDEF_H__
#define __COMMONDEF_H__

#include <QException>
#include <QString>
#include <ctime>

//**************** 胜利和失败条件 ****************
class GameOverException : public QException {
    //* QException 不能书写 QObject 标识
public:
    virtual QString what() noexcept = 0;
};

class WinGameException : public GameOverException {
public:
    QString what() noexcept override { return "You Win"; }
};

class LoseGameException : public GameOverException {
public:
    QString what() noexcept override { return "You Lose"; }
};

//* 写个模板，有助于简单比较
template <class T>
inline T max(T x, T y)
{
    return (x > y) ? x : y;
}

template <class T>
inline T min(T x, T y)
{
    return (x < y) ? x : y;
}

//* 给出任意单位，用于整合运动的初始入口确定
template <class T>
T getRandomUnit(QVector<T>& units)
{
    srand(static_cast<unsigned>(clock()));
    return units[rand() % units.size()];
}

#endif // __COMMONDEF_H__