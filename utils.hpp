#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QtGlobal>
#include <QCryptographicHash>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

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
signals:

public slots:
};

#endif // UTILS_H
