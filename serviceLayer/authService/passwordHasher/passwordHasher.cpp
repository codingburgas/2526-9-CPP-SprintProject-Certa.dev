#include "passwordHasher.h"
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QStringList>

namespace PasswordHasher {
    QString hashPassword(const QString &password) {
        const int iterations = 100000;
        const int keyLength = 32;
        const int saltLength = 16;

        QByteArray salt(saltLength, '\0');
        for (int i = 0; i < saltLength; i++) {
            salt[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
        }

        QByteArray key = QPasswordDigestor::deriveKeyPbkdf2(
            QCryptographicHash::Sha256,
            password.toUtf8(),
            salt,
            iterations,
            keyLength
        );

        return QString("%1:%2:%3")
                .arg(iterations)
                .arg(QString::fromLatin1(salt.toHex()))
                .arg(QString::fromLatin1(key.toHex()));
    }

    bool verifyPassword(const QString &password, const QString &stored) {
        const int keyLength = 32;

        QStringList parts = stored.split(':');
        if (parts.size() != 3) return false;

        int iterations = parts[0].toInt();
        QByteArray salt = QByteArray::fromHex(parts[1].toLatin1());
        QByteArray expected = QByteArray::fromHex(parts[2].toLatin1());

        QByteArray actual = QPasswordDigestor::deriveKeyPbkdf2(
            QCryptographicHash::Sha256,
            password.toUtf8(),
            salt,
            iterations,
            keyLength
        );

        return actual == expected;
    }
}
