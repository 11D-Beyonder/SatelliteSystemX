#pragma once
#include "CPlane.h"
class CCircle {
private:
	// 圆心
	CLocation center;
	// 所在平面
	CPlane planeOf;
	// 半径
	double r;
public:
	CCircle();

	/// <summary>
	/// 给出圆心和半径
	/// </summary>
	/// <param name="x">圆心x坐标</param>
	/// <param name="y">圆心y坐标</param>
	/// <param name="z">圆心z坐标</param>
	/// <param name="r">半径</param>
	CCircle(double x, double y, double z, double r);

	/// <summary>
	/// 三点确定一个圆
	/// 用CLocation表示一个坐标
	/// </summary>
	/// <param name="point1">点1</param>
	/// <param name="point2">点2</param>
	/// <param name="point3">点3</param>
	CCircle(const CLocation& point1, const CLocation& point2, const CLocation& point3);

	/// <summary>
	/// 球面点是否在球冠
	/// </summary>
	/// <param name="loc">点</param>
	/// <returns>在球冠上返回1 否则为0</returns>
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