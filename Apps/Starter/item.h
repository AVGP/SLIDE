#include <QIcon>
#include <QPushButton>
#include <string>
#include <unistd.h>

class Item : public QPushButton
{
  Q_OBJECT
  public:
    Item(QWidget *parent,std::string iconfile,std::string command, int x, int y);
  public slots:
    void click();
  private:
    std::string command;
};
