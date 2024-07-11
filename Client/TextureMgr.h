#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* Get_Texture(const TCHAR* pObjKey,
		const TCHAR* pStateKey = L"",
		const int& iCount = 0);

public:
	HRESULT		ReadImgPath(const wstring& wstrPath);

	HRESULT		Insert_Texture(const TCHAR* pFilePath,
		TEXTYPE eType,
		const TCHAR* pObjKey,
		const TCHAR* pStateKey = L"",
		const int& iCount = 0);

	void		Release();

private:
	map<wstring, CTexture*>		m_mapTex;
};

