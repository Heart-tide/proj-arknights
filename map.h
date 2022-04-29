#ifndef MAP_H
#define MAP_H

#include "place.h"
#include <QVector>
#include <QWidget>

class GameState;

//* 一行，由很多格子组成
class Line : public QWidget {
    Q_OBJECT

public:
    Line() = default;
    explicit Line(size_t size);
    explicit Line(QVector<Place*> places);
    ~Line() = default;

    Place*& operator[](size_t i) { return _places[i]; }
    const Place* operator[](size_t i) const { return _places[i]; }

    void addPlace(Place* place);
    size_t giveSize() const { return _places.size(); }

private:
    QVector<Place*> _places;
};

//* 一张地图，由很多行组成
namespace Ui {
class Map;
}

class Map : public QWidget {
    Q_OBJECT

public:
    friend class GameState;

    Map();
    Map(size_t height, size_t width);
    Map(QVector<Line*> lines);
    ~Map();

    Line& operator[](size_t i) { return *_lines[i]; }
    const Line& operator[](size_t i) const { return *_lines[i]; }

    void addLine(Line* line);
    void loadMap();
    void loadRoutes(); //* 只按坐标保存路径，载入时换算成Place*链表

    size_t giveHeight() const { return _lines.size(); }
    size_t giveWidth() const { return _lines.empty() ? 0 : _lines[0]->giveSize(); }
    QVector<Place*> giveRandomRoute() const;

private:
    Ui::Map* ui;

    QVector<Line*> _lines;
    Base* _base;
    Entrance* _entrance;
    QVector<QVector<Place*>> _routes;
};

#endif // MAP_H
