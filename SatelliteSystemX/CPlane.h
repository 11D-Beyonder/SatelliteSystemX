#pragma once
#include "CLocation.h"
class CPlane {
private:
	// ����ȷ��ƽ��
	// Ax+By+Cz+D=0
	double A, B, C, D;
public:
	CPlane();

	/// <summary>
	/// ����ȷ��ƽ��
	/// </summary>
	/// <param name="point1">��1</param>
	/// <param name="point2">��2</param>
	/// <param name="point3">��3</param>
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