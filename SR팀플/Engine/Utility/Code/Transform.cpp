#include "Transform.h"
#include "..\..\Reference\Headers\Pipeline.h"

ENGINE::CTransform::CTransform(const D3DXVECTOR3& vLook)
: m_vPos(0.f, 0.f, 0.f)
, m_vDir(vLook)
, m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
}

ENGINE::CTransform::~CTransform()
{

}

void ENGINE::CTransform::Update(void)
{
	ENGINE::CPipeline::MakeWorldMatrix(&m_matWorld, 
		&m_vScale, 
		m_fAngle,
		&m_vPos);
}

ENGINE::CTransform* ENGINE::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

