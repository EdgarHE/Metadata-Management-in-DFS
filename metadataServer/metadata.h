#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <functional>
using namespace std;

#define BLOCK_SIZE 100
#define BLOCK_MAX 30


extern int* block; 
extern int numofNode;
struct metadata;
unsigned int SDBMHash(string a);


unsigned int string_to_unsigned_int(string str)
{
	unsigned int result(0);//最大可表示值为4294967296（=2‘32-1）
	//从字符串首位读取到末位（下标由0到str.size() - 1）
	for (int i = str.size()-1;i >= 0;i--)
	{
		unsigned int temp(0),k = str.size() - i - 1;
		//判断是否为数字
		if (isdigit(str[i]))
		{
			//求出数字与零相对位置
			temp = str[i] - '0';
			while (k--)
				temp *= 10;
			result += temp;
		}
		else
			//exit(-1);
			break;
	}
	return result;
}

unsigned int SDBMHash(string a)
{
    unsigned int hash = 0;
    int i=0;

    while (a[i])
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (a[i++]) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

struct metadata
{
	string name;
	unsigned int mdID;
	int size; 
	int blockNum;
	int nodeID[BLOCK_MAX];
	int node[BLOCK_MAX];
	time_t birthTime;
	time_t changeTime;
	time_t accessTime;

	void init(string nm, string addrinTree);
	void addtoNode();
	void printMetadata();
	void setSize(int sz);
};

void metadata::init(string nm, string addrinTree)
{
	name = nm;
	mdID = SDBMHash(addrinTree);
	size = 100;
	blockNum = 0;
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		nodeID[i] = -1;
		node[i] = -1;
	}
	addtoNode();
	time(&birthTime);
	time(&changeTime);
	time(&accessTime);

}

void metadata::addtoNode()
{
	blockNum++;
	int tempNode = (rand()%(numofNode-0));
	nodeID[blockNum-1] = tempNode;
	node[blockNum-1] = block[tempNode];
	//cout<<"tempnode: "<<tempNode<<endl;
	//node.push_back(block[tempNode]);
	block[tempNode] = block[tempNode] + numofNode;
}

void metadata::printMetadata()
{
	cout<<"name: "<<name<<endl;
	cout<<"Metadata ID: "<<mdID<<endl;
	cout<<"size: "<<size << " Bytes"<<endl;
	cout<<"Total blocks: "<<blockNum<<endl;
	for(int i = 0; i < blockNum; i++)
	{
		cout<<"block "<<i<<": ";
		cout<<left;
		cout<<setw(8) << node[i] << "in node " << nodeID[i] <<endl;
	}
}

void metadata::setSize(int sz)
{
	if (sz == 0)
	{
		cout << "File size cannot be 0!" << endl;
	}
	else
	{
		int prevSize = size;
		size = sz;
		if (size > prevSize)
		{
			int newBlkNum = ceil(size / BLOCK_SIZE);
			if (newBlkNum > blockNum)
			{
				while (blockNum < newBlkNum)
				{
					addtoNode();
				}
			}
		}
		else if (size < prevSize)
		{
			int newBlkNum;
			if(ceil(size / BLOCK_SIZE) < 1)
			{
				newBlkNum = 1;
			} 
			else
			{
			 	newBlkNum = ceil(size / BLOCK_SIZE);
			}
			//cout<<"newBlkNum "<<newBlkNum<<endl;
			//cout<<"blockNum "<<blockNum<<endl;
			if (newBlkNum < blockNum)
			{
				while (blockNum > newBlkNum)
				{
					nodeID[blockNum-1] = -1;
					node[blockNum-1] = -1;
					blockNum--;
				}
			}
		}
	}
	
}

string genPkt(unsigned int ID)
{
	string sendTemp = "$receive ";

	stringstream ssID;
	ssID << ID;
	string strID;
	ssID >> strID;
	sendTemp = sendTemp + " " + strID ; //name + mdID
}

string genPkt(metadata md)
{
	string sendTemp = md.name;

	stringstream ssID;
	ssID << md.mdID;
	string strID;
	ssID >> strID;
	sendTemp = sendTemp + " " + strID ; //name + mdID

	stringstream ssSize;
	ssSize << md.size;
	string strSize;
	ssSize >> strSize;
	sendTemp = sendTemp + " " + strSize ; //name + mdID + size

	stringstream ssBlkNum;
	ssBlkNum << md.blockNum;
	string strBlkNum;
	ssBlkNum >> strBlkNum;
	sendTemp = sendTemp + " " + strBlkNum ; //name + mdID + size + blockNum

	for (int i = 0; i < md.blockNum; i++) //name + mdID + size + blockNum + nodeID
	{
		stringstream ssNodeID;
		ssNodeID << md.nodeID[i];
		string strNodeID;
		ssNodeID >> strNodeID;
		sendTemp = sendTemp + " " + strNodeID;
	}

	for (int i = 0; i < md.blockNum; i++) //name + mdID + size + blockNum + nodeID + node
	{
		stringstream ssNode;
		ssNode << md.node[i];
		string strNode;
		ssNode >> strNode;
		sendTemp = sendTemp + " " + strNode;
	}

	stringstream ssBirthTime; //name + mdID + size + blockNum + nodeID + node + birthTime
	ssBirthTime << md.birthTime;
	string strBirthTime;
	ssBirthTime >> strBirthTime;
	sendTemp = sendTemp + " " + strBirthTime;

	stringstream ssChangeTime;
	ssChangeTime << md.changeTime;
	string strChangeTime;
	ssChangeTime >> strChangeTime;
	sendTemp = sendTemp + " " + strChangeTime;

	stringstream ssAccessTime;
	ssAccessTime << md.accessTime;
	string strAccessTime;
	ssAccessTime >> strAccessTime;
	sendTemp = sendTemp + " " + strAccessTime;

	return sendTemp;
}

metadata splitPkt(string input)
{
	metadata md;
	int inputLen = input.length();
	//cout << "inputLen: " << inputLen << endl;
	int i = 0, j = 0, n = 1, k = 0;
	for (k; k < inputLen; k++ )
	{
		string strName;
		if(k > 0 && input[k] == ' ')
		{
			j = k - 1;
			if (j >= i)
			{
				while (i <= j)
				{
					strName += input[i];
					i++;
				}
			}
			//cout<<"Name: "<<strName<<endl;
			md.name = strName;
			//cout<<md.name;
			i = k + 1;
			k++;
			break;
		}
		
	}

	for (k; k < inputLen; k++ )
	{
		string strID;
		if(input[k] == ' ')
		{

			j = k - 1;
			//cout<<"k: "<<k<<" i: "<<i<<" j: "<<j<<endl;
			if (j >= i)
			{
				while (i <= j)
				{
					strID += input[i];
					i++;
				}
			}
			//cout<<"IDDDD: "<<strID<<endl;
			md.mdID = atol(strID.c_str());
			i = k + 1;
			k++;
			break;
		}
		
	}

	for (k; k < inputLen; k++ )
	{
		string strBlkSize;
		if(input[k] == ' ')
		{

			j = k - 1;
			//cout<<"k: "<<k<<" i: "<<i<<" j: "<<j<<endl;
			if (j >= i)
			{
				while (i <= j)
				{
					strBlkSize += input[i];
					i++;
				}
			}
			//cout<<"Size: "<<strBlkSize<<endl;
			md.size = atoi(strBlkSize.c_str());
			i = k + 1;
			k++;
			break;
		}
		
	}

	for (k; k < inputLen; k++ )
	{
		string strBlkNum;
		if(input[k] == ' ')
		{
			j = k - 1;
			if (j >= i)
			{
				while (i <= j)
				{
					strBlkNum += input[i];
					i++;
				}
			}
			md.blockNum = atoi(strBlkNum.c_str());
			i = k + 1;
			k++;
			break;
		}
	}

	for (int iTemp = 0; iTemp < md.blockNum; iTemp++)
	{
		for (k; k < inputLen; k++)
		{
			string strNodeID;
			if(input[k] == ' ')
			{
				j = k - 1;
				if (j >= i)
				{
					while (i <= j)
					{
						strNodeID += input[i];
						i++;
					}
				}
				md.nodeID[iTemp] = atoi(strNodeID.c_str());
				i = k + 1;
				k++;
				break;
			}
		}
	}

	for (int iTemp = 0; iTemp < md.blockNum; iTemp++)
	{
		for (k; k < inputLen; k++)
		{
			string strNode;
			if(input[k] == ' ')
			{
				j = k - 1;
				if (j >= i)
				{
					while (i <= j)
					{
						strNode += input[i];
						i++;
					}
				}
				md.node[iTemp] = atoi(strNode.c_str());
				i = k + 1;
				k++;
				break;
			}
		}
	}

	for (k; k < inputLen; k++)
	{
		string strBirthTime;
		if(input[k] == ' ')
		{
			j = k - 1;
			if (j >= i)
			{
				while (i <= j)
				{
					strBirthTime += input[i];
					i++;
				}
			}
			stringstream ssBirthTime;
			ssBirthTime << strBirthTime;
			time_t tempBirthTime;
			ssBirthTime >> tempBirthTime;
			md.birthTime = tempBirthTime;
			i = k + 1;
			k++;
			break;
		}		
	}

	for (k; k < inputLen; k++)
	{
		string strChangeTime;
		if(input[k] == ' ')
		{
			j = k - 1;
			if (j >= i)
			{
				while (i <= j)
				{
					strChangeTime += input[i];
					i++;
				}
			}
			stringstream ssChangeTime;
			ssChangeTime << strChangeTime;
			time_t tempChangeTime;
			ssChangeTime >> tempChangeTime;
			md.changeTime = tempChangeTime;
			i = k + 1;
			k++;
			break;
		}	
	}

	for (k; k < inputLen; k++)
	{
		string strAccessTime;
		if(k == inputLen - 1)
		{
			j = k;
			if (j >= i)
			{
				while (i <= j)
				{
					strAccessTime += input[i];
					i++;
				}
			}
			stringstream ssAccessTime;
			ssAccessTime << strAccessTime;
			time_t tempAccessTime;
			ssAccessTime >> tempAccessTime;
			md.accessTime = tempAccessTime;
			break;
		}
	}
	return md;
}

