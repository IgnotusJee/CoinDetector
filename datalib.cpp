
#include "datalib.h"

DataLib::DataLib(QString filename, QString libname, QObject *parent)
	: QObject{parent}, libname(libname), filename(filename)
{
    QFile loadfile(filename);
	if(loadfile.open(QFileDevice::ReadOnly)) {
        QTextStream in(&loadfile);
        QString name;
        double amp, diff, freq;
		while(!in.atEnd()) {
			in >> name >> amp >> diff >> freq;
			datamap.insert(name, DataGroup(amp, diff, freq));
        }
    }
}

DataLib::~DataLib() {
    QFile savefile(filename);
	if(savefile.open(QFileDevice::WriteOnly)) {
        QTextStream out(&savefile);
		foreach(const QString &key, datamap.keys()) {
			const auto &i = datamap[key];
			if(key.length() > 0) out << key << ' ' << i.amp << ' ' << i.diff << ' ' << i.freq << Qt::endl;
		}
    }
}

void DataLib::ModifyData(QString name, DataGroup data) {
	datamap[name] = data;
}

DataGroup DataLib::GetData(QString name) {
	if(datamap.find(name) == datamap.end()) return DataGroup(0, 0, 0);
	return datamap[name];
}

QList<QString> DataLib::GetNames() {
	return datamap.keys();
}

QString DataLib::GetCloest(DataGroup tar) {
	static const double limit = 10;
	QString ans("No match found.");
	bool found = false;
	double err = 1e20;
	foreach(const QString &key, datamap.keys()) {
		const auto &obj = datamap[key];
		double now_dif = tar.dist(obj);
		if(!found || now_dif < err) {
			ans = key;
			err = now_dif;
		}
	}
	if(err > limit) ans = QString("No match found.");

	return ans;
}
