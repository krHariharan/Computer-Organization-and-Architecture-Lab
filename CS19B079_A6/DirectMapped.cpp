#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;;

class Cache{
	string **cache;
	long capacity;
	long sets;
	long size;
	int ways;
	int replacementPolicy;
	long block;
	public:
	Cache();
	~Cache();
	void load(string address);
};

Cache::Cache()
{
	cin>>size>>block>>ways>>replacementPolicy;
	capacity = size/block;
	if(ways==0)
	{
		sets = 1;
		cache = new string*[1];
		cache[0] = new string[ways];
	}
	else
	{
		sets = capacity/ways;
		cache = new string*[sets];
		for(int i=0; i<sets; i++)
			cache[i]=new string[ways];
	}
}

Cache::~Cache()
{
	for(int i=0; i<sets; i++)
		delete[] cache[i];
	delete[] cache;

}

void Cache::load(string address)
{
	
}

int main(){
	Cache c;
	string inp;
	cin>>inp;
	ifstream addresses(inp);
	while(getline (addresses, inp))
		c.load(inp);
	addresses.close();
	return 0;
}
