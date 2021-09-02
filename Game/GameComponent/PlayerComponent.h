#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PlayerComponent : public nc::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<PlayerComponent>(*this); }
	
	virtual ~PlayerComponent();
	void Create() override;
	virtual void onCollisionEnter(const nc::Event& event);
	virtual void onCollisionExit(const nc::Event& event);
	virtual void Update() override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;

public:
	float speed{ 0 };
	float jump{ 0 };

private:
	std::list<nc::Actor*> contacts;
};