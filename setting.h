#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QMap>

#include "datagroup.h"
#include "datalib.h"
#include "serialport.h"

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
	Q_OBJECT

public:
	explicit Setting(SerialPort* serialport, QWidget *parent = nullptr);
	~Setting();

	SerialPort* serialport;
	QMap<QString, DataLib*> libmap;

	QString GetCoinType(DataGroup);


public slots:
	void LibChange(const QString &libname);
	void DataChange(const QString &dataname);
	void SetData();
	void ReadData();

private:
	Ui::Setting *ui;
};

#endif // SETTING_H
