#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include "metadata.h"

#include "host.h"


using namespace std;



void *thread_recvMd(void* args);

int sockClient;
struct sockaddr_in addrSrv0, addrSrv1;

struct LinkNode;
class listString;
extern listString inputCommand;

void printTime();
void addCommand(string inCommand);
void addCommand(string input);

struct LinkNode
{
	string command;
	int num;
	LinkNode* next;
};

class listString
{
public:
	LinkNode* head;
	LinkNode* curr;
	LinkNode* tail;
	int cnt;

	void init();
	void append(string cm, int n);
	void delFirst();
	void delAll();
	void print();
};

void listString::init()
{
	curr = head = tail = new LinkNode;
	cnt = 0;
	head -> command = "";
	head -> num = 0;

}
void listString::append(string cm, int n)
{
	tail = tail -> next = new LinkNode;
	tail -> command = cm;
	tail -> num = n;
	tail -> next = NULL;
	cnt++;
}
void listString::delFirst()
{
	if (head -> next != NULL)
	{
		curr = head -> next;
		head -> next = curr -> next;
		if (curr == tail)
		{
			tail = head;
		}
		delete curr;
		cnt--;
	}
	else return;
}
void listString::delAll()
{
	while (head -> next != NULL)
	{
		delFirst();
	}
}
void listString::print()
{
	curr = head;
	while (curr -> next != NULL)
	{
		curr = curr -> next;
		cout << curr -> command << " ";
	}
	cout << endl;
}


void printTime()
{
	/*Welcome & Time*/
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	cout << ptminfo->tm_year+1900 << "-" << ptminfo->tm_mon + 1 << "-" << ptminfo->tm_mday << " ";
	if (ptminfo->tm_hour < 10) cout << "0" << ptminfo->tm_hour << ":";
	else cout << ptminfo->tm_hour << ":";
	if (ptminfo->tm_min < 10) cout << "0" << ptminfo->tm_min << ":";
	else cout << ptminfo->tm_min << ":";
	if (ptminfo->tm_sec < 10) cout << "0" << ptminfo->tm_sec << endl;
	else cout << ptminfo->tm_sec << endl;
}

void addCommand(string input)
{
	int inputLen = input.length();
	//cout << "inputLen: " << inputLen << endl;
	int i = 0, j = 0, n = 1;
	for (int k = 0; k < inputLen; k++)
	{
		if((k > 0 && input[k] == ' ') && k < (inputLen - 1))
		{
			j = k - 1;
			//cout << "i: " << i << endl;
			//cout << "j: " << j << endl;
			if (j >= i)
			{
				string inTemp;
				while (i <= j)
				{
					inTemp += input[i];
					i++;
				}
				//cout << "len2: " << inTemp.length() << endl;
				//cout << "append2: " << inTemp << endl;
				inputCommand.append(inTemp, n);
			}
			i = k + 1;
			n++;
		}
		else if (k == inputLen - 1)
		{
			j = k;
			//cout << "i: " << i << endl;
			//cout << "j: " << j << endl;
			if (j >= i)
			{
				string inTemp;
				while (i <= j)
				{
					inTemp += input[i];
					i++;
				}
				//cout << "len2: " << inTemp.length() << endl;
				//cout << "append2: " << inTemp << endl;
				inputCommand.append(inTemp, n);
			}
		}
	}
}



void buildSocket()
{
    
    sockClient = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockClient<0)
     {
        fprintf(stderr,"Socket Error:%s\n",strerror(errno));
        exit(1);
     }

    addrSrv0.sin_family = AF_INET;
    addrSrv0.sin_addr.s_addr = inet_addr(mdserver0);//"10.163.59.186"
    addrSrv0.sin_port = htons(6101);

    addrSrv1.sin_family = AF_INET;
    addrSrv1.sin_addr.s_addr = inet_addr(mdserver1);//"10.163.59.186"
    addrSrv1.sin_port = htons(6101);
}