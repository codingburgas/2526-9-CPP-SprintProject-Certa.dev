#include "home.h"
#include <QRandomGenerator>
#include "actorCard.h"
#include "actorService.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_home.h"
#include "userSession.h"

Home::Home(QWidget *parent) : QWidget(parent), ui(new Ui::Home) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->recommendationsGrid->setColumnStretch(col, 1);
        ui->favoritesGrid->setColumnStretch(col, 1);
        ui->actorsGrid->setColumnStretch(col, 1);
    }

    refresh();
}

Home::~Home() {
    delete ui;
}

void Home::refresh() {
    if (!UserSession::instance().isLoggedIn()) {
        ui->homeStackedWidget->setCurrentWidget(ui->guestPage);
        return;
    }

    ui->homeStackedWidget->setCurrentWidget(ui->signedInPage);
    loadRecommendations();
    loadFavorites();
    loadActors();
}

void Home::clearRecommendationsGrid() {
    QLayoutItem *item;
    while ((item = ui->recommendationsGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Home::clearFavoritesGrid() {
    QLayoutItem *item;
    while ((item = ui->favoritesGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Home::loadRecommendations() {
    clearRecommendationsGrid();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser) {
        return;
    }

    GetMoviesResponse response = MovieService::getRecommendedMovies(currentUser->username, PREVIEW_LIMIT);
    if (!response.success) {
        qDebug() << "Failed to load recommendations:" << response.errorMessage;
        ui->recommendationsStack->setCurrentWidget(ui->recommendationsEmptyPage);
        return;
    }

    if (response.movies.isEmpty()) {
        ui->recommendationsStack->setCurrentWidget(ui->recommendationsEmptyPage);
        return;
    }

    ui->recommendationsStack->setCurrentWidget(ui->recommendationsGridPage);

    const QVector<MovieDto> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Home::movieClicked);
        ui->recommendationsGrid->addWidget(card, 0, i);
    }
}

void Home::loadFavorites() {
    clearFavoritesGrid();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser) {
        return;
    }

    GetMoviesResponse response = MovieService::getFavoriteMovies(currentUser->username);
    if (!response.success) {
        qDebug() << "Failed to load favorites:" << response.errorMessage;
        ui->favoritesStack->setCurrentWidget(ui->favoritesEmptyPage);
        return;
    }

    if (response.movies.isEmpty()) {
        ui->favoritesStack->setCurrentWidget(ui->favoritesEmptyPage);
        return;
    }

    ui->favoritesStack->setCurrentWidget(ui->favoritesGridPage);

    const QVector<MovieDto> movies = response.movies;
    const int count = movies.size() > PREVIEW_LIMIT ? PREVIEW_LIMIT : movies.size();

    for (int i = 0; i < count; i++) {
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Home::movieClicked);
        ui->favoritesGrid->addWidget(card, 0, i);
    }
}

void Home::clearActorsGrid() {
    QLayoutItem *item;
    while ((item = ui->actorsGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Home::loadActors() {
    clearActorsGrid();

    GetActorsResponse response = ActorService::getAllActors();
    if (!response.success) {
        qDebug() << "Failed to load actors:" << response.errorMessage;
        ui->actorsStack->setCurrentWidget(ui->actorsEmptyPage);
        return;
    }

    if (response.actors.isEmpty()) {
        ui->actorsStack->setCurrentWidget(ui->actorsEmptyPage);
        return;
    }

    ui->actorsStack->setCurrentWidget(ui->actorsGridPage);

    QVector<ActorDto> actors = response.actors;
    const int count = actors.size() > PREVIEW_LIMIT ? PREVIEW_LIMIT : actors.size();

    for (int i = 0; i < count; i++) {
        int j = i + QRandomGenerator::global()->bounded(actors.size() - i);
        ActorDto temp = actors[i];
        actors[i] = actors[j];
        actors[j] = temp;

        ActorCard *card = new ActorCard(actors[i], this);
        connect(card, &ActorCard::clicked, this, &Home::actorClicked);
        ui->actorsGrid->addWidget(card, 0, i);
    }
}

void Home::on_guestSignInButton_clicked() {
    emit signInRequested();
}

void Home::on_exploreMoreButton_clicked() {
    emit exploreRecommendationsRequested();
}

void Home::on_openFavoritesButton_clicked() {
    emit openFavoritesRequested();
}

void Home::on_exploreActorsButton_clicked() {
    emit exploreActorsRequested();
}
