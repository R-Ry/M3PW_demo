#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector{
public:
	Vector(const Vector& other):
	x_(other.x_),
	y_(other.y_)
	{
	}
	
	Vector(float x, float y):
	x_(x),
	y_(y)
	{
	}
	
	Vector():
	x_(0.0),
	y_(0.0)
	{
	}
	
	bool operator==(const Vector& rhs){
		return (x_ == rhs.x_) && (y_ == rhs.y_);
	}
	
	const Vector& operator=(const Vector& rhs){
		x_ = rhs.x_;
		y_ = rhs.y_;
		return *this;
	}
	
	Vector operator+(const Vector& rhs){
		return Vector(x_ + rhs.x_, y_ + rhs.y_);
	}
	
	Vector operator-(const Vector& rhs){
		return Vector(x_ - rhs.x_, y_ - rhs.y_);
	}
	
	Vector operator-(){
		return Vector(-x_, -y_);
	}
	
	Vector operator*(float scalar){
		return Vector(x_ * scalar, y_ * scalar);
	}
	
	Vector operator/(float scalar){
		return Vector(x_ / scalar, y_ / scalar);
	}
	
	
	const Vector& operator+=(const Vector& rhs){
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}
	
	const Vector& operator-=(const Vector& rhs){
		x_ -= rhs.x_;
		y_ -= rhs.y_;
		return *this;
	}
	
	const Vector& operator*=(float scalar){
		x_ *= scalar;
		y_ *= scalar;
		return *this;
	}
	
	const Vector& operator/=(float scalar){
		x_ /= scalar;
		y_ /= scalar;
		return *this;
	}
	
	//this returns the dot product of the vectors
	int operator*(const Vector& rhs){
		return x_ * rhs.x_ + y_ * rhs.y_;
	}
	
	int x_, y_;
};

#endif