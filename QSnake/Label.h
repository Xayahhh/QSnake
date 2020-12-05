#ifndef LABLE_H
#define LABLE_H

#include "Object.h"

class Label: public Object{
protected:
    class QString mString;
    QFont mFont;
public:
    virtual void* getPtr() override{return (Label*)this;}

    void paint(class QPainter* p)override;
    void update(int deltaTime)override;
    void setText(class QString text){mString = text;}

    Label(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50, class QString text = "", QColor color = QColor(0, 0, 0));
    ~Label();
};

class DyingLabel: public Label{
private:
    int restOfTime;
    float dAlpha;
public:
    virtual void* getPtr() override{return (DyingLabel*)this;}

    void paint(class QPainter* p)override;
    void update(int deltaTime)override;

    DyingLabel(class Game* g, int time, float x = 0, float y = 0, int w = 50, int h = 50, float xv = 0, float yv = 0, class QString text = "", QColor color = QColor(0, 0, 0));
    ~DyingLabel();
};


class Bar: public Object{
protected:
    float mValue;
public:
    virtual void* getPtr() override{return (Bar*)this;}

    void paint(class QPainter* p)override;
    void setValue(float value){mValue = value;}

    Bar(class Game* g,float x = 0, float y = 0, int w = 50, int h = 50, QColor color = QColor(0, 0, 0));
    ~Bar();
};

class Partical : public Object{
protected:
    int restOfTime;

    float dAlpha;
public:
    virtual void update(int deltaTime) override;

    void paint(class QPainter* p)override;

    Partical(class Game* g, int time, float x = 0, float y = 0, int w = 10, int h = 10, float xv = 0, float yv = 0, QColor color = QColor(0, 0, 0));
    ~Partical();
};

#endif // LABLE_H
