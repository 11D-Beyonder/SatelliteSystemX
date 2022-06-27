#pragma once
const double EARTH_R = 6371;
class CLocation {
private:
	// 三维坐标表示位置
	double x, y, z;
public:
	CLocation();

	/// <summary>
	/// 三维直角坐标确定位置
	/// </summary>
	/// <param name="x">x坐标</param>
	/// <param name="y">y坐标</param>
	/// <param name="z">z坐标</param>
	CLocation(double x, double y, double z);
	
	/// <summary>
	/// 经纬度确定坐标
	/// </summary>
	/// <param name="longitude">经度</param>
	/// <param name="latitude">纬度</param>
	CLocation(double longitude, double latitude);

	// getter setter

	double getX() const;
	void setX(double x);

	double getY() const;
	void setY(double y);

	double getZ() const;
	void setZ(double z);

};

/// <summary>
/// 角度到弧度
/// </summary>
/// <param name="angle">角度制</param>
/// <returns>弧度制</returns>
double AngleToRad(double angle);