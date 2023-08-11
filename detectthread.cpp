#include "detectthread.h"

DetectThread::DetectThread(SerialPort* serial, QLabel* label, Setting* setpannel, QObject *parent)
	: QThread{parent}, serial(serial), label(label), setpannel(setpannel)
{

}

void DetectThread::run() {
	while(1) {
		static const DataGroup max_del(1, 1, 1);
		auto nowdel = serial->GetCurDel(), nowdata = serial->GetCurData();
		if(nowdel < max_del) {
			label->setText(setpannel->GetCoinType(nowdata));
		}
		else label->setText(QString("数据不稳定喵，请稍等😭"));
		sleep(10);
	}
	emit runDone();
}
