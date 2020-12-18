/*
 * 这个文件用来实现各个自定义窗口、组件类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */

#include "widgets.h"
#include "Maps.h"
#include <QKeyEvent>

QString SettingsWidget::UpKey = "w";
QString SettingsWidget::DownKey = "s";
QString SettingsWidget::LeftKey = "a";
QString SettingsWidget::RightKey = "d";
int SettingsWidget::SnakeSize = 50;
int SettingsWidget::SnakeSpeed = 250;

GameWidget::GameWidget(QWidget *parent)  //游戏组件的初始化，目前还很粗糙
    : QScrollArea(parent)
{
    setFixedSize(1000, 1000);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameWidget::addObject(class Object *obj){  //添加对象
    qDebug() << "[GameWidget] Add Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    int i;
    for(i = 0; i < mObjects.length(); i++)
        if(mObjects[i]->getPaintOrder() > obj->getPaintOrder()){
            break;
    }
    mObjects.insert(i, obj);
}

void GameWidget::removeObject(class Object *obj){
    qDebug() << "[GameWidget] Remove Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    mObjects.removeOne(obj);
}

GameWidget::~GameWidget()
{
}

void GameWidget::Initialize(QString gameToStart){
    setStyleSheet("QScrollArea{border:0px solid #707070}");

    if (gameToStart == "QSnake-Single")
        mMap = new Map_Snake_1(this, this);
    else if (gameToStart == "QSnake-Multi")
        mMap = new Map_Snake_2(this, this);
    else if (gameToStart == "QSnake-AI")
        mMap = new Map_Snake_3(this, this);
    else if (gameToStart == "QSnake-Load"){
        mMap = new Map(this, this);
    }
    else mMap = new Map(this, this);

    setWidget(mMap);

    if (gameToStart == "QSnake-Load") mGame->load();
}

void GameWidget::paint(){ //遍历所有对象，逐个调用对象的paint方法，直接传送QPainter
    mMap->update();
}

void GameWidget::keyPressEvent(QKeyEvent *k){
    if (k->text().length()>0)
        emit(keyPressSignal(k->text()));
    else{
        if (k->key() == Qt::Key_Right)
            emit(keyPressSignal("Right"));
        if (k->key() == Qt::Key_Left)
            emit(keyPressSignal("Left"));
        if (k->key() == Qt::Key_Up)
            emit(keyPressSignal("Up"));
        if (k->key() == Qt::Key_Down)
            emit(keyPressSignal("Down"));
    }
    QWidget::keyPressEvent(k);
}

void GameWidget::keyReleaseEvent(QKeyEvent *k){
    if (k->text().length()>0)
        emit(keyReleaseSignal(k->text()));
    else{
        if (k->key() == Qt::Key_Up)
            emit(keyReleaseSignal("Up"));
        if (k->key() == Qt::Key_Down)
            emit(keyReleaseSignal("Down"));
        if (k->key() == Qt::Key_Left)
            emit(keyReleaseSignal("Left"));
        if (k->key() == Qt::Key_Right)
            emit(keyReleaseSignal("Right"));
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *k){
    emit(mouseMoveSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mouseMoveEvent(k);
}

void GameWidget::mousePressEvent(QMouseEvent* k){
    emit(mousePressSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mousePressEvent(k);
}

void GameWidget::mouseReleaseEvent(QMouseEvent* k){
    emit(mouseReleaseSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mouseReleaseEvent(k);
}

InfoWidget::InfoWidget(QWidget* parent)
    :QWidget(parent){
    setFixedSize(350, 1000);

    gameInfoWidget = new QWidget(this);
    gameInfoWidget->move(0, 100);
    gameInfoWidget->setFixedSize(350, 400);

    gameInfoLayout = new QFormLayout(gameInfoWidget);
    mFPSLabel = new QLabel("", gameInfoWidget);
    mSCORELabel = new QLabel("", gameInfoWidget);

    gameInfoLayout->addRow("FPS: ", mFPSLabel);
    gameInfoLayout->addRow("SCORE: ", mSCORELabel);

    gameInfoWidget->setLayout(gameInfoLayout);
    gameInfoWidget->show();
    gameInfoWidget->setStyleSheet("QLabel{font-family:Bulletproof BB;font-size:30px}");


    selectedInfoWidget = new QWidget(this);
    selectedInfoWidget->move(0, 550);
    selectedInfoWidget->setFixedSize(350, 400);

    selectedInfoLayout = new QFormLayout(selectedInfoWidget);
    typeName = new QLabel("", selectedInfoWidget);
    info = new QLabel("", selectedInfoWidget);
    position = new QLabel("", selectedInfoWidget);
    sizeLabel = new QLabel("", selectedInfoWidget);
    health = new QLabel("", selectedInfoWidget);

    selectedInfoLayout->addRow("Type: ", typeName);
    selectedInfoLayout->addRow("Info: ", info);
    selectedInfoLayout->addRow("Pos: ", position);
    selectedInfoLayout->addRow("Size: ", sizeLabel);
    selectedInfoLayout->addRow("HP: ", health);

    selectedInfoWidget->setLayout(selectedInfoLayout);
    selectedInfoWidget->show();
    selectedInfoWidget->setStyleSheet("QLabel{font-family:Bulletproof BB;font-size:30px}");
}

void InfoWidget::addObject(class Object *obj){  //添加对象
    qDebug() << "[InfoWidget] Add Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    mObjects.push_back(obj);
}

void InfoWidget::removeObject(class Object *obj){
    qDebug() << "[InfoWidget] Remove Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    mObjects.removeOne(obj);
}

InfoWidget::~InfoWidget(){
}

void InfoWidget::Initialize(){
    addObject(new Label(mGame, 150, 50, 300, 30, "GAME INFO"));
    addObject(new Label(mGame, 150, 500, 300, 30, "OBJ INFO"));
}

void InfoWidget::updateInfo(){
    mFPSLabel->setText(QString::number(mGame->getFPS()));
    if (mGame->getPlayers()->length() == 1)
        mSCORELabel->setText(QString::number((*mGame->getPlayers())[0]->getScore()));
    else if (mGame->getPlayers()->length() == 2)
        mSCORELabel->setText(QString::number((*mGame->getPlayers())[0]->getScore()) + " : " + QString::number((*mGame->getPlayers())[1]->getScore()));
    else mSCORELabel->setText("");
    selected = mGame->getSelected();
}

void InfoWidget::paint(){
    this->update();
    if (selected){
        typeName->setText(selected->getType());
        info->setText(selected->getInfo());
        position->setText(QString::number(selected->getX()) + ", " + QString::number(selected->getY()));
        sizeLabel->setText(QString::number(selected->getW()) + ", " + QString::number(selected->getH()));
        if (selected->getType() == "Being" || selected->getType() == "Snake" || selected->getType() == "Player")
            health->setText(QString::number(static_cast<Being* >(selected)->getCurHealth()));
        else health->setText("-");
    }
    else{
        typeName->setText("");
        info->setText("");
        position->setText("");
        sizeLabel->setText("");
        health->setText("");
    }
}

void InfoWidget::paintEvent(QPaintEvent*){
    QPainter p(this);

    for (auto i: mObjects)
        i->paint(&p);
}


MainWindow::MainWindow(QWidget *parent)  //没用
    : QMainWindow(parent)
{
    setFixedSize(1350, 1000);
    //setWindowFlag(Qt::FramelessWindowHint);

    QString font;

    QFile fontFile("BulletproofBB.ttf");
    if(!fontFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open font file error";
    }

    int loadedFontID = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
    {
        font = loadedFontFamilies.at(0);
    }
    fontFile.close();

    qDebug() << font;


    mGame = new Game();
    connect(mGame, &Game::gameOver, this, &MainWindow::Gameover);
    connect(this, &MainWindow::addSomething, mGame, &Game::addSomething);
    qDebug() << "[Build] Game Core Built";


    mGameWidget = new GameWidget(this);
    mGameWidget->hide();
    mGameWidget->setGame(mGame);
    mGame->setGameWidget(mGameWidget);

    qDebug() << "[Build] GameWidget Built";

    mInfoWidget = new InfoWidget(this);
    mInfoWidget->move(1000, 0);
    mInfoWidget->hide();
    mInfoWidget->setGame(mGame);
    mGame->setInfoWidget(mInfoWidget);

    qDebug() << "[Build] InfoWidget Built";

    setWindowTitle("Jumpy - MainWindow");
    show();

    mWelWidget = new WelcomeWidget(this);
    mChoWidget = new ChooseWidget(this);
    mSetWidget = new SettingsWidget(this);
    mOptionWidget = new OptionWidget(this);
    mOverWidget = new GameoverWidget(this);
    mEditWidget = new EditToolWidget(this);
}

void MainWindow::StartGameSingle(){

    mChoWidget->hide();

    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->Initialize("QSnake-Single");
    mInfoWidget->Initialize();
    mGame->Initialize();
    qDebug() << "[Build] Game Initialized Succeessfully";

    mGameWidget->setFocus();
    mGameWidget->setMouseTracking(true);
    setMouseTracking(true);
    mGame->start();
    isStarted = true;
    mGameMode = "QSnake-Single";
}

void MainWindow::StartGameMulti(){

    mChoWidget->hide();

    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->Initialize("QSnake-Multi");
    mInfoWidget->Initialize();
    mGame->Initialize();
    qDebug() << "[Build] Game Initialized Succeessfully";

    mGameWidget->setFocus();
    mGameWidget->setMouseTracking(true);
    setMouseTracking(true);
    mGame->start();
    isStarted = true;
    mGameMode = "QSnake-Multi";
}

void MainWindow::StartGameAI(){

    mChoWidget->hide();

    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->Initialize("QSnake-AI");
    mInfoWidget->Initialize();
    mGame->Initialize();
    qDebug() << "[Build] Game Initialized Succeessfully";

    mGameWidget->setFocus();
    mGameWidget->setMouseTracking(true);
    setMouseTracking(true);
    mGame->start();
    isStarted = true;
    mGameMode = "QSnake-AI";
}

void MainWindow::StartGameLoad(){
    mChoWidget->hide();

    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->Initialize("QSnake-Load");
    mInfoWidget->Initialize();
    mGame->Initialize();
    qDebug() << "[Build] Game Initialized Succeessfully";

    mGameWidget->setFocus();
    mGameWidget->setMouseTracking(true);
    setMouseTracking(true);
    mGame->start();

    isStarted = true;
    mGameMode = "QSnake-Load";
}

void MainWindow::saveAndExit(){
    mGame->save();
    mGame->removeEverything();
    mOptionWidget->hide();
    mWelWidget->show();
    qDebug() << "[System] Game Saved Succeessfully";

    isStarted = false;
    mGameMode = "";
}

void MainWindow::ChangePause(){
    if (mGameWidget->isHidden()){
        mOptionWidget->hide();
        mGameWidget->show();
        mInfoWidget->show();
        mGameWidget->setFocus();
        mGame->unPause();
    }
    else{
        mOptionWidget->show();
        mGameWidget->hide();
        mInfoWidget->hide();
        mGame->Pause();
    }
}

void MainWindow::StartEdit(){
    setFixedSize(1700, 1000);
    mOptionWidget->hide();
    mGameWidget->show();
    mInfoWidget->show();
    mEditWidget->show();
}

void MainWindow::FinishEdit(){
    setFixedSize(1350, 1000);
    mEditWidget->hide();
    mGameWidget->hide();
    mInfoWidget->hide();
    mOptionWidget->show();
}

void MainWindow::WelToCho(){
    mWelWidget->hide();
    mChoWidget->show();
}

void MainWindow::ChoToWel(){
    mChoWidget->hide();
    mWelWidget->show();
}

void MainWindow::WelToSet(){
    mWelWidget->hide();
    mSetWidget->show();
}

void MainWindow::SetToWel(){
    mSetWidget->hide();
    mWelWidget->show();

    mSetWidget->save();
}

void MainWindow::OverToWel(){
    mOverWidget->hide();
    mWelWidget->show();
}

void MainWindow::Gameover(int sn, QColor c1, int s1, QColor c2, int s2){
    mGameWidget->hide();
    mInfoWidget->hide();
    mOverWidget->show();
    mOverWidget->setScore(sn, c1, s1, c2, s2);
    isStarted = false;
}

void MainWindow::RestartInGame(){
    mGame->removeEverything();
    mOptionWidget->hide();
    mGameWidget->Initialize(mGameMode);
    mInfoWidget->Initialize();
    mGame->Initialize();
    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->setFocus();
    mGame->unPause();
}

void MainWindow::RestartOutOfGame(){
    mOverWidget->hide();
    mGameWidget->show();
    mInfoWidget->show();
    mGameWidget->Initialize(mGameMode);
    mInfoWidget->Initialize();
    mGame->Initialize();
    mGame->start();
    mGameWidget->setFocus();
    isStarted = true;
}

void MainWindow::keyPressEvent(QKeyEvent* k){
    if (k->key() == Qt::Key_Escape && isStarted){
        ChangePause();
    }
    QMainWindow::keyPressEvent(k);
}

MainWindow::~MainWindow()
{
}



WelcomeWidget::WelcomeWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    playButton = new QPushButton("Play", this);
    settingButton = new QPushButton("SETTINGS", this);
    exitButton = new QPushButton("✖", this);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(230);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

    label = new QLabel("QSNAKE", this);
    label2 = new QLabel("519021910798 蒋哲", this);

    label->move(215, 180);
    label->resize(970, 300);
    label2->move(1000, 920);
    label2->resize(300, 50);
    label->setFont(font);
    font.setPixelSize(30);
    label2->setFont(font);

    playButton->move(325, 500);
    playButton->resize(700, 150);
    connect(playButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::WelToCho);

    settingButton->move(325, 700);
    settingButton->resize(700, 150);
    connect(settingButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::WelToSet);

    exitButton->move(1190, 0);
    exitButton->resize(160, 120);
    connect(exitButton, &QPushButton::clicked, parent, &QWidget::close);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );

    show();
}

WelcomeWidget::~WelcomeWidget(){

}

OptionWidget::OptionWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    restartButton = new QPushButton("Restart", this);
    exitButton = new QPushButton("save and exit", this);
    backButton = new QPushButton("<<<", this);
    editButton = new QPushButton("edit mode", this);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(140);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

    label = new QLabel("Options",this);
    label->move(390, 175);
    label->resize(1000, 200);
    label->setFont(font);

    restartButton->move(325, 400);
    restartButton->resize(700, 150);
    connect(restartButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::RestartInGame);

    exitButton->move(325, 600);
    exitButton->resize(700, 150);
    connect(exitButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::saveAndExit);

    backButton->move(0, 0);
    backButton->resize(160, 120);
    connect(backButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::ChangePause);

    editButton->move(325, 800);
    editButton->resize(700, 150);
    connect(editButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartEdit);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );
    hide();
}

OptionWidget::~OptionWidget(){

}

ChooseWidget::ChooseWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    singleButton = new QPushButton("Single player", this);
    multiButton = new QPushButton("Two Players", this);
    aiButton = new QPushButton("fight ai", this);
    backButton = new QPushButton("<<<", this);
    lastButton = new QPushButton("last game", this);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(140);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

    label = new QLabel("Choose Mode",this);
    label->move(180, 50);
    label->resize(1000, 200);
    label->setFont(font);

    lastButton->move(325, 250);
    lastButton->resize(700, 150);
    connect(lastButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartGameLoad);

    singleButton->move(325, 440);
    singleButton->resize(700, 150);
    connect(singleButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartGameSingle);

    multiButton->move(325, 630);
    multiButton->resize(700, 150);
    connect(multiButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartGameMulti);

    aiButton->move(325, 820);
    aiButton->resize(700, 150);
    connect(aiButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartGameAI);

    backButton->move(0, 0);
    backButton->resize(160, 120);
    connect(backButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::ChoToWel);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );
    hide();
}

