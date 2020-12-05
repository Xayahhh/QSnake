/*
 * main函数在这里，没事不用动这个文件。
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */


#include "Game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;

    return a.exec();
}
