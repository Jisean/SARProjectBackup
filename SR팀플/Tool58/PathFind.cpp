// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool58.h"
#include "PathFind.h"
#include "FileInfo.h"

#include "MainFrm.h"
#include "Tool58View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(CPathFind::IDD, pParent)
{
	m_bStart = false;
}

CPathFind::~CPathFind()
{
	for_each(m_PathList.begin(), m_PathList.end(), DELETEOBJ());
	m_PathList.clear();


}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);

	if(m_bStart == false)
	{
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_bPathFind = true;

		m_Radio[0].SetCheck(1);
		m_bStart = true;
	}
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind �޽��� ó�����Դϴ�.

void CPathFind::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CPathFind::OnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_Radio[0].GetCheck())
	{
		wofstream		SaveFile;

		SaveFile.open(L"../Data/ImgPath_dds.txt", ios::out);	// ���� ����

		int		iCount = 0;

		for(list<IMGPATH*>::iterator iter = m_PathList.begin();
			iter != m_PathList.end(); ++iter)
		{
			++iCount;

			//SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
			SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
			//SaveFile << (*iter)->iCount << L"|";

			if(iCount == m_PathList.size())
				SaveFile << (*iter)->wstrPath.c_str() <<  flush;
			else
				SaveFile << (*iter)->wstrPath.c_str() << endl;

		}

		SaveFile.close();
	}
	else if(m_Radio[1].GetCheck())
	{
		wofstream		SaveFile;

		SaveFile.open(L"../Data/ImgPath_png.txt", ios::out);	// ���� ����

		int		iCount = 0;

		for(list<IMGPATH*>::iterator iter = m_PathList.begin();
			iter != m_PathList.end(); ++iter)
		{
			++iCount;

			SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
			SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
			SaveFile << (*iter)->iCount << L"|";

			if(iCount == m_PathList.size())
				SaveFile << (*iter)->wstrPath.c_str() <<  flush;
			else
				SaveFile << (*iter)->wstrPath.c_str() << endl;

		}

		SaveFile.close();
	}
		

	//WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	if(m_Radio[0].GetCheck())
	{
		wifstream		LoadFile;
		LoadFile.open(L"../Data/ImgPath_dds.txt", ios::in);	// ���� ����

		TCHAR	szObjKey[MIN_STR]	= L"";
		TCHAR	szStateKey[MIN_STR] = L"";
		TCHAR	szCount[MIN_STR]	= L"";
		TCHAR	szImgPath[MAX_PATH] = L"";

		wstring	wstrCombine = L"";

		m_ListBox.ResetContent();

		while(!LoadFile.eof())
		{
			//LoadFile.getline(szObjKey, MIN_STR, '|');
			LoadFile.getline(szStateKey, MIN_STR, '|');
			//LoadFile.getline(szCount, MIN_STR, '|');
			LoadFile.getline(szImgPath, MAX_PATH);

			wstrCombine = wstring(szObjKey) /*+ L"|" + wstring(szStateKey) + L"|"*/;
			//wstrCombine += szCount;
			wstrCombine += L"|" + wstring(szImgPath);

			m_ListBox.AddString(wstrCombine.c_str());
		}

		LoadFile.close();	

		WinExec("notepad.exe ../Data/ImgPath_dds.txt", SW_SHOW);
	}
	else if(m_Radio[1].GetCheck())
	{
		wifstream		LoadFile;
		LoadFile.open(L"../Data/ImgPath_png.txt", ios::in);	// ���� ����

		TCHAR	szObjKey[MIN_STR]	= L"";
		TCHAR	szStateKey[MIN_STR] = L"";
		TCHAR	szCount[MIN_STR]	= L"";
		TCHAR	szImgPath[MAX_PATH] = L"";

		wstring	wstrCombine = L"";

		m_ListBox.ResetContent();

		while(!LoadFile.eof())
		{
			LoadFile.getline(szObjKey, MIN_STR, '|');
			LoadFile.getline(szStateKey, MIN_STR, '|');
			LoadFile.getline(szCount, MIN_STR, '|');
			LoadFile.getline(szImgPath, MAX_PATH);

			wstrCombine = wstring(szObjKey) + L"|" + wstring(szStateKey) + L"|";
			wstrCombine += szCount;
			wstrCombine += L"|" + wstring(szImgPath);

			m_ListBox.AddString(wstrCombine.c_str());
		}

		LoadFile.close();	

		WinExec("notepad.exe ../Data/ImgPath_dds.txt", SW_SHOW);
	}
	

	UpdateData(FALSE);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	if(m_Radio[0].GetCheck())
	{
		int	iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);
		TCHAR	szFullPath[MAX_PATH] = L"";

		for(int i = 0; i < iFileNumber; ++i)
		{
			DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

			CFileInfo		FileInfo;
			FileInfo.DirInfoExtraction(szFullPath, m_PathList);

			wstring	wstrCombine		= L"";
			TCHAR	szBuf[MIN_STR]	= L"";

			for(list<IMGPATH*>::iterator	iter = m_PathList.begin();
				iter != m_PathList.end(); ++iter)
			{
				_itow_s((*iter)->iCount, szBuf, 10);	// 10������ �ٲ㼭 ���ڿ��� ��ȯ

				wstrCombine	= /*(*iter)->wstrObjKey + L"|" +*/ (*iter)->wstrStateKey + L"|";
				//wstrCombine += szBuf;
				wstrCombine += /*L"|" + */(*iter)->wstrPath;

				m_ListBox.AddString(wstrCombine.c_str());
			}
		}
	}
	else if(m_Radio[1].GetCheck())
	{
		int	iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);
		TCHAR	szFullPath[MAX_PATH] = L"";

		for(int i = 0; i < iFileNumber; ++i)
		{
			DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

			CFileInfo		FileInfo;
			FileInfo.DirInfoExtractionPng(szFullPath, m_PathList);

			wstring	wstrCombine		= L"";
			TCHAR	szBuf[MIN_STR]	= L"";

			for(list<IMGPATH*>::iterator	iter = m_PathList.begin();
				iter != m_PathList.end(); ++iter)
			{
				_itow_s((*iter)->iCount, szBuf, 10);	// 10������ �ٲ㼭 ���ڿ��� ��ȯ

				wstrCombine	= (*iter)->wstrObjKey + L"|" + (*iter)->wstrStateKey + L"|";
				wstrCombine += szBuf;
				wstrCombine += L"|" + (*iter)->wstrPath;

				m_ListBox.AddString(wstrCombine.c_str());
			}
		}
	}
	

	UpdateData(FALSE);
}
