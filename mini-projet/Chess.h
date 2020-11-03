#include <iostream>

using namespace std;

class Chess{
public:
	int top;
	int bot;
	int right;
	int left;
	bool used = false;


	
	void setValue(int left, int top, int right, int bot){
		this->top = top;
		this->bot = bot;
		this->left = left;
		this->right = right;
	}
};