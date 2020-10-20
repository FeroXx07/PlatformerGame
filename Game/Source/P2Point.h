#pragma once

#ifndef __PointOperations_h__
#define __PointOperations_h__

#include <math.h>

template<class TYPE>
class PointOperations
{
public:

	TYPE x, y;

	PointOperations() {}

	PointOperations(const PointOperations& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	PointOperations(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	PointOperations& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	PointOperations operator -(const PointOperations& v) const
	{
		PointOperations r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	PointOperations operator +(const PointOperations& v)const
	{
		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	const PointOperations& operator-=(const PointOperations& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const PointOperations& operator+=(const PointOperations& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const PointOperations& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator!=(const PointOperations& v) const
	{
		return (x != v.x || y != v.y);
	}

	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	PointOperations& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	PointOperations& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	TYPE DistanceTo(const PointOperations& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx * fx) + (fy * fy);
	}

	TYPE DistanceManhattan(const PointOperations& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}

};

typedef PointOperations<int> iPoint;
typedef PointOperations<float> fPoint;

#endif __PointOperations_h__