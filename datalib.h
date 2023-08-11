
#ifndef DATALIB_H
#define DATALIB_H


#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QFile>
#include <QMap>
#include <QStringList>
#include <QTextStream>
#include <QtMath>

#include "datagroup.h"


class DataLib : public QObject
{
    Q_OBJECT
public:
    explicit DataLib(QString filename, QString libname, QObject *parent = nullptr);
    ~DataLib();

	typedef QPair<QString, DataGroup> data_t;

    QString libname, filename;
	QMap<QString, DataGroup> datamap;

	void ModifyData(QString name, DataGroup data);
	DataGroup GetData(QString name);
	QList<QString> GetNames();

	QString GetCloest(DataGroup);

signals:

};

#endif // DATALIB_H
