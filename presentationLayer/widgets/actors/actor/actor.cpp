#include "actor.h"
#include <QPixmap>
#include <QScrollBar>
#include "actorService.h"
#include "movieCard.h"
#include "ui_actor.h"

Actor::Actor(QWidget *parent) : QWidget(parent), ui(new Ui::Actor) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->filmographyGrid->setColumnStretch(col, 1);
    }
}

Actor::~Actor() {
    delete ui;
}

void Actor::loadActor(int actorId) {
    currentActorId = actorId;

    GetActorResponse response = ActorService::getActorById(actorId);
    if (!response.success) {
        ui->actorNameLabel->setText(response.errorMessage);
        return;
    }

    renderActor(response.actor);
    renderFilmography(response.movies);

    ui->actorScrollArea->verticalScrollBar()->setValue(0);
}

void Actor::renderActor(const ActorDto &actor) {
    ui->actorNameLabel->setText(actor.firstName + " " + actor.lastName);
    ui->actorMetaLabel->setText(QString("%1 movies").arg(actor.movieCount));
    ui->actorDescriptionLabel->setText(actor.description);

    QPixmap poster(actor.actorPath);
    if (!poster.isNull()) {
        ui->actorPosterLabel->setPixmap(poster.scaled(POSTER_WIDTH, POSTER_HEIGHT, Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation));
    } else {
        ui->actorPosterLabel->setPixmap(QPixmap());
    }
}

void Actor::renderFilmography(const QVector<MovieDto> &movies) {
    clearFilmographyGrid();

    if (movies.isEmpty()) {
        ui->filmographyEmptyLabel->setText("No movies on file for this actor.");
        ui->filmographyEmptyLabel->setVisible(true);
        return;
    }

    ui->filmographyEmptyLabel->setVisible(false);

    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Actor::movieClicked);
        ui->filmographyGrid->addWidget(card, row, col);
    }

    int lastRow = movies.isEmpty() ? 0 : (movies.size() - 1) / COLUMNS + 1;
    ui->filmographyGrid->setRowStretch(lastRow, 1);
}

void Actor::clearFilmographyGrid() {
    QLayoutItem *item;
    while ((item = ui->filmographyGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Actor::on_actorBackButton_clicked() {
    emit backRequested();
}
