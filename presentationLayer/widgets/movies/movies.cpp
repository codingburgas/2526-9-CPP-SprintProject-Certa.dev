#include "movies.h"
#include "ui_movies.h"

Movies::Movies(QWidget *parent) : QWidget(parent), ui(new Ui::Movies) {
    ui->setupUi(this);
}

Movies::~Movies() {
    delete ui;
}
