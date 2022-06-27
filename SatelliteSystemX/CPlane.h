#pragma once
#include "CLocation.h"
class CPlane {
private:
	// 参数确定平面
	// Ax+By+Cz+D=0
	double A, B, C, D;
public:
	CPlane();

	/// <summary>
	/// 三点确定平面
	/// </summary>
	/// <param name="point1">点1</param>
	/// <param name="point2">点2</param>
	/// <param name="point3">点3</param>
	CPlane(CLocation point1, CLocation point2, CLocation point3);


	// getter setter
	double getA() const;
	void setA(double A);

	double getB() const;
	void setB(double B);

	double getC() const;
	void setC(double C);

	double getD() const;
	void setD(double D);

};