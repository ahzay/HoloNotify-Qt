#include <QApplication>
#include "window.h"
#include "tubers.h"
int main(int argc, char **argv) {
    QApplication app (argc, argv);
    app.setApplicationName("HoloNotify");
    TabDialog * window = new TabDialog;
    window->show();
    window->getTrayIconMenu()->addAction("Exit Application",&app,SLOT(quit()));
    return app.exec();
} 
