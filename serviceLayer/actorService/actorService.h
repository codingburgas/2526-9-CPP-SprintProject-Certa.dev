#pragma once

#include "actorResponse.h"

namespace ActorService {
    GetActorsResponse getAllActors();

    GetActorResponse getActorById(int id);

    GetActorsResponse getActorsForMovie(int movieId);
}
