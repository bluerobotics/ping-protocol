#include <QApplication>
#include <QDebug>

#include "ping.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Ping ping;
    return app.exec();
}