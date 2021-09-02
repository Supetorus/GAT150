#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"


class EnemyComponent : public nc::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<EnemyComponent>(*this); }
	
	void Create() override;
	virtual void Update() override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual void onCollisionEnter(const nc::Event& event);

public:
	float speed{ 0 };

};