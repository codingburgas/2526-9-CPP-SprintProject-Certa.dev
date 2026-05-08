#pragma once

#include <QVector>
#include "movieResponse.h"
#include "movieDto.h"

namespace MovieService {
    MovieResponse getAllGenres();

    GetMoviesResponse getAllMovies();

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit = -1);

    GetMovieResponse getMovieById(int id);

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment);

    GetReviewsResponse getReviewsForMovie(int movieId);

    FavoriteStatusResponse isFavorite(const QString &username, int movieId);

    FavoriteResponse addFavorite(const QString &username, int movieId);

    FavoriteResponse removeFavorite(const QString &username, int movieId);

    GetMoviesResponse getFavoriteMovies(const QString &username);

    void sortByRatingDesc(QVector<MovieDto> &movies);

    void sortByRatingAsc(QVector<MovieDto> &movies);

    void sortByYearDesc(QVector<MovieDto> &movies);

    void sortByYearAsc(QVector<MovieDto> &movies);
}
