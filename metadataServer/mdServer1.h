#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include "metadata.h"
using namespace std;

extern metadata *ms;
extern int ms1Pos;

int sockSrv;
struct sockaddr_in addrSrv;
struct sockaddr_in addrClient;
//struct sockaddr_in addrClient;

int findMdID(int id);

void BuildSocket()
{
    sockSrv = socket(AF_INET,SOCK_DGRAM,0);
    if(sockSrv<0)
    {
        fprintf(stderr,"Socket Error:%s\n",strerror(errno));
        exit(1);
    }
    //socklen_t len=sizeof(sockaddr);
    memset(&addrSrv,0,sizeof(addrSrv));
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
    addrSrv.sin_port = htons(6101);
    bind(sockSrv,(sockaddr*)&addrSrv,sizeof(sockaddr));
}

unsigned int findID(string input)
{
    int inputLen = input.length();
    unsigned int mdID;
    int i = 0, j = 0;
    for (int k = 0; k < inputLen; k++)
    {
        if((k > 0 && input[k] == ' ') && k < (inputLen - 1))
        {
            j = k - 1;
            if (j >= i)
            {
                string inTemp;
                while (i <= j)
                {
                    inTemp += input[i];
                    i++;
                }
            }
            i = k + 1;
        }
        else if (k == inputLen - 1)
        {
            j = k;
            if (j >= i)
            {
                string inTemp;
                while (i <= j)
                {
                    inTemp += input[i];
                    i++;
                }
                mdID = atol(inTemp.c_str());
            }
        }
    }
    return mdID;
}

struct MdLinkNode
{
	int mdID;
	metadata md;
	MdLinkNode* next;
};

class mdList
{
public:
	MdLinkNode* head;
	MdLinkNode* curr;
	MdLinkNode* tail;
	int cnt;

	void init();
	void append(metadata inMd);
	void delFirst();
	void print();
	metadata findNode(unsigned int ID);
    void delMd(unsigned int ID);
};

void mdList::init()
{
	curr = head = tail = new MdLinkNode;
	cnt = 0;
}
void mdList::append(metadata inMd)
{
	tail = tail -> next = new MdLinkNode;
	tail -> mdID = inMd.mdID;
	tail -> md = inMd;
	tail -> next = NULL;
	cnt++;
}

metadata mdList::findNode(unsigned int ID)
{
	
	curr = head;
	while (curr -> next != NULL)
	{
		if(curr -> mdID != ID)
		{
			curr = curr -> next;
		}
		else
		{
			break;
		}
	}
	return curr -> md;
	
}

void mdList::delMd(unsigned int ID)
{
    curr = head;
	while (curr -> next != NULL)
	{
        //cout<<"aaaaa1"<<endl;
		if(curr -> next -> mdID != ID)
		{
			curr = curr -> next;
		}
		else
		{
			break;
		}
	}
    MdLinkNode* temp = curr;
    curr = curr -> next;
    if (curr == tail)
    {
        //cout<<"aaaaa1"<<endl;
        temp -> next = NULL;
        tail = temp;
    }
    else
    {
        temp -> next = curr -> next;
    }
    delete curr;
    cnt--;

}
