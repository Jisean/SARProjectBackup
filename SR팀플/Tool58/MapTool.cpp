// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool58.h"
#include "MapTool.h"
#include "FileInfo.h"
#include "Back.h"
#include "MainFrm.h"
#include "Tool58View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
{
	//m_iDrawID = 0;
}

CMapTool::~CMapTool()
{
	for(map<CString, CImage*>::iterator iter = m_MapPngImage.begin();
		iter != m_MapPngImage.end(); ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_MapPngImage.clear();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveButton)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	int	iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 1. 드롭된 파일을 나타내는 내부 데이터 구조체를 조작하기 위한 핸들
	// 2. 파일의 인덱스를 지정. -1을 지정하면 함수의 반환 값으로 드래그 앤 드롭된 파일의 수를 반환
	// 3. 취득한 파일명을 격납하는 버퍼의 포인터를 지정
	// 4. 지정한 버퍼 사이즈 

	TCHAR	szFullPath[MAX_PATH] = L"";
	CFileInfo		FileInfo;

	for(int i = 0; i < iFileNumber; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);
		wstring	wstrRelativePath = FileInfo.ConvertRelativePath(szFullPath);
		CString strImageName	 = PathFindFileName(wstrRelativePath.c_str());
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		map<CString, CImage*>::iterator	iter = m_MapPngImage.find(strImageName);

		if(iter == m_MapPngImage.end())
		{
			CImage*		pPngImage = new CImage;

			pPngImage->Load(wstrRelativePath.c_str());
			m_MapPngImage.insert(make_pair(strImageName, pPngImage));
			m_ListBox.AddString(strImageName);
		}

	}

	HorizontalScroll();

	UpdateData(FALSE);
}

void CMapTool::HorizontalScroll(void)
{
		CString		strName;
		CSize		csize;	
		int			iDCX = 0;		// 리스트박스 출력을 위한 dc x길이를 저장

		CDC*		pDC = m_ListBox.GetDC();

		for(int i = 0; i < m_ListBox.GetCount(); ++i) // 리스트 박스의 목록 개수 반환 
		{
			m_ListBox.GetText(i, strName);
			csize = pDC->GetTextExtent(strName);	// 글자의 크기를 픽셀 크기로 변환

			if(csize.cx > iDCX)
				iDCX = csize.cx;
		}
		m_ListBox.ReleaseDC(pDC);

		if(m_ListBox.GetHorizontalExtent() < iDCX)
		{
			m_ListBox.SetHorizontalExtent(iDCX);
		}

}

void CMapTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString		strSelectName;

	int		iSelectNumber = m_ListBox.GetCurSel();

	if(iSelectNumber < 0)
		return;

	m_ListBox.GetText(iSelectNumber, strSelectName);

	map<CString, CImage*>::iterator	iter = m_MapPngImage.find(strSelectName);

	if(iter == m_MapPngImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);

	int i = 0;

	

	for(i = 0; i < strSelectName.GetLength(); ++i)
	{
		if(isdigit(strSelectName[i]) != 0)		// 반환값이 0인 경우는 글자 문자인 경우
			break;								// 반환값이 0이 아닌 경누는 숫자 문자인 경우
	}

	strSelectName.Delete(0, i);
	m_iDrawID = _tstoi(strSelectName);	// 문자열을 정수형 데이터로 바꿔주는 유니코드 기반 함수

	UpdateData(FALSE);
}

void CMapTool::OnSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"*.dat", this);

	// 1. 저장 모드 지원 2. 파일의 확장자명 3. 최초에 띄워 줄 문자열 
	// 4. 중복 파일 처리에 대한 매개 변수 5. 저장 형식에 따른 확장자명

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	SetCurrentDirectory(szDirPath);
	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 설정해주는 함수

	PathRemoveFileSpec(szDirPath);
	// 마지막 경로를 제외한 경로를 추출하는 함수

	lstrcat(szDirPath, L"..\\Data");


	Dlg.m_ofn.lpstrInitialDir = szDirPath;		// 상대 경로

	CObj*		pBack = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;

	const vector<TILE*>*	pVecTile = ((CBack*)pBack)->GetTile();

	if(pVecTile == NULL)
		return;

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		WriteFile(hFile, (*pVecTile)[i], sizeof(TILE), &dwByte, NULL);
	}

	CloseHandle(hFile);
}
