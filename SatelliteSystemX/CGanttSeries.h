// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ����

//#import "D:\\Microsoft Visual Studio\\TeeChart.Pro.v5.ActiveX\\body\\TeeChart5.ocx" no_namespace
// CGanttSeries ��װ����

class CGanttSeries : public COleDispatchDriver {
public:
	CGanttSeries() {} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CGanttSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGanttSeries(const CGanttSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IGanttSeries ����
public:
	LPDISPATCH get_Pointer() {
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	BOOL get_Dark3D() {
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Dark3D(BOOL newValue) {
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_ConnectingPen() {
		LPDISPATCH result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_EndValues() {
		LPDISPATCH result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_StartValues() {
		LPDISPATCH result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_NextTask() {
		LPDISPATCH result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	long AddGantt(double AStart, double AEnd, double AY, LPCTSTR SomeLabel) {
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_BSTR;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, AStart, AEnd, AY, SomeLabel);
		return result;
	}
	long AddGanttColor(double AStart, double AEnd, double AY, LPCTSTR AXLabel, unsigned long Value) {
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_BSTR VTS_UI4;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms, AStart, AEnd, AY, AXLabel, Value);
		return result;
	}

	// IGanttSeries ����
public:

};
