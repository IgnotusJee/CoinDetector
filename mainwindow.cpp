#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	serial = new SerialPort(this);
	portlist = new QList<QSerialPortInfo>(serial->GetSerialList());
	for(const auto &i: *portlist) {
		ui->comboBox->addItem(i.portName());
	}

	thr = nullptr;

	connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::SetCurSerial);
	connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::ShowCurData);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::ShowCurCoin);
	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::SetBase);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::SettingWindow);

	setpannel = new Setting(serial, this);

	this->setWindowTitle("硬币检测起爆程序");
}

MainWindow::~MainWindow()
{
	thr->terminate();
	delete ui;
}

void MainWindow::SetCurSerial(const QString &text) {
	for(const auto &i: *portlist) {
		if(text == i.portName()) {
			serial->SetPort(i);
			break;
		}
	}
}

void MainWindow::ShowCurData() {
	auto data = serial->GetCurData();
	QString now;
	QTextStream out(&now);
	out << "当前数据🤤: " << data.amp << ' ' << data.diff << ' ' << data.freq << Qt::endl;
	ui->label_3->setText(now);
}

void MainWindow::ShowCurCoin() {
	if(thr == nullptr) {
		thr = new DetectThread(serial, ui->label_2, setpannel, this);
		thr->start();
	}
	else thr->terminate();
}

void MainWindow::SetBase() {
	serial->SetBase();
	auto data = serial->base;
	QString now;
	QTextStream out(&now);
	out << "将基本数据设为😋: " << data.amp << ' ' << data.diff << ' ' << data.freq << Qt::endl;
	ui->label_3->setText(now);
}

void MainWindow::SettingWindow() {
	setpannel->show();
}
