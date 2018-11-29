#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#define NONE          "\033[m"
#define RED           "\033[0;32;31m"
#define LIGHT_RED     "\033[1;31m"
#define GREEN         "\033[0;32;32m"
#define LIGHT_GREEN   "\033[1;32m"
#define BLUE          "\033[0;32;34m"
#define LIGHT_BLUE    "\033[1;34m"
#define DARY_GRAY     "\033[1;30m"
#define CYAN          "\033[0;36m"
#define LIGHT_CYAN    "\033[1;36m"
#define PURPLE        "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN         "\033[0;33m"
#define YELLOW        "\033[1;33m"
#define LIGHT_GRAY    "\033[0;37m"
#define WHITE         "\033[1;37m"

struct TreeNode;
class addrTree;
extern addrTree addressTree;

struct TreeNode
{
	string name;
	int type; //file: 0, dir: 1;
	TreeNode* firstChild;
	TreeNode* nextSibling;
	TreeNode* parent;
	TreeNode* siblingTail;
};

class addrTree
{
public:
	TreeNode* root;
	TreeNode* curr;
	TreeNode* extCurrent;

	void init();
	void addNode(int tp, string nm, TreeNode* extCurr);
	void delNode(TreeNode* extCurr, string nm);
	void printAddr(TreeNode* extCurr, int tp);
	void printChild(TreeNode* extCurr);
	void printParent(TreeNode* extCurr);
	void enterDir(TreeNode* extCurr, string nm);
	int findFile(TreeNode* extCurr, string nm);
	string fileAddr(TreeNode* extCurr, string nm);
	string findParent(TreeNode* extCurr);
};

void addrTree::init()
{
	root = new TreeNode;
	root -> name = "root";
	root -> type = 1;
	root -> firstChild = NULL;
	root -> nextSibling = NULL;
	root -> siblingTail = root;
	root -> parent = NULL;
	extCurrent = root;
}

void addrTree::addNode(int tp, string nm, TreeNode* extCurr)
{
	if (extCurr -> firstChild == NULL)
	{
		curr = extCurr -> firstChild = new TreeNode;
		curr -> name = nm;
		curr -> type = tp;
		curr -> firstChild = NULL;
		curr -> nextSibling = NULL;
		curr -> siblingTail = curr;
		curr -> parent = extCurr;
		//cout<<"OK"<<endl;
	}
	else
	{
		//cout<<"in1"<<endl;
		curr = extCurr -> firstChild -> siblingTail -> nextSibling = new TreeNode;
		curr -> name = nm;
		curr -> type = tp;
		curr -> firstChild = NULL;
		curr -> nextSibling = NULL;
		extCurr -> firstChild -> siblingTail = curr;
		curr -> parent = extCurr;
		//cout<<"OK"<<endl;
	}
}

void addrTree::delNode(TreeNode* extCurr, string nm)
{
	curr = extCurr;

	if (curr -> firstChild != NULL)
	{
		if (curr -> firstChild -> name == nm)
		{
			if(curr -> firstChild -> type == 0)
			{
				TreeNode *temp = curr;
				curr = curr -> firstChild;
				if(curr -> nextSibling == NULL)
				{
					temp -> firstChild = NULL;
					temp -> siblingTail = temp;
					delete curr;
				}
				else
				{
					temp -> firstChild = curr -> nextSibling;
					delete curr;
				}

			}			
		}

		else if (curr -> firstChild -> nextSibling != NULL)
		{
			curr = curr -> firstChild;
			while (curr -> nextSibling != NULL)
			{
				if (curr -> nextSibling -> name == nm)
				{
					if (curr -> nextSibling -> type == 0)
					{
						TreeNode *temp = curr;
						curr = curr -> nextSibling;
						if(curr -> nextSibling == NULL)
						{
							temp -> nextSibling = NULL;
							temp -> siblingTail = temp;
							delete curr;
						}
						else
						{
							temp -> nextSibling = curr -> nextSibling;
							delete curr;
						}
						break;
					}
				}
				curr = curr -> nextSibling;
			}
		}
	}
	
}

