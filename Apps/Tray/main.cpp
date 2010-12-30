#include "tray.h"
#include "ctrlthread.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        cout << "Too few arguments. Usage: Tray <Width> <Height>" << endl;
        return 1;
    }
    QApplication app(argc,argv);

    Tray t(atoi(argv[1]),atoi(argv[2]));
    CtrlThread ctrl(&t);
    ctrl.start();
    
    return app.exec();

}
