#pragma once

#include <QTimer>
#include <QVector>
#include <QWidget>
#include "actorDto.h"
#include "movieDto.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Search;
}

QT_END_NAMESPACE

class Search : public QWidget {
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);

    ~Search() override;

    void setQuery(const QString &query);

signals:
    void movieClicked(int movieId);

    void actorClicked(int actorId);

private:
    Ui::Search *ui;
    QString _currentQuery;
    QTimer *_debounceTimer;

    QVector<MovieDto> _allMovies;
    QVector<ActorDto> _allActors;
    QVector<QString> _allActorMoviesTexts;

    void preloadData();

    bool textMatches(const QString &text, const QString &query);

    QVector<int> findMatchingMovieIndexes(const QString &query);

    QVector<int> findMatchingActorIndexes(const QString &query);

    void renderResults();

    void clearResults();
};
