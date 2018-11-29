//#include <iostream>

#include "api.h"
#include "client.h"
#include "addrTree.h"

//#include "mdServer1.h"
using namespace std;

listString inputCommand;
addrTree addressTree;

int numofMdServer = 2;
int numofNode = 3;
int *block = new int[numofNode]; //the next block num in each node
//metadata *mdTemp;
int ms1Pos;
socklen_t len = sizeof(sockaddr);


int main (int argc, char *argv[])
{	

	/*Declare*/
	string input;


	cout << YELLOW << "\nWelcome to the system!" << endl;
	printTime();
	cout<<endl;
	inputCommand.init();
	addressTree.init();

	buildSocket();
	char sendBuf[500], recvBuf[500];

	/*Init for nodes and block*/
	int r = ((rand()%(10000-1000))+10000)*numofNode;
	for(int i = 0; i < numofNode; i++)
	{
		block[i] = r + i;
		//cout << block[i]<<endl;
	}

	while(1)
	{
		cout << LIGHT_PURPLE <<"project4@BigData:";
		cout << BROWN;
		addressTree.printAddr(addressTree.extCurrent, 0);
		cout<<NONE;
		getline(cin, input);
		//cout << "test: " << input << endl;
		if (input.length() != 0) addCommand(input);
		else addCommand(" ");
		//inputCommand.print();
		

		int apiID= runAPI(inputCommand.head -> next -> command);
		inputCommand.delFirst();
		
		if (apiID == 0) //exit
		{
			cout << YELLOW << "\nExit the system at ";
			printTime();
			break;
		}
		else if (apiID == 1) // mkdir
		{
			if (inputCommand.head -> next != NULL)
			{
				string dirName = inputCommand.head -> next -> command;
				addressTree.addNode(1, dirName, addressTree.extCurrent);
				cout << "mkdir: created the directory named \"" << dirName << "\"" << endl;
			}
			else
			{
				cout << "mkdir: missing operand" << endl;
			}
			
		}
		else if (apiID == 2) //touch
		{
			//cout<<"ooook1!"<<endl;
			if (inputCommand.head -> next != NULL)
			{
				string fileName = inputCommand.head -> next -> command;
				metadata md;
				int mdServerID;

				if(addressTree.findFile(addressTree.extCurrent, fileName) == 0)
				{
					//cout<<"NOOOOO!"<<endl;
					addressTree.addNode(0, fileName, addressTree.extCurrent);
					//cout<<addressTree.fileAddr(addressTree.extCurrent, fileName)<<endl;

					md.init(fileName, addressTree.fileAddr(addressTree.extCurrent, fileName));
					unsigned int mdID = SDBMHash(addressTree.fileAddr(addressTree.extCurrent, fileName)); 
					mdServerID = mdID % numofMdServer;
					cout<<"Created the file"<<endl;
				}
				else
				{
					//cout<<"FOUND!!!"<<endl;
					unsigned int mdID = SDBMHash(addressTree.fileAddr(addressTree.extCurrent, fileName)); 
					//cout<<mdID<<endl;
					mdServerID = mdID % numofMdServer;
					string sendTemp = genFindPkt(mdID, 1);
					sendTemp.copy(sendBuf, sendTemp.length(), 0);
					//cout<<sendTemp<<endl;

					if (mdServerID == 0)
					{
						sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv0,len);
						recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrSrv0, &len);
					}
					else if (mdServerID == 1)
					{
						sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv1,len);
						recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrSrv1, &len);
					}
					// else if (mdServerID == 1)
					// {
					// 	sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv1,len);
					// 	recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrSrv1, &len);
					// }
					
					if (mdServerID == 0) cout<<"Found the Metadata in MDServer "<<mdserver0<<endl;
					else if (mdServerID == 1) cout<<"Found the Metadata in MDServer "<<mdserver1<<endl;
					string strRecvBuf = recvBuf;
					md = splitPkt(strRecvBuf);
					memset(sendBuf, 0, sizeof(sendBuf));
					memset(recvBuf, 0, sizeof(recvBuf));
				}
				
				
				inputCommand.delFirst();
				

				while (inputCommand.head -> next != NULL)
				{
					LinkNode *currNode = inputCommand.head -> next;
					string cmd1 = currNode -> command;
					if (currNode -> next != NULL)
					{
						string cmd2 = currNode -> next -> command;
						int fileCmdID = addFile(cmd1, cmd2);
						if (fileCmdID == 0) 
						{
							md.setSize(atoi(cmd2.c_str()));
							cout<<"Changed the file size " <<endl;
						}
						//md->printMetadata();

						inputCommand.delFirst();
						inputCommand.delFirst();
					}
					else
					{
						cout << "touch: missing operand" << endl;
						break;
					}
				}

				//md.printMetadata();

				//memset(sendBuf,0,sizeof(sendBuf));
				string sendTemp = genPkt(md);

				//cout<<sendTemp<<endl;

				sendTemp.copy(sendBuf, sendTemp.length(), 0);

				//cout<<sendBuf<<endl;
				if (mdServerID == 0) sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv0,sizeof(sockaddr));
				else if (mdServerID == 1) sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv1,sizeof(sockaddr));

				if (mdServerID == 0) cout<<"Stored the Metadata in MDServer "<<mdserver0<<endl;
				else if (mdServerID == 1) cout<<"Stored the Metadata in MDServer "<<mdserver1<<endl;

				


				memset(sendBuf, 0, sizeof(sendBuf));
				memset(recvBuf, 0, sizeof(recvBuf));
			}
			else
			{
				cout << "touch: missing file operand" << endl;
			}
		}
		else if (apiID == 3) //ls
		{
			addressTree.printChild(addressTree.extCurrent);
		}
		
		else if (apiID == 4) //rm
		{
			if (inputCommand.head -> next != NULL)
			{
				string fileName = inputCommand.head -> next -> command;
				//cout<<"ooook12"<<endl;
				metadata md;// = new metadata;
				if(addressTree.findFile(addressTree.extCurrent, fileName) == 0)
				{
					cout << "rm: No such file" << endl;
				}
				else
				{
					//cout<<"FOUND!!!"<<endl;
					
					unsigned int mdID = SDBMHash(addressTree.fileAddr(addressTree.extCurrent, fileName)); 
					int mdServerID = mdID % numofMdServer;
					//cout<<mdID<<endl;
					string sendTemp = genFindPkt(mdID, 2);
					sendTemp.copy(sendBuf, sendTemp.length(), 0);
					//cout<<sendTemp<<endl;

					if (mdServerID == 0) sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv0,len);
					else if (mdServerID == 1) sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv1,len);

					if (mdServerID == 0) cout<<"Removed the file \""<<fileName << "\" and the metadata in MDServer "<< mdserver0 <<endl;
					else if (mdServerID == 1) cout<<"Removed the file \""<<fileName << "\" and the metadata in MDServer "<< mdserver1 <<endl;

					addressTree.delNode(addressTree.extCurrent, fileName);
				}
				memset(sendBuf, 0, sizeof(sendBuf));
				memset(recvBuf, 0, sizeof(recvBuf));
			}
			else
			{
				cout << "rm: missing file operand" << endl;
			}
		}
		else if (apiID == 5) //stat
		{
			if (inputCommand.head -> next != NULL)
			{
				string fileName = inputCommand.head -> next -> command;
				//cout<<"ooook12"<<endl;
				metadata md;// = new metadata;
				if(addressTree.findFile(addressTree.extCurrent, fileName) == 0)
				{
					cout << "stat: No such file" << endl;
				}
				else
				{
					//cout<<"FOUND!!!"<<endl;
					unsigned int mdID = SDBMHash(addressTree.fileAddr(addressTree.extCurrent, fileName)); 
					int mdServerID = mdID % numofMdServer;
					
					//cout<<mdID<<endl;
					string sendTemp = genFindPkt(mdID, 3);
					sendTemp.copy(sendBuf, sendTemp.length(), 0);
					//cout<<sendTemp<<endl;

					if (mdServerID == 0)
					{
						sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv0,len);
						recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrSrv0, &len);
					}
					else if (mdServerID == 1)
					{
						sendto(sockClient,sendBuf,sizeof(sendBuf),0,(sockaddr*)&addrSrv1,len);
						recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrSrv1, &len);
					}


					//cout<<"RECEIVE_STAT!!!"<<endl;
					if (mdServerID == 0) cout<<"Found the Metadata in MDServer "<<mdserver0<<endl;
					else if (mdServerID == 1) cout<<"Found the Metadata in MDServer "<<mdserver1<<endl;
					string strRecvBuf = recvBuf;
					md = splitPkt(strRecvBuf);
					md.printMetadata();
				}
				memset(sendBuf, 0, sizeof(sendBuf));
				memset(recvBuf, 0, sizeof(recvBuf));
			}
			else
			{
				cout << "rm: missing file operand" << endl;
			}
		}
		else if (apiID == 6) ; //mv
		else if (apiID == 7) //cd
		{
			//inputCommand.print();
			//cout<< "curr: " <<addressTree.extCurrent -> name << endl;
			if (inputCommand.head -> next != NULL)
			{
				//cout << "ok1" << endl;
				string cmd = inputCommand.head -> next -> command;
				addressTree.enterDir(addressTree.extCurrent, cmd);
			}
			else
			{
				addressTree.enterDir(addressTree.extCurrent, "");
			}
			
		}
		else if (apiID == 8)// pwd
		{
			addressTree.printAddr(addressTree.extCurrent, 1);
			//cout<<addressTree.fileAddr(addressTree.extCurrent, "apple")<<endl;
		}
		else if (apiID == 100) //NULL
		{

		}


		inputCommand.delAll();
	}


	return 0;
}

