// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CValueList;

/////////////////////////////////////////////////////////////////////////////
// CValueLists wrapper class

class CValueLists : public COleDispatchDriver
{
public:
	CValueLists() {}		// Calls COleDispatchDriver default constructor
	CValueLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueLists(const CValueLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	CValueList GetItems(long Index);
};
