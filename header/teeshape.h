// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CGradient;
class CChartFont;
class CTeeShadow;

/////////////////////////////////////////////////////////////////////////////
// CTeeShape wrapper class

class CTeeShape : public COleDispatchDriver
{
public:
	CTeeShape() {}		// Calls COleDispatchDriver default constructor
	CTeeShape(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeeShape(const CTeeShape& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetTransparency();
	void SetTransparency(long nNewValue);
	long GetBevel();
	void SetBevel(long nNewValue);
	long GetBevelWidth();
	void SetBevelWidth(long nNewValue);
	unsigned long GetColor();
	void SetColor(unsigned long newValue);
	CGradient GetGradient();
	CChartFont GetFont();
	CTeeShadow GetShadow();
	long GetShapeStyle();
	void SetShapeStyle(long nNewValue);
	void LoadImage(LPCTSTR FileName);
	void AssignImage(long ImageHandle);
	void ClearImage();
};