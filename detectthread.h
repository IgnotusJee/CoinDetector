#ifndef DETECTTHREAD_H
#define DETECTTHREAD_H

#include <QThread>
#include <QString>
#include <QLabel>

#include "serialport.h"
#include "setting.h"

class DetectThread : public QThread
{
	Q_OBJECT
public:
	explicit DetectThread(SerialPort* serial, QLabel* label, Setting* setpannel, QObject *parent = nullptr);
	SerialPort* serial;
	QLabel* label;
	Setting* setpannel;

protected:
	void run();

signals:
	void runDone();

};

#endif // DETECTTHREAD_H
