#include <QApplication>
#include <iostream>
#include "starter.h"

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        std::cout << "Usage: Starter <Width> <Height>" << std::endl;
        return 1;
    }
    QApplication app(argc,argv);
    Starter *s = new Starter(atoi(argv[1]),atoi(argv[2]));
    s->show();
    return app.exec();
}
