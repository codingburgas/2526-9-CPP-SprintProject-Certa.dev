#pragma once

#include <QFrame>
#include "actorDto.h"

class ActorChip : public QFrame {
    Q_OBJECT

public:
    explicit ActorChip(const ActorDto &actor, QWidget *parent = nullptr);

signals:
    void clicked(int actorId);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int AVATAR_SIZE = 56;

    int _actorId;
};
