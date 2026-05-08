#include "actorService.h"
#include "actorRepository.h"

namespace ActorService {
    GetActorsResponse getAllActors() {
        return ActorRepository::getAllActors();
    }

    GetActorResponse getActorById(int id) {
        return ActorRepository::getActorById(id);
    }

    GetActorsResponse getActorsForMovie(int movieId) {
        return ActorRepository::getActorsForMovie(movieId);
    }

    void sortByLastNameAsc(QVector<ActorDto> &actors) {
        int n = actors.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (actors[j].lastName.compare(actors[j + 1].lastName, Qt::CaseInsensitive) > 0) {
                    ActorDto temp = actors[j];
                    actors[j] = actors[j + 1];
                    actors[j + 1] = temp;
                }
            }
        }
    }

    void sortByLastNameDesc(QVector<ActorDto> &actors) {
        int n = actors.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (actors[j].lastName.compare(actors[j + 1].lastName, Qt::CaseInsensitive) < 0) {
                    ActorDto temp = actors[j];
                    actors[j] = actors[j + 1];
                    actors[j + 1] = temp;
                }
            }
        }
    }

    void sortByMoviesDesc(QVector<ActorDto> &actors) {
        int n = actors.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (actors[j].movieCount < actors[j + 1].movieCount) {
                    ActorDto temp = actors[j];
                    actors[j] = actors[j + 1];
                    actors[j + 1] = temp;
                }
            }
        }
    }

    void sortByMoviesAsc(QVector<ActorDto> &actors) {
        int n = actors.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (actors[j].movieCount > actors[j + 1].movieCount) {
                    ActorDto temp = actors[j];
                    actors[j] = actors[j + 1];
                    actors[j + 1] = temp;
                }
            }
        }
    }
}
