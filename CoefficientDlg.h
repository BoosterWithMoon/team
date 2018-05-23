#pragma once
#include "afxwin.h"


// CCoefficient ��ȭ �����Դϴ�.

class CCoefficientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCoefficientDlg)

public:
	CCoefficientDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCoefficientDlg();

	int		m_EmptyStringFlag;

	void	CheckEmptyValue(CString str);

	void	PrintZoneInf();

	CPyroSoftMDoc * pDoc;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COEFFICIENT_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAccept();
	afx_msg void OnBnClickedCancel();
	CEdit m_Third_Coeff;
	CEdit m_Second_Coeff;
	CEdit m_First_Coeff;
	CEdit m_Constant;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
