#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Movies;
}

QT_END_NAMESPACE

class Movies : public QWidget {
    Q_OBJECT

public:
    Movies(QWidget *parent = nullptr);

    ~Movies();

private:
    Ui::Movies *ui;
};
