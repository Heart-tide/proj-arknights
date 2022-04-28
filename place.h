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

class Place : public QWidget {
    Q_OBJECT

public:
    Place();
    virtual ~Place();

    virtual void addOperator(Operator* op) { _op = op; }
    virtual void addReunion(Reunion* reunion) { _reunions.push_back(reunion); }
    virtual void removeOperator(Operator* op) { _op = nullptr; }
    virtual void removeReunion(Reunion* reunion);

    static constexpr bool is_base = false;
    static constexpr bool is_entrance = false;

protected:
    Operator* _op;
    vector<Reunion*> _reunions;

private:
    Ui::Place* ui;
};

//************* Place有三个子类：不可到达的地块、低地、高地 *************

//* 这里是不可到达地块
class UnreachablePlace : public Place {
public:
    UnreachablePlace() = default;
    ~UnreachablePlace() = default;

    void addOperator(Operator* op) override;
    void addReunion(Reunion* reunion) override;
    void removeOperator(Operator* op) override;
    void removeReunion(Reunion* reunion) override;
};

class Base : public UnreachablePlace {
    Q_OBJECT

public:
    Base() = default;
    ~Base() = default;

    void addReunion(Reunion* reunion) override { }

    static constexpr bool is_base = true;
};

class Entrance : public UnreachablePlace {
    Q_OBJECT

public:
    Entrance() = default;
    ~Entrance() = default;

    void addReunion(Reunion* reunion) override;

    static constexpr bool is_entrance = true;

private:
};

#endif // PLACE_H
