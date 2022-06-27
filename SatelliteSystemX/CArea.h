#pragma once
class CArea {
private:
	//���ϽǺ����½Ǳ�ʾһ������
	double longitude_l;
	double latitude_t;
	double longitude_r;
	double latitude_b;
public:
	CArea();

	/// <summary>
	/// ������������ȷ��һ����������
	/// </summary>
	/// <param name="longitude_l">���ϽǾ���</param>
	/// <param name="latitude_t">���Ͻ�γ��</param>
	/// <param name="longitude_r">���½Ǿ���</param>
	/// <param name="latitude_b">���½�γ��</param>
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