// MyPropertyGridProperty.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "FloatPropertyGridProperty.h"


// CFLoatPropertyGridProperty

IMPLEMENT_DYNAMIC(CFLoatPropertyGridProperty, CMFCPropertyGridProperty)

CFLoatPropertyGridProperty::CFLoatPropertyGridProperty(const CString& strName, const COleVariant& varValue)
:CMFCPropertyGridProperty(strName, varValue, NULL, 0)
{

}

CFLoatPropertyGridProperty::~CFLoatPropertyGridProperty()
{
}
