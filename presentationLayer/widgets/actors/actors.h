#pragma once

#include <QWidget>
#include <QVector>
#include "actor.h"
#include "actorDto.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Actors;
}

QT_END_NAMESPACE

class Actors : public QWidget {
    Q_OBJECT

public:
    explicit Actors(QWidget *parent = nullptr);

    ~Actors() override;

    void showList();

    void openActorPage(int actorId);

signals:
    void movieClicked(int movieId);

private slots:
    void onSortChanged(int index);

private:
    const int COLUMNS = 3;

    enum class SortMode {
        NameAsc,
        NameDesc,
        MoviesHighLow,
        MoviesLowHigh
    };

    Ui::Actors *ui;
    Actor *actorPage;
    QVector<ActorDto> _allActors;
    SortMode _currentSort = SortMode::NameAsc;

    void loadActors();

    void applySort();

    void renderActors(const QVector<ActorDto> &actors);
};
