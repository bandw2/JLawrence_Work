#pragma once

struct Loc
{
	float mx;
	float my;
	float drwmx;
	float drwmy;
	Loc():mx(0),my(0),drwmx(0),drwmy(0){}
	Loc(Loc &other):mx(other.mx),my(other.my),drwmx(other.drwmx),drwmy(other.drwmy){}
};