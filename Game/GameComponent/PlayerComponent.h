#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PlayerComponent : public nc::Component
{
public:
	void Create() override;
	virtual void onCollisionEnter(const nc::Event& event);
	virtual void onCollisionExit(const nc::Event& event);
	virtual void Update() override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;

public:
	float speed{ 0 };


private:
	std::list<nc::Actor*> contacts;
};