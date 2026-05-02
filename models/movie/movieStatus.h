#pragma once

enum class GetAllGenresStatus {
    Success,
    Failure
};

enum class GetAllMoviesStatus {
    Success,
    Failure
};

enum class CreateReviewStatus {
    Success,
    UserNotFound,
    Failure
};

enum class GetReviewsStatus {
    Success,
    Failure
};
