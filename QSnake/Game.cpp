/*
 * 这个文件用来实现游戏主进程
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */


#include "Game.h"

Game::Game()     //直接调用初始化函数
    :QThread(0){
}
Game::~Game(){
}

void Game::run(){         //简简单单的循环，输入->计算->输出
    timer = new QElapsedTimer();
    timer->start();
    qDebug() << "[Process] START GAME";

    while(isRunning){
        ProcessInput();
        Update();
        GenerateOutput();
    }
}

void Game::Initialize(){       //目前暂时直接调出一个游戏组件，以后应该显示主界面，而游戏组件是主界面的一部分
    isRunning = true;
    stopAfter = -100;
    playerNum = mPlayers.length();
    Paused = false;
    if (gameWidget){
        connect(gameWidget, &GameWidget::keyPressSignal, this, &Game::keyPress);
        connect(gameWidget, &GameWidget::keyReleaseSignal, this, &Game::keyRelease);
        connect(gameWidget, &GameWidget::mouseMoveSignal, this, &Game::mouseMove);
        connect(gameWidget, &GameWidget::mousePressSignal, this, &Game::mousePress);
        connect(gameWidget, &GameWidget::mouseReleaseSignal, this, &Game::mouseRelease);
   }else qDebug() << "[Warning] Game: Fail to Connect with GameWidget";
    if (infoWidget)
        connect(this, &Game::updateInfo, infoWidget, &InfoWidget::updateInfo);
    else qDebug() << "[Warning] Game: Fail to Connect with InfoWidget";
}

void Game::ProcessInput(){      //遍历所有对象响应输入
    for(auto i : mObjects)
        i->processInput(&mPressedKeys);
}

void Game::Update(){      //遍历更新所有对象
    int deltaTime = timer->elapsed();
    while(deltaTime < 10){
        deltaTime = timer->elapsed();
    }
    FPS = 1000 / deltaTime;
    timer->restart();
    if (Paused) deltaTime = 0;

    for(auto i : mObjects){
        i->update(deltaTime);
        if (i->getClearStatus())
            mDeadObjects.push_back(i);
    }

    for(auto i : mPendingObjects){
        int j;
        for(j = 0; j < mObjects.length(); j++)
            if(mObjects[j]->getPaintOrder() < i->getPaintOrder()){
                break;
        }
        mObjects.insert(j, i);

        qDebug() << "[Game] Add Object:" << i->getType();
        gameWidget->addObject(i);
    }
    mPendingObjects.clear();


    for(auto i : mDeadObjects){

        mObjects.removeOne(i);
        qDebug() << "[Game] Remove Object:" << i->getType();
        gameWidget->removeObject(i);
        infoWidget->removeObject(i);

        if (i == selected) selected = nullptr;
        delete i;
    }
    mDeadObjects.clear();
    if (mPlayers.length() == 1 && !Paused){
        gameWidget->horizontalScrollBar()->setValue(mPlayers[0]->getX() - 500);
        gameWidget->verticalScrollBar()->setValue(mPlayers[0]->getY() - 500);
    }
    emit(updateInfo(this));

    if (mPlayers.length() < playerNum && stopAfter == -100){
        stopAfter = 2100;
    }
    if (stopAfter > 0)
        stopAfter -= deltaTime;

    if (stopAfter < 1100 && stopAfter > 1000){
        removeEverything();
        stopAfter = 1000;
    }

    if (stopAfter < 100 && stopAfter > 0){
        qDebug() << "[Process] GAME OVER";
        if (playerNum == 1)
            emit(gameOver(1, mScores[0].color, mScores[0].score, QColor(0, 0, 0), 0));
        else emit(gameOver(2, mScores[0].color, mScores[0].score, mScores[1].color, mScores[1].score));
        mScores.clear();
        isRunning = false;
    }
}

void Game::GenerateOutput(){       //调用游戏组件的update，注意这个update默认包含了paintEvent
    gameWidget->paint();
    infoWidget->paint();
}

void Game::Pause(){
    Paused = true;
    timer->restart();
    qDebug() << "[Process] Pause";
}

void Game::unPause(){
    Paused = false;
    timer->restart();
    qDebug() << "[Process] Continue";
}

void Game::save(){
    QFile file("history.snk");
    file.open(QIODevice::WriteOnly);
    QString info = QString::number(gameWidget->widget()->width()) +","+ QString::number(gameWidget->widget()->height()) + "\n";
    for(auto i : mObjects){
        if(i->getType()!="Partical" && i->getType()!="SizeController" && i->getType()!="DyingLabel")
            info += i->toString() + "\n";
    }
    file.write(info.toStdString().c_str());
}
void Game::load(){
    QFile file("history.snk");
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }

    QString info;

    info = file.readLine();
    gameWidget->widget()->resize(info.split(",")[0].toInt(),info.split(",")[1].toInt());

    while(!file.atEnd()){
        info = file.readLine();
        QStringList list = info.split(":");
        if (list[0] == "Block")
            addBlock(new Block(this, list[1].toFloat(), list[2].toFloat(), list[5].toInt(), list[6].toInt(), QColor(list[7].toInt(),list[8].toInt(),list[9].toInt())));
        if (list[0] == "Snake"){
            Snake* p = new Snake(this, list[1].toFloat(), list[2].toFloat(), list[5].toInt(), list[6].toInt(), QColor(list[7].toInt(),list[8].toInt(),list[9].toInt()), qAbs(list[3].toFloat()+list[4].toFloat()));
            addBeing(p);
            addPlayer(p);
            p->setVel(list[3].toFloat(), list[4].toFloat());
            if (list[3].toFloat()) p->setMVel(qAbs(list[3].toFloat()));
            else p->setMVel(qAbs(list[4].toFloat()));
            if (mPlayers.length() == 2) p->setKeys("Up", "Down", "Left", "Right");
        }
        if (list[0] == "AISnake"){
            AISnake* p = new AISnake(this, list[1].toFloat(), list[2].toFloat(), list[5].toInt(), list[6].toInt(), QColor(list[7].toInt(),list[8].toInt(),list[9].toInt()), qAbs(list[3].toFloat()+list[4].toFloat()));
            addBeing(p);
            addPlayer(p);
            p->setVel(list[3].toFloat(), list[4].toFloat());
            if (list[3].toFloat()) p->setMVel(qAbs(list[3].toFloat()));
            else p->setMVel(qAbs(list[4].toFloat()));
            if (mPlayers.length() == 2) p->setKeys("Up", "Down", "Left", "Right");
        }
        if (list[0] == "SnakeBody"){
            SnakeBody* body = new SnakeBody(this, list[1].toFloat(), list[2].toFloat(), list[5].toInt(), list[6].toInt(), QColor(list[7].toInt(),list[8].toInt(),list[9].toInt()));
            addBlock(body);
            for(auto i :mPlayers)
                if (i->getColor() == QColor(list[7].toInt(),list[8].toInt(),list[9].toInt()))
                    static_cast<Snake*>(i)->addSnakeBody(body);
        }
        if (list[0] == "FoodGenerator")
            addBlock(new FoodGenerator(this,list[10].toInt(), list[1].toFloat(), list[2].toFloat(), list[5].toInt(), list[6].toInt(), QColor(list[7].toInt(),list[8].toInt(),list[9].toInt())));
    }
}

void Game::keyPress(QString key){
    mPressedKeys.push_back(key);
    ProcessInput();

    if (Paused && selected && key == "\u007F"){
        removeObject(selected);
        setSelected(nullptr);
    }
}

void Game::keyRelease(QString key){
    mPressedKeys.removeOne(key);
    ProcessInput();
}

void Game::mouseMove(int x, int y){
    if (selected && Paused && mousePressed){
        selected->setXY(selected->getX() + x - mouseX, selected->getY() + y - mouseY);
        mouseX = x; mouseY = y;
    }
}

void Game::mousePress(int x, int y){
    setSelected(nullptr);
    mouseX = x; mouseY = y;
    mousePressed = true;

    bool flag = true;
    for (auto i: mObjects){
        if (flag && i->getX()-i->getW()/2 < mouseX &&i->getX()+i->getW()/2 > mouseX &&i->getY()-i->getH()/2 < mouseY &&i->getY()+i->getH()/2 > mouseY){
            setSelected(i);
            i->setSelected();
            flag = false;
        }
        else i->setNotSelected();
    }
}

void Game::mouseRelease(int x, int y){
    mousePressed = false;
    mouseX = x; mouseY = y;
}

void Game::addSomething(QString obj){
    if (obj == "Block")
        addBlock(new Block(this, mouseX, mouseY));
    if (obj == "BasicFood")
        addProp(new BasicFood(this, mouseX, mouseY));
    if (obj == "SpeedUpFood")
        addProp(new SpeedUpFood(this, mouseX, mouseY));
    if (obj == "LifeUpFood")
        addProp(new LifeUpFood(this, mouseX, mouseY));
    if (obj == "ScoreBonusFood")
        addProp(new ScoreBonusFood(this, mouseX, mouseY));
    if (obj == "SnakeBody")
        addBlock(new SnakeBody(this, mouseX, mouseY));
    if (obj == "FoodGenerator")
        addBlock(new FoodGenerator(this, 300, mouseX, mouseY));
}

void Game::addObject(class Object *obj){        //添加对象时，不仅添加到Game类中，也要添加到游戏组件中
    mPendingObjects.push_back(obj);
}

void Game::removeObject(class Object *obj){
    obj->setClearStatus(true);
    if (mBlocks.contains(static_cast<Block*>(obj)))
        mBlocks.removeOne(static_cast<Block*>(obj));
    if (mBeings.contains(static_cast<Being*>(obj)))
        mBeings.removeOne(static_cast<Being*>(obj));
    if (mProps.contains(static_cast<Prop*>(obj)))
        mProps.removeOne(static_cast<Prop*>(obj));
    if (mPlayers.contains(static_cast<Player*>(obj))){
        mPlayers.removeOne(static_cast<Player*>(obj));
        mScores.append(ColorAndScore(obj->getColor(), static_cast<Player*>(obj)->getScore()));
    }
}

void Game::addBlock(class Block *block){
    mBlocks.push_back(block);
    addObject(block);
}

void Game::addBeing(class Being *being){
    mBeings.push_back(being);
    addObject(being);
}

void Game::addProp(class Prop *prop){
    mProps.push_back(prop);
    addObject(prop);
}

void Game::removeProp(class Prop *prop){
    mProps.removeOne(prop);
    removeObject(prop);
}

void Game::addPlayer(class Player *player){
    mPlayers.push_back(player);
}

void Game::setSelected(class Object *obj){
    selected = obj;
}

void Game::removeEverything(){
    for(auto i: mObjects){
        removeObject(i);
    }
}
