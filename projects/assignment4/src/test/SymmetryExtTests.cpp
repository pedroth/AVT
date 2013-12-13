#include "..\test\SymmetryExtTests.h"

#include <iostream>

void TestChildSymm::draw(ShaderProgram *program)
{
	std::cout << "Test Symm draw '" << name() << "'" << std::endl;
	_parent->draw(program);
	std::cout << "Test Symm draw end. " << name() << std::endl;
}
TestChildSymm::TestChildSymm(std::string name)
: RealSymmetry(name)
{}

SymmetryTree *testTree;
const std::string sym1name = "Sym1";
const std::string sym2name = "Sym2";
const std::string sym3name = "Sym3";
const std::string sym4name = "Sym4";
const std::string sym5name = "Sym5";
const std::string sym6name = "Sym6";
TestChildSymm *sym1, *sym2, *sym3, *sym4, *sym5, *sym6;

void buildTree()
{
	testTree = new SymmetryTree("Test Tree");
}
void destroyTree()
{
	delete testTree;
}
void drawTree(ShaderProgram *program, std::string message)
{
	std::cout << "Draw Tree " << message << ": Start" << std::endl;
	testTree->draw(program);
	std::cout << "Draw Tree " << message << ": End" << std::endl;
}

void createSymmetrys()
{
	sym1 = new TestChildSymm(sym1name);
	sym2 = new TestChildSymm(sym2name);
	sym3 = new TestChildSymm(sym3name);
	sym4 = new TestChildSymm(sym4name);
	sym5 = new TestChildSymm(sym5name);
	sym6 = new TestChildSymm(sym6name);
}
void destroySymmetrys()
{
	delete sym1; delete sym2; delete sym3;
	delete sym4; delete sym5; delete sym6;
}

void buildChain()
{
	testTree->addSymmetry(sym1);
	testTree->addSymmetry(sym2, sym1);
	testTree->addSymmetry(sym3, sym2);
}

void buildTwinChain()
{
	buildChain();
	testTree->addSymmetry(sym4, sym1);
	testTree->addSymmetry(sym5, sym4);
}


void standardTest()
{
	buildTree();
	//some stuff
	destroyTree();
}

void chainTest()
{
	buildTree();
	createSymmetrys();
	buildChain();
	drawTree(nullptr, "ChainTest");
	destroyTree();
	destroySymmetrys();
}
void twinChainTest()
{
	buildTree();
	createSymmetrys();
	buildTwinChain();
	drawTree(nullptr, "TwinChain");
	destroyTree();
	destroySymmetrys();
}
void detachTest()
{
	buildTree();
	createSymmetrys();
	buildChain();
	drawTree(nullptr, "Detach before");
	//sym2->detach();
	testTree->removeSymmetry(sym2);
	drawTree(nullptr, "Detach after");
	destroyTree();
	destroySymmetrys();
}
void insertBetweenTest()
{
	buildTree();
	createSymmetrys();
	buildTwinChain();
	drawTree(nullptr, "Insert Between Before");
	testTree->addSymmetryBetweenParentAndChildren(sym6, sym1);
	drawTree(nullptr, "Insert Between After");
	destroyTree();
	destroySymmetrys();
}
void smallTwinTest()
{
	createSymmetrys();
	buildTree();
	testTree->addSymmetry(sym1);
	testTree->addSymmetry(sym2);
	drawTree(nullptr, "SmallTwinTest");
	destroyTree();
	destroySymmetrys();
}


void testSymmetrys()
{
	standardTest();
	chainTest();
	detachTest();
	twinChainTest();
	insertBetweenTest();
	smallTwinTest();
}