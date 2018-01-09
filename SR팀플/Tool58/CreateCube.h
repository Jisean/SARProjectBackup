#pragma once
#include "afxwin.h"

// CCreateCube 대화 상자입니다.

class CBack;
class CCreateCube : public CDialog
{
	DECLARE_DYNAMIC(CCreateCube)

public:
	CCreateCube(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreateCube();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATECUBE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CBack*		m_pBack;
	bool		m_bStart;

public:
	float	m_fScaleVal;
	float	m_fRotVal;
	float	m_fPosVal;
	

public:
	CString	m_PosX;
	CString	m_PosY;
	CString	m_PosZ;

	CString m_ScaleX;
	CString m_ScaleY;
	CString m_ScaleZ;

	CString m_RotX;
	CString m_RotY;
	CString m_RotZ;

public:
	CButton m_Radio[2];
	CButton m_RadioPoint[2];
	CButton m_CubeType[11];

public:
	afx_msg void OnAddCube();

	afx_msg void OnScaleX();
	afx_msg void OnScaleY();
	afx_msg void OnScaleZ();

	afx_msg void OnPosX();
	afx_msg void OnPosY();
	afx_msg void OnPosZ();
	
	afx_msg void OnRotX();
	afx_msg void OnRotY();
	afx_msg void OnRotZ();


	afx_msg void OnDel();
	afx_msg void OnSel();


	afx_msg void OnScaleXAdd();
	afx_msg void OnScaleXDe();
	afx_msg void OnScaleXEu();

	afx_msg void OnScaleYIn();
	afx_msg void OnScaleYDe();
	afx_msg void OnScaleYEu();

	afx_msg void OnScaleZIn();
	afx_msg void OnScaleZDe();
	afx_msg void OnScaleZEu();


	afx_msg void OnRotXIn();
	afx_msg void OnRotXDe();
	afx_msg void OnRotXEu();

	afx_msg void OnRotYIn();
	afx_msg void OnRotYDe();
	afx_msg void OnRotYEu();

	afx_msg void OnRotZIn();
	afx_msg void OnRotZDe();
	afx_msg void OnRotZEu();

	afx_msg void OnPosXIn();
	afx_msg void OnPosXDe();
	afx_msg void OnPosXEu();

	afx_msg void OnPosYIn();
	afx_msg void OnPosYDe();
	afx_msg void OnPosYEu();

	afx_msg void OnPosZIn();
	afx_msg void OnPosZDe();
	afx_msg void OnPosZEu();

	afx_msg void OnSelectCube();
};
