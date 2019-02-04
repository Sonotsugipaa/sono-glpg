#include "sneka/world.hpp"



namespace sneka {

	const Direction
			Direction::FORWARD  = Direction(0),
			Direction::LEFT     = Direction(1),
			Direction::BACKWARD = Direction(2),
			Direction::RIGHT    = Direction(3);


	Direction::Direction(int value):
			value(value)
	{ }

	GLfloat Direction::radians() const {
		return value * (3.141592f / 2.0f);
	}

	GLfloat Direction::degrees() const {
		return value * (90.0f);
	}

	Direction Direction::operator + (Direction other) const {
		GLuint dir = (value + other.value) % 4;
		return Direction(dir);
	}

	Direction Direction::relative(Direction other) const {
		return *this + other;
	}

	bool Direction::operator == (Direction other) const {
		return value == other.value;
	}

}
