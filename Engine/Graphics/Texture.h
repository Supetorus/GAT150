#pragma once
#include "Resource/Resource.h"
#include <SDL.h>

namespace nc
{
	class Texture : public Resource
	{
	public:
		bool Load(const std::string& filename, void* data) override;

		friend class Renderer;

	private:
		SDL_Texture* texture{ nullptr };
		SDL_Renderer* renderer{ nullptr };
	};

}