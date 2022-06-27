#pragma once
#include "CCircle.h"

// CCoverageArea 对话框

class CCoverageAreaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoverageAreaDlg)

public:
	CCoverageAreaDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCoverageAreaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COVERAGE_AREA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedLoadArea();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	// 内存DC
	CDC memDC;
	// 覆盖区域已经画入内存的标志
	bool drawOK;
	// 用于鼠标落下时在内存DC中的真实坐标
	CPoint memPos;
	// 鼠标移动标志
	bool moving;
	// 拖曳区画面左上角在内存DC中的真实坐标
	CPoint picturePos;
	// 缩放区显示的区域范围
	int magnification;
public:
	// 消息响应
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	// 圆区域
	CCircle** circle = nullptr;
private:
	// 小时
	UINT _hour;
	// 分钟
	UINT _minute;
	// 秒
	UINT _second;
	// 控制选项
	CComboBox m_cb;
	// 进度条控制
	CProgressCtrl m_progress;
	// 覆盖率
	CString ans;
public:
	afx_msg void OnBnClickedButtonInfo();
	afx_msg void OnEnChangeEditHour();
	afx_msg void OnEnChangeEditMinute();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
