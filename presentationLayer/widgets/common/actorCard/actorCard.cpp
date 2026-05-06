#include "actorCard.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QVBoxLayout>

ActorCard::ActorCard(const ActorDto &actor, QWidget *parent) : QFrame(parent), _actorId(actor.id) {
    setObjectName("actorCard");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);

    QVBoxLayout *cardLayout = new QVBoxLayout(this);
    cardLayout->setContentsMargins(8, 8, 8, 0);
    cardLayout->setSpacing(0);

    QLabel *posterLabel = new QLabel(this);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    posterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const qreal dpr = devicePixelRatioF();
    const QString posterCacheKey = QStringLiteral("actor:%1@%2x%3")
            .arg(actor.actorPath)
            .arg(POSTER_HEIGHT)
            .arg(dpr);
    QPixmap pixmap;
    if (!QPixmapCache::find(posterCacheKey, &pixmap)) {
        QPixmap raw(actor.actorPath);
        if (!raw.isNull()) {
            pixmap = raw.scaledToHeight(static_cast<int>(POSTER_HEIGHT * dpr), Qt::SmoothTransformation);
            pixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(posterCacheKey, pixmap);
        }
    }
    if (!pixmap.isNull()) {
        posterLabel->setPixmap(pixmap);
    }

    QWidget *infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color: transparent;");
    infoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(10, 8, 10, 10);
    infoLayout->setSpacing(6);

    QLabel *nameLabel = new QLabel(actor.firstName + " " + actor.lastName, infoWidget);
    nameLabel->setObjectName("actorCardName");
    nameLabel->setWordWrap(true);

    QWidget *statsRow = new QWidget(infoWidget);
    statsRow->setStyleSheet("background-color: transparent;");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsRow);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(4);

    QLabel *moviesIcon = new QLabel(statsRow);
    const QString moviesCacheKey = QStringLiteral("icon:movies@%1x%2").arg(ICON_SIZE).arg(dpr);
    QPixmap moviesPixmap;
    if (!QPixmapCache::find(moviesCacheKey, &moviesPixmap)) {
        QPixmap rawMovies(":/icons/movies.svg");
        if (!rawMovies.isNull()) {
            moviesPixmap = rawMovies.scaled(static_cast<int>(ICON_SIZE * dpr), static_cast<int>(ICON_SIZE * dpr),
                                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
            moviesPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(moviesCacheKey, moviesPixmap);
        }
    }
    if (!moviesPixmap.isNull()) {
        moviesIcon->setPixmap(moviesPixmap);
    }
    moviesIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *movieCountLabel = new QLabel(QString("%1 movies").arg(actor.movieCount), statsRow);
    movieCountLabel->setObjectName("actorCardMovieCount");

    statsLayout->addWidget(moviesIcon);
    statsLayout->addWidget(movieCountLabel);
    statsLayout->addStretch();

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(statsRow);

    cardLayout->addWidget(posterLabel);
    cardLayout->addWidget(infoWidget);
}

void ActorCard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(_actorId);
    }
    QFrame::mouseReleaseEvent(event);
}
