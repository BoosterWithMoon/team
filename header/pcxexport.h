// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CPCXExport wrapper class

class CPCXExport : public COleDispatchDriver
{
public:
	CPCXExport() {}		// Calls COleDispatchDriver default constructor
	CPCXExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPCXExport(const CPCXExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetHeight();
	void SetHeight(long nNewValue);
	void SaveToFile(LPCTSTR FileName);
	VARIANT SaveToStream();
};
