/*
 * 这个文件用来声明游戏主进程类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */

#ifndef GAME_H
#define GAME_H

#include <QVector>
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>
#include "widgets.h"
#include "Object.h"
#include "Block.h"
#include "Prop.h"
#include "Being.h"
#include "Label.h"

class Game : public QThread{ //Game类继承了QThread，因为游戏的逻辑循环独立于QT组件的事件循环
    Q_OBJECT

private:
    bool isRunning = true, Paused = false, mousePressed = false; //是否在运行
    int stopAfter;
    int playerNum = 0;
    double FPS; //懂得都懂
    QVector<class Object*> mObjects, mPendingObjects, mDeadObjects; //储存所有的对象
    QVector<class Block*> mBlocks; //储存所有的方块
    QVector<class Being*> mBeings; //储存所有生物
    QVector<class Prop*> mProps;
    QVector<class Player*> mPlayers;

    struct ColorAndScore{
        QColor color;
        int score;

        ColorAndScore(QColor c, int s):color(c),score(s){}
    };
    QVector<ColorAndScore> mScores;

    class Object* selected = nullptr;

    QVector<QString> mPressedKeys;


    class GameWidget* gameWidget; //游戏组件
    class InfoWidget* infoWidget;

    QElapsedTimer* timer;
    void ProcessInput(); //响应输入
    void Update(); //更新
    void GenerateOutput(); //显示输出

    void keyPress(QString key);
    void keyRelease(QString key);
    void mouseMove(int x, int y);
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);

protected:
    void run() override; //重写QThread的run函数，作为游戏逻辑的主循环

public:
    int mouseX = 0, mouseY = 0;

    void addSomething(QString obj);
    void addObject(class Object* obj); //向游戏中添加对象
    void removeObject(class Object* obj);
    void addBlock(class Block* block);
    void addBeing(class Being* being);
    void addProp(class Prop* prop);
    void removeProp(class Prop* prop);
    void addPlayer(class Player* player);
    void setSelected(class Object* obj);
    int getFPS(){return FPS;}
    bool getPaused(){return Paused;}
    Object* getSelected(){return selected;}
    QVector<Block*>* getBlocks(){return &mBlocks;}
    QVector<Being*>* getBeings(){return &mBeings;}
    QVector<Prop*>* getProps(){return &mProps;}
    QVector<Player*>* getPlayers(){return &mPlayers;}

    void Initialize(); //初始化参数
    void Pause();
    void unPause();

    void save();
    void load();

    void setGameWidget(GameWidget* w){
        gameWidget = w;
    }
    void setInfoWidget(InfoWidget* w){
        infoWidget = w;
    }

    void removeEverything();

    Game();
    ~Game();

signals:
    void updateInfo(Game*);
    void gameOver(int, QColor, int, QColor, int);
};

#endif // GAME_H
