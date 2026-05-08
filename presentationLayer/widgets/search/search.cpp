#include "search.h"
#include <QDebug>
#include "actorService.h"
#include "movieService.h"
#include "searchActorCard.h"
#include "searchMovieCard.h"
#include "ui_search.h"

Search::Search(QWidget *parent) : QWidget(parent), ui(new Ui::Search) {
    ui->setupUi(this);

    _debounceTimer = new QTimer(this);
    _debounceTimer->setSingleShot(true);
    _debounceTimer->setInterval(250);
    connect(_debounceTimer, &QTimer::timeout, this, &Search::renderResults);
    preloadData();
}

Search::~Search() {
    delete ui;
}

void Search::preloadData() {
    GetMoviesResponse moviesResponse = MovieService::getAllMovies();
    if (moviesResponse.success) {
        _allMovies = moviesResponse.movies;
    } else {
        qDebug() << "Search: failed to load movies:" << moviesResponse.errorMessage;
    }

    GetActorsResponse actorsResponse = ActorService::getAllActors();
    if (!actorsResponse.success) {
        qDebug() << "Search: failed to load actors:" << actorsResponse.errorMessage;
        return;
    }
    _allActors = actorsResponse.actors;

    for (int i = 0; i < _allActors.size(); i++) {
        GetActorResponse actorResponse = ActorService::getActorById(_allActors[i].id);
        QString text;
        if (actorResponse.success) {
            for (int j = 0; j < actorResponse.movies.size(); j++) {
                if (j > 0) {
                    text.append(", ");
                }
                text.append(actorResponse.movies[j].title);
            }
        }
        _allActorMoviesTexts.append(text);
    }
}

void Search::setQuery(const QString &query) {
    _currentQuery = query.trimmed();
    if (_currentQuery.isEmpty()) {
        _debounceTimer->stop();
        renderResults();
        return;
    }
    _debounceTimer->start();
}

bool Search::textMatches(const QString &text, const QString &query) {
    if (query.isEmpty()) {
        return false;
    }
    return text.contains(query, Qt::CaseInsensitive);
}

QVector<int> Search::findMatchingMovieIndexes(const QString &query) {
    QVector<int> matches;
    for (int i = 0; i < _allMovies.size(); i++) {
        const MovieDto &movie = _allMovies[i];
        if (textMatches(movie.title, query) || textMatches(movie.description, query)) {
            matches.append(i);
        }
    }
    return matches;
}

QVector<int> Search::findMatchingActorIndexes(const QString &query) {
    QVector<int> matches;
    for (int i = 0; i < _allActors.size(); i++) {
        const ActorDto &actor = _allActors[i];
        const QString fullName = actor.firstName + " " + actor.lastName;
        if (textMatches(fullName, query) || textMatches(actor.description, query) ||
            textMatches(_allActorMoviesTexts[i], query)) {
            matches.append(i);
        }
    }
    return matches;
}

void Search::clearResults() {
    QLayoutItem *item;
    while ((item = ui->resultsLayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Search::renderResults() {
    clearResults();

    ui->searchTitleLabel->setText(QString("Search results for \"%1\"").arg(_currentQuery));

    if (_currentQuery.isEmpty()) {
        QLabel *empty = new QLabel("Type to search movies and actors.", ui->scrollContent);
        empty->setObjectName("searchEmptyLabel");
        ui->resultsLayout->addWidget(empty);
        ui->resultsLayout->addStretch();
        return;
    }

    QVector<int> movieIndexes = findMatchingMovieIndexes(_currentQuery);
    QVector<int> actorIndexes = findMatchingActorIndexes(_currentQuery);

    if (movieIndexes.isEmpty() && actorIndexes.isEmpty()) {
        QLabel *empty = new QLabel(QString("No results found for \"%1\".").arg(_currentQuery), ui->scrollContent);
        empty->setObjectName("searchEmptyLabel");
        ui->resultsLayout->addWidget(empty);
        ui->resultsLayout->addStretch();
        return;
    }

    if (!movieIndexes.isEmpty()) {
        QLabel *moviesHeading = new QLabel(QString("Movies (%1)").arg(movieIndexes.size()), ui->scrollContent);
        moviesHeading->setObjectName("searchSectionLabel");
        ui->resultsLayout->addWidget(moviesHeading);

        for (int i = 0; i < movieIndexes.size(); i++) {
            const MovieDto &movie = _allMovies[movieIndexes[i]];
            SearchMovieCard *card = new SearchMovieCard(movie, _currentQuery, ui->scrollContent);
            connect(card, &SearchMovieCard::clicked, this, &Search::movieClicked);
            ui->resultsLayout->addWidget(card);
        }
    }

    if (!actorIndexes.isEmpty()) {
        QLabel *actorsHeading = new QLabel(QString("Actors (%1)").arg(actorIndexes.size()), ui->scrollContent);
        actorsHeading->setObjectName("searchSectionLabel");
        ui->resultsLayout->addWidget(actorsHeading);

        for (int i = 0; i < actorIndexes.size(); i++) {
            const int idx = actorIndexes[i];
            SearchActorCard *card = new SearchActorCard(_allActors[idx], _allActorMoviesTexts[idx], _currentQuery,
                                                        ui->scrollContent);
            connect(card, &SearchActorCard::clicked, this, &Search::actorClicked);
            ui->resultsLayout->addWidget(card);
        }
    }

    ui->resultsLayout->addStretch();
}
