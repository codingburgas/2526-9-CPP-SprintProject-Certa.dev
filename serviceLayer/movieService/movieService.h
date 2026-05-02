#pragma once

#include "movieResponse.h"

namespace MovieService {
    MovieResponse getAllGenres();

    GetMoviesResponse getAllMovies();

    GetMovieResponse getMovieById(int id);

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment);

    GetReviewsResponse getReviewsForMovie(int movieId);
}
