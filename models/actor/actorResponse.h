#pragma once
#include <QVector>
#include "actorDto.h"
#include "movieDto.h"

struct GetActorsResponse {
    bool success;
    QString errorMessage;
    QVector<ActorDto> actors;
};

struct GetActorResponse {
    bool success;
    QString errorMessage;
    ActorDto actor;
    QVector<MovieDto> movies;
};
