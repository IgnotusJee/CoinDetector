#include "serialport.h"

#include <QTextStream>

SerialPort::SerialPort(QObject *parent)
	: QObject{parent}
{

	serialport = nullptr;

	length = 0;
}

QList<QSerialPortInfo> SerialPort::GetSerialList() const {
	return QSerialPortInfo::availablePorts();
}

bool SerialPort::SetPort(QSerialPortInfo port) {
	if(serialport != nullptr) {
		serialport->close();
		delete serialport;
	}
	serialport = new QSerialPort(port);
	if(!serialport->open(QIODevice::ReadWrite)) {
		delete serialport;
		return false;
	}
	serialport->setBaudRate(115200);
	serialport->setDataBits(QSerialPort::Data8);
	serialport->setParity(QSerialPort::NoParity);
	serialport->setStopBits(QSerialPort::OneStop);
	serialport->setFlowControl(QSerialPort::NoFlowControl);
	return true;
}

DataGroup SerialPort::GetCurData() const {
	if(serialport == nullptr) return DataGroup(0, 0, 0);
	serialport->write("p\n");
	QByteArray buf = serialport->readAll();
	if(buf.isEmpty()) return DataGroup(0, 0, 0);
	QTextStream In(buf);
	float a, b, c;
	In >> a >> b >> c;
	return DataGroup(a, b, c);
}

DataGroup SerialPort::GetCurDel() const {
	if(serialport == nullptr) return DataGroup(0, 0, 0);
	serialport->write("t\n");
	QByteArray buf = serialport->readAll();
	if(buf.isEmpty()) return DataGroup(0, 0, 0);
	QTextStream In(buf);
	float a, b, c;
	In >> a >> b >> c;
	return DataGroup(a, b, c);
}

bool SerialPort::SetBase() {
	DataGroup now = GetCurData();
	if(!now.amp && !now.diff && !now.freq) return false;
	base = now;
	return true;
}
