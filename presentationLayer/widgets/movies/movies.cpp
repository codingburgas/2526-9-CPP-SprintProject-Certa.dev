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

    ui->sortComboBox->addItem("Default", static_cast<int>(SortMode::Default));
    ui->sortComboBox->addItem("Highest rated first", static_cast<int>(SortMode::RatingHighLow));
    ui->sortComboBox->addItem("Lowest rated first", static_cast<int>(SortMode::RatingLowHigh));
    ui->sortComboBox->addItem("Newest first", static_cast<int>(SortMode::YearNewOld));
    ui->sortComboBox->addItem("Oldest first", static_cast<int>(SortMode::YearOldNew));

    populateGenreFilter();

    connect(ui->sortComboBox, &QComboBox::currentIndexChanged, this, &Movies::onSortChanged);
    connect(ui->genreComboBox, &QComboBox::currentTextChanged, this, &Movies::onGenreChanged);

    moviePage = new Movie(this);
    ui->moviePageHostLayout->addWidget(moviePage);
    connect(moviePage, &Movie::backRequested, this, &Movies::showList);
    connect(moviePage, &Movie::reviewSubmitted, this, &Movies::reloadMovies);
    connect(moviePage, &Movie::actorClicked, this, &Movies::actorClicked);

    showList();

    loadMovies();
}

void Movies::populateGenreFilter() {
    ui->genreComboBox->addItem("All genres");

    MovieResponse response = MovieService::getAllGenres();
    if (!response.success) {
        qDebug() << "Failed to load genres:" << response.errorMessage;
        return;
    }

    for (int i = 0; i < response.data.size(); i++) {
        ui->genreComboBox->addItem(response.data[i]);
    }
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
    applyFilters();
}

void Movies::reloadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    _allMovies = response.movies;
    applyFilters();
}

void Movies::onSortChanged(int index) {
    _currentSort = static_cast<SortMode>(ui->sortComboBox->itemData(index).toInt());
    applyFilters();
}

void Movies::onGenreChanged(const QString &genre) {
    _currentGenre = (genre == "All genres") ? "" : genre;
    applyFilters();
}

void Movies::applyFilters() {
    QVector<MovieDto> result;
    for (int i = 0; i < _allMovies.size(); i++) {
        const MovieDto movie = _allMovies[i];

        if (!_currentGenre.isEmpty() &&
            !movie.genres.contains(_currentGenre, Qt::CaseInsensitive)) {
            continue;
        }

        result.append(movie);
    }

    switch (_currentSort) {
        case SortMode::RatingHighLow: MovieService::sortByRatingDesc(result);
            break;
        case SortMode::RatingLowHigh: MovieService::sortByRatingAsc(result);
            break;
        case SortMode::YearNewOld: MovieService::sortByYearDesc(result);
            break;
        case SortMode::YearOldNew: MovieService::sortByYearAsc(result);
            break;
        case SortMode::Default: break;
    }

    renderMovies(result);
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
