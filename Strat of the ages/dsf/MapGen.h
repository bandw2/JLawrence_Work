struct MapGenTile{
	int ProvID;
	int Type;
	MapGenTile(){
		ProvID = 0;
		Type = 0;
	}
	MapGenTile(int a_prov,int a_type){
		ProvID = a_prov;
		Type = a_type;
	}
};