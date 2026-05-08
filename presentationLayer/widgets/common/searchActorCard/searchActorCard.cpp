#include "searchActorCard.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QVBoxLayout>

SearchActorCard::SearchActorCard(const ActorDto &actor, const QString &moviesText, const QString &query,
                                 QWidget *parent) : QFrame(parent) {
    _actorId = actor.id;
    setObjectName("searchActorCard");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QHBoxLayout *cardLayout = new QHBoxLayout(this);
    cardLayout->setContentsMargins(12, 12, 12, 12);
    cardLayout->setSpacing(14);

    QWidget *infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color: transparent;");
    infoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(6);

    QLabel *nameLabel = new QLabel(infoWidget);
    nameLabel->setObjectName("searchActorCardName");
    nameLabel->setTextFormat(Qt::RichText);
    nameLabel->setText(highlight(actor.firstName + " " + actor.lastName, query));
    nameLabel->setWordWrap(true);

    QLabel *moviesLabel = new QLabel(infoWidget);
    moviesLabel->setObjectName("searchActorCardMovies");
    moviesLabel->setTextFormat(Qt::RichText);
    moviesLabel->setText(QStringLiteral("Movies: ") + highlight(moviesText, query));
    moviesLabel->setWordWrap(true);

    QLabel *descLabel = new QLabel(infoWidget);
    descLabel->setObjectName("searchActorCardDescription");
    descLabel->setTextFormat(Qt::RichText);
    descLabel->setText(highlight(actor.description, query));
    descLabel->setWordWrap(true);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(moviesLabel);
    infoLayout->addWidget(descLabel);
    infoLayout->addStretch();

    QLabel *posterLabel = new QLabel(this);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    posterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const qreal dpr = devicePixelRatioF();
    const QString posterCacheKey = QStringLiteral("searchActor:%1@%2x%3")
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
        posterLabel->setFixedWidth(static_cast<int>(pixmap.width() / dpr));
    }

    cardLayout->addWidget(infoWidget, 1);
    cardLayout->addWidget(posterLabel, 0, Qt::AlignTop);
}

QString SearchActorCard::highlight(const QString &text, const QString &query) {
    if (query.isEmpty()) {
        return text;
    }

    QString result;
    const QString lowerText = text.toLower();
    const QString lowerQuery = query.toLower();
    const int qLen = lowerQuery.size();

    int i = 0;
    while (i < text.size()) {
        int found = -1;
        if (i + qLen <= lowerText.size()) {
            for (int j = i; j <= lowerText.size() - qLen; j++) {
                bool match = true;
                for (int k = 0; k < qLen; k++) {
                    if (lowerText[j + k] != lowerQuery[k]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    found = j;
                    break;
                }
            }
        }

        if (found == -1) {
            result.append(text.mid(i));
            break;
        }

        if (found > i) {
            result.append(text.mid(i, found - i));
        }
        result.append("<span style=\"background-color: rgba(174, 26, 25, 120); color: #FFFFFF;\">");
        result.append(text.mid(found, qLen));
        result.append("</span>");
        i = found + qLen;
    }

    return result;
}

void SearchActorCard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(_actorId);
    }
    QFrame::mouseReleaseEvent(event);
}
