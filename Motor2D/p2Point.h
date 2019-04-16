// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __P2POINT_H__
#define __P2POINT_H__

#include "p2Defs.h"
#include <math.h>

template<class TYPE>
class p2Point
{
public:

	TYPE x, y;

	p2Point()
	{}

	p2Point(const p2Point<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	p2Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	p2Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	p2Point operator -(const p2Point &v) const
	{
		//p2Vector2 r;
		p2Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	p2Point operator + (const p2Point &v) const
	{
		//p2Vector2 r;
		p2Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const p2Point& operator -=(const p2Point &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const p2Point& operator +=(const p2Point &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	p2Point operator *(const TYPE & num) const 
	{
		p2Point r;
		r.x = x * num;
		r.y = y * num;
		return r;
	}

	p2Point operator/ (const TYPE & num) const {
		p2Point r;
		r.x = x / num;
		r.y = y / num;
		return r;
	}

	p2Point RoundPoint()
	{
		return p2Point
		(
			round(x),
			round(y)
		);
	}

	bool operator ==(const p2Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const p2Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	const p2Point operator /=(const TYPE& v)
	{
		x /= v;
		y /= v;
		return (*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	p2Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	p2Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	void Normalize() {
		double module = sqrt(x * x + y * y);
		x /= module;
		y /= module;
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const p2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const p2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const p2Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}

	p2Point<int> ReturniPoint() const
	{
		p2Point<int> r((int)x, (int)y);
		return r;
	}

	p2Point<float> Return_fPoint() const
	{
		p2Point<float> r((float)x, (float)y);
		return r;
	}

	explicit operator p2Point<int>() const
	{
		return p2Point<int>((int)x, (int)y);
	}

	explicit operator p2Point<float>() const
	{
		return p2Point<float>((float)x, (float)y);
	}

};

typedef p2Point<int> iPoint;
typedef p2Point<float> fPoint;

#endif // __P2POINT_H__