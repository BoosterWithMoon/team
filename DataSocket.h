#pragma once

class CPyroSoftMApp;

// CDataSocket ��� ����Դϴ�.

class CDataSocket : public CSocket
{
public:
	CPyroSoftMApp* m_pApp;

	CDataSocket();
	CDataSocket(CPyroSoftMApp* pApp);

	virtual ~CDataSocket();
};


