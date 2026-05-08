#pragma once

#include <QFrame>
#include "movieDto.h"

class SearchMovieCard : public QFrame {
    Q_OBJECT

public:
    explicit SearchMovieCard(const MovieDto &movie, const QString &query, QWidget *parent = nullptr);

signals:
    void clicked(int movieId);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int POSTER_HEIGHT = 160;
    const int ICON_SIZE = 14;

    int _movieId;

    QString highlight(const QString &text, const QString &query);
};
