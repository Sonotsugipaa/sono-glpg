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

	GLfloat Direction::sin() const {
		switch(value) {
			case 0:  return  1.0f;  break;
			case 1:  return  0.0f;  break;
			case 2:  return -1.0f;  break;
			case 3:  return  0.0f;  break;
		}
		return 2.0f;
	}

	GLfloat Direction::cos() const {
		switch(value) {
			case 0:  return  0.0f;  break;
			case 1:  return -1.0f;  break;
			case 2:  return  0.0f;  break;
			case 3:  return  1.0f;  break;
		}
		return 2.0f;
	}


	Direction Direction::operator + (const Direction & other) const {
		GLuint dir = (value + other.value) % 4;
		return Direction(dir);
	}

	Direction Direction::combined(const Direction & other) const {
		return *this + other;
	}

	Direction Direction::operator - (const Direction & other) const {
		GLuint dir = (value - other.value) % 4;
		return Direction(dir);
	}

	Direction Direction::relative(const Direction & other) const {
		return operator - (other);
	}

	bool Direction::operator == (const Direction & other) const {
		return value == other.value;
	}


	Direction& Direction::operator += (const Direction & other) {
		operator = (operator + (other));
		return *this;
	}
	Direction& Direction::operator += (Direction&& other) {
		operator = (operator + (other));
		return *this;
	}

	Direction& Direction::operator -= (const Direction & other) {
		operator = (operator - (other));
		return *this;
	}
	Direction& Direction::operator -= (Direction&& other) {
		operator = (operator - (other));
		return *this;
	}

}
