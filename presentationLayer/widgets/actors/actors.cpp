#include "actors.h"
#include "actor.h"
#include "actorCard.h"
#include "actorService.h"
#include "ui_actors.h"

Actors::Actors(QWidget *parent) : QWidget(parent), ui(new Ui::Actors) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->actorsGrid->setColumnStretch(col, 1);
    }

    ui->sortComboBox->addItem("Name A-Z", static_cast<int>(SortMode::NameAsc));
    ui->sortComboBox->addItem("Name Z-A", static_cast<int>(SortMode::NameDesc));
    ui->sortComboBox->addItem("Most movies first", static_cast<int>(SortMode::MoviesHighLow));
    ui->sortComboBox->addItem("Fewest movies first", static_cast<int>(SortMode::MoviesLowHigh));

    connect(ui->sortComboBox, &QComboBox::currentIndexChanged, this, &Actors::onSortChanged);

    actorPage = new Actor(this);
    ui->actorPageHostLayout->addWidget(actorPage);
    connect(actorPage, &Actor::backRequested, this, &Actors::showList);
    connect(actorPage, &Actor::movieClicked, this, &Actors::movieClicked);

    showList();

    loadActors();
}

Actors::~Actors() {
    delete ui;
}

void Actors::showList() {
    ui->actorsStackedWidget->setCurrentWidget(ui->actorsListPage);
}

void Actors::openActorPage(int actorId) {
    actorPage->loadActor(actorId);
    ui->actorsStackedWidget->setCurrentWidget(ui->actorPageHost);
}

void Actors::loadActors() {
    GetActorsResponse response = ActorService::getAllActors();
    if (!response.success) {
        qDebug() << "Failed to load actors:" << response.errorMessage;
        return;
    }

    _allActors = response.actors;
    applySort();
}

void Actors::onSortChanged(int index) {
    _currentSort = static_cast<SortMode>(ui->sortComboBox->itemData(index).toInt());
    applySort();
}

void Actors::applySort() {
    QVector<ActorDto> result = _allActors;

    switch (_currentSort) {
        case SortMode::NameAsc: sortByLastNameAsc(result);
            break;
        case SortMode::NameDesc: sortByLastNameDesc(result);
            break;
        case SortMode::MoviesHighLow: sortByMoviesDesc(result);
            break;
        case SortMode::MoviesLowHigh: sortByMoviesAsc(result);
            break;
    }

    renderActors(result);
}

void Actors::sortByLastNameAsc(QVector<ActorDto> &actors) {
    int n = actors.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (actors[j].lastName.compare(actors[j + 1].lastName, Qt::CaseInsensitive) > 0) {
                ActorDto temp = actors[j];
                actors[j] = actors[j + 1];
                actors[j + 1] = temp;
            }
        }
    }
}

void Actors::sortByLastNameDesc(QVector<ActorDto> &actors) {
    int n = actors.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (actors[j].lastName.compare(actors[j + 1].lastName, Qt::CaseInsensitive) < 0) {
                ActorDto temp = actors[j];
                actors[j] = actors[j + 1];
                actors[j + 1] = temp;
            }
        }
    }
}

void Actors::sortByMoviesDesc(QVector<ActorDto> &actors) {
    int n = actors.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (actors[j].movieCount < actors[j + 1].movieCount) {
                ActorDto temp = actors[j];
                actors[j] = actors[j + 1];
                actors[j + 1] = temp;
            }
        }
    }
}

void Actors::sortByMoviesAsc(QVector<ActorDto> &actors) {
    int n = actors.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (actors[j].movieCount > actors[j + 1].movieCount) {
                ActorDto temp = actors[j];
                actors[j] = actors[j + 1];
                actors[j + 1] = temp;
            }
        }
    }
}

void Actors::renderActors(const QVector<ActorDto> &actors) {
    QLayoutItem *item;
    while ((item = ui->actorsGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    for (int i = 0; i < actors.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        ActorCard *card = new ActorCard(actors[i], this);
        connect(card, &ActorCard::clicked, this, &Actors::openActorPage);
        ui->actorsGrid->addWidget(card, row, col);
    }

    int lastRow = actors.isEmpty() ? 0 : (actors.size() - 1) / COLUMNS + 1;
    ui->actorsGrid->setRowStretch(lastRow, 1);
}
