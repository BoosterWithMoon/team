// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CAxisScrollTool wrapper class

class CAxisScrollTool : public COleDispatchDriver
{
public:
	CAxisScrollTool() {}		// Calls COleDispatchDriver default constructor
	CAxisScrollTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAxisScrollTool(const CAxisScrollTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	VARIANT GetAxis();
	void SetAxis(const VARIANT& newValue);
	BOOL GetScrollInverted();
	void SetScrollInverted(BOOL bNewValue);
};