ChooseWidget::~ChooseWidget(){

}

SettingsWidget::SettingsWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(140);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

    label = new QLabel("Settings",this);
    label->move(320, 50);
    label->resize(1000, 200);
    label->setFont(font);

    backButton = new QPushButton("<<<", this);
    backButton->move(0, 0);
    backButton->resize(160, 120);
    connect(backButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::SetToWel);

    mEditPanel = new QWidget(this);
    mEditPanel->resize(750, 750);
    mEditPanel->move(300, 250);
    mLayout = new QFormLayout(mEditPanel);

    sizeEdit = new QLineEdit(QString::number(SnakeSize), mEditPanel);
    mLayout->addRow("Snake Size:", sizeEdit);
    speedEdit = new QLineEdit(QString::number(SnakeSpeed), mEditPanel);
    mLayout->addRow("Snake Speed:", speedEdit);
    upEdit = new QPushButton(UpKey, mEditPanel);
    mLayout->addRow("Up Key:", upEdit);
    downEdit = new QPushButton(DownKey, mEditPanel);
    mLayout->addRow("Down Key:", downEdit);
    leftEdit = new QPushButton(LeftKey, mEditPanel);
    mLayout->addRow("Left Key:", leftEdit);
    rightEdit = new QPushButton(RightKey, mEditPanel);
    mLayout->addRow("Right Key:", rightEdit);

    mEditPanel->setLayout(mLayout);
    mEditPanel->setStyleSheet(
                "QLabel{"
                " font-family: 'Bulletproof BB';"
                " font-size: 60px;"
                "}"
                "QLineEdit{"
                " border: 0px;"
                " background: transparent;"
                " border-bottom:5px solid black;"
                " padding-left: 90px;"
                " font-family: 'Bulletproof BB';"
                " font-size: 60px;"
                "}"
                );
    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );
    hide();
}

