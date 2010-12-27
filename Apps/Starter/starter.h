#include <QWidget>
#include <vector>
#include <string>
#include <fstream>
#include "item.h"

class Starter : public QWidget
{
  public:
    Starter(int width, int height);
  private:
    std::vector<Item *> items;
};
