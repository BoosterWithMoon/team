// PropertyGridCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "PropertyGridCtrl.h"


// CPropertyGridCtrl

IMPLEMENT_DYNAMIC(CPropertyGridCtrl, CMFCPropertyGridCtrl)

CPropertyGridCtrl::CPropertyGridCtrl()
{

}

CPropertyGridCtrl::~CPropertyGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CPropertyGridCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()



// CPropertyGridCtrl �޽��� ó�����Դϴ�.
void CPropertyGridCtrl::SetLeftColumnWidth(int cx)
{
	m_nLeftColumnWidth = cx;
    AdjustLayout();
}

void CPropertyGridCtrl::AdjustLayout()
{
	//m_nLeftColumnWidth= 200;

	return CMFCPropertyGridCtrl::AdjustLayout();
}
