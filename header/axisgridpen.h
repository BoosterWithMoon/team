// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CAxisGridPen wrapper class

class CAxisGridPen : public COleDispatchDriver
{
public:
	CAxisGridPen() {}		// Calls COleDispatchDriver default constructor
	CAxisGridPen(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAxisGridPen(const CAxisGridPen& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetDrawEvery();
	void SetDrawEvery(long nNewValue);
	double GetZPosition();
	void SetZPosition(double newValue);
};
