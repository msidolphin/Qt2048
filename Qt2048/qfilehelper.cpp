#include "qfilehelper.h"

QFileHelper::QFileHelper()
{

}

QSettings * QFileHelper::openIniFile(const QString & fileName)
{
    //If the file doesn't already exist, it is created.
    QSettings *setting = new QSettings(QApplication::applicationDirPath() +  "/" + fileName, QSettings::IniFormat);
    return setting;
}

QVariant QFileHelper::getValue(const QSettings & iniObj, const QString &key, const QVariant &defaultValue)
{
    return iniObj.value(key, defaultValue);
}
