#pragma once
#include "Component/Component.h"

class EnemyComponent : public nc::Component
{
public:
	virtual void Update() override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;

public:
	float speed{ 0 };

};