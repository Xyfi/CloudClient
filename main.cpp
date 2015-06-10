#include "cloud9.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Cloud9 client;

    return a.exec();
}
