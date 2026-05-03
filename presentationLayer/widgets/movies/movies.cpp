#include "movies.h"
#include "movie.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_movies.h"

Movies::Movies(QWidget *parent) : QWidget(parent), ui(new Ui::Movies) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->moviesGrid->setColumnStretch(col, 1);
    }

    moviePage = new Movie(this);
    ui->moviePageHostLayout->addWidget(moviePage);
    connect(moviePage, &Movie::backRequested, this, &Movies::showList);
    connect(moviePage, &Movie::reviewSubmitted, this, &Movies::reloadMovies);

    showList();

    loadMovies();
}

Movies::~Movies() {
    delete ui;
}

void Movies::showList() {
    ui->moviesStackedWidget->setCurrentWidget(ui->moviesListPage);
}

void Movies::openMoviePage(int movieId) {
    moviePage->loadMovie(movieId);
    ui->moviesStackedWidget->setCurrentWidget(ui->moviePageHost);
}

void Movies::loadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    _allMovies = response.movies;
    renderMovies(_allMovies);
}

void Movies::reloadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    _allMovies = response.movies;
    filterMovies(_currentQuery);
}

void Movies::filterMovies(const QString &query) {
    _currentQuery = query;

    if (query.isEmpty()) {
        renderMovies(_allMovies);
        return;
    }

    QVector<MovieDto> filtered;
    for (int i = 0; i < _allMovies.size(); i++) {
        MovieDto movie = _allMovies[i];
        if (movie.title.toLower().contains(query.toLower())) {
            filtered.append(movie);
        }
    }
    renderMovies(filtered);
}

void Movies::renderMovies(const QVector<MovieDto> &movies) {
    QLayoutItem *item;
    while ((item = ui->moviesGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Movies::openMoviePage);
        ui->moviesGrid->addWidget(card, row, col);
    }

    int lastRow = movies.isEmpty() ? 0 : (movies.size() - 1) / COLUMNS + 1;
    ui->moviesGrid->setRowStretch(lastRow, 1);
}
