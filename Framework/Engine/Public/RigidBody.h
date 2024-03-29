#pragma once
#include "Component.h"



BEGIN(Engine)

class ENGINE_DLL CRigidBody : public CComponent
{
public:
    typedef struct tagRigidBodyDesc
    {
        class CTransform* pTransform = nullptr;
    } RIGID_BODY_DESC;

private:
    CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CRigidBody(CRigidBody& rhs);
    virtual ~CRigidBody() = default;


public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);

public:
    void Update_RigidBody(_float fTimeDelta);

public:
    void Add_Velocity(_vector vDir, _float fForce, _bool bClear);

    void Set_Velocity(Vec3 vVelocity) { m_vVelocity = vVelocity; }
    Vec3 Get_Velocity() { return m_vVelocity; }

public:
    _bool Is_Use_Gravity() { return m_bUseGravity; }
    void Set_Use_Gravity(_bool bGravity) { m_bUseGravity = bGravity; }

    _bool Is_Ground() { return m_bGround; }
    void Set_Ground(_bool bGround) { 

        m_bGround = bGround; 

        if (false == bGround)
        {
            m_fFrictionScale = 1.f;
        }
        else
        {
            m_vVelocity.y = 0.f;
            m_fFrictionScale = 10.f;
        }
            
    }

public:
    _float Get_FrictionScale() { return m_fFrictionScale; }
    void Set_FrictionScale(_float fFrictionScale) { m_fFrictionScale = fFrictionScale; }

    const _bool& Is_Sleep() const { return m_bSleep; }
    void Set_Sleep(const _bool& bSleep) { m_bSleep = bSleep; }
    const _bool& Check_Sleep();

private:
    void Update_Gravity(_float fTimeDelta);
    void Update_Velocity(_float fTimeDelta);
    

private:
    class CTransform* m_pTransformCom = nullptr;

private:
    _bool m_bUseGravity = true;
    _bool m_bGround = false;
    Vec3 m_vVelocity = {};
    _float m_fFrictionScale = 1.f;

    _bool   m_bSleep = true;
    const _float m_fSleepThreshold = 0.05f;


public:
    static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;

};
END


//public:
//    void Set_Gravity(_float fGravity) { m_fGravity = fGravity; }
//    void Set_Gravity(_bool bGravity)
//    {
//        if (true == bGravity)
//            m_fGravity = 14.7f;
//        else
//            m_fGravity = 0.f;
//
//        m_bIsGravity = bGravity;
//    }
//
//    _bool Is_Gravity() { return m_bIsGravity; }
//
//    _bool Is_Ground() { return m_bIsGround; }
//    void Set_Ground(_bool bGround) { m_bIsGround = bGround; }
//
//    void Set_PushVelocity(_fvector vPushVelocity, _float fTimeDelta);
//
//    void Add_Velocity(_fvector vVelocity);
//    void Add_Velocity(_fvector vDir, _float fForce);
//
//    void Add_Velocity_Acc(_fvector vVelocity);
//    void Add_Velocity_Acc(_fvector vDir, _float fForce);
//
//    void Set_RefHeight(_float fHeight) { m_fRefHeight = fHeight; }
//    _float Get_RefHeight() { return m_fRefHeight; }
//
//    _float3 Get_Velocity() { return m_vVelocity; }
//    void Set_Velocity(_float3 vVelocity) { m_vVelocity = vVelocity; }
//
//    void Set_Friction_Scale(_float fScale) { m_fFrictionScale = fScale; }
//
//private:
//    void Update_Gravity(_float fTimeDelta);
//    void Update_Velocity(_float fTimeDelta);
//
//private:
//    _float m_fGravity = 14.7f;               // 중력 계수
//    _float3 m_vVelocity = { 0.f, 0.f, 0.f };    // 현재 속도
//
//    _float3 m_vMaxVelocity = { 100.f, 100.f, 100.f }; // 속도 최대치.
//    _float3 m_vFriction = { 0.f, 0.f, 0.f };    // 현재 속도에 대응하는 마찰력.
//
//    _bool m_bIsGround = false;   // 현재 땅에 있는가?
//    _bool m_bIsGravity = true;  // 중력의 영향을 받는가?
//
//    _float m_fFrictionScale = 20.f; // 마찰력 크기.
//    _float m_fRefHeight = 0.f; // 기준 Y