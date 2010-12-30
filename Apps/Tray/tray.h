#ifndef Tray_H_INCLUDED
#define Tray_H_INCLUDED

#include <QPushButton>
#include <QMainWindow>
#include <QHBoxLayout>
#include <cstdio>
#include <vector>
#include <Slide/Global.h>

typedef struct
{
    Window window;
    QPushButton *button;
} WindowButton;

class Tray : public QMainWindow
{
  Q_OBJECT
  public:
    Tray(int width, int height);
  public slots:
    void startClicked();  
    void addWindowButton(unsigned long window, QString str);
    void deleteWindowButton(unsigned long window);
  private:
    QPushButton *startButton;
    QHBoxLayout *layout;
    std::vector<WindowButton> windows;
    int width,height;
};
#endif // Tray_H_INCLUDED
