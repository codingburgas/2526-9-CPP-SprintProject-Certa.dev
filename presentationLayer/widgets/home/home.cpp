#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent) : QWidget(parent), ui(new Ui::Home) {
    ui->setupUi(this);
}

Home::~Home() {
    delete ui;
}
