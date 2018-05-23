// StepDiffDlg.cpp : ���� �����Դϴ�.
//


#include "stdafx.h"
#include "PyroSoftM.h"
#include "StepDiffDlg.h"
#include "afxdialogex.h"


// CStepDiffDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStepDiffDlg, CDialogEx)

CStepDiffDlg::CStepDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STEP_DIFF, pParent)
{

}

CStepDiffDlg::~CStepDiffDlg()
{
}

void CStepDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UP_STEP_DIFF, m_upStep_diff);
	DDX_Control(pDX, IDC_MIDDLE_STEP_DIFF, m_middleStep_diff);
	DDX_Control(pDX, IDC_DOWN_STEP_DIFF, m_downStep_diff);
}

BEGIN_MESSAGE_MAP(CStepDiffDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStepDiffDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStepDiffDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CStepDiffDlg �޽��� ó�����Դϴ�.
BOOL CStepDiffDlg::PreTranslateMessage(MSG* pMsg)
{
	//����Ű�� ������ UPdate�� �� �ֵ��� 
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}

void CStepDiffDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	float tempUpStemp = 0.0f, tempMiddleStep = 0.0f, tempDownStep = 0.0f;
	int checkApply = 0;

	m_upStep_diff.GetWindowTextA(str);
	tempUpStemp = _ttof(str);
	CheckEmptyValue(str, &checkApply);

	m_middleStep_diff.GetWindowTextA(str);
	tempMiddleStep = _ttof(str);
	CheckEmptyValue(str, &checkApply);

	m_downStep_diff.GetWindowTextA(str);
	tempDownStep = _ttof(str);
	CheckEmptyValue(str, &checkApply);

	if (checkApply)	
	{
		AfxMessageBox("�������� �ʴ� ���� �����մϴ�.");	// -�����ӹ�
	}
	else
	{
		pDoc->m_upStepCoeff = tempUpStemp;
		pDoc->m_middleStepCoeff = tempMiddleStep;
		pDoc->m_downStepCoeff = -tempDownStep;

		AfxMessageBox("���� ���� �Ǿ����ϴ�.");
		CDialogEx::OnOK();
	}
}


void CStepDiffDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
	CDialogEx::OnCancel();
}

void CStepDiffDlg::CheckEmptyValue(CString str, int* checkCount)
{
	if (str == "" || str == "0")
		(*checkCount)++;
}


BOOL CStepDiffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	PrintStepDiff();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CStepDiffDlg::PrintStepDiff()
{
	CString str;

	str.Format(_T("%.4f"), pDoc->m_upStepCoeff);
	m_upStep_diff.SetWindowText(str);

	str.Format(_T("%.4f"), pDoc->m_middleStepCoeff);
	m_middleStep_diff.SetWindowText(str);

	str.Format(_T("%.4f"), -pDoc->m_downStepCoeff);
	m_downStep_diff.SetWindowText(str);

	//int a = 1, b = 2, c = 0;

	//::CalPlus(a, b, &c);

	//CString ss;
	//ss.Format(_T("%d"), c);

	//AfxMessageBox(ss);
}
