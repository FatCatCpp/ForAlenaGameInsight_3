#include <QCoreApplication>
#include <QTextCodec>
#include "tu.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp866"));

    tu client;

    return a.exec();
}

