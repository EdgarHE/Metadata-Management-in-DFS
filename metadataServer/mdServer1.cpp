#include "mdServer1.h"


int ms1Pos = 0;
metadata *ms = new metadata[100];

int numofNode = 3;
int *block = new int[numofNode]; //the next block num in each node


int findMdID(int id)
{

}
int main()
{
	//metadata mdRecv;
	char recvBuf[500], sendBuf[500];
	socklen_t len=sizeof(sockaddr);
	mdList metadataList;
	metadataList.init();
	
	BuildSocket();
	cout << "Enter the Metadata Server 192.168.1.198" << endl;

	while (true)
	{
		cout<<"MetadataServer@192.168.1.198: ";
		
		//
		socklen_t len=sizeof(sockaddr);
		memset(sendBuf, 0, sizeof(sendBuf));
		memset(recvBuf,0,sizeof(recvBuf));
		recvfrom(sockSrv,recvBuf,sizeof(recvBuf),0,(sockaddr*)&addrClient,&len);
		//cout<<recvBuf<<endl;
		string strRecvBuf = recvBuf;
		if(!strRecvBuf.compare(0, 7, "$receive", 0, 7))
		{
			unsigned int mdID = findID(strRecvBuf);
			metadata mmm = metadataList.findNode(mdID);
			string sendTemp = genPkt(mmm);
			sendTemp.copy(sendBuf, sendTemp.length(), 0);
			//cout<<sendBuf<<endl;
			sendto(sockSrv, sendBuf, sizeof(sendBuf), 0, (sockaddr*)&addrClient, len);
			metadataList.delMd(mdID);
			cout << "Send the Metadata " << mdID << endl;
			//metadataList.tail->md.printMetadata();
		}
		else if(!strRecvBuf.compare(0, 6, "$remove", 0, 6))
		{
			
			unsigned int mdID = findID(strRecvBuf);
			//metadata mmm = metadataList.findNode(mdID);
			//string sendTemp = genPkt(mmm);
			//sendTemp.copy(sendBuf, sendTemp.length(), 0);
			//cout<<sendBuf<<endl;
			//sendto(sockSrv, sendBuf, sizeof(sendBuf), 0, (sockaddr*)&addrClient, len);
			metadataList.delMd(mdID);
			cout << "Remove the Metadata " << mdID << endl;
			//cout<<"DELETED!"<<endl;
			//metadataList.tail->md.printMetadata();
		}
		else if(!strRecvBuf.compare(0, 4, "$stat", 0, 4))
		{
			//cout << "Print the Metadata" << endl;
			unsigned int mdID = findID(strRecvBuf);
			metadata mmm = metadataList.findNode(mdID);
			string sendTemp = genPkt(mmm);
			sendTemp.copy(sendBuf, sendTemp.length(), 0);
			//cout<<sendBuf<<endl;
			sendto(sockSrv, sendBuf, sizeof(sendBuf), 0, (sockaddr*)&addrClient, len);
			cout << "Send the Metadata " << mdID << endl;
			//metadataList.delMd(mdID);
			//cout<<"SEND_STAT!"<<endl;
			//metadataList.tail->md.printMetadata();
		}
		else
		{
			metadata mmm = splitPkt(strRecvBuf);
			metadataList.append(mmm);
			cout << "Receive the Metadata " << mmm.mdID << endl;
			metadataList.tail->md.printMetadata();
		}
		memset(sendBuf, 0, sizeof(sendBuf));
		memset(recvBuf,0,sizeof(recvBuf));
		cout<<"Num in MDServer: "<<metadataList.cnt<<endl;
	}
	

	return 0;
}
