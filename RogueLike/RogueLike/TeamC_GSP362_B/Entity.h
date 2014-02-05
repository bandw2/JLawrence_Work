class Entity
{
private:
	int hitpoints, speed, armor, damage;
	bool friendly;
	//type
	//texture

public:
	Entity();
	Entity(bool);

	void Init();
	void special();
	void attack();
	virtual void AI();
	void update();
	void Draw();


	void setHP(int);
	void setSpeed(int);
	void setArmor(int);
	void setDmg();

	int getHP();
	int getSpeed();
	int getArmor();
	int getDmg();


	

};