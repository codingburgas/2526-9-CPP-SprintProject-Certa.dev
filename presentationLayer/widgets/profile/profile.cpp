#include "profile.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_profile.h"
#include "userSession.h"

Profile::Profile(QWidget *parent) : QWidget(parent), ui(new Ui::Profile) {
    ui->setupUi(this);

    for (int col = 0; col < FAVORITES_PREVIEW_COLUMNS; col++) {
        ui->favoritesPreviewGrid->setColumnStretch(col, 1);
    }
}

Profile::~Profile() {
    delete ui;
}

void Profile::clearFavoritesPreview() {
    QLayoutItem *item;
    while ((item = ui->favoritesPreviewGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Profile::clearGenresChips() {
    QLayout *layout = ui->genresChipsRow->layout();
    QLayoutItem *item;
    while (layout->count() > 0) {
        item = layout->takeAt(0);
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Profile::refresh() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser) {
        ui->usernameLabel->setText("Guest");
        ui->avatarLabel->setText("?");
        ui->favoritesStatValue->setText("0");
        ui->genresStatValue->setText("0");
        ui->recommendationsStatValue->setText("0");
        clearGenresChips();
        clearFavoritesPreview();
        ui->genresStack->setCurrentWidget(ui->genresEmptyPage);
        ui->favoritesPreviewStack->setCurrentWidget(ui->favoritesPreviewEmptyPage);
        return;
    }

    const QString username = currentUser->username;
    ui->usernameLabel->setText(username);
    const QString initial = username.isEmpty() ? "?" : username.left(1).toUpper();
    ui->avatarLabel->setText(initial);

    ui->genresStatValue->setText(QString::number(currentUser->interestsGenres.size()));

    clearGenresChips();
    QHBoxLayout *chipsLayout = qobject_cast<QHBoxLayout *>(ui->genresChipsRow->layout());
    if (currentUser->interestsGenres.isEmpty()) {
        ui->genresStack->setCurrentWidget(ui->genresEmptyPage);
    } else {
        ui->genresStack->setCurrentWidget(ui->genresChipsPage);
        for (int i = 0; i < currentUser->interestsGenres.size(); i++) {
            QLabel *chip = new QLabel(currentUser->interestsGenres[i], ui->genresChipsRow);
            chip->setStyleSheet(
                "background-color: #181818; color: #FFFFFF; border-radius: 12px; padding: 6px 12px; font-size: 12px;");
            chipsLayout->addWidget(chip);
        }
        chipsLayout->addStretch(1);
    }

    GetMoviesResponse favoritesResponse = MovieService::getFavoriteMovies(username);
    if (!favoritesResponse.success) {
        qDebug() << "Profile: failed to load favorites:" << favoritesResponse.errorMessage;
    }

    QVector<MovieDto> favorites = favoritesResponse.movies;
    int favoritesCount = favorites.size();
    ui->favoritesStatValue->setText(QString::number(favoritesCount));

    GetMoviesResponse recommendationsResponse = MovieService::getRecommendedMovies(username);
    if (!recommendationsResponse.success) {
        qDebug() << "Profile: failed to load recommendations:" << recommendationsResponse.errorMessage;
    }

    int recommendationsCount = recommendationsResponse.movies.size();
    ui->recommendationsStatValue->setText(QString::number(recommendationsCount));

    clearFavoritesPreview();
    if (favorites.isEmpty()) {
        ui->favoritesPreviewStack->setCurrentWidget(ui->favoritesPreviewEmptyPage);
    } else {
        ui->favoritesPreviewStack->setCurrentWidget(ui->favoritesPreviewGridPage);
        int shown = favorites.size();
        if (shown > FAVORITES_PREVIEW_LIMIT) {
            shown = FAVORITES_PREVIEW_LIMIT;
        }
        for (int i = 0; i < shown; i++) {
            int row = i / FAVORITES_PREVIEW_COLUMNS;
            int col = i % FAVORITES_PREVIEW_COLUMNS;
            MovieCard *card = new MovieCard(favorites[i], this);
            connect(card, &MovieCard::clicked, this, &Profile::movieClicked);
            ui->favoritesPreviewGrid->addWidget(card, row, col);
        }
        int lastRow = (shown - 1) / FAVORITES_PREVIEW_COLUMNS + 1;
        ui->favoritesPreviewGrid->setRowStretch(lastRow, 1);
    }
}

void Profile::on_logoutButton_clicked() {
    emit logoutRequested();
}

void Profile::on_editProfileButton_clicked() {
    emit editProfileRequested();
}

void Profile::on_viewAllFavoritesButton_clicked() {
    emit viewFavoritesRequested();
}

void Profile::on_openSettingsLink_clicked() {
    emit editProfileRequested();
}
