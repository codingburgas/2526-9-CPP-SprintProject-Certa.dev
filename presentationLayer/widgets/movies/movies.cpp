#include "movies.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QScrollBar>

#include "movie.h"
#include "movieService.h"
#include "ui_movies.h"

Movies::Movies(QWidget *parent) : QWidget(parent), ui(new Ui::Movies) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->moviesGrid->setColumnStretch(col, 1);
    }

    moviePage = new Movie(this);
    ui->moviePageHostLayout->addWidget(moviePage);
    connect(moviePage, &Movie::backRequested, this, &Movies::showList);
    connect(moviePage, &Movie::reviewSubmitted, this, &Movies::reloadMovies);

    showList();

    loadMovies();
}

Movies::~Movies() {
    delete ui;
}

void Movies::showList() {
    ui->moviesStackedWidget->setCurrentWidget(ui->moviesListPage);
}

void Movies::openMoviePage(int movieId) {
    moviePage->loadMovie(movieId);
    ui->moviesStackedWidget->setCurrentWidget(ui->moviePageHost);
}

bool Movies::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QWidget *widget = qobject_cast<QWidget *>(watched);
            if (widget && widget->rect().contains(mouseEvent->pos())) {
                int movieId = widget->property("movieId").toInt();
                openMoviePage(movieId);
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Movies::loadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    const QVector<MovieDto> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        ui->moviesGrid->addWidget(createMovieCard(movies[i]), row, col);
    }

    int lastRow = (movies.size() - 1) / COLUMNS + 1;
    ui->moviesGrid->setRowStretch(lastRow, 1);
}

void Movies::reloadMovies() {
    QLayoutItem *item;
    while ((item = ui->moviesGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    loadMovies();
}

QWidget *Movies::createMovieCard(const MovieDto &movie) {
    QFrame *card = new QFrame(this);
    card->setObjectName("movieCard");
    card->setProperty("movieId", movie.id);
    card->setCursor(Qt::PointingHandCursor);
    card->setAttribute(Qt::WA_Hover, true);
    card->installEventFilter(this);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(8, 8, 8, 0);
    cardLayout->setSpacing(0);

    QLabel *posterLabel = new QLabel(card);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    posterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const qreal dpr = devicePixelRatioF();
    const QString posterCacheKey = QStringLiteral("poster:%1@%2x%3")
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
    }

    QWidget *infoWidget = new QWidget(card);
    infoWidget->setStyleSheet("background-color: transparent;");
    infoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(10, 8, 10, 10);
    infoLayout->setSpacing(6);

    QLabel *titleLabel = new QLabel(movie.title, infoWidget);
    titleLabel->setObjectName("movieCardTitle");
    titleLabel->setWordWrap(true);

    QLabel *genresLabel = new QLabel(movie.genres, infoWidget);
    genresLabel->setObjectName("movieCardGenres");
    genresLabel->setWordWrap(true);

    QWidget *statsRow = new QWidget(infoWidget);
    statsRow->setStyleSheet("background-color: transparent;");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsRow);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(4);

    QLabel *starIcon = new QLabel(statsRow);
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

    QLabel *ratingLabel = new QLabel(QString::number(movie.rating, 'f', 1), statsRow);
    ratingLabel->setObjectName("movieCardRating");

    QLabel *commentsIcon = new QLabel(statsRow);
    const QString commentsCacheKey = QStringLiteral("icon:comments@%1x%2").arg(ICON_SIZE).arg(dpr);
    QPixmap commentsPixmap;
    if (!QPixmapCache::find(commentsCacheKey, &commentsPixmap)) {
        QPixmap rawComments(":/icons/comments.svg");
        if (!rawComments.isNull()) {
            commentsPixmap = rawComments.scaled(static_cast<int>(ICON_SIZE * dpr), static_cast<int>(ICON_SIZE * dpr),
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
            commentsPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(commentsCacheKey, commentsPixmap);
        }
    }
    if (!commentsPixmap.isNull()) {
        commentsIcon->setPixmap(commentsPixmap);
        commentsIcon->setPixmap(commentsPixmap.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation));
    }
    commentsIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *reviewCountLabel = new QLabel(QString::number(movie.reviewCount), statsRow);
    reviewCountLabel->setObjectName("movieCardReviews");

    statsLayout->addWidget(starIcon);
    statsLayout->addWidget(ratingLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(commentsIcon);
    statsLayout->addWidget(reviewCountLabel);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(genresLabel);
    infoLayout->addWidget(statsRow);

    cardLayout->addWidget(posterLabel);
    cardLayout->addWidget(infoWidget);

    return card;
}
