#pragma once
#include "CPlane.h"
class CCircle {
private:
	// Բ��
	CLocation center;
	// ����ƽ��
	CPlane planeOf;
	// �뾶
	double r;
public:
	CCircle();

	/// <summary>
	/// ����Բ�ĺͰ뾶
	/// </summary>
	/// <param name="x">Բ��x����</param>
	/// <param name="y">Բ��y����</param>
	/// <param name="z">Բ��z����</param>
	/// <param name="r">�뾶</param>
	CCircle(double x, double y, double z, double r);

	/// <summary>
	/// ����ȷ��һ��Բ
	/// ��CLocation��ʾһ������
	/// </summary>
	/// <param name="point1">��1</param>
	/// <param name="point2">��2</param>
	/// <param name="point3">��3</param>
	CCircle(const CLocation& point1, const CLocation& point2, const CLocation& point3);

	/// <summary>
	/// ������Ƿ������
	/// </summary>
	/// <param name="loc">��</param>
	/// <returns>������Ϸ���1 ����Ϊ0</returns>
	bool atSphericalCrown(const CLocation& loc);

	// getter setter
	double getX() const;
	void setX(double x);

	double getY() const;
	void setY(double y);

	double getZ() const;
	void setZ(double z);

	double getR() const;
	void setR(double r);
};