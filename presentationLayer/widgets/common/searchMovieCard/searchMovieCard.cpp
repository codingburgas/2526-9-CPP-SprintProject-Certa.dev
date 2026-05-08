#include "searchMovieCard.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QVBoxLayout>

SearchMovieCard::SearchMovieCard(const MovieDto &movie, const QString &query, QWidget *parent) : QFrame(parent) {
    _movieId = movie.id;
    setObjectName("searchMovieCard");
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

    QLabel *titleLabel = new QLabel(infoWidget);
    titleLabel->setObjectName("searchMovieCardTitle");
    titleLabel->setTextFormat(Qt::RichText);
    titleLabel->setText(highlight(movie.title, query));
    titleLabel->setWordWrap(true);

    QWidget *metaRow = new QWidget(infoWidget);
    metaRow->setStyleSheet("background-color: transparent;");
    QHBoxLayout *metaLayout = new QHBoxLayout(metaRow);
    metaLayout->setContentsMargins(0, 0, 0, 0);
    metaLayout->setSpacing(10);

    QLabel *yearLabel = new QLabel(QString::number(movie.year), metaRow);
    yearLabel->setObjectName("searchMovieCardYear");

    const qreal dpr = devicePixelRatioF();
    QLabel *starIcon = new QLabel(metaRow);
    const QString starCacheKey = QStringLiteral("icon:star@%1x%2").arg(ICON_SIZE).arg(dpr);
    QPixmap starPixmap;
    if (!QPixmapCache::find(starCacheKey, &starPixmap)) {
        QPixmap rawStar(":/icons/star.svg");
        if (!rawStar.isNull()) {
            starPixmap = rawStar.scaled(static_cast<int>(ICON_SIZE * dpr), static_cast<int>(ICON_SIZE * dpr),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
            starPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(starCacheKey, starPixmap);
        }
    }
    if (!starPixmap.isNull()) {
        starIcon->setPixmap(starPixmap);
    }
    starIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *ratingLabel = new QLabel(QString::number(movie.rating, 'f', 1), metaRow);
    ratingLabel->setObjectName("searchMovieCardRating");

    metaLayout->addWidget(yearLabel);
    metaLayout->addWidget(starIcon);
    metaLayout->addWidget(ratingLabel);
    metaLayout->addStretch();

    QLabel *genresLabel = new QLabel(infoWidget);
    genresLabel->setObjectName("searchMovieCardGenres");
    genresLabel->setTextFormat(Qt::RichText);
    genresLabel->setText(highlight(movie.genres, query));
    genresLabel->setWordWrap(true);

    QLabel *descLabel = new QLabel(infoWidget);
    descLabel->setObjectName("searchMovieCardDescription");
    descLabel->setTextFormat(Qt::RichText);
    descLabel->setText(highlight(movie.description, query));
    descLabel->setWordWrap(true);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(metaRow);
    infoLayout->addWidget(genresLabel);
    infoLayout->addWidget(descLabel);
    infoLayout->addStretch();

    QLabel *posterLabel = new QLabel(this);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    posterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const QString posterCacheKey = QStringLiteral("searchPoster:%1@%2x%3")
            .arg(movie.posterPath)
            .arg(POSTER_HEIGHT)
            .arg(dpr);
    QPixmap pixmap;
    if (!QPixmapCache::find(posterCacheKey, &pixmap)) {
        QPixmap raw(movie.posterPath);
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

QString SearchMovieCard::highlight(const QString &text, const QString &query) {
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

void SearchMovieCard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(_movieId);
    }
    QFrame::mouseReleaseEvent(event);
}
