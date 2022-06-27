#include "pch.h"
#include "CPlane.h"

CPlane::CPlane() {
	A = B = C = D = 0;
}

CPlane::CPlane(CLocation point1, CLocation point2, CLocation point3) {

	double x1 = point1.getX(), y1 = point1.getY(), z1 = point1.getZ();
	double x2 = point2.getX(), y2 = point2.getY(), z2 = point2.getZ();
	double x3 = point3.getX(), y3 = point3.getY(), z3 = point3.getZ();

	A = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2);
	B = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2);
	C = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2);
	D = -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1);
}

double CPlane::getA() const {
	return A;
}

void CPlane::setA(double A) {
	this->A = A;
}

double CPlane::getB() const {
	return B;
}

void CPlane::setB(double B) {
	this->B = B;
}

double CPlane::getC() const {
	return C;
}

void CPlane::setC(double C) {
	this->C = C;
}

double CPlane::getD() const {
	return D;
}

void CPlane::setD(double D) {
	this->D = D;
}

