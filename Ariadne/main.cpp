#include "Ariadne.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ariadne Window;
    // BK: dataContainer->setWindow(&Window);
    Window.show();
    return a.exec();
}
