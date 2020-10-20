#include "../Chess.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <map>

using namespace std;

class Test
{
public:
	int N;
	vector<Chess> allChess;
	Chess table[100];

	Test(int N, vector<Chess> allChess){
		this->N = N;
		this->allChess = allChess;
	}

	void affichage(){
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				cout<<table[i*N+j].id<<",";
			}
			cout<<endl;
		}
	}
	
	bool startTheGame(int position){

		if (position==N*N)
		{
			return true;
		}

		for (int i = 0; i < N*N; ++i){

			if (!allChess[i].used){

				if (checkPosition(allChess[i], position)){
					allChess[i].used = true;
					
					table[position] = allChess[i];
					
					if (startTheGame(position+1)){
						return true;
					}

					allChess[i].used = false;
				}	
				
			}
		}
		return false;
	}

	/*
	 * Check is good position
	 */
	bool checkPosition(Chess chess, int position){

		// if top is not out of table
		if (position-N >= 0)
		{
			// num isn't the same 
			if (chess.top != table[position-N].bot )
			{
				return false;
			}
		}

		// if left is not out of table
		if (position%N != 0 && position != 0)
		{
			if (chess.left != table[position-1].right )
			{
				return false;
			}
		}

		return true;
	}
};

int main(int argc,char** argv)
{
	ifstream file(argv[1]);
	int left, top, right, bot, N;
	vector<Chess> allChess;
	Chess c;
	if(! file)
		cerr << "Erreur ouverture fichier" << argv[1] << endl;
	else{			
		file >> N;
		file >> N;
		for (int i = 0; i < N*N; ++i)
		{
			file >> left;
			file >> top;
			file >>	right;
			file >> bot;
			
			c.setValue(i,left,top,right,bot);

			allChess.push_back(c);
		}

		file.close();
	}

	Test game(N,allChess);
	game.startTheGame(0);

	game.affichage();
	return 0;
}