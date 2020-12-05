/*
 * 这个文件用来定义所有的自定义窗口、组件
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QMainWindow>
#include <QScrollArea>
#include <QWidget>
#include <QPainter>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QFontDatabase>
#include <QLabel>
#include "Object.h"
#include "Game.h"

class GameWidget : public QScrollArea{  //游戏组件，是承载游戏画面的容器
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void setGame(class Game* game){mGame = game;}
    void addObject(class Object* obj); //添加对象
    void removeObject(class Object* obj);
    QVector<Object*>* getObjects(){return &mObjects;}
    Game* getGame(){return mGame;}

    void Initialize(QString gameToStart);
    void paint();  //重写paintEvent函数，用于绘制对象
protected:
    void keyPressEvent(QKeyEvent *)override;  //重写keyPressEvent函数，用于获取输入
    void keyReleaseEvent(QKeyEvent *)override;
    void mouseMoveEvent(QMouseEvent *)override;
    void mousePressEvent(QMouseEvent *)override;
    void mouseReleaseEvent(QMouseEvent *)override;
private:
    QVector<class Object*> mObjects; //储存所有待显示的对象
    class Game* mGame;
    class Map* mMap;

signals:
    void keyPressSignal(QString key);
    void keyReleaseSignal(QString key);
    void mouseMoveSignal(int x, int y);
    void mousePressSignal(int x, int y);
    void mouseReleaseSignal(int x, int y);
};

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    InfoWidget(QWidget* parent = nullptr);
    ~InfoWidget();

    void setGame(class Game* game){mGame = game;}

    void addObject(class Object* obj); //添加对象
    void removeObject(class Object* obj);
    QVector<Object*>* getObjects(){return &mObjects;}
    Game* getGame(){return mGame;}

    void Initialize();
    void updateInfo();

    void paint();
protected:
    void paintEvent(QPaintEvent*)override;
private:
    class Game* mGame;
    QVector<class Object*> mObjects; //储存所有待显示的对象

    class Object* selected = nullptr;
    QWidget* selectedInfoWidget, *gameInfoWidget;
    QFormLayout* selectedInfoLayout, *gameInfoLayout;
    QLabel* mFPSLabel, *mSCORELabel;
    QLabel* typeName,* info, * position, *sizeLabel, *health;
};


class MainWindow : public QMainWindow      //主界面类
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Initialize();
    void ChangePause();

    void StartEdit();
    void FinishEdit();

    void WelToCho();
    void ChoToWel();
    void OverToWel();
    void Gameover(int, QColor, int, QColor, int);
    void RestartInGame();
    void RestartOutOfGame();
    void StartGameSingle();
    void StartGameMulti();
    void StartGameAI();
    void StartGameLoad();

    void addBlock(){emit(addSomething("Block"));}
    void addBasicFood(){emit(addSomething("BasicFood"));}
    void addGenerator(){emit(addSomething("FoodGenerator"));}
    void addSpeedFood(){emit(addSomething("SpeedUpFood"));}
    void addLifeFood(){emit(addSomething("LifeUpFood"));}
    void addScoreFood(){emit(addSomething("ScoreBonusFood"));}
    void addSnakeBody(){emit(addSomething("SnakeBody"));}

    void saveAndExit();
protected:
    void keyPressEvent(QKeyEvent*);

private:
    class WelcomeWidget* mWelWidget;
    class ChooseWidget* mChoWidget;
    class OptionWidget* mOptionWidget;
    class GameoverWidget* mOverWidget;
    class EditToolWidget* mEditWidget;
    GameWidget* mGameWidget;
    InfoWidget* mInfoWidget;
    QHBoxLayout* mHLayout;
    Game* mGame;
    QString mGameMode;
    bool isStarted = false;

signals:
    void addSomething(QString obj);
};

class WelcomeWidget : public QWidget
{
    Q_OBJECT
public:
    WelcomeWidget(QWidget* parent = nullptr);
    ~WelcomeWidget();
private:
    QPushButton* playButton,* settingButton,* exitButton, *lastButton;
    QLabel* label, *label2;
};

class OptionWidget : public QWidget{
    Q_OBJECT
public:
    OptionWidget(QWidget* parent = nullptr);
    ~OptionWidget();
private:
    QPushButton* restartButton, *exitButton, *backButton, *editButton;
    QLabel* label;
};

class ChooseWidget : public QWidget{
    Q_OBJECT
public:
    ChooseWidget(QWidget* parent = nullptr);
    ~ChooseWidget();
private:
    QPushButton* singleButton, *multiButton, *backButton, *aiButton, *lastButton;
    QLabel* label;
};

class GameoverWidget : public QWidget{
    Q_OBJECT
public:
    void setScore(int scoreNum = 1, QColor color1 = QColor(0, 0, 0), int score1 = 0, QColor color2 = QColor(0, 0, 0), int score2 = 0);

    GameoverWidget(QWidget* parent = nullptr);
    ~GameoverWidget();
private:
    QPushButton* restartButton, *backButton;
    QLabel* label,* scoreLabel1,* scoreLabel2;
    QPalette p1, p2;
};

class EditToolWidget : public QWidget{
    Q_OBJECT
public:
    EditToolWidget(QWidget* parent = nullptr);
    ~EditToolWidget();
private:
    QPushButton* finishButton;
    QFormLayout* mLayout;
    QPushButton* block, *basicfood, *speedfood, *scorefood, *lifefood, *snakebody, *generator;
    QLabel* label;
};




#endif // MAINWINDOW_H
