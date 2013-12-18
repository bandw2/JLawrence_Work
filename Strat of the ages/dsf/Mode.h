#pragma once
#include "Card.h"
#define MaxSize 13
struct Mode
{
bool H_V;
int mX,mY;// position
int type;
int Size;
Card* CardIndex[MaxSize];
Mode();
void update();//update positions of the cards
bool operator==(const Mode);
bool operator!=(const Mode);
bool operator<(const Mode);
bool operator>(const Mode);

};