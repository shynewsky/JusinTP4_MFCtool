#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);


	// PathRelativePathTo : szCurDirPath 에서 strFullPath로 가는 상대 경로를 구해서 szRelativePath에 저장
	//						단, 같은 드라이브 안에서만 적용 가능

	PathRelativePathTo(szRelativePath, 
		szCurDirPath, FILE_ATTRIBUTE_DIRECTORY, 
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathInfoList)
{
	// CFileFind : 파일 및 파일 경로 제어 관련 mfc 클래스

	CFileFind		Find;

	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : 매개 변수로 전달된 경로에 파일이 있는지 유무를 확인하는 함수, 존재하지 않으면 FALSE, 존재하면 TRUE를 리턴

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에서 다음 파일들을 찾아나선다. 더이상 찾을게 없거나 마지막 대상인 경우 0을 반환
		bContinue = Find.FindNextFile();
		
		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : 현재 파일을 찾은 경로를 반환하는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}

		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			PathRemoveFileSpec(szPathBuf);
			// szPathBuf = 0x0115e0b0 L"D:\\유준환\\145기\\Frame145\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : 온전한 파일 이름만 얻어오는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			// wstrTextureName = L"BossMultiAttack0"

			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 얻어오는 함수
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			// PathCombine(dst, src1, src2) : src1과 src2를 이어붙여 완성된 경로를 dst에 저장, src1과 src2 사이에 '\\'가 자동으로 삽입된 채 저장됨
			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());
			// szPathBuf = 0x0115e0b0 L"D:\\유준환\\145기\\Frame145\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = CFileInfo::ConvertRelativePath(szPathBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szPathBuf);
			// PathFindFileName : 파일명을 찾거나 마지막 폴더명을 찾아냄
			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);

			PathRemoveFileSpec(szPathBuf);
			// PathFindFileName : 파일명을 찾거나 마지막 폴더명을 찾아냄
			pImgPath->wstrObjKey = PathFindFileName(szPathBuf);

			rPathInfoList.push_back(pImgPath);

			bContinue = 0;
		}

	}

}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	CFileFind		Find;

	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : 매개 변수로 전달된 경로에 파일이 있는지 유무를 확인하는 함수, 존재하지 않으면 FALSE, 존재하면 TRUE를 리턴

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int		iFileCnt(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		if(Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
