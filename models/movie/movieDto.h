#pragma once
#include <QString>

struct MovieDto {
    int id;
    QString title;
    QString description;
    int year;
    QString director;
    double rating;
    QString posterPath;
    int reviewCount;
    QString genres;
};

struct Review {
    int id;
    QString username;
    double rating;
    QString comment;
};
