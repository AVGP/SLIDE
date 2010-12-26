#include "starter.h" 

Starter::Starter(int width, int height) : QWidget()
{
    this->resize(500,500);//width-20,height-20);
    this->move(10,10);
    setWindowTitle("Starter");
    //Get the config-file:
    char filenamebuffer[512];
    snprintf(filenamebuffer,512,"%s/.slidestarter",getenv("HOME"));
    std::ifstream conf(filenamebuffer);
    if(conf.is_open())
    {
        int x=5,y=5;
        while(!conf.eof())
        {
            std::string iconfile,cmd;
            conf >> iconfile;
            conf >> cmd;
            if(iconfile.length() > 0)
            {
                Item *item = new Item(this,iconfile,cmd,x,y);
                items.push_back(item);
                if(x < width-58) x+=58;
                else
                {
                    x  =  5;
                    y += 58;
                }
            }
        }
    }

}
