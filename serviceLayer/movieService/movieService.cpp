#include "movieService.h"
#include "movieRepository.h"

namespace MovieService {
    MovieResponse getAllGenres() {
        return MovieRepository::getAllGenres();
    }

    GetMoviesResponse getAllMovies() {
        return MovieRepository::getAllMovies();
    }

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit) {
        return MovieRepository::getRecommendedMovies(username, limit);
    }

    GetMovieResponse getMovieById(int id) {
        return MovieRepository::getMovieById(id);
    }

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment) {
        return MovieRepository::createReview(username, movieId, rating, comment);
    }

    GetReviewsResponse getReviewsForMovie(int movieId) {
        return MovieRepository::getReviewsForMovie(movieId);
    }

    FavoriteStatusResponse isFavorite(const QString &username, int movieId) {
        return MovieRepository::isFavorite(username, movieId);
    }

    FavoriteResponse addFavorite(const QString &username, int movieId) {
        return MovieRepository::addFavorite(username, movieId);
    }

    FavoriteResponse removeFavorite(const QString &username, int movieId) {
        return MovieRepository::removeFavorite(username, movieId);
    }

    GetMoviesResponse getFavoriteMovies(const QString &username) {
        return MovieRepository::getFavoriteMovies(username);
    }

    void sortByRatingDesc(QVector<MovieDto> &movies) {
        int n = movies.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (movies[j].rating < movies[j + 1].rating) {
                    MovieDto temp = movies[j];
                    movies[j] = movies[j + 1];
                    movies[j + 1] = temp;
                }
            }
        }
    }

    void sortByRatingAsc(QVector<MovieDto> &movies) {
        int n = movies.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (movies[j].rating > movies[j + 1].rating) {
                    MovieDto temp = movies[j];
                    movies[j] = movies[j + 1];
                    movies[j + 1] = temp;
                }
            }
        }
    }

    void sortByYearDesc(QVector<MovieDto> &movies) {
        int n = movies.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (movies[j].year < movies[j + 1].year) {
                    MovieDto temp = movies[j];
                    movies[j] = movies[j + 1];
                    movies[j + 1] = temp;
                }
            }
        }
    }

    void sortByYearAsc(QVector<MovieDto> &movies) {
        int n = movies.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (movies[j].year > movies[j + 1].year) {
                    MovieDto temp = movies[j];
                    movies[j] = movies[j + 1];
                    movies[j + 1] = temp;
                }
            }
        }
    }
}
