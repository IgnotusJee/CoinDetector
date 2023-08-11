#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QVector>
#include <QPair>
#include <QString>

#include "datagroup.h"

class SerialPort : public QObject
{
	Q_OBJECT
public:
	explicit SerialPort(QObject *parent = nullptr);
	int length;
	DataGroup base;

	QSerialPort *serialport;

	QList<QSerialPortInfo> GetSerialList() const;
	bool SetPort(QSerialPortInfo);
	DataGroup GetCurData() const;
	DataGroup GetCurDel() const;
	bool SetBase();


signals:

};

#endif // SERIALPORT_H
