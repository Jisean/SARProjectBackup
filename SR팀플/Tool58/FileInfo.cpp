#include "StdAfx.h"
#include "FileInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFileInfo::CFileInfo(void)
{
}

CFileInfo::~CFileInfo(void)
{
}

std::wstring CFileInfo::ConvertRelativePath(wstring wstrFullPath)
{
	TCHAR		szRelativePath[MAX_PATH]	= L"";
	TCHAR		szDirPath[MAX_PATH]			= L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	PathRelativePathTo((LPWSTR)szRelativePath, 
		szDirPath, FILE_ATTRIBUTE_DIRECTORY, 
		wstrFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, 
								  list<IMGPATH*>& rPathList)
{
	CFileFind			Find;
	
	Find.FindFile(wstrPath.c_str());

	int			iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile(); // 파일의 유무 관계를 판단하는 함수

		if(Find.IsDots())
			continue;

		if(Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}	

		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPathBuf);
			pImgPath->iCount = DirFileCount(szPathBuf);

			wstring	wstrTextureName = Find.GetFileTitle().operator LPCWSTR();	 
			// 경로를 제외한 파일의 이름만 얻어오는 함수

			wstring wstrName = wstrTextureName.substr(0, wstrTextureName.size());

			//wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.dds";
			wstrTextureName += L".dds";

			TCHAR		szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, (LPWSTR)Find.GetFilePath().operator LPCWSTR());

			PathRemoveFileSpec(szBuf);

			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());
			pImgPath->wstrPath = ConvertRelativePath(szBuf);

			PathRemoveFileSpec(szBuf);
			PathFindFileName(szBuf);
			pImgPath->wstrStateKey = wstrName;

			PathRemoveFileSpec(szBuf);
			/*pImgPath->wstrObjKey =*/ PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();

		}
	}

}

void CFileInfo::DirInfoExtractionPng(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	CFileFind			Find;

	Find.FindFile(wstrPath.c_str());

	int			iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile(); // 파일의 유무 관계를 판단하는 함수

		if(Find.IsDots())
			continue;

		if(Find.IsDirectory())
		{
			DirInfoExtractionPng(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}	

		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPathBuf);
			pImgPath->iCount = DirFileCountPng(szPathBuf);

			wstring	wstrTextureName = Find.GetFileTitle().operator LPCWSTR();	 
			// 경로를 제외한 파일의 이름만 얻어오는 함수

			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			

			TCHAR		szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, (LPWSTR)Find.GetFilePath().operator LPCWSTR());

			PathRemoveFileSpec(szBuf);

			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());
			pImgPath->wstrPath = ConvertRelativePath(szBuf);

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();

		}
	}
}

int CFileInfo::DirFileCount(const wstring& wstrPath)
{
	wstring		wstrTemp = wstrPath + L"\\*.dds";

	CFileFind		Find;

	Find.FindFile(wstrTemp.c_str());

	int		iContinue = 1;
	int		iCount = 0;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(!Find.IsDirectory())
			++iCount;

	}

	return iCount;
}

int CFileInfo::DirFileCountPng(const wstring& wstrPath)
{
	wstring		wstrTemp = wstrPath + L"\\*.png";

	CFileFind		Find;

	Find.FindFile(wstrTemp.c_str());

	int		iContinue = 1;
	int		iCount = 0;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(!Find.IsDirectory())
			++iCount;

	}

	return iCount;
}
