/*
Author: Peter O'Donohue
Creation Date: 05/31/2018
Modification Date: 05/31/2018
Purpose:
*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

class Path
{
public:
	vector<string> currentPath;
	int pathLength;
	int distanceToGoal;
	int underestimator;
	Path();

};

Path::Path()
{
	pathLength = 0;
	distanceToGoal = 0;
	underestimator = 0;
}

struct compare
{

	bool operator()(const Path& lhs, const Path& rhs)
	{
		return lhs.underestimator > rhs.underestimator;
	}
};


// function definitions
void expand(const vector<string>& path, priority_queue<Path, vector<Path>, compare>& listOfPaths, int, string);
void removeRedundents(priority_queue<Path, vector<Path>, compare>&, priority_queue<Path, vector<Path>, compare>&);
void solve(priority_queue<Path, vector<Path>, compare>& listOfPaths, string start, string finish);
void underestimator(Path&, int, string);
void printQueue(priority_queue<Path, vector<Path>, compare> listOfPaths);

int main()
{
	Path foo;
	int count = 0;
	string input;
	string start;
	string finish;
	priority_queue<Path, vector<Path>, compare> paths;

	cout << "Enter your start and final words, seperating each with a space." << endl;
	getline(cin, input);
	do
	{
		while (input[count] != ' ')
		{
			start += input[count];
			++count;
		}
		++count;
		while (count <= (input.length() - 1))
		{
			finish += input[count];
			++count;
		}
		foo.currentPath.push_back(start);
paths.push(foo);
solve(paths, start, finish);
if (!paths.empty())
{
	foo = paths.top();
	cout << "An optimal set of moves would be as follows:" << endl;
	for (int i = foo.currentPath.size() - 1; i >= 0; --i)
		cout << foo.currentPath[i] << endl;
	cout << endl;
}
else
cout << "Goal is impossible to obtain from given word." << endl;
start.clear();
finish.clear();
input.clear();
cout << "Please enter a new start and finish word, or enter 'done' to exit." << endl;
cin >> input;
	} while (input != "done");

	system("pause");
	return 0;
}

void expand(const vector<string>& path, priority_queue<Path, vector<Path>, compare>& listOfPaths, int distanceToGoal, string finalWord)
{
	Path newPath;
	string currentMove = path[0];
	string newMove;
	char tempMove;

	newPath.currentPath = path;
	for (int i = 0; i < (currentMove.size() - 1); ++i)
	{

		if (currentMove.at(i) != '-')
		{
			if (currentMove.at(i + 1) == '-')
			{
				newMove = currentMove;
				tempMove = newMove[i];
				swap(newMove[i], newMove[i + 1]);
				newPath.currentPath.insert(newPath.currentPath.begin(), newMove);
				newPath.pathLength += 1;
				listOfPaths.push(newPath);
				newPath.currentPath.erase(newPath.currentPath.begin());
			}
			if ((i < (currentMove.size() - 2) && (currentMove.at(i + 1) != '-') && (currentMove.at(i + 2) == '-')))
			{
				newMove = currentMove;
				swap(newMove.at(i), newMove.at(i + 2));
				newPath.currentPath.insert(newPath.currentPath.begin(), newMove);
				newPath.pathLength += 1;
				underestimator(newPath, distanceToGoal, finalWord);
				listOfPaths.push(newPath);
				newPath.currentPath.erase(newPath.currentPath.begin());
			}
		}
	}

}

void underestimator(Path& recentPath, int totalDistance, string finalState)
{
	int currentDistance = totalDistance;
	string recentMove = recentPath.currentPath[0];

	for (int i = 0; i < finalState.size(); ++i)
		if ((recentMove[i] != '-') && (recentMove[i] == finalState[i]))
			--totalDistance;
	recentPath.distanceToGoal = totalDistance;
	recentPath.underestimator = recentPath.pathLength + recentPath.distanceToGoal;
}

void solve(priority_queue<Path, vector<Path>, compare>& listOfPaths, string start, string finish)
{
	Path recentPath = listOfPaths.top();
	int totalGoalDistance = 0;
	priority_queue<Path, vector<Path>, compare> noRedundentPaths;

	for (auto letter : finish)
		if (letter != '-')
			++totalGoalDistance;

	while (recentPath.currentPath[0] != finish && (!listOfPaths.empty()))
	{
		listOfPaths.pop();
		expand(recentPath.currentPath, listOfPaths, totalGoalDistance, finish);
		removeRedundents(listOfPaths, noRedundentPaths);
		printQueue(listOfPaths);
		recentPath = listOfPaths.top();
	}
}

void removeRedundents(priority_queue<Path, vector<Path>, compare>& listOfPaths, priority_queue<Path, vector<Path>, compare>& noRedundentPaths)
{
	bool inPath = false;
	Path compareMove;
	Path recentMove = listOfPaths.top();
	string recentState = recentMove.currentPath[0];
	noRedundentPaths.push(recentMove);
	listOfPaths.pop();
	while (!listOfPaths.empty())
	{
		compareMove = listOfPaths.top();
		listOfPaths.pop();
		for (int i = 0; i < compareMove.currentPath.size(); ++i)
		{
			if (recentState == compareMove.currentPath[i])
				inPath = true;
		}
		if (!inPath)
			noRedundentPaths.push(compareMove);
		inPath = false;
	}
	while (!noRedundentPaths.empty())
	{
		compareMove = noRedundentPaths.top();
		noRedundentPaths.pop();
		listOfPaths.push(compareMove);
	}
}

void printQueue(priority_queue<Path, vector<Path>, compare> listOfPaths)
{
	Path recentPath;

	while (!listOfPaths.empty())
	{
		recentPath = listOfPaths.top();
		listOfPaths.pop();
		for (auto move : recentPath.currentPath)
			cout << move << " ";
		cout << endl;
	}

}