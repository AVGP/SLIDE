#include "item.h"

Item::Item(QWidget *parent,std::string iconfile,std::string command, int x, int y) : QPushButton(parent)
{
    this->move(x,y);
    this->setIcon(QIcon(QString(iconfile.c_str())));
    this->resize(48,48);
    this->command = command;
    connect(this,SIGNAL(clicked(bool)),this,SLOT(click()));
}

void Item::click()
{
    if(fork() == 0)
    {
        execl(command.c_str(),command.c_str(),NULL);
    }
}