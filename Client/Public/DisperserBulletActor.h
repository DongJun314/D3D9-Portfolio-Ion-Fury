#pragma once
#include "BulletActor.h"

class CCubeColliderComponent;

class CDisperserBulletActor : public CBulletActor
{
public:
	explicit CDisperserBulletActor() = default;
	virtual ~CDisperserBulletActor() = default;

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual int32 Update(const float& _fDeltaSeconds) override;
	virtual int32 LateUpdate(const float& _fDeltaSeconds) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

public:
	virtual int32 OnCollisionEnter(std::weak_ptr<CColliderComponent> _wpSrcCollider, std::weak_ptr<CColliderComponent> _wpDstCollider);
	virtual void OnCollisionStay(std::weak_ptr<CColliderComponent> _wpSrcCollider, std::weak_ptr<CColliderComponent> _wpDstCollider);
	virtual void OnCollisionExit(std::weak_ptr<CColliderComponent> _wpSrcCollider, std::weak_ptr<CColliderComponent> _wpDstCollider);

};

