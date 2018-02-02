#ifndef QFILEHELPER_H
#define QFILEHELPER_H

#include <QDir>
#include <QFile>
#include <QVariant>
#include <QSettings>
#include <QApplication>

class QFileHelper
{
public:
    QFileHelper();
    static QSettings * openIniFile(const QString & fileName);
    static QVariant getValue(const QSettings & iniObj, const QString &key, const QVariant &defaultValue = QVariant());
};

#endif // QFILEHELPER_H
