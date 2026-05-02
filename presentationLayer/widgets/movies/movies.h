#pragma once

#include <QWidget>
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

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    const int COLUMNS = 3;
    const int POSTER_HEIGHT = 260;
    const int ICON_SIZE = 14;

    Ui::Movies *ui;
    Movie *moviePage;

    void loadMovies();

    void reloadMovies();

    QWidget *createMovieCard(const MovieDto &movie);

    void openMoviePage(int movieId);
};
