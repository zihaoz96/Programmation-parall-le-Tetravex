#include "Chess.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

using namespace std;

class Game
{
public:
	int N;
	vector<Chess> allChess;
	Chess T[100];
	
	vector<thread> threads;
	queue<int> queueJob;
	bool terminate = false;
	mutex m;
	condition_variable cond_var;
	
	Game(int N, vector<Chess> allChess){
		this->N = N;
		this->allChess = allChess;	
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				queueJob.push(N*i+j);
			}
		}
	}
	
	~Game(){

		terminate = true;

		cond_var.notify_all();

		for(auto& thread : threads){
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	void affichage(Chess table[100]){
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				cout<<"———————";
			}
			cout<<endl;
			for (int k = 0; k < N; ++k)
			{
				cout<<"|  "<<table[i*N+k].top<<"  |";
			}
			cout<<endl;
			for (int h = 0; h < N; ++h)
			{
				cout<<"|"<<table[i*N+h].left<<"   "<<table[i*N+h].right<<"|";
			}
			cout<<endl;
			for (int l = 0; l < N; ++l)
			{
				cout<<"|  "<<table[i*N+l].bot<<"  |";
			}
			cout<<endl;
			for (int o = 0; o < N; ++o)
			{
				cout<<"———————";
			}
			cout<<endl;
		}
	}

	// Backtracking séquentielle
	bool backtrackingSequentielle(Chess table[100], int position){
		if (position==N*N)
		{
			affichage(table);
			return true;
		}
		// Cycle all the pieces
		for (int i = 0; i < N*N; ++i){
			// No used 
			if (!allChess[i].used){
				// Right position
				if (checkPosition(table, allChess[i], position)){
					// Set this chess used
					allChess[i].used = true;
					// Set chess in this position
					table[position] = allChess[i];
					// Recusif
					if (backtrackingSequentielle(table, position+1)){
						return true;
					}
					// Set this chess NO used
					allChess[i].used = false;
				}	
			}
		}
		return false;
	}

	/*
	 * Verifier c'est bon position
	 * Pour verifier en version de backtracking séquentielle
	 */
	bool checkPosition(Chess table[100], Chess chess, int position){

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


	
	// Version de backtracking parallèles
	void startTheGame(){
		for (int i = 0; i < N ; ++i)
		{
			threads.push_back(std::thread(&Game::doWork,this));
		}
	}

	void doWork(){
		Chess table[100];

		while(!terminate){
			int job;
			{
				unique_lock<mutex> lok(m);

				cond_var.wait(lok, [&]{return !queueJob.empty() || terminate;});

				if (terminate)
				{
					break;
				}

				job = queueJob.front();

				queueJob.pop();
			}

			if(backtrackingSequentielle(table, job)){
				
				terminate = true;
			}

			cond_var.notify_one();
		}
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
			
			c.setValue(left,top,right,bot);

			allChess.push_back(c);
		}

		file.close();
	}

	Game game(N,allChess);

	int op;
	cout<<"Entre 1 pour test backtracking Sequentielle"<<endl<<"Entre 2 pour test backtracking parallele"<<endl;
	cin>>op;

	//backtracking Sequentielle
	if (op == 1)
	{
		game.backtrackingSequentielle(game.T,0); 
	} 
	//backtracking parallele
	else
	{
		game.startTheGame();
	}

	return 0;
}