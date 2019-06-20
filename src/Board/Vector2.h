#ifndef VEC2_H
#define VEC2_H
class Vector2 {
public:
	int xPos, yPos;
	Vector2();
	Vector2(int _xPos, int _yPos);

	Vector2 normalize() const;

	inline Vector2 operator+(const Vector2 &B) const { return Vector2(xPos + B.xPos, yPos + B.yPos); }
	inline bool operator!= (const Vector2 &B) const { return !(*this == B); }
	inline bool operator== (const Vector2 &B) const { return xPos == B.xPos && yPos == B.yPos; }
	inline Vector2 operator-(const Vector2&B) const { return Vector2(xPos - B.xPos, yPos - B.yPos); }
	inline Vector2 operator-() {
		this->xPos = -this->xPos;
		this->yPos = -this->yPos;
		return *this; }
};
#endif
