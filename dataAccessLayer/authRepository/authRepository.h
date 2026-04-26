#pragma once
#include <QString>
#include "authStatus.h"

namespace AuthRepository {
    SignUpStatus signUp(const QString &username, const QString &hashedPassword);

    SignInStatus signIn(const QString &username, QString &storedHashedPassword);

    bool userExists(const QString &username);
}
