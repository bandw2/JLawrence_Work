#pragma once
struct V2D
{
	float x;
	float y;
	V2D()
	{
		y=x=0;
	}
	V2D(int j,int k)
	{
		x = j;
		y = k;
	}

	bool operator== (const V2D& otherVec)
	{
		return x == otherVec.x && y == otherVec.y;
	}

	void V2D::operator-= ( V2D& otherVec )
	{
		x -= otherVec.x;
		y -= otherVec.y;
	}
};