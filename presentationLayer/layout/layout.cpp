#include "layout.h"
#include "ui_layout.h"

Layout::Layout(QWidget *parent) : QMainWindow(parent), ui(new Ui::Layout) {
    ui->setupUi(this);

    homePage = new Home(this);
    ui->homePageLayout->addWidget(homePage);

    moviesPage = new Movies(this);
    ui->moviesPageLayout->addWidget(moviesPage);

    on_sidebarHomeButton_clicked();
}

Layout::~Layout() {
    delete ui;
}

void Layout::setNavActive(NavPage page) {
    ui->sidebarHomeButton->setChecked(page == NavPage::Home);
    ui->sidebarMoviesButton->setChecked(page == NavPage::Movies);
    ui->sidebarLoginButton->setChecked(page == NavPage::Login);
    ui->sidebarSettingsButton->setChecked(page == NavPage::Settings);
}

void Layout::on_sidebarHomeButton_clicked() {
    setNavActive(NavPage::Home);
    ui->stackedWidget->setCurrentWidget(ui->homeStackPage);
}

void Layout::on_sidebarMoviesButton_clicked() {
    setNavActive(NavPage::Movies);
    ui->stackedWidget->setCurrentWidget(ui->moviesStackPage);
}

void Layout::on_sidebarLoginButton_clicked() {
    setNavActive(NavPage::Login);
}

void Layout::on_sidebarSettingsButton_clicked() {
    setNavActive(NavPage::Settings);
}
