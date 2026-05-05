#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Profile;
}

QT_END_NAMESPACE

class Profile : public QWidget {
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);

    ~Profile() override;

    void refresh();

signals:
    void logoutRequested();

    void editProfileRequested();

    void viewFavoritesRequested();

    void movieClicked(int movieId);

private slots:
    void on_logoutButton_clicked();

    void on_editProfileButton_clicked();

    void on_viewAllFavoritesButton_clicked();

    void on_openSettingsLink_clicked();

private:
    const int FAVORITES_PREVIEW_COLUMNS = 3;
    const int FAVORITES_PREVIEW_LIMIT = 6;

    Ui::Profile *ui;

    void clearFavoritesPreview();

    void clearGenresChips();
};
