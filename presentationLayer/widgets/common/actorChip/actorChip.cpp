#include "actorChip.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPixmapCache>
#include <QVBoxLayout>

ActorChip::ActorChip(const ActorDto &actor, QWidget *parent) : QFrame(parent), _actorId(actor.id) {
    setObjectName("actorChip");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setFixedSize(96, 116);

    QVBoxLayout *chipLayout = new QVBoxLayout(this);
    chipLayout->setContentsMargins(8, 8, 8, 8);
    chipLayout->setSpacing(4);
    chipLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QLabel *avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(AVATAR_SIZE, AVATAR_SIZE);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const qreal dpr = devicePixelRatioF();
    const QString cacheKey = QStringLiteral("chip:%1@%2x%3")
            .arg(actor.actorPath)
            .arg(AVATAR_SIZE)
            .arg(dpr);
    QPixmap rounded;
    if (!QPixmapCache::find(cacheKey, &rounded)) {
        QPixmap raw(actor.actorPath);
        if (!raw.isNull()) {
            const int targetPx = static_cast<int>(AVATAR_SIZE * dpr);
            QPixmap scaled = raw.scaled(targetPx, targetPx, Qt::KeepAspectRatioByExpanding,
                                        Qt::SmoothTransformation);

            rounded = QPixmap(targetPx, targetPx);
            rounded.fill(Qt::transparent);

            QPainter painter(&rounded);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            QPainterPath path;
            path.addEllipse(0, 0, targetPx, targetPx);
            painter.setClipPath(path);

            int xOffset = (targetPx - scaled.width()) / 2;
            int yOffset = (targetPx - scaled.height()) / 2;
            painter.drawPixmap(xOffset, yOffset, scaled);
            painter.end();

            rounded.setDevicePixelRatio(dpr);
            QPixmapCache::insert(cacheKey, rounded);
        }
    }
    if (!rounded.isNull()) {
        avatarLabel->setPixmap(rounded);
    }

    QLabel *nameLabel = new QLabel(actor.firstName + " " + actor.lastName, this);
    nameLabel->setObjectName("actorChipName");
    nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    nameLabel->setWordWrap(true);
    nameLabel->setFixedHeight(36);
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    chipLayout->addWidget(avatarLabel, 0, Qt::AlignHCenter);
    chipLayout->addWidget(nameLabel);
}

void ActorChip::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(_actorId);
    }

    QFrame::mouseReleaseEvent(event);
}
