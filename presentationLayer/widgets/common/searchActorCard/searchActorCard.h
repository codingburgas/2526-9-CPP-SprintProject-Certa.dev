#pragma once

#include <QFrame>
#include "actorDto.h"

class SearchActorCard : public QFrame {
    Q_OBJECT

public:
    explicit SearchActorCard(const ActorDto &actor, const QString &moviesText, const QString &query,
                             QWidget *parent = nullptr);

signals:
    void clicked(int actorId);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int POSTER_HEIGHT = 160;

    int _actorId;

    QString highlight(const QString &text, const QString &query);
};
