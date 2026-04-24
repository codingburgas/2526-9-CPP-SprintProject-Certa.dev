#include <QApplication>
#include "layout.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Layout layout;
    layout.show();

    return app.exec();
}
