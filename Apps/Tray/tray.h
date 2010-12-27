#ifndef Tray_H_INCLUDED
#define Tray_H_INCLUDED

#include <QPushButton>
#include <QMainWindow>
#include <cstdio>

class Tray : public QMainWindow
{
  Q_OBJECT
  public:
    Tray(int width, int height);
  public slots:
    void startClicked();  
  private:
    QPushButton *startButton;
    int width,height;
};
#endif // Tray_H_INCLUDED
