// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPen1;
class CSeries;

/////////////////////////////////////////////////////////////////////////////
// CRVIFunction wrapper class

class CRVIFunction : public COleDispatchDriver
{
public:
	CRVIFunction() {}		// Calls COleDispatchDriver default constructor
	CRVIFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRVIFunction(const CRVIFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CPen1 GetSignalPen();
	CSeries GetSignal();
};