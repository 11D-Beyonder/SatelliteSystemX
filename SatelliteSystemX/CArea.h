#pragma once
class CArea {
private:
	//左上角和右下角表示一个区域
	double longitude_l;
	double latitude_t;
	double longitude_r;
	double latitude_b;
public:
	CArea();

	/// <summary>
	/// 地球上两个点确定一个矩形区域
	/// </summary>
	/// <param name="longitude_l">左上角经度</param>
	/// <param name="latitude_t">左上角纬度</param>
	/// <param name="longitude_r">右下角经度</param>
	/// <param name="latitude_b">右下角纬度</param>
	CArea(double longitude_l, double latitude_t, double longitude_r, double latitude_b);

	//getter setter
	double getLongitudel() const;
	void setLongitudel(double longitudel);

	double getLatitudet() const;
	void setLatitudet(double latitudet);

	double getLongituder() const;
	void setLongituder(double longituder);

	double getLatitudeb() const;
	void setLatitudeb(double latitudeb);

};