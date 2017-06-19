#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

class DirectedGraph
{
private:
	int** data;
	int nodeCount;
	int isDirected;
	DirectedGraph* CloneWithoutNode(int);
public:
	static const int INDEX_OUT_OF_RANGE = -1;
	static const int EDGE_EXISTED = -2;
	static const int EDGE_NOT_EXISTED = -3;
	static const int NODE_DUPLICATED = -4;
	static const int GRAPH_NOT_CONNECTED = -5;
	DirectedGraph(int, int);
	~DirectedGraph();
	int NodeCount();
	int AddEdge(int, int);
	int RemoveEdge(int, int);
	int IsConnected();
	int FindCutOffPoint();
	int FindShortestPath(int, int, list<int>&);
	void Print();
};

int main(int argc, char* argv[])
{
	int nodeCount;
	do
	{
		cout << "Enter number of nodes (>= 2): ";
		cin >> nodeCount;
	} while (nodeCount < 2);
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	// input data
	DirectedGraph* g = new DirectedGraph(nodeCount, 1);
	unordered_map<string, int>* m = new unordered_map<string, int>();
	cout << "Enter nodes name" << endl;
	for (int i = 0; i < nodeCount; i++)
	{
		string nodeName;
		do
		{
			cout << "Enter node " << i << ": ";
			getline(cin, nodeName);
		} while (m->find(nodeName) != m->end());
		(*m)[nodeName] = i;
	}

	// find shortest path
	char choice;
	int exit = 0;
	do
	{
		do
		{
			cout << endl << "Select operation:" << endl;
			cout << "1. Add edge" << endl;
			cout << "2. Remove edge" << endl;
			cout << "3. Find cut-off point" << endl;
			cout << "4. Find shortest path" << endl;
			cout << "5. Exit" << endl;
			cin >> choice;
		} while ('1' > choice || choice > '5');
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		switch (choice)
		{
		case '1':
		{
			string start;
			string end;
			do
			{
				cout << "Start node (name): ";
				getline(cin, start);
			} while (m->find(start) == m->end());
			do
			{
				cout << "End node (name): ";
				getline(cin, end);
			} while (m->find(end) == m->end());
			int ret = g->AddEdge((*m)[start], (*m)[end]);
			switch (ret)
			{
			case DirectedGraph::INDEX_OUT_OF_RANGE:
				cout << "Index out of range" << endl;
				break;
			case DirectedGraph::NODE_DUPLICATED:
				cout << "Start and end node is the same node" << endl;
				break;
			case DirectedGraph::EDGE_EXISTED:
				cout << "Edge already exist" << endl;
				break;
			default:
				cout << "Done!" << endl;
				break;
			}
			break;
		}
		case '2':
		{
			string start;
			string end;
			do
			{
				cout << "Start node (name): ";
				getline(cin, start);
			} while (m->find(start) == m->end());
			do
			{
				cout << "End node (name): ";
				getline(cin, end);
			} while (m->find(end) == m->end());
			int ret = g->RemoveEdge((*m)[start], (*m)[end]);
			switch (ret)
			{
			case DirectedGraph::INDEX_OUT_OF_RANGE:
				cout << "Index out of range" << endl;
				break;
			case DirectedGraph::NODE_DUPLICATED:
				cout << "Start and end node is the same node" << endl;
				break;
			case DirectedGraph::EDGE_NOT_EXISTED:
				cout << "Edge does not exist" << endl;
				break;
			default:
				cout << "Done!" << endl;
				break;
			}
			break;
		}
		case '3':
		{
			int ret = g->FindCutOffPoint();
			switch (ret)
			{
			case DirectedGraph::GRAPH_NOT_CONNECTED:
				cout << "The graph is not connected" << endl;
				break;
			case -1:
				cout << "No cut off point" << endl;
				break;
			default:
				cout << "Cut off point: " << ret;
				break;
			}
			break;
		}
		case '4':
		{
			string start;
			string end;
			do
			{
				cout << "Start node (name): ";
				getline(cin, start);
			} while (m->find(start) == m->end());
			do
			{
				cout << "End node (name): ";
				getline(cin, end);
			} while (m->find(end) == m->end());
			list<int> path;
			int ret = g->FindShortestPath((*m)[start], (*m)[end], path);
			switch (ret)
			{
			case 0:
				cout << "Shortest path:" << endl;
				for (list<int>::const_iterator i = path.begin(); i != path.end(); i++)
				{
					cout << *i << " ";
				}
				cout << endl;
				break;
			case DirectedGraph::INDEX_OUT_OF_RANGE:
				cout << "Index out of range" << endl;
				break;
			case DirectedGraph::NODE_DUPLICATED:
				cout << "Start and end node is the same node" << endl;
				break;
			default:
				break;
			}
			break;
		}
		case '5':
		{
			exit = 1;
			break;
		}
		default:
			break;
		}
	} while (!exit);

	// clean up
	delete g;
	return 0;
}

DirectedGraph::DirectedGraph(int nodeCount, int isDirected)
{
	this->nodeCount = nodeCount;
	this->isDirected = isDirected;
	this->data = new int*[nodeCount];
	for (int i = 0; i < nodeCount; i++)
	{
		this->data[i] = new int[nodeCount];
		for (int j = 0; j < nodeCount; j++)
		{
			this->data[i][j] = 0;
		}
	}
}

DirectedGraph::~DirectedGraph()
{
	for (int i = 0; i < nodeCount; i++)
	{
		delete[] this->data[i];
	}
	delete[] this->data;
}

int DirectedGraph::NodeCount()
{
	return this->nodeCount;
}

int DirectedGraph::AddEdge(int start, int end)
{
	if (0 > start || start >= this->nodeCount ||
		0 > end || end >= this->nodeCount)
	{
		return DirectedGraph::INDEX_OUT_OF_RANGE;
	}
	if (start == end)
	{
		return DirectedGraph::NODE_DUPLICATED;
	}
	if (this->data[start][end])
	{
		return DirectedGraph::EDGE_EXISTED;
	}
	this->data[start][end] = 1;
	if (!this->isDirected)
	{
		this->data[end][start] = 1;
	}
	return 0;
}

int DirectedGraph::RemoveEdge(int start, int end)
{
	if (0 > start || start >= this->nodeCount ||
		0 > end || end >= this->nodeCount)
	{
		return DirectedGraph::INDEX_OUT_OF_RANGE;
	}
	if (start == end)
	{
		return DirectedGraph::NODE_DUPLICATED;
	}
	if (!this->data[start][end])
	{
		return DirectedGraph::EDGE_NOT_EXISTED;
	}
	this->data[start][end] = 0;
	if (!this->isDirected)
	{
		this->data[end][start] = 0;
	}
	return 0;
}

int DirectedGraph::IsConnected()
{
	int** workingData;
	DirectedGraph* tmp;
	if (this->isDirected)
	{
		// create an undirected graph
		tmp = new DirectedGraph(this->nodeCount, 0);
		for (int i = 0; i < this->nodeCount; i++)
		{
			for (int j = 0; j < this->nodeCount; j++)
			{
				if (this->data[i][j])
				{
					tmp->AddEdge(i, j);
				}
			}
		}
		workingData = tmp->data;
	}
	else
	{
		workingData = this->data;
	}
	// this mask will be used to mark visited node
	// 0: the node has not been checked yet
	// 1: the node is reachable and thus will be further processed
	// 2: the node is done, no more cheking needed
	int* mask = new int[this->nodeCount];
	for (int i = 0; i < this->nodeCount; i++)
	{
		mask[i] = 0;
	}
	// pick a starting node, let's just choose node 0 
	mask[0] = 1;
	// start visiting other node
	int hasMoreNode;
	do
	{
		hasMoreNode = 0;
		for (int i = 0; i < this->nodeCount; i++)
		{
			if (mask[i] == 1)
			{
				// this node is marked as reachable, let's find its neighbours
				mask[i] = 2; // done with this node
				for (int j = 0; j < this->nodeCount; j++)
				{
					// find unchecked, reachable node
					if (!mask[j] && workingData[i][j])
					{
						// node j is reachable
						mask[j] = 1;
						hasMoreNode = 1;
					}
				}
			}
		}
	} while (hasMoreNode);
	// check the final result
	hasMoreNode = 0;
	for (int i = 0; i < this->nodeCount; i++)
	{
		if (!mask[i])
		{
			hasMoreNode = 1;
			break;
		}
	}
	// cleaning up
	delete[] mask;
	if (this->isDirected)
	{
		delete tmp;
	}
	return !hasMoreNode;
}

DirectedGraph* DirectedGraph::CloneWithoutNode(int node)
{
	if (this->nodeCount <= 1)
	{
		// not enough node to perform this operation
		return nullptr;
	}
	// create a coppy of this graph
	DirectedGraph* tmp = new DirectedGraph(this->nodeCount - 1, this->isDirected);
	for (int i = 0; i < this->nodeCount; i++)
	{
		for (int j = 0; j < this->nodeCount; j++)
		{
			// add all edge that is not related to given node
			if (i != node && j != node && this->data[i][j])
			{
				tmp->AddEdge(i - (i > node ? 1 : 0), j - (j > node ? 1 : 0));
			}
		}
	}
	return tmp;
}

int DirectedGraph::FindCutOffPoint()
{
	if (this->nodeCount <= 1)
	{
		// not enough node to perform this operation
		return -1;
	}
	if (!this->IsConnected())
	{
		return DirectedGraph::GRAPH_NOT_CONNECTED;
	}
	int cutOff = -1;
	for (int i = 0; i < this->nodeCount; i++)
	{
		DirectedGraph* tmp = this->CloneWithoutNode(i);
		if (!tmp->IsConnected())
		{
			// after remove node i, the graph is no longer connected
			cutOff = i;
			delete tmp;
			break;
		}
		delete tmp;
	}
	return cutOff;
}

int DirectedGraph::FindShortestPath(int start, int end, list<int>& path)
{
	if (0 > start || start >= this->nodeCount ||
		0 > end || end >= this->nodeCount)
	{
		return DirectedGraph::INDEX_OUT_OF_RANGE;
	}
	if (start == end)
	{
		return DirectedGraph::NODE_DUPLICATED;
	}

	// this mask will be used to mark visited node
	// 0: the node has not been checked yet
	// 1: the node is reachable and thus will be further processed
	// 2: the node is done, no more cheking needed
	int* mask = new int[this->nodeCount];
	// this mask will be used to mark the path
	// if before[i] = j that means there's an edge from j to i
	// before[i] = -1 means there's no edge to i that will be used as final path
	int* before = new int[this->nodeCount];
	for (int i = 0; i < this->nodeCount; i++)
	{
		mask[i] = 0;
		before[i] = -1;
	}
	// now we will perform BFS (breadth-first search) on the graph to find the shortest path
	// starting from "start" node
	mask[start] = 1;
	int hasMoreNode;
	int forceStop = 0;
	do
	{
		hasMoreNode = 0;
		for (int i = 0; i < this->nodeCount && !forceStop; i++)
		{
			if (mask[i] == 1)
			{
				// this node is marked as reachable, let's find its neighbours
				mask[i] = 2; // done with this node
				for (int j = 0; j < this->nodeCount; j++)
				{
					// find unchecked, reachable node
					if (!mask[j] && this->data[i][j])
					{
						// node j is reachable
						mask[j] = 1;
						before[j] = i;
						if (j != end)
						{
							hasMoreNode = 1;
						}
						else
						{
							// stop the search, since we've reached the "end" node
							forceStop = 1;
							break;
						}
					}
				}
			}
		}
	} while (!forceStop && hasMoreNode);
	// build the path
	if (forceStop)
	{
		path.insert(path.begin(), end);
		while (before[end] != start)
		{
			path.insert(path.begin(), before[end]);
			end = before[end];
		}
		path.insert(path.begin(), start);
	}
	// cleaning up
	delete[] mask;
	delete[] before;
	return 0;
}

void DirectedGraph::Print()
{
	cout << "test" << endl;
}
