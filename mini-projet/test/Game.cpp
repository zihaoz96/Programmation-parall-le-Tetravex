#include "Chess.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <map>

using namespace std;

class Game
{
public:
	int N;
	vector<Chess> allChess;
	queue<Chess> queueChess;
	int *table;

	vector<thread> threads;
	bool terminate = false;
	queue<int> queueJob;
	mutex m;
	condition_variable cond_var;
	map<int, bool*> alreadyUse;

	Game(int N, vector<Chess> allChess){
		this->N = N;
		this->allChess = allChess;

		table = new int(N*N);
		bool *temp = new bool(N*N);
		for (int i = 0; i < N*N; ++i)
		{
			temp[i] = false;
		}

		for (int i = 0; i < N; ++i)
		{
			// -1 => empty 
			for (int j = 0; j < N; ++j)
			{
	 			table[i*N+j] = -1;
	 			alreadyUse.insert(std::pair<int, bool*>(i*N+j,temp));
	 			queueChess.push(allChess[i]);
			}
		} 


		queueJob.push(0);		
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
  
  		affiche();
	}

	void affiche(){
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				cout<<table[i*N+j]<<",";
			}
			cout<<endl;
		}
	}

	void startTheGame(){
		for (int i = 0; i < 4; ++i)
		{
			threads.push_back(std::thread(&Game::doWork,this));
		}
	}

	void doWork(){
		
		while(terminate){
			int indexTable;
			{
				unique_lock<mutex> lok(m);

				cond_var.wait(lok, [&]{return !queueJob.empty() || terminate;});

				if (terminate)
				{
					break;
				}

				indexTable = queueJob.front();

				queueJob.pop();
			}
			checkJob(indexTable);
		}
	}

	// job is id of Chess
	void checkJob(int indexTable){
		Chess chess = queueChess.front();
		queueChess.pop();
		
		map<int, bool*>::iterator iter = alreadyUse.find(indexTable);
		// if this chess not use in this position
		if (iter != alreadyUse.end() && !iter->second[chess.id])
		{
			if (checkPosition(chess,indexTable))
			{		
				table[indexTable] = chess.id;
			}
			iter->second[chess.id] = true;
			if (indexTable<(N*N-1))
			{
				unique_lock<mutex> lok(m);
				queueJob.push(indexTable+1);
			} 
		} 
		else 
		{
			for (int i = 0; i < N*N; ++i)
			{
				iter->second[i] = false;
			}

			unique_lock<mutex> lok(m);
			queueJob.push(indexTable-1);
		}
		
		
		cond_var.notify_one();

	}
	
	bool checkPosition(Chess chess, int position){
		// if (table[i*N+j] != -1)
		// {
		// 	return false;
		// }

		// if top is not out of table
		if (position-N > 0)
		{
			// if not empty
			if (table[position-N] != -1)
			{
				// num isn't the same 
				if (chess.top != findById(table[position-N]).bot )
				{
					return false;
				}
			}
		}

		// if bot is not out of table
		if (position+N < N*N)
		{
			if (table[position+N] != -1)
			{
				if (chess.bot != findById(table[position+N]).top )
				{
					return false;
				}
			}
		}

		// if left is not out of table
		if (position/N > N-1 && position%N != 0)
		{
			if (table[position-1] != -1)
			{
				if (chess.left != findById(table[position-1]).right )
				{
					return false;
				}
			}
		}

		// if right is not out of table
		if ((position+1)/N > N-1 && (position+1)%N != 0)
		{
			if (table[position+1] != -1)
			{
				if (chess.right != findById(table[position+1]).left )
				{
					return false;
				}
			}
		}

		return true;
	}

	/*
	 * find Chess by id
	*/
	Chess findById(int id){
		Chess c;
		for (int i = 0; i < allChess.size(); ++i)
		{
			if (allChess[i].id == id)
			{
				c = allChess[i];
			}
		}
		return c;
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

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout<<allChess[i*N+j].top<<",";
		}
		cout<<endl;
	}

	Game game(N,allChess);
	game.startTheGame();
	
	return 0;
}