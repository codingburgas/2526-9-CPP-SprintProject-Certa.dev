#include "actorRepository.h"
#include <QSqlQuery>

namespace ActorRepository {
    GetActorsResponse getAllActors() {
        QSqlQuery query;
        query.prepare(
            "SELECT a.id, a.first_name, a.last_name, a.description, a.actor_path, "
            "COUNT(ma.movie_id) AS movie_count "
            "FROM actors a "
            "LEFT JOIN movie_actors ma ON ma.actor_id = a.id "
            "GROUP BY a.id "
            "ORDER BY a.last_name, a.first_name"
        );

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch actors.", .actors = {}};
        }

        QVector<ActorDto> actors;
        while (query.next()) {
            ActorDto actor;
            actor.id = query.value(0).toInt();
            actor.firstName = query.value(1).toString();
            actor.lastName = query.value(2).toString();
            actor.description = query.value(3).toString();
            actor.actorPath = query.value(4).toString();
            actor.movieCount = query.value(5).toInt();
            actors.append(actor);
        }

        return {.success = true, .errorMessage = "", .actors = actors};
    }

    GetActorResponse getActorById(int id) {
        QSqlQuery actorQuery;
        actorQuery.prepare(
            "SELECT a.id, a.first_name, a.last_name, a.description, a.actor_path, "
            "COUNT(ma.movie_id) AS movie_count "
            "FROM actors a "
            "LEFT JOIN movie_actors ma ON ma.actor_id = a.id "
            "WHERE a.id = ? "
            "GROUP BY a.id"
        );
        actorQuery.addBindValue(id);

        if (!actorQuery.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch actor.", .actor = {}, .movies = {}};
        }

        if (!actorQuery.next()) {
            return {.success = false, .errorMessage = "Actor not found.", .actor = {}, .movies = {}};
        }

        ActorDto actor;
        actor.id = actorQuery.value(0).toInt();
        actor.firstName = actorQuery.value(1).toString();
        actor.lastName = actorQuery.value(2).toString();
        actor.description = actorQuery.value(3).toString();
        actor.actorPath = actorQuery.value(4).toString();
        actor.movieCount = actorQuery.value(5).toInt();

        QSqlQuery moviesQuery;
        moviesQuery.prepare(
            "SELECT m.id, m.title, m.description, m.year, m.director, "
            "COALESCE(AVG(r.rating), 0) AS avg_rating, m.poster_path, "
            "COUNT(r.id) AS review_count, "
            "(SELECT GROUP_CONCAT(g.name, ', ') "
            " FROM movie_genres mg JOIN genres g ON g.id = mg.genre_id "
            " WHERE mg.movie_id = m.id) AS genres "
            "FROM movies m "
            "JOIN movie_actors ma ON ma.movie_id = m.id "
            "LEFT JOIN reviews r ON r.movie_id = m.id "
            "WHERE ma.actor_id = ? "
            "GROUP BY m.id "
            "ORDER BY m.year DESC, m.title"
        );
        moviesQuery.addBindValue(id);

        if (!moviesQuery.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch filmography.", .actor = actor, .movies = {}};
        }

        QVector<MovieDto> movies;
        while (moviesQuery.next()) {
            MovieDto movie;
            movie.id = moviesQuery.value(0).toInt();
            movie.title = moviesQuery.value(1).toString();
            movie.description = moviesQuery.value(2).toString();
            movie.year = moviesQuery.value(3).toInt();
            movie.director = moviesQuery.value(4).toString();
            movie.rating = moviesQuery.value(5).toDouble();
            movie.posterPath = moviesQuery.value(6).toString();
            movie.reviewCount = moviesQuery.value(7).toInt();
            movie.genres = moviesQuery.value(8).toString();
            movies.append(movie);
        }

        return {.success = true, .errorMessage = "", .actor = actor, .movies = movies};
    }

    GetActorsResponse getActorsForMovie(int movieId) {
        QSqlQuery query;
        query.prepare(
            "SELECT a.id, a.first_name, a.last_name, a.description, a.actor_path "
            "FROM actors a "
            "JOIN movie_actors ma ON ma.actor_id = a.id "
            "WHERE ma.movie_id = ? "
            "ORDER BY a.last_name, a.first_name"
        );
        query.addBindValue(movieId);

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch cast.", .actors = {}};
        }

        QVector<ActorDto> actors;
        while (query.next()) {
            ActorDto actor;
            actor.id = query.value(0).toInt();
            actor.firstName = query.value(1).toString();
            actor.lastName = query.value(2).toString();
            actor.description = query.value(3).toString();
            actor.actorPath = query.value(4).toString();
            actor.movieCount = 0;
            actors.append(actor);
        }

        return {.success = true, .errorMessage = "", .actors = actors};
    }
}
