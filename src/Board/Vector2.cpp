#include"Vector2.h"
#include<cmath>

Vector2::Vector2() {
	Vector2(0,0);
}

Vector2::Vector2(int _xPos, int _yPos) : xPos(_xPos), yPos(_yPos) {

}

//	returns ROUNDED normalized Vector2
Vector2 Vector2::normalize() const{
	float tempLength = sqrt(this->xPos * this->xPos + this->yPos * this->yPos);
	float TempX = floor((float)this->xPos / tempLength);
	float TempY = floor((float)this->yPos / tempLength);
	Vector2 temp = Vector2(round((float)this->xPos / tempLength), round((float)this->yPos / tempLength));
	return temp;
	//return Vector2((float)this->xPos / tempLength, (float)this->yPos / tempLength);
}