#pragma once
#include "afxwin.h"


// CZoneInfDlg ��ȭ �����Դϴ�.
class CZoneInfDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZoneInfDlg)

	//Ư���Դϴ�.
public:
	CPyroSoftMDoc * pDoc;

	CFont m_bFont;
	CFont m_mFont;
	CFont m_sFont;

	CEdit m_Zone_Distance[MAX_ZONE];
	CEdit m_Zone_Speed[MAX_ZONE];
	CEdit m_Zone_Temp[MAX_ZONE];
	CEdit m_Start_Position;
	CEdit m_End_Position;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REF_INF};
#endif

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	bool m_EmptyStringFlag;

	void LoadRecipeConfig();

	DECLARE_MESSAGE_MAP()

	//�۾��Դϴ�.
public:

	void InitRefDlg();
	void ZeroInit();
	void PrintZoneInf();
	void CheckEmptyValue(CString str);

	CZoneInfDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	
	virtual ~CZoneInfDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedApplyZone();
	afx_msg void OnBnClickedLoadRecipe();
	afx_msg void OnBnClickedSaveZone();
	afx_msg void OnBnClickedCloseZone();
};
