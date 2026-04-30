#pragma once

#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class InterestsDialog;
}

QT_END_NAMESPACE

class InterestsDialog : public QWidget {
    Q_OBJECT

public:
    explicit InterestsDialog(QWidget *parent = nullptr);

    ~InterestsDialog() override;

private slots:
    void on_saveButton_clicked();

    void on_skipButton_clicked();

private:
    Ui::InterestsDialog *ui;
};
