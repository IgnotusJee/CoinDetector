#ifndef DATAGROUP_H
#define DATAGROUP_H

#include <QObject>
#include <QtMath>

class DataGroup
{
public:
    DataGroup(double amp=0, double diff=0, double freq=0);

    double amp, diff, freq;
	DataGroup operator - (const DataGroup &x) const ;
	bool operator < (const DataGroup &x) const ;
//	DataGroup& operator = (const DataGroup&);
	DataGroup operator + (const DataGroup&) const;
    void SetData(double, double, double);
    double dist(const DataGroup &x) const ;

signals:

};

#endif // DATAGROUP_H
