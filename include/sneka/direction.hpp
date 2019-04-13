#ifndef SNEKA_DIRECTION_HPP
#define SNEKA_DIRECTION_HPP



namespace sneka {

	class Direction {
	private:
		int value;

		Direction(int);
	public:
		static const Direction FORWARD, LEFT, BACKWARD, RIGHT;

		Direction() = default;
		Direction(const Direction &) = default;
		Direction(Direction &&) = default;

		// "moves" the direction
		Direction combined(const Direction &) const;
		Direction operator + (const Direction &) const;

		Direction relative(const Direction &) const;
		Direction operator - (const Direction &) const;
		bool operator == (const Direction &) const;

		Direction& operator = (const Direction &) = default;
		Direction& operator = (Direction &&) = default;

		Direction& operator += (const Direction &);
		Direction& operator += (Direction&&);
		Direction& operator -= (const Direction &);
		Direction& operator -= (Direction&&);


		float radians() const;
		float degrees() const;
		float sin() const;
		float cos() const;
	};

}

#endif
