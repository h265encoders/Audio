#include <QCoreApplication>
#include "Link.h"
#include "Audio.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Link::init();
    Audio audio;
    audio.init();
    audio.startStream();

    return a.exec();
}

