#include "pch.h"
#include "CArea.h"

CArea::CArea() {
	this->longitude_l = 0;
	this->latitude_t = 0;
	this->longitude_r = 0;
	this->latitude_b = 0;
}
CArea::CArea(double longitude_l, double latitude_t, double longitude_r, double latitude_b) {
	this->longitude_l = longitude_l;
	this->latitude_t = latitude_t;
	this->longitude_r = longitude_r;
	this->latitude_b = latitude_b;
}

double CArea::getLongitudel() const {
	return longitude_l;
}

void CArea::setLongitudel(double longitudel) {
	longitude_l = longitudel;
}

double CArea::getLatitudet() const {
	return latitude_t;
}

void CArea::setLatitudet(double latitudet) {
	latitude_t = latitudet;
}

double CArea::getLongituder() const {
	return longitude_r;
}

void CArea::setLongituder(double longituder) {
	longitude_r = longituder;
}

double CArea::getLatitudeb() const {
	return latitude_b;
}

void CArea::setLatitudeb(double latitudeb) {
	latitude_b = latitudeb;
}