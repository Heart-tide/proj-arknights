#ifndef MAP_H
#define MAP_H

#include "place.h"
#include <QVector>
#include <QWidget>

class GameState;

//* 一张地图，由很多行组成
namespace Ui {
class Map;
}
class Map : public QWidget {
    Q_OBJECT

public:
    friend class GameState;

    Map(size_t height, size_t width, GameState* gamestate, QWidget* parent);
    ~Map();

    QVector<Place*>& operator[](size_t i) { return _lines[i]; }
    const QVector<Place*>& operator[](size_t i) const { return _lines[i]; }

    size_t giveHeight() const { return _lines.size(); }
    size_t giveWidth() const { return _lines.empty() ? 0 : _lines[0].size(); }
    QVector<Place*> giveRandomRoute() const;

    void paintEvent(QPaintEvent*) override;

private:
    void loadMap(size_t height, size_t width);
    void loadRoutes(); //* 只按坐标保存路径，载入时换算成Place*链表

    Ui::Map* ui;

    GameState* _gamestate;
    QVector<QVector<Place*>> _lines;
    Base* _base;
    Entrance* _entrance;
    QVector<QVector<Place*>> _routes;
};

#endif // MAP_H
