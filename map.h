#ifndef MAP_H
#define MAP_H

#include "place.h"
#include <QTextStream>
#include <QVector>
#include <QWidget>

class GameState;

//* 一张地图，由很多行组成
QT_BEGIN_NAMESPACE namespace Ui
{
    class Map;
}
QT_END_NAMESPACE

class Map : public QWidget {
    Q_OBJECT

public:
    friend class GameState;
    friend void deployOperator(GameState* gamestate, Place* place, int orientation);
    friend void printLog(const QString& color, const QString& type, const QString& info, Map* map_init, bool gameover);

    Map(GameState* gamestate, QWidget* parent);
    void connectOperators();
    void arrangeUiWidgets();
    ~Map();

    QVector<Place*>& operator[](size_t i) { return _lines[i]; }
    const QVector<Place*>& operator[](size_t i) const { return _lines[i]; }

    size_t giveHeight() const { return _lines.size(); }
    size_t giveWidth() const { return _lines.empty() ? 0 : _lines[0].size(); }

    void paintEvent(QPaintEvent*) override;

private:
    void loadMap();
    void loadRoutes(QTextStream& in);

    Ui::Map* ui;

    GameState* _gamestate;
    QVector<QVector<Place*>> _lines;
    QVector<Base*> _base;
    QVector<Entrance*> _entrance;
    //* 0 号位存储地面路径，1 号位存储空中路径，与 bool is_air_route 配合使用
    QVector<QVector<Place*>> _routes[2];
    int _operatorSelected;
};

void printLog(const QString& color, const QString& type, const QString& info, Map* map_init = nullptr, bool gameover = false);

#endif // MAP_H
