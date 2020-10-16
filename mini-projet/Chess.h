#include <iostream>

using namespace std;

class Chess{
public:
	int id;
	int top;
	int bot;
	int right;
	int left;

	
	void setValue(int id, int left, int top, int right, int bot){
		this->id = id;
		this->top = top;
		this->bot = bot;
		this->left = left;
		this->right = right;
	}

};