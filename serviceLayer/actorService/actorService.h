#pragma once

#include <QVector>
#include "actorResponse.h"
#include "actorDto.h"

namespace ActorService {
    GetActorsResponse getAllActors();

    GetActorResponse getActorById(int id);

    GetActorsResponse getActorsForMovie(int movieId);

    void sortByLastNameAsc(QVector<ActorDto> &actors);

    void sortByLastNameDesc(QVector<ActorDto> &actors);

    void sortByMoviesDesc(QVector<ActorDto> &actors);

    void sortByMoviesAsc(QVector<ActorDto> &actors);
}
