#include "utils.hpp"

Utils::Utils(QObject *parent) : QObject(parent)
{

}

Utils::~Utils()
{

}

QString Utils::generateHash(QString pathname) {
    QCryptographicHash crypto(QCryptographicHash::Md5);
    QFile file(pathname);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()) {
      crypto.addData(file.read(8192));
    }
    return crypto.result();
}

QByteArray Utils::intToQByteArray(quint32 number) {
    QByteArray temp;
    QDataStream ds(&temp, QIODevice::WriteOnly);
    ds << number;
    return temp;
}

quint32 Utils::qByteArrayToInt(QByteArray byteArray) {
    quint32 temp;
    QDataStream ds(&byteArray, QIODevice::ReadOnly);
    ds >> temp;
    return temp;
}

// BUG: Relative path should not include
//      the sync folder root foldername.
QString Utils::convertRelativePathToAbsolutePath(QString absSyncFolderPath, QString relativePath){
    return absSyncFolderPath + "/" + relativePath;
}

QString Utils::convertAbsolutePathToRelativePath(QString absSyncFolderPath, QString absolutePath){
    return absolutePath.remove(0, absSyncFolderPath.length() + 1);
}