void addrTree::printChild(TreeNode* extCurr)
{
	int cnt = 1;
	cout << left;
	if (extCurr -> firstChild != NULL)
	{
		curr = extCurr -> firstChild;
		if (curr -> type == 0)
		{
			cout << setw(12) << curr -> name ;//<< "     ";
			cnt++;
		}
		else
		{
			cout << LIGHT_CYAN <<setw(12) << curr -> name << NONE;// << "     ";
			cnt++;
		}

		while (curr -> nextSibling != NULL)
		{
			//cout << "curr: "<< curr -> name <<endl;
			//cout << "next: "<< curr -> nextSibling-> name <<endl;
			curr = curr -> nextSibling;
			if (curr -> type == 0)
			{
				if(cnt % 4 == 1)
				{
					cout<<endl;
					cout << left;
					cout << setw(12) << curr -> name;// << "     ";
					cnt++;
				}
				else
				{
					cout << setw(12) << curr -> name;// << "     ";
					cnt++;
				}
				
			}
			else
			{
				if(cnt % 4 == 1)
				{
					cout<<endl;
					cout << left;
					cout << LIGHT_CYAN << setw(12) << curr -> name << NONE;// << "     ";
					cnt++;
				}
				else
				{
					cout << LIGHT_CYAN << setw(12) << curr -> name << NONE;// << "     ";
					cnt++;
				}
			}
		}
	}
	cout<<endl;
}

void addrTree::printAddr(TreeNode* extCurr, int tp)
{
	curr = extCurr;
	if (curr == root)
	{
		cout << "/root";
	}
	else
	{
		printParent(curr);
	}
	
	if (tp == 0) cout << "$ ";
	else if (tp == 1) cout<<endl;
}

void addrTree::printParent(TreeNode* extCurr)
{
	if (extCurr -> parent != NULL)
	{
		printParent(extCurr -> parent);
	}
	cout << "/" << extCurr -> name;
}

string addrTree::fileAddr(TreeNode* extCurr, string nm)
{
	curr = extCurr;
	string tempAddr = "";
	
	{
		tempAddr = findParent(curr);
	}
	return tempAddr + "/" + nm;
}

string addrTree::findParent(TreeNode* extCurr)
{
	string temp = "";
	if (extCurr -> parent != NULL)
	{
		temp += findParent(extCurr -> parent);
	}
	temp += "/" + extCurr -> name;
	return temp;
}

void addrTree::enterDir(TreeNode* extCurr, string nm)
{
	int judge = 0; // 0: not found, 1: not a dir, 2: success
	curr = extCurrent;
	//cout << "name: " << nm << " FIN"<<endl;
	if (nm == "")
	{
		//cout << "ok1" << endl;
		extCurrent = root;
		judge = 2;
	}
	else if (nm == "..")
	{
		if(curr == root)
		{
			extCurrent = root;
			judge = 2;
		}
		else
		{
			extCurrent = curr -> parent;
			judge = 2;
		}
	}
	else if (curr -> firstChild != NULL)
	{
		//cout << "ok2" << endl;
		if (curr -> firstChild -> name == nm)
		{
			if(curr -> firstChild -> type == 1)
			{
				extCurrent = curr -> firstChild;
				judge = 2;
			}
			else
			{
				judge = 1;
			}
			
		}

		else if (curr -> firstChild -> nextSibling != NULL)
		{
			//cout << "ok3" << endl;
			curr = curr -> firstChild;
			while (curr -> nextSibling != NULL)
			{
				if (curr -> nextSibling -> name == nm)
				{
					if (curr -> nextSibling -> type == 1)
					{
						extCurrent = curr -> nextSibling;
						judge = 2;
						break;
					}
					else
					{
						judge = 1;
						break;	
					}
				}
				curr = curr -> nextSibling;
			}
		}
	}
	if (judge == 0)
	{
		cout << "cd: No such directory" << endl;
	}
	else if (judge == 1)
	{
		cout << "cd: Not a directory" << endl;
	}
	//else cout << "SSSSSSSSS" << endl;
}

int addrTree::findFile(TreeNode* extCurr, string nm)
{
	int judge = 0; // 0: not found, 2: success
	curr = extCurr;
	if (curr -> firstChild != NULL)
	{
		if (curr -> firstChild -> name == nm)
		{
			if(curr -> firstChild -> type == 0)
			{
				judge = 2;
			}			
		}

		else if (curr -> firstChild -> nextSibling != NULL)
		{
			curr = curr -> firstChild;
			while (curr -> nextSibling != NULL)
			{
				if (curr -> nextSibling -> name == nm)
				{
					if (curr -> nextSibling -> type == 0)
					{
						judge = 2;
						break;
					}
				}
				curr = curr -> nextSibling;
			}
		}
	}
	//cout<<judge<<endl;
	return judge;
}