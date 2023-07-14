#include <QApplication>

#include "utils.h"
#include "windowinterface.h"


void task_manager(){
    while(1);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Création du vecteur de thread

    WindowInterface::initialize(NB_MINES, NB_FACTORIES, NB_WHOLESALER);
    auto interface = new WindowInterface();

    Mine::set_interface(interface);
    Factory::set_interface(interface);
    Wholesale::set_interface(interface);

    Utils utils = Utils(NB_MINES, NB_FACTORIES, NB_WHOLESALER);

    return a.exec();
}
