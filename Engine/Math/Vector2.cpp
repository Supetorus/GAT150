#include "Vector2.h"
#include <string>

namespace nc
{
	const Vector2 Vector2::up{ 0, -1 };
	const Vector2 Vector2::down{ 0, 1 };
	const Vector2 Vector2::left{ -1, 0 };
	const Vector2 Vector2::right{ 1, 0 };
	const Vector2 Vector2::one{ 1, 1 };
	const Vector2 Vector2::zero{ 0, 0 };


	std::istream& operator >> (std::istream& stream, Vector2& v)
	{
		std::string line;
		std::getline(stream, line);

		if (line.find("{") != std::string::npos)
		{
			// x
			std::string x = line.substr(line.find("{") + 1, line.find(",") - line.find("{") - 1);
			v.x = std::stof(x);
			// y
			std::string y = line.substr(line.find(",") + 1, line.find("}") - line.find(",") - 1);
			v.y = std::stof(y);
		}
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, Vector2& v)
	{
		stream << v.x << " " << v.y;

		return stream;
	}

	Vector2 Vector2::CircleClamp(const Vector2& v, float radius)
	{
		if (v.x * v.x + v.y * v.y <= radius * radius) return v;
		else
		{
			float angle = v.Angle();
			Vector2 returned;
			returned.x = radius * std::cos(angle);
			returned.y = radius * std::sin(angle);
			return returned;
		}
	}

	void Vector2::CircleClamp(float radius)
	{
		if (x * x + y * y > radius * radius)
		{
			float angle = this->Angle();
			x = radius * std::cos(angle);
			y = radius * std::sin(angle);
		}
	}
}