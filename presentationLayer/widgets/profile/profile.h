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

signals:
    void logoutRequested();

private slots:
    void on_logoutButton_clicked();

private:
    Ui::Profile *ui;
};
