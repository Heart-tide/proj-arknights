#ifndef PLACE_H
#define PLACE_H

#include <QWidget>
#include <vector>
using std::vector;

class Infected;
class Operator;
class Reunion;

//* 一个格子，很普通
namespace Ui {
class Place;
}

class Place : public QWidget
{
    Q_OBJECT

public:
    explicit Place(QWidget *parent = nullptr);
    virtual ~Place();

    virtual void addOperator(Operator* op);
    virtual void addReunion(Reunion* reunion);
    virtual void removeOperator(Operator* op);
    virtual void removeReunion(Reunion* reunion);

    static constexpr bool is_base = false;
    static constexpr bool is_entrance = false;

protected:
    Operator* op;
    vector<Reunion*> reunions;

private:
    Ui::Place *ui;
};

//************* Place有三个子类：不可到达的地块、低地、高地 *************

//* 这里是不可到达地块
class UnreachablePlace: public Place
{
public:
    explicit UnreachablePlace(QWidget *parent = nullptr);
    ~UnreachablePlace();

    void addOperator(Operator* op) override;
    void addReunion(Reunion* reunion) override;
    void removeOperator(Operator* op) override;
    void removeReunion(Reunion* reunion) override;
};

class Base: public UnreachablePlace
{
    Q_OBJECT

public:
    explicit Base(QWidget *parent = nullptr);
    ~Base();

    void addReunion(Reunion* reunion) override;

    static constexpr bool is_base = true;
};

class Entrance: public UnreachablePlace
{
    Q_OBJECT

public:
    explicit Entrance(QWidget *parent = nullptr);
    ~Entrance();

    void strategy(); //* 每次出怪时，按费用随机生成一个，随机选取行进路线

    static constexpr bool is_entrance = true;

private:
    vector<vector<Place*>> routes;
    size_t create_interval;
    size_t reunion_dp;
};

#endif // PLACE_H
