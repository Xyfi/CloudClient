#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QtGlobal>
#include <QCryptographicHash>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDir>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);
    ~Utils();

    /**
     * @brief generateHash
     * @param pathname
     * @return
     */
    static QString generateHash(QString pathname);

    /**
     * @brief intToQByteArray
     * @param number
     * @return
     */
    static QByteArray intToQByteArray(quint32 number);

    /**
     * @brief byteArrayToInt
     * @param byteArray
     * @return
     */
    static quint32 qByteArrayToInt(QByteArray byteArray);

    /**
     * @brief convertRelativePathToAbsolutePath
     * @param syncFolderPath
     * @param relativePath
     * @return
     */
    static QString convertRelativePathToAbsolutePath(QString syncFolderPath, QString relativePath);

    /**
     * @brief convertAbsolutePathToRelativePath
     * @param syncFolderPath
     * @param absolutePath
     * @return
     */
    static QString convertAbsolutePathToRelativePath(QString syncFolderPath, QString absolutePath);
signals:

public slots:
};

#endif // UTILS_H
