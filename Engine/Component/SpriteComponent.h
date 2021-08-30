#pragma once
#include "GraphicsComponent.h"
#include <memory>

namespace nc
{
	class Texture;
	class SpriteComponent : public GraphicsComponent
	{
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<SpriteComponent>(*this); }
		
		void Update() override;
		void Draw(Renderer* renderer) override;

		// Inherited via GraphicsComponent
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::shared_ptr<nc::Texture> texture;
	};
}