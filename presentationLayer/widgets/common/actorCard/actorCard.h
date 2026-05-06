#pragma once

#include <QFrame>
#include "actorDto.h"

class ActorCard : public QFrame {
    Q_OBJECT

public:
    explicit ActorCard(const ActorDto &actor, QWidget *parent = nullptr);

signals:
    void clicked(int actorId);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int POSTER_HEIGHT = 260;
    const int ICON_SIZE = 14;

    int _actorId;
};
