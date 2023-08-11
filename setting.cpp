#include "setting.h"
#include "ui_setting.h"

Setting::Setting(SerialPort* serialport, QWidget *parent) :
	QDialog(parent),
	serialport(serialport),
	ui(new Ui::Setting)
{
	ui->setupUi(this);

	libmap.insert("A", new DataLib("F://WorkSpace/ele/pc/Data/A.sd", "A", this));
	libmap.insert("B", new DataLib("F://WorkSpace/ele/pc/Data/B.sd", "B", this));
	libmap.insert("C", new DataLib("F://WorkSpace/ele/pc/Data/C.sd", "C", this));

	ui->comboBox_2->addItem(QString(""));
	for(const auto &i: libmap.keys()) {
		ui->comboBox_2->addItem(i);
	}

	connect(ui->comboBox_2, &QComboBox::currentTextChanged, this, &Setting::LibChange);
	connect(ui->comboBox, &QComboBox::currentTextChanged, this, &Setting::DataChange);
	connect(ui->pushButton, &QPushButton::clicked, this, &Setting::ReadData);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &Setting::SetData);

	this->setWindowTitle("设置检测的组别和硬币数据捏");
}

Setting::~Setting()
{
	delete ui;
}

void Setting::LibChange(const QString &libname) {
	ui->comboBox->clear();
	auto now = libmap.find(libname);
	if(now != libmap.end()) {
		for(const auto &i: now.value()->GetNames()) {
			ui->comboBox->addItem(i);
		}
	}
}

void Setting::DataChange(const QString &dataname) {
	auto nowlib = libmap.find(ui->comboBox_2->currentText());
	if(nowlib != libmap.end()) {
		auto nowdata = nowlib.value()->GetData(dataname);
		ui->doubleSpinBox->setValue(nowdata.amp);
		ui->doubleSpinBox_2->setValue(nowdata.diff);
		ui->doubleSpinBox_3->setValue(nowdata.freq);
	}
}

void Setting::ReadData() {
	auto nowlib = libmap.find(ui->comboBox_2->currentText());
	if(nowlib != libmap.end()) {
		auto nowdata = serialport->GetCurData();
		ui->doubleSpinBox->setValue(nowdata.amp);
		ui->doubleSpinBox_2->setValue(nowdata.diff);
		ui->doubleSpinBox_3->setValue(nowdata.freq);
	}
}

void Setting::SetData() {
	auto nowlib = libmap.find(ui->comboBox_2->currentText());
	if(nowlib != libmap.end()) {
		nowlib.value()->ModifyData(ui->comboBox->currentText(), DataGroup(ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value(), ui->doubleSpinBox_3->value()));
	}
}

QString Setting::GetCoinType(DataGroup tar) {
	auto nowlib = libmap.find(ui->comboBox_2->currentText());
	if(nowlib != libmap.end() && nowlib.key().length()>0) {
		return nowlib.value()->GetCloest(serialport->base + tar);
	}
	return QString("还没选你要测的是哪一组捏😰");
}
