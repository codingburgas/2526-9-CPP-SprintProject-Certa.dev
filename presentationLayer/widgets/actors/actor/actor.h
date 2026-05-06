#pragma once

#include <QWidget>
#include "actorDto.h"
#include "movieDto.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Actor;
}

QT_END_NAMESPACE

class Actor : public QWidget {
    Q_OBJECT

public:
    explicit Actor(QWidget *parent = nullptr);

    ~Actor() override;

    void loadActor(int actorId);

signals:
    void backRequested();

    void movieClicked(int movieId);

private slots:
    void on_actorBackButton_clicked();

private:
    const int POSTER_WIDTH = 220;
    const int POSTER_HEIGHT = 320;
    const int COLUMNS = 3;

    Ui::Actor *ui;
    int currentActorId = -1;

    void renderActor(const ActorDto &actor);

    void renderFilmography(const QVector<MovieDto> &movies);

    void clearFilmographyGrid();
};
