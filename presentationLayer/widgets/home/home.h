#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Home;
}

QT_END_NAMESPACE

class Home : public QWidget {
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);

    ~Home() override;

    void refresh();

signals:
    void signInRequested();

    void exploreRecommendationsRequested();

    void openFavoritesRequested();

    void exploreActorsRequested();

    void movieClicked(int movieId);

    void actorClicked(int actorId);

private slots:
    void on_guestSignInButton_clicked();

    void on_exploreMoreButton_clicked();

    void on_openFavoritesButton_clicked();

    void on_exploreActorsButton_clicked();

private:
    const int PREVIEW_LIMIT = 3;
    const int COLUMNS = 3;

    Ui::Home *ui;

    void clearRecommendationsGrid();

    void clearFavoritesGrid();

    void clearActorsGrid();

    void loadRecommendations();

    void loadFavorites();

    void loadActors();
};
