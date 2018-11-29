#include "addrTree.h"

int main()
{
	addrTree aaa;
	aaa.init();

	aaa.addNode(1, "a1", aaa.extCurrent);
	aaa.addNode(0, "a2", aaa.extCurrent);
	aaa.addNode(0, "a3", aaa.extCurrent);
	aaa.printChild(aaa.extCurrent);


	aaa.enterDir(aaa.extCurrent, "a1");

	cout<< "curr: " <<aaa.extCurrent -> name << endl;
	aaa.addNode(1, "b1", aaa.extCurrent);

	aaa.enterDir(aaa.extCurrent, "a1");

	aaa.printChild(aaa.extCurrent);
	aaa.printAddr(aaa.extCurrent, 1);
	//aaa.extCurrent = aaa.extCurrent -> firstChild;
	//aaa.printAddr(aaa.extCurrent);
}