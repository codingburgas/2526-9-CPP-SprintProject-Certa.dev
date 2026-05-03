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

private:
    const int COLUMNS = 3;

    Ui::Movies *ui;
    Movie *moviePage;
    QVector<MovieDto> _allMovies;
    QString _currentQuery;

    void loadMovies();

    void reloadMovies();

    void renderMovies(const QVector<MovieDto> &movies);
};
