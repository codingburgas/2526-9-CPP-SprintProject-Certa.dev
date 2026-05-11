#include "movieService.h"
#include "movieRepository.h"

namespace MovieService {
    double sumRatings(const QVector<double> &ratings, const int i) {
        if (i >= ratings.size()) {
            return 0.0;
        }

        return ratings[i] + sumRatings(ratings, i + 1);
    }

    void fillRating(MovieDto &movie) {
        QVector<double> ratings = MovieRepository::getRatingsForMovie(movie.id);
        int count = ratings.size();
        double sum = sumRatings(ratings, 0);
        movie.reviewCount = count;
        movie.rating = count > 0 ? sum / count : 0.0;
    }

    void fillRatings(QVector<MovieDto> &movies) {
        int n = movies.size();
        for (int i = 0; i < n; i++) {
            fillRating(movies[i]);
        }
    }

    MovieResponse getAllGenres() {
        return MovieRepository::getAllGenres();
    }

    GetMoviesResponse getAllMovies() {
        GetMoviesResponse response = MovieRepository::getAllMovies();
        if (response.success) {
            fillRatings(response.movies);
        }
        return response;
    }

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit) {
        GetMoviesResponse response = MovieRepository::getRecommendedMovies(username, limit);
        if (response.success) {
            fillRatings(response.movies);
        }
        return response;
    }

    GetMovieResponse getMovieById(int id) {
        GetMovieResponse response = MovieRepository::getMovieById(id);
        if (response.success) {
            fillRating(response.movie);
        }
        return response;
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
        GetMoviesResponse response = MovieRepository::getFavoriteMovies(username);
        if (response.success) {
            fillRatings(response.movies);
        }
        return response;
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
