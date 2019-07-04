#include "Ariadne.h"
#include <QtWidgets/QApplication>


#include "DataContainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataContainer* dataContainer = DataContainer::getInstance();
    Ariadne Window;
    // BK: dataContainer->setWindow(&Window);
    Window.show();
    return a.exec();
}
