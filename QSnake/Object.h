/*
 * 这个文件用来定义基本游戏对象类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 * JZ   2020/10/23   01:28   增加Component相关内容、其他小修改
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QObject>

class Object{
protected:
    bool isEnabled, isVisible, isSelected, isWaitingForClear;
    bool dying;
    float xPos, yPos;     //位置
    float xVel, yVel;     //速度
    float xAcc, yAcc;
    double rot, rotVel;     //角度和角速度
    int width, height; //宽度和高度
    int mTime;
    class Game* mGame;
    QColor mColor;
    int paintOrder;
    QString mType, mInfo;
    int selectOrder = 0;
public:
    virtual void* getPtr(){return (Object*)this;}
    virtual QString toString();

    bool getClearStatus(){return isWaitingForClear;}
    float getX(){return xPos;}
    float getY(){return yPos;}
    int getW(){return width;}
    int getH(){return height;}
    int getPaintOrder(){return paintOrder;}
    QString getType(){return mType;}
    QString getInfo(){return mInfo;}
    QColor getColor(){return mColor;}
    void setClearStatus(bool dead = true){isWaitingForClear = dead;}
    void setXY(float x, float y){xPos = x; yPos = y;}
    void setWH(int w, int h){width = w; height = h;}
    void setVel(float xv, float yv){xVel = xv; yVel = yv;}
    void setSelected(){isSelected = true;}
    void setNotSelected(){isSelected = false;}
    virtual void destroy();
    virtual void paint(class QPainter* p);
    virtual void update(int deltaTime);
    virtual void processInput(QVector<QString>*){}  //子类需要重写，用于响应输入

    Object(class Game* g){
        mGame = g;
        xPos = 500.0; yPos = 500.0;
        xVel = 0.0; yVel = 0.0;
        xAcc = 0.0; yAcc = 0.0;
        width = 0; height = 0;
        mTime = 0;
        mColor = QColor(0, 0, 0); paintOrder = 0;
        isEnabled = true;
        isVisible = true;
        isSelected = false;
        isWaitingForClear = false;
        dying = false;
        mType = "Object";
        mInfo = "";
    }
    virtual ~Object();
};

#endif // OBJECT_H
