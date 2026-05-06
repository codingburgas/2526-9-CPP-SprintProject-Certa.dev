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
}
