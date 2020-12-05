#include "Label.h"
#include "Game.h"
#include <QDebug>

Label::Label(class Game* g, float x, float y, int w, int h, class QString text, QColor color)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h; mString = text; mColor = color;
    mType = "Label";
    mInfo = "Instructions";
    mFont.setFamily("Bulletproof BB");
    mFont.setPixelSize(height * 1.3);
    mFont.setLetterSpacing(QFont::AbsoluteSpacing, 0);
}

void Label::paint(class QPainter *p){
    if (isVisible){      //显示文字
        p->save();
        p->setBrush(mColor);
        p->setPen(mColor);
        p->setFont(mFont);
        p->drawText(int(xPos - width/2) , int(yPos + height/2), mString);
        p->restore();
    }
    Object::paint(p);
}

void Label::update(int){

}

Label::~Label(){
}

DyingLabel::DyingLabel(class Game* g, int time, float x, float y, int w, int h, float xv, float yv, QString text, QColor color)
    :Label(g, x, y, w, h, text, color){
    mType = "DyingLabel";
    mInfo = "Instructions";

    xVel = xv; yVel = yv;
    restOfTime = time;
    dAlpha = 1.0 / restOfTime;
}

void DyingLabel::paint(class QPainter *p){
    if (isVisible){      //显示文字
        p->save();
        p->setBrush(mColor);
        p->setPen(mColor);
        p->setFont(mFont);
        p->drawText(int(xPos - width/2) , int(yPos + height/2), mString);
        p->restore();
    }
    Object::paint(p);
}

void DyingLabel::update(int deltaTime){
    restOfTime -= deltaTime;
    if (restOfTime <= 0){
        mGame->removeObject(this);
        return;
    }

    mColor.setAlphaF(mColor.alphaF() - dAlpha * deltaTime);
    Object::update(deltaTime);
}

DyingLabel::~DyingLabel(){
}


void Bar::paint(class QPainter *p){

    if (isVisible){      //显示
        p->save();
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块

        p->setPen(Qt::NoPen);
        p->setBrush(mColor);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), int(width* mValue), height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);

}

Bar::Bar(class Game* g,float x, float y, int w, int h, QColor color)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h;
    mValue = 1.0;
    mColor = color;
    mType = "Bar";
    mInfo = "Show process";
}

Bar::~Bar(){
}


void Partical::update(int deltaTime){
    restOfTime -= deltaTime;
    if (restOfTime <= 0){
        mGame->removeObject(this);
        return;
    }

    mColor.setAlphaF(mColor.alphaF() - dAlpha * deltaTime);
    Object::update(deltaTime);
}

void Partical::paint(class QPainter *p){
    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(mColor));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

Partical::Partical(class Game* g, int time, float x, float y, int w, int h, float xv, float yv, QColor color)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h;
    xVel = xv; yVel = yv;
    mColor = color;
    mType = "Partical";
    mInfo = "Basic \n Partical";

    restOfTime = time;
    dAlpha = 1.0 / restOfTime;
}

Partical::~Partical(){

}
