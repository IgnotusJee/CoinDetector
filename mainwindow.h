#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <serialport.h>

#include "setting.h"
#include "detectthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	SerialPort* serial;
	QList<QSerialPortInfo>* portlist;
	Setting* setpannel;
	DetectThread* thr;

public slots:
	void SetCurSerial(const QString &text);
	void ShowCurData();
	void ShowCurCoin();
	void SetBase();
    void SettingWindow();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
