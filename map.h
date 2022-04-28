#ifndef MAP_H
#define MAP_H

#include "place.h"
#include <QWidget>
#include <vector>
using std::vector;

//* 一行，由很多格子组成
class Line : public QWidget
{
    Q_OBJECT

public:
    explicit Line(QWidget *parent = nullptr);
    Line(size_t width);
    Line(vector<Place*> places);
    ~Line();

    Place& operator[](size_t i);
    const Place& operator[](size_t i) const;

    Line& addPlace(Place* place);

private:
    vector<Place*> places;
    size_t width;
};

//* 一张地图，由很多行组成
namespace Ui {
class Map;
}

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = nullptr);
    Map(size_t height, size_t width);
    Map(vector<Line*> lines);
    ~Map();

    Line& operator[](size_t i);
    const Line& operator[](size_t i) const;

    Map& addLine(Line* line);
    void loadMap();
    void loadRoutes(); //* 只按坐标保存路径，载入时换算成Place*链表，并存入entrance中

private:
    Ui::Map *ui;

    vector<Line*> lines;
    size_t height;
    size_t width;

    Base* base;
    Entrance* entrance;
};

#endif // MAP_H
