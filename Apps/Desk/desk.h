#ifndef Desk_H_INCLUDED
#define Desk_H_INCLUDED

#include <QPixmap>
#include <QLabel>
#include <QMainWindow>
#include <QString>

class Desk : public QMainWindow
{
    public:
        Desk(int width, int height, QString bgFile);
    private:
        int width,height;
        QPixmap *pixmap;
        QLabel  *wallpaper;
};
#endif // Desk_H_INCLUDED
