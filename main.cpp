#include "Slide.h"

int main(int argc, char **argv)
{
    Slide *environment = Slide::getInstance();
    environment->startUp(false);
    environment->run();
    return 0;
}
