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
    Home(QWidget *parent = nullptr);

    ~Home();

private:
    Ui::Home *ui;
};
