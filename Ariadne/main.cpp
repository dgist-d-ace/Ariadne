#include "Ariadne.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ariadne Window;
    Window.show();
    return a.exec();
}
