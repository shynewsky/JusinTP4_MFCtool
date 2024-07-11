#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR* pFilePath,
	const TCHAR* pStateKey,
	const int& iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 D3DXIMAGE_INFO 구조체에 기록하는 함수

	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);

		ERR_MSG(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT		: 가장 적합한 메모리에 보관(그래픽 카드 메모리)
	// D3DPOOL_MANAGED		: DX에 의해 자원을 관리하고 그래픽 메모리를 사용하지마 이를 RAM에 백업
	// D3DPOOL_SYSTEMMEM	: RAM에 자원을 저장
	// D3DPOOL_SCRATCH		: RAM에 저장하지만 DX 장치가 접근하지 못함

	// D3DXCreateTextureFromFileEx : 이미지 담당 com 객체를 생성하는 함수
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,		// 0인 기본적인 텍스처 형식, D3DUSAGE_RENDERTARGET : 렌더 타겟용 텍스처
		m_pTexInfo->tImgInfo.Format,		// 텍스처의 픽셀 포멧
		D3DPOOL_MANAGED,		// 메모리 POOL 방식
		D3DX_DEFAULT,		// 이미지 필터링 방식, 이미지 확대, 축소 시 픽셀이 모자라거나 남을 때 어떻게 할 것인가
		D3DX_DEFAULT,		// 밉맵 이용 때 이미지 확대, 축소 시 픽셀이 모자라거나 남을 때 어떻게 할 것인가
		0,					// 제거할 색상, 0을 주면 컬러키 무효화
		nullptr,			// m_pTexInfo->tImgInfo 정보 기록
		nullptr,			// 팔레트(8비트 시절 ARGB 값을 표현하기 위한 구조체) 이미지 정보 주소
		&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);

		ERR_MSG(L"Single Texture Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
