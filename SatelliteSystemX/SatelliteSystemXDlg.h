
// SatelliteSystemXDlg.h: 头文件
//

#pragma once
#include "CCircle.h"

// CSatelliteSystemXDlg 对话框
class CSatelliteSystemXDlg : public CDialogEx
{
// 构造
public:
	CSatelliteSystemXDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SATELLITESYSTEMX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 圆区域
	CCircle** circle = nullptr;
	// 初始化时读取文件并生成圆
	void getCircles();
public:
	afx_msg void OnBnClickedToTimewindow();
	afx_msg void OnClose();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedToGreatFinder();
	afx_msg void OnBnClickedToCoverageProportion();
	afx_msg void OnBnClickedToCoverageArea();
};
