#include <QThread>
#include "tray.h"
#include <Slide/Slideconnection.h>
#include <Slide/Slidewindow.h>
#include <cstring>

class CtrlThread : public QThread
{
  Q_OBJECT
  public:
    CtrlThread(Tray *t);
    void run();
  signals:
    void addedWindow(unsigned long window, QString str);
    void deletedWindow(unsigned long window);
  private:
    Tray *tray;
    SlideConnection *conn;
};
