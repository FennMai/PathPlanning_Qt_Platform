#include "pathplanning_platform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PathPlanning_Platform w;
    w.show();
    return a.exec();
}
