#include "Mode.h"
Mode::Mode(){
	mX=0;
	mY=0;// position
	type =0;
	Size =0;
	bool H_V = true;
}

void Mode::update(){
	bool NotDone=true;
	while(NotDone){
		NotDone = false;
		for(int i = 1; i < MaxSize; i++){
			if(CardIndex[i-1] == NULL && CardIndex[i] != NULL){
				CardIndex[i-1] = CardIndex[i];
				CardIndex[i] = NULL;
				NotDone = true;
			}
		}
	}
	NotDone = true;
	while(NotDone){
		NotDone = false;
		Card* Previous = CardIndex[0];
		Card* Current = CardIndex[0];
		Card* Hld = CardIndex[0];
		for(int i = 0; i < this->Size; i++){
			Previous = Current;
			Current = this->CardIndex[i];
			if(*Previous > *Current){
				Hld = CardIndex[i];
				CardIndex[i] = CardIndex[i-1];
				CardIndex[i-1] = Hld;
				NotDone = true;
			}
		}
	}
	NotDone = true;
	bool Type = true;
	type = 0;
	if(Size == 1){
		type = 1;
	}
	for(int i = 0; i < Size-1; i++){
		Type = (Type && (CardIndex[i]->Number == CardIndex[i+1]->Number));
	}
	if(Type == true && Size != 0){
		type = 1;
	}
	Type = true;
	for(int i = 0; i < Size-1; i++){
		Type = (Type && (CardIndex[i]->Number+1 == CardIndex[i+1]->Number));
	}
	if(Type == true && Size != 0){
		type = 2;
	}

	int X=0;
	for(int i = 0; i < Size; i++){
		if(H_V){
		CardIndex[i]->mX = mX+X;
		CardIndex[i]->mY = mY;
		X += 32;
		}
		else
		{
		CardIndex[i]->mX = mX;
		CardIndex[i]->mY = mY+X;
		X += 32;
		}
		CardIndex[i]->Me.top = CardIndex[i]->mY;
		CardIndex[i]->Me.left = CardIndex[i]->mX;
		CardIndex[i]->Me.bottom = CardIndex[i]->mY+96;
		CardIndex[i]->Me.right = CardIndex[i]->mX+72;
	}
}
bool Mode::operator==(const Mode other){
	if((other.type == type || other.type == 0) && (other.Size == Size || other.Size == 0))
		return true;
	else 
		return false;
}
bool Mode::operator!=(const Mode other){
	if(!(other.type == type) && (other.Size == Size))
		return true;
	else 
		return false;
}
bool Mode::operator<(const Mode other){
	if(this == NULL)
		return false;
	if(&other == NULL)
		return true;
	if(this->CardIndex[0]->Value < other.CardIndex[0]->Value)
		return true;
	else
		return false;

}
bool Mode::operator>(const Mode other){
	if(other.Size == 0)
		return true;
	if(CardIndex[0]){
		if(this->CardIndex[0]->Value > other.CardIndex[0]->Value)
			return true;
		else
			return false;
	}
	else
		return false;

}