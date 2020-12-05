/*
 * 这个文件用来定义生物
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 * JZ   2020/10/23   01:25   增加玩家类Player
 */

#ifndef BEING_H
#define BEING_H

#include "Object.h"
#include "Game.h"
#include <QDebug>

class Being: public Object{
protected:
    int curHealth, maxHealth;
    int lifeNum;
    enum Status{
        Stay,
        Walk,
        Run,
        Jump
    } mStatus;
    QVector<class Prop*> mProps;

public:
    virtual void* getPtr() override{return (Being*)this;}

    int getMaxHealth(){return maxHealth;}
    int getCurHealth(){return curHealth;}

    void paint(class QPainter*) override;
    void update(int deltaTime) override;

    void addLife(int x){lifeNum += x;}

    void addProp(class Prop* prop);
    virtual void useProp(class Prop* prop);

    Being(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    virtual ~Being();
};

class Player: public Being{
private:

protected:
    int score = 0;
public:
    virtual void* getPtr() override{return (Player*)this;}

    void paint(class QPainter*) override;
    void update(int deltaTime) override;
    void processInput(QVector<QString>*) override;


    int getScore(){return score;}
    void addScore(int s);
    Player(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    virtual ~Player() override;
};


class Snake: public Player{
private:
    QString upKey = "w", downKey = "s", leftKey = "a", rightKey = "d";

    float xVel_LastUpdated, yVel_LastUpdated;
protected:
    QVector<Block*> mBlocks;
    Block* head;

    int growNum = 0;
    int updateIntv = 250;

    int initX, initY;
    int strongTime = 0;

    int speedUpTimeLast = 0;

public:
    virtual void* getPtr() override{return (Snake*)this;}

    void paint(class QPainter*) override;
    void update(int deltaTime) override;
    void processInput(QVector<QString>*)override;

    virtual void useProp(class Prop* prop) override;

    void setKeys(QString uk, QString dk, QString lk, QString rk){
        upKey = uk; downKey = dk; leftKey = lk; rightKey = rk;
    }
    void addSnakeBody(SnakeBody* body){mBlocks.push_back(body);}

    void grow(int n){growNum += n;}
    void speedUp(int time);

    Snake(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50, QColor color = QColor(0, 0, 0));
    virtual ~Snake() override;
};


class AISnake: public Snake{
private:
    int inteligence;
public:
    virtual void* getPtr() override{return (AISnake*)this;}

    void processInput(QVector<QString>*)override;

    AISnake(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50, QColor color = QColor(0, 0, 0), int intel = 100);
    virtual ~AISnake() override;
};

#endif // BEING_H
