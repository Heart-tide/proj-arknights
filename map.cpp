#include "map.h"
#include "ui_map.h"
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>

Map::Map(GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Map)
    , _gamestate(gamestate)
    , _operatorSelected(-1)
{
    ui->setupUi(this);
    connectOperators();
    loadMap();
    arrangeUiWidgets();
    printLog("#000000", "", QString("地图生成成功, 大小 %1 * %2").arg(getHeight()).arg(getWidth()));
    printLog("#000000", "", "游戏开始了哦!");
}

Map::~Map()
{
    delete ui;
}

void Map::connectOperators()
{
    connect(ui->photo_irene, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 0;
    });
    connect(ui->photo_kroos, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 1;
    });
    connect(ui->photo_honey_berry, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 2;
    });
    connect(ui->photo_skadi, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 3;
    });
}

//* 让窗口部件大小自适应地图大小
void Map::arrangeUiWidgets()
{
    auto width = getWidth();
    auto height = getHeight();
    //* 1. log, speed and pause
    ui->text_log->setGeometry(10, height * 100 + 10, 280, 80);
    ui->push_speed->setGeometry(width * 100 - 70, 20, 40, 40);
    ui->push_pause->setGeometry(width * 100 - 130, 20, 40, 40);
    //* 2. dp, enemy stats and hp
    ui->picture_dp->setGeometry(width * 100 - 75, height * 100 - 32, 23, 23);
    ui->label_dp->setGeometry(width * 100 - 45, height * 100 - 32, 40, 20);
    ui->picture_enemy_stats->setGeometry(static_cast<float>(width) / 2 * 100 - 80, 13, 31, 31);
    ui->label_enemy_stats->setGeometry(static_cast<float>(width) / 2 * 100 - 40, 17, 30, 20);
    ui->picture_hp->setGeometry(static_cast<float>(width) / 2 * 100 + 10, 13, 31, 31);
    ui->label_hp->setGeometry(static_cast<float>(width) / 2 * 100 + 50, 17, 30, 20);
    //* 3. photos of operators
    ui->photo_irene->setGeometry((width - 1) * 100, height * 100, 100, 100);
    ui->photo_skadi->setGeometry((width - 2) * 100, height * 100, 100, 100);
    ui->photo_honey_berry->setGeometry((width - 3) * 100, height * 100, 100, 100);
    ui->photo_kroos->setGeometry((width - 4) * 100, height * 100, 100, 100);
}

//* 读取在 map 文件夹下的所有地图文件
void Map::loadMap()
{
    //* currentPath 指出 exe 文件所在目录，QStringList("map-*.txt")过滤出所有地图文件
    QDir map_dir(QDir::currentPath() + "/map");
    QList<QFileInfo> fileInfo(map_dir.entryInfoList(QStringList("map-*.txt")));
    //* 从已有地图中，随机选取一张地图执行
    srand(static_cast<unsigned>(time(nullptr)));
    size_t map_choice = rand() % fileInfo.size(); //* 去掉上级目录和当前目录两项，其余的即地图数目
    QFile file(map_dir.absolutePath() + QString("/map-%1.txt").arg(map_choice));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    printLog("#000000", "", QString("地图文件 %1 读取成功!").arg(map_choice), this);
    QTextStream in(&file);
    size_t height = 0;
    size_t width = 0;
    in >> height >> width;
    resize(width * 100, (height + 1) * 100);
    for (size_t i = 0; i < height; i++) {
        QVector<Place*> line;
        for (size_t j = 0; j < width; j++) {
            //* 读取一个格子
            Place* place = nullptr;
            int place_type = 0;
            in >> place_type;
            switch (place_type) {
            case 0:
                place = new Base(_gamestate, this);
                _base.push_back(dynamic_cast<Base*>(place));
                break; //? 忘记加 break，在此 debug 半个小时
            case 1:
                place = new Entrance(_gamestate, this);
                _entrance.push_back(dynamic_cast<Entrance*>(place));
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
            place->setGeometry(j * 100, i * 100, 100, 100);
            place->_id = QPair<size_t, size_t>(i, j);
        }
        _lines.push_back(line);
    }
    loadRoutes(in); //* 读取路径
}

void Map::loadRoutes(QTextStream& in)
{
    //* 路径总条数
    size_t n = 0;
    in >> n;
    /**
     * @brief 每条路径的格式：
     * 路径类型 路径点长度
     * 路径点一横坐标 路径点一纵坐标 路径点二横坐标 路径点二纵坐标 ……
     *
     * 其中：
     * 路径类型：地面路径 0，空中路径 1
     * 路径点：相邻路径点不必是相邻的格子！从 Base 到 Entrance 为一条完整路径
     */
    for (size_t p = 0; p < n; p++) {
        size_t is_air_route, length;
        in >> is_air_route >> length;
        QVector<Place*> route;
        for (size_t q = 0; q < length; q++) {
            size_t i, j;
            in >> i >> j;
            route.push_back((*this)[i][j]);
        }
        _routes[is_air_route].push_back(route);
    }
}

void printLog(const QString& color, const QString& type, const QString& info, Map* map_init, bool gameover)
{
    //* 仅在第一次调用时传参 map_init，形成闭包
    static Map* map = map_init;
    if (map) {
        map->ui->text_log->appendHtml(QString(R"(<html><head/><body><p>
                <span style=" font-weight:600;color:%1;">
                %2 </span>%3</p></body></html>)")
                                          .arg(color)
                                          .arg(type)
                                          .arg(info));
    }
    //* 用于游戏结束时脱绑
    if (gameover) {
        map = nullptr;
    }
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
    for (size_t i = 0; i < getHeight(); i++) {
        for (size_t j = 0; j < getWidth(); j++) {
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
    //* 多画一行干员预备栏的底色
    for (size_t j = 0; j < getWidth(); j++) {
        painter.drawPixmap(j * 100, getHeight() * 100, 100, 100, common_place);
    }
}
