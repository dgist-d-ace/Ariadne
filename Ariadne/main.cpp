#include "Ariadne.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ariadne Window;
	Window.setWindowState(Qt::WindowMaximized);
    Window.show();
    return a.exec();
}
