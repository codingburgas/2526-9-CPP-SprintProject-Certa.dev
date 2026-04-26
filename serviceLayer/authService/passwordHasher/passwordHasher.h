#pragma once

#include <QString>

namespace PasswordHasher {
    QString hashPassword(const QString &password);

    bool verifyPassword(const QString &password, const QString &stored);
}
