#pragma once

#include <QWidget>
#include <QVector>
#include "movie.h"
#include "movieDto.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Movies;
}

QT_END_NAMESPACE

class Movies : public QWidget {
    Q_OBJECT

public:
    explicit Movies(QWidget *parent = nullptr);

    ~Movies() override;

    void showList();

    void openMoviePage(int movieId);

public slots:
    void filterMovies(const QString &query);

private slots:
    void onSortChanged(int index);

    void onGenreChanged(const QString &genre);

private:
    const int COLUMNS = 3;

    enum class SortMode {
        Default,
        RatingHighLow,
        RatingLowHigh,
        YearNewOld,
        YearOldNew
    };

    Ui::Movies *ui;
    Movie *moviePage;
    QVector<MovieDto> _allMovies;
    QString _currentQuery;
    QString _currentGenre;
    SortMode _currentSort = SortMode::Default;

    void loadMovies();

    void reloadMovies();

    void populateGenreFilter();

    void applyFilters();

    void sortByRatingDesc(QVector<MovieDto> &movies);

    void sortByRatingAsc(QVector<MovieDto> &movies);

    void sortByYearDesc(QVector<MovieDto> &movies);

    void sortByYearAsc(QVector<MovieDto> &movies);

    void renderMovies(const QVector<MovieDto> &movies);
};
