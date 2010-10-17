#include "Slide.h"

int main(int argc, char **argv)
{
    Slide *environment = Slide::getInstance();
    environment->startUp(true);
    environment->run();
    return 0;
}
