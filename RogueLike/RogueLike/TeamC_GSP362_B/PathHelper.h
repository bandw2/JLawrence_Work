

struct Path{
	bool path[5];
	int x,y;
	Path()
	{
		for(int i =0; i < 5; i++)
			path[i] =0;
		x=y=0;
	}
};