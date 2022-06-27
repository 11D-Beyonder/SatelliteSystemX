#pragma once
const double EARTH_R = 6371;
class CLocation {
private:
	// ��ά�����ʾλ��
	double x, y, z;
public:
	CLocation();

	/// <summary>
	/// ��άֱ������ȷ��λ��
	/// </summary>
	/// <param name="x">x����</param>
	/// <param name="y">y����</param>
	/// <param name="z">z����</param>
	CLocation(double x, double y, double z);
	
	/// <summary>
	/// ��γ��ȷ������
	/// </summary>
	/// <param name="longitude">����</param>
	/// <param name="latitude">γ��</param>
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
/// �Ƕȵ�����
/// </summary>
/// <param name="angle">�Ƕ���</param>
/// <returns>������</returns>
double AngleToRad(double angle);