#include <iostream>

using namespace std;

class Chess{
public:
	int id = -1;
	int top;
	int bot;
	int right;
	int left;
	bool used = false;


	
	void setValue(int id, int left, int top, int right, int bot){
		this->id = id;
		this->top = top;
		this->bot = bot;
		this->left = left;
		this->right = right;
	}

	bool empty(){
		if (this->id == -1)
		{
			return true;
		}
		return false;
	}
};