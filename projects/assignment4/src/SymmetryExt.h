#pragma once

#include <set>
#include <map>
#include <string>

//TODO define SymmetryI* comparator

class SymmetryI {
	std::string _name;
protected:
	// Symmetry's children. Protected-access is READ-ONLY
	std::set<SymmetryI *> _children;
protected:
	SymmetryI(std::string name);
public:
	virtual ~Symmetry();
	virtual void draw() = 0;
	void addChild(SymmetryI *child);
	void removeChild(SymmetryI *child);
	std::string name() const;
};

class SymmWorldObjContainer : public SymmetryI {
	std::map<std::string, WorldObject*> _worldObjects;
public:
	SymmWorldObjContainer(std::string name);
	void draw();
	void addWorldObject(WorldObject *object);
	void removeWorldObject(std::string objectName); //or the object itself...
};

class RealSymmetry : public SymmetryI {
protected:
	SymmetryI *_parent;
public:
	virtual ~RealSymmetry();// change children's parent to this' parent
	RealSymmetry(SymmetryI *parent);
	void setParent(SymmetryI *parent);
	//virtual void draw()=0;
};