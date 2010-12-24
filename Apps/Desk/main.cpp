#include "desk.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    if(argc < 4)
    {
        cout << "Too few arguments. Usage: Desk <Width> <Height> <Wallpaper>" << endl;
        return 1;
    }
    QApplication app(argc,argv);

    Desk d(atoi(argv[1]),atoi(argv[2]),argv[3]);
    
    return app.exec();

}