void SettingsWidget::save(){
    SnakeSize = sizeEdit->text().toInt();
    SnakeSpeed = speedEdit->text().toInt();
    UpKey = upEdit->text();
    DownKey = downEdit->text();
    LeftKey = leftEdit->text();
    RightKey = rightEdit->text();
}

SettingsWidget::~SettingsWidget(){

}


void GameoverWidget::setScore(int scoreNum, QColor color1, int score1, QColor color2, int score2){
    if (scoreNum == 1){
        p1.setColor(QPalette::WindowText, color1);

        scoreLabel1->setPalette(p1);
        scoreLabel1->setText("SCORE:" + QString::number(score1));
        scoreLabel1->move(450, 350);
        scoreLabel1->resize(800, 200);

        scoreLabel2->hide();
    }
    else{
        p1.setColor(QPalette::WindowText, color1);
        p2.setColor(QPalette::WindowText, color2);

        scoreLabel1->setPalette(p1);
        scoreLabel1->setText(QString::number(score1));
        scoreLabel1->move(500, 350);
        scoreLabel1->resize(400, 200);

        scoreLabel2->setPalette(p2);
        scoreLabel2->setText(QString::number(score2));
        scoreLabel2->move(750, 350);
        scoreLabel2->resize(400, 200);

        scoreLabel2->show();
    }
}

GameoverWidget::GameoverWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    restartButton = new QPushButton("Restart", this);
    backButton = new QPushButton("Back to menu", this);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(140);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);


    label = new QLabel("Game Over",this);
    label->move(280, 175);
    label->resize(1000, 200);
    label->setFont(font);

    font.setPixelSize(90);
    scoreLabel1 = new QLabel("", this);
    scoreLabel1->setFont(font);

    scoreLabel2 = new QLabel("", this);
    scoreLabel2->setFont(font);

    restartButton->move(325, 500);
    restartButton->resize(700, 150);
    connect(restartButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::RestartOutOfGame);

    backButton->move(325, 700);
    backButton->resize(700, 150);
    connect(backButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::OverToWel);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );
    hide();
}

GameoverWidget::~GameoverWidget(){

}

EditToolWidget::EditToolWidget(QWidget* parent)
    :QWidget(parent){
    move(1350, 0);
    setFixedSize(350, 1000);

    QString ss = ""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 30px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 30px;"
                  "}"
                  "QLabel{font-family:Bulletproof BB;font-size:30px}"
                  ;

    finishButton = new QPushButton("FINISH", this);
    finishButton->move(25, 800);
    finishButton->resize(300, 120);
    connect(finishButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::FinishEdit);

    mLayout = new QFormLayout(this);

    block = new QPushButton("add", this);
    mLayout->addRow("Block: ", block);
    block->setStyleSheet(ss);
    connect(block, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addBlock);

    basicfood = new QPushButton("add", this);
    mLayout->addRow("Basic\n Food: ", basicfood);
    basicfood->setStyleSheet(ss);
    connect(basicfood, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addBasicFood);

    speedfood = new QPushButton("add", this);
    mLayout->addRow("SpeedUp\n Food: ", speedfood);
    speedfood->setStyleSheet(ss);
    connect(speedfood, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addSpeedFood);

    lifefood = new QPushButton("add", this);
    mLayout->addRow("LifeUp\n Food: ", lifefood);
    lifefood->setStyleSheet(ss);
    connect(lifefood, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addLifeFood);

    snakebody = new QPushButton("add", this);
    mLayout->addRow("Snake\n Body: ", snakebody);
    snakebody->setStyleSheet(ss);
    connect(snakebody, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addSnakeBody);

    generator = new QPushButton("add", this);
    mLayout->addRow("Food\n Source: ", generator);
    generator->setStyleSheet(ss);
    connect(generator, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addGenerator);

    scorefood = new QPushButton("add", this);
    mLayout->addRow("Score\n food: ", scorefood);
    scorefood->setStyleSheet(ss);
    connect(scorefood, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::addScoreFood);


    setLayout(mLayout);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 60px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 60px;"
                  "}"
                  "QLabel{font-family:Bulletproof BB;font-size:30px}"
                  );

    hide();
}

EditToolWidget::~EditToolWidget(){

}
