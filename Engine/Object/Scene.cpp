#include "Scene.h"
#include "Actor.h"
#include "Math/Random.h"
#include <algorithm>

namespace nc
{
	void Scene::Update(float dt)
	{
		// add new actors
		actors.insert(actors.end(), std::make_move_iterator(newActors.begin()), std::make_move_iterator(newActors.end()));
		newActors.clear();

		// update actors
		std::for_each(actors.begin(), actors.end(), [dt](auto& actor) { actor->Update(dt); });
		
		// check collisions
		if (actors.size() > 1)
		{
			for (size_t i = 0; i < actors.size(); i++)
			{
				for (size_t j = i + 1; j < actors.size(); j++)
				{
					if (actors[i]->destroy || actors[j]->destroy) continue;

					nc::Vector2 dir = actors[i]->transform.position - actors[j]->transform.position;
					float distance = dir.Length() * collisionGive;
					if (distance < (actors[i]->GetRadius() + actors[j]->GetRadius()))
					{
						actors[i]->OnCollision(actors[j].get());
						actors[j]->OnCollision(actors[i].get());
					}
				}
			}
		}

		// destroy actors
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->destroy)
			{
				iter = actors.erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	void Scene::Draw(Renderer* renderer)
	{
		std::for_each(actors.begin(), actors.end(), [renderer](auto& actor) { actor->Draw(renderer); });
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->scene = this;
		actor->Intitialize();

		id++;
		actor->id = id;
		newActors.push_back(std::move(actor));
	}

	void Scene::RemoveActor(Actor* actor)
	{
	}

	void Scene::RemoveAllActors()
	{
		actors.clear();
	}

	void Scene::RemoveByTag(const std::string& tag)
	{
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->hasTag(tag))
			{
				(*iter)->destroy = true;
			}
			iter++;
		}
	}

	Vector2 Scene::SafeLocation(float radius, float buffer)
	{
		nc::Vector2 location;
		bool invalidLocation = true;
		while (invalidLocation)
		{
			location =
			{
				nc::RandomRange(0.0f, 800),
				nc::RandomRange(0.0f, 600)
			};
			for (std::unique_ptr<Actor>& actor : actors)
			{
				if (Vector2::Distance(location, actor->transform.position) < radius + buffer + actor->GetRadius()) break;
				invalidLocation = false;
			}
		}
		return location;
	}
	int Scene::ActorCount()
	{
		return actors.size();
	}
}