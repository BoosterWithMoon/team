// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGammaCorrectionFilter wrapper class

class CGammaCorrectionFilter : public COleDispatchDriver
{
public:
	CGammaCorrectionFilter() {}		// Calls COleDispatchDriver default constructor
	CGammaCorrectionFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGammaCorrectionFilter(const CGammaCorrectionFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetPercent();
	void SetPercent(BOOL bNewValue);
	long GetAmount();
	void SetAmount(long nNewValue);
};
