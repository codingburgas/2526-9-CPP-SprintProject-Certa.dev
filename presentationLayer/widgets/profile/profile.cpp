#include "profile.h"
#include "ui_profile.h"

Profile::Profile(QWidget *parent) : QWidget(parent), ui(new Ui::Profile) {
    ui->setupUi(this);
}

Profile::~Profile() {
    delete ui;
}

void Profile::on_logoutButton_clicked() {
    emit logoutRequested();
}
