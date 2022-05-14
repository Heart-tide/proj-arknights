#include "map.h"
#include "ui_map.h"
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QTextStream>
#include <ctime>

Map::Map(GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Map)
    , _gamestate(gamestate)
    , _operatorSelected(-1)
{
    ui->setupUi(this);
    connectOperators();
    loadMap();
    loadRoutes(false); //* 生成地面路径
    loadRoutes(true); //* 生成空中路径
    printLog("000000", "", QString("地图生成成功, 大小 %1 * %2").arg(giveHeight()).arg(giveWidth()));
    printLog("000000", "", "游戏开始了哦!");
}

Map::~Map()
{
    delete ui;
}

void Map::connectOperators()
{
    connect(ui->photo_irene, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 0;
        printLog("cc00cc", "CHOOSE", "Irene");
    });
    connect(ui->photo_kroos, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 1;
        printLog("cc00cc", "CHOOSE", "Kroos");
    });
}

//* 读取在 map 文件夹下的所有地图文件
void Map::loadMap()
{
    //* currentPath 指出 exe 文件所在目录，QStringList("map-*.txt")过滤出所有地图文件
    QList<QFileInfo> fileInfo(QDir(QDir::currentPath()).entryInfoList(QStringList("map-*.txt")));
    //* 从已有地图中，随机选取一张地图执行
    srand(static_cast<unsigned>(clock()));
    size_t map_choice = rand() % (fileInfo.size() - 2); //* 去掉上级目录和当前目录两项，其余的即地图数目
    QFile file(QDir::currentPath() + QString("/map/map-%1.txt").arg(map_choice));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    printLog("000000", "", QString("地图文件 %1 读取成功!").arg(map_choice), this);
    QTextStream in(&file);
    size_t height = 0;
    size_t width = 0;
    in >> height >> width;

    for (size_t i = 0; i < height; i++) {
        QVector<Place*> line;
        for (size_t j = 0; j < width; j++) {
            //* 读取一个格子
            Place* place = nullptr;
            int place_type = 0;
            in >> place_type;
            switch (place_type) {
            case 0:
                _base = new Base(_gamestate, this);
                place = _base;
                break; //? 忘记加 break，在此 debug 半个小时
            case 1:
                _entrance = new Entrance(_gamestate, this);
                place = _entrance;
                break;
            case 2:
                place = new LowerPlace(_gamestate, this);
                break;
            case 3:
                place = new HigherPlace(_gamestate, this);
                break;
            case 4:
                place = new SpecialPlace(_gamestate, this);
                break;
            }
            line.push_back(place);
            place->setGeometry(j * 100, i * 100 + 5, 100, 100);
            place->_id = QPair<size_t, size_t>(i, j);
        }
        _lines.push_back(line);
    }
}

void Map::loadRoutes(bool is_air_routes)
{
    //* 右上 --> 左下，随机生成 10 条路径
    //* 存储时由终点向起点

    //* 试一下 lambda 表达式作为过程抽象的使用，它们自动内联
    //* 编写具有通用性的程序真是爽快极了！
    auto properPlaceToReach = [=](Place* place) -> bool {
        return is_air_routes || place->isReachable();
    };
    int width = giveWidth();
    int height = giveHeight();
    srand(static_cast<unsigned>(clock()));
    for (int p = 0; p < 10; p++) {
        QVector<Place*> route;
        int i = height - 1, j = 0;
        route.push_back((*this)[i][j]);
        for (int q = 0; q < height + width - 1; q++) {
            if (i == 0 || !properPlaceToReach((*this)[i - 1][j])) {
                route.push_back((*this)[i][j++]);
                continue;
            }
            if (j == width - 1 || !properPlaceToReach((*this)[i][j + 1])) {
                route.push_back((*this)[i--][j]);
                continue;
            }
            if (rand() % 2 == 0) {
                route.push_back((*this)[i--][j]);
            } else {
                route.push_back((*this)[i][j++]);
            }
        }
        _routes[is_air_routes].push_back(route);
    }
}

void printLog(const QString& color, const QString& type, const QString& info, Map* map_init)
{
    //* 仅在第一次调用时传参 map_init，形成闭包
    static Map* map = map_init;
    map->ui->text_log->appendHtml(QString(R"(<html><head/><body><p>
                <span style=" font-weight:600;color:#%1;">
                %2 </span>%3</p></body></html>)")
                                      .arg(color)
                                      .arg(type)
                                      .arg(info));
}

//* 画出地图上的所有格子
void Map::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //? 修改完资源文件后勿忘及时在 Qt 中保存
    QPixmap common_place("://res/place/common.png");
    QPixmap unreachable_place("://res/place/unreachable-1.png");
    QPixmap higher_place("://res/place/higher-1.png");
    QPixmap lower_place("://res/place/lower-1.png");
    QPixmap base("://res/place/base.png");
    QPixmap entrance("://res/place/entrance.png");
    for (size_t i = 0; i < giveHeight(); i++) {
        for (size_t j = 0; j < giveWidth(); j++) {
            Place* place = (*this)[i][j];
            if (place->isBase()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), common_place);
                painter.drawPixmap(place->x() - 5, place->y() - 5, place->height() + 10, place->width() + 10, base);
            } else if (place->isEntrance()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), common_place);
                painter.drawPixmap(place->x() - 5, place->y() - 5, place->height() + 10, place->width() + 10, entrance);
            } else if (place->isLower()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), lower_place);
            } else if (place->isHigher()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), higher_place);
            } else {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), unreachable_place);
            }
        }
    }
}

QVector<Place*> Map::giveRandomRoute(bool is_air_route) const
{
    srand(static_cast<unsigned>(clock()));
    auto i = rand() % _routes[is_air_route].size();
    return _routes[is_air_route][i];
}
