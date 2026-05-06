#pragma once

#include "actorResponse.h"

namespace ActorRepository {
    GetActorsResponse getAllActors();

    GetActorResponse getActorById(int id);

    GetActorsResponse getActorsForMovie(int movieId);
}
