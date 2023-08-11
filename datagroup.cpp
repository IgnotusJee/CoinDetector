#include "datagroup.h"

DataGroup::DataGroup(double amp, double diff, double freq)
	: amp(amp), diff(diff), freq(freq)
{

}

DataGroup DataGroup::operator - (const DataGroup &x) const {
	return DataGroup(this->amp - x.amp, this->diff - x.diff, this->freq - x.freq);
}
bool DataGroup::operator < (const DataGroup &x) const {
	return this->amp < x.amp && this->diff < x.diff && this->freq < x.freq;
}
double DataGroup::dist(const DataGroup &x) const {
	return sqrt((this->amp - x.amp) * (this->amp - x.amp) + (this->diff - x.diff) * (this->diff - x.diff) + (this->freq - x.freq) * (this->freq - x.freq));
}

void DataGroup::SetData(double amp, double diff, double freq) {
	this->amp = amp;
	this->diff = diff;
	this->freq = freq;
}

//DataGroup& DataGroup::operator = (const DataGroup& x) {
//	this->amp = x.amp;
//	this->diff = x.diff;
//	this->freq = x.freq;
//	return *this;
//}

DataGroup DataGroup::operator + (const DataGroup& x) const {
	return DataGroup(this->amp + x.amp, this->diff + x.diff, this->freq + x.freq);
}
