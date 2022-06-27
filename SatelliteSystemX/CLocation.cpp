#include "pch.h"
#include "CLocation.h"
#include <cmath>

CLocation::CLocation() {
	x = y = z = 0;
}

CLocation::CLocation(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

CLocation::CLocation(double longitude, double latitude) {
	longitude = AngleToRad(longitude);
	latitude = AngleToRad(latitude);
	this->x = EARTH_R * cos(latitude) * cos(longitude);
	this->y = EARTH_R * cos(latitude) * sin(longitude);
	this->z = EARTH_R * sin(latitude);
}

double AngleToRad(double angle) {
	return angle * acos(-1) / 180.0;
}

double CLocation::getX() const {
	return x;
}

void CLocation::setX(double x) {
	this->x = x;
}

double CLocation::getY() const {
	return y;
}

void CLocation::setY(double y) {
	this->y = y;
}

double CLocation::getZ() const {
	return z;
}

void CLocation::setZ(double z) {
	this->z = z;
}