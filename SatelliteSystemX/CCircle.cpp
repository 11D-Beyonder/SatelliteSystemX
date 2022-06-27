#include "pch.h"
#include "CCircle.h"
#include <cmath>

CCircle::CCircle() {
	this->center.setX(0);
	this->center.setY(0);
	this->center.setZ(0);
	this->planeOf.setA(0);
	this->planeOf.setB(0);
	this->planeOf.setC(0);
	this->planeOf.setD(0);
	this->r = 0;
}

CCircle::CCircle(double x, double y, double z, double r) {
	this->center.setX(x);
	this->center.setY(y);
	this->center.setZ(z);
	this->r = r;
}

CCircle::CCircle(const CLocation& point1, const CLocation& point2, const CLocation& point3) {
	// 定义参数
	double A1 = 0.0, B1 = 0.0, C1 = 0.0, D1 = 0.0;
	double A2 = 0.0, B2 = 0.0, C2 = 0.0, D2 = 0.0;
	double A3 = 0.0, B3 = 0.0, C3 = 0.0, D3 = 0.0;

	// 获得三个点 
	// 三点确定一平面 
	// 平面上三点确定一个圆
	double x1 = point1.getX(), y1 = point1.getY(), z1 = point1.getZ();
	double x2 = point2.getX(), y2 = point2.getY(), z2 = point2.getZ();
	double x3 = point3.getX(), y3 = point3.getY(), z3 = point3.getZ();

	// 计算参数
	A1 = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2);
	B1 = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2);
	C1 = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2);
	D1 = -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 + x3 * y1 * z2 - x3 * y2 * z1);

	A2 = 2 * (x2 - x1);
	B2 = 2 * (y2 - y1);
	C2 = 2 * (z2 - z1);
	D2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

	A3 = 2 * (x3 - x1);
	B3 = 2 * (y3 - y1);
	C3 = 2 * (z3 - z1);
	D3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

	// 获得圆心
	this->center.setX(-(B1 * C2 * D3 - B1 * C3 * D2 - B2 * C1 * D3 + B2 * C3 * D1 + B3 * C1 * D2 - B3 * C2 * D1)
		/ (A1 * B2 * C3 - A1 * B3 * C2 - A2 * B1 * C3 + A2 * B3 * C1 + A3 * B1 * C2 - A3 * B2 * C1));
	this->center.setY((A1 * C2 * D3 - A1 * C3 * D2 - A2 * C1 * D3 + A2 * C3 * D1 + A3 * C1 * D2 - A3 * C2 * D1)
		/ (A1 * B2 * C3 - A1 * B3 * C2 - A2 * B1 * C3 + A2 * B3 * C1 + A3 * B1 * C2 - A3 * B2 * C1));
	this->center.setZ(-(A1 * B2 * D3 - A1 * B3 * D2 - A2 * B1 * D3 + A2 * B3 * D1 + A3 * B1 * D2 - A3 * B2 * D1)
		/ (A1 * B2 * C3 - A1 * B3 * C2 - A2 * B1 * C3 + A2 * B3 * C1 + A3 * B1 * C2 - A3 * B2 * C1));

	// 半径取平均值
	this->r = 0.0;
	this->r += sqrt((x1 - center.getX()) * (x1 - center.getX()) + (y1 - center.getY()) * (y1 - center.getY()) + (z1 - center.getZ()) * (z1 - center.getZ()));
	this->r += sqrt((x2 - center.getX()) * (x2 - center.getX()) + (y2 - center.getY()) * (y2 - center.getY()) + (z2 - center.getZ()) * (z2 - center.getZ()));
	this->r += sqrt((x3 - center.getX()) * (x3 - center.getX()) + (y3 - center.getY()) * (y3 - center.getY()) + (z3 - center.getZ()) * (z3 - center.getZ()));
	this->r /= 3.0;

	// 获得所在平面
	this->planeOf.setA(A1);
	this->planeOf.setB(B1);
	this->planeOf.setC(C1);
	this->planeOf.setD(D1);
}

bool CCircle::atSphericalCrown(const CLocation& loc) {
	// 获取点和圆的所在平面
	double x = loc.getX();
	double y = loc.getY();
	double z = loc.getZ();
	double A = planeOf.getA();
	double B = planeOf.getB();
	double C = planeOf.getC();
	double D = planeOf.getD();

	if (fabs(center.getZ()) < 1e-7) {
		//z=0 特判
		if (fabs(center.getX()) < 1e-7) {
			//x=0
			//圆心在y轴上
			if (center.getY() >= 0) {
				return y >= center.getY();
			}
			else {
				return y < center.getY();
			}
		}
		else if (fabs(center.getY()) < 1e-7) {
			//y=0
			//圆心在x轴上
			if (center.getX() >= 0) {
				return x >= center.getX();
			}
			else {
				return x < center.getX();
			}
		}
	}
	else {
		//一般情况
		return center.getZ() * (A * x + B * y + C * z + D) / C > 0;
	}
}

double CCircle::getX() const {
	return center.getX();
}

void CCircle::setX(double x) {
	center.setX(x);
}

double CCircle::getY() const {
	return center.getY();
}

void CCircle::setY(double y) {
	center.setY(y);
}

double CCircle::getZ() const {
	return center.getZ();
}

void CCircle::setZ(double z) {
	center.setZ(z);
}

double CCircle::getR() const {
	return r;
}

void CCircle::setR(double r) {
	this->r = r;
}