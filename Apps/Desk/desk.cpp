#include "desk.h"

Desk::Desk(int width, int height,QString bgFile) : QWidget()
{
    this->setWindowTitle("__SLIDE__Desktop");
    this->move(0,40);
    this->resize(width,height-40);
    this->width  = width;
    this->height = height-40;
    
    this->pixmap    = new QPixmap(bgFile);
    this->wallpaper = new QLabel(this);
    this->wallpaper->setPixmap(pixmap->scaled(this->size()));
    this->wallpaper->resize(this->size());
    show();
}
