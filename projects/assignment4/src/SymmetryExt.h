#pragma once

#include <map>
#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "WorldObject.h"
 
//TODO define Symmetry comparator
//TODO define Symmetry "equals"
//It is assumed that Symmetrys with the same name are the same object.

struct TransformedWO;

class SymmetryI {
private:
	//The symmetrys name.
	std::string _name;
private:
	//Is this symmetry active.
	bool _isActive;
public:
	typedef std::map<std::string, SymmetryI*> children_type;
protected:
	//Symmetry's children. Protected-access is READ-ONLY
	children_type _children;
	//Symmetry's parent. Can be nullptr.
	SymmetryI *_parent;
protected:
	SymmetryI(std::string name);

protected:
	virtual std::vector<TransformedWO> getOriginalTransfWO()=0;
	virtual std::vector<TransformedWO> getGhostTransfWO() = 0;
public:
	//Sets this symmetry as an active symmetry.
	void activate();
	//Sets this symmetry as an inactive symmetry.
	void deactivate();
	//Returns if the symmetry is active.
	bool isActive();
public:
	//Reassigns the parent of this' children to the parent of this.
	virtual ~SymmetryI();

	//Returns all the Transformed World objects by this Symmetry.
	//Originals and ghosts.
	std::vector<TransformedWO> getAllTransfWO();
	//Each symmetry should know how to draw itself using it's parents.
	virtual void draw(ShaderProgram *program) = 0;
	
	//Adds a child to this node. Also sets the parent of the child if
	//not already set to this.
	void addChild(SymmetryI *child);
	//Removes a child from this node if it's really a child of this symmetry.
	//The child's parent is removed.
	void removeChild(SymmetryI *child);
	//Sets this node's parent to parent. Also adds this node to the 
	//parent's children if not already a child of it.
	void setParent(SymmetryI *parent);
	//Detachs this symmetry from it's parent and children.
	void detach();
	
	//Returns this symmetry's children set/map.
	//The set/map should only be READ.
	const children_type &getChildren();

	//Check if child is a child of this symmetry.
	bool isChild(SymmetryI *child);
	//Check if parent is the parent of this symmetry.
	bool isParent(SymmetryI *parent);

	SymmetryI *parent();
	std::string name() const;
};

class SymmWorldObjContainer : public SymmetryI {
	typedef std::map<std::string, WorldObject*> world_obj_type;
	world_obj_type _worldObjects;
protected:
	std::vector<TransformedWO> getOriginalTransfWO();
	std::vector<TransformedWO> getGhostTransfWO();
public:
	//Create a SymmWorldObjContainer with the given name.
	SymmWorldObjContainer(std::string name);
	//Draws all the WorldObjects of this Container.
	void draw(ShaderProgram *program);
	//Adds a WorldObject to this container.
	//It is assumed that WorldObjects with the same name are the same.
	void addWorldObject(WorldObject *object);
	//Removes the given WorldObject from this container if it exists.
	void removeWorldObject(WorldObject *object);
	//Returns the WorldObject with the given name if it is present
	//in this container.
	WorldObject *getWorldObject(std::string objectName);
};

class RealSymmetry : public SymmetryI {
protected:
	std::vector<TransformedWO> getOriginalTransfWO();
	//virtual std::set<TransformedWO *> getGhostTransfWO()=0;
public:
	RealSymmetry(std::string name);
	virtual ~RealSymmetry();
	void draw(ShaderProgram *program);
};

struct TransformedWO {
	glm::mat4 transform;
	WorldObject *object;
	bool frontFaceCCW;
public:
	TransformedWO(WorldObject *object, glm::mat4 transform, bool ccw);
	void draw(ShaderProgram *program);
	//getTransform
	//getObject
	//getOwner
};

class SymmetryTree {
	typedef std::map<std::string, RealSymmetry*> symm_map_type;

	//The Tree's name.
	std::string _name;
	//WorldObject container.
	SymmWorldObjContainer *_container;
	//All the symmetrys currently in this Tree.
	symm_map_type _symmetrys;
	//The deepest symmetrys of this Tree.
	symm_map_type _deepesSymmetrys;
public:
	//Adds a symmetry with the container as parent.
	void addSymmetry(RealSymmetry *symmetry);
	//Adds a symmetry with parent as parent if it is in this Tree.
	void addSymmetry(RealSymmetry *symmetry, RealSymmetry *parent);
	//Adds a symmetry between parent and it's children. The added symmetry
	//becomes parent of the parent's children.
	void addSymmetryBetweenParentAndChildren(RealSymmetry *symmetry, RealSymmetry *parent);
	//Removes a symmetry from the Tree reassigning it's children to it's parent.
	//It is only removed if present in the Tree.
	void removeSymmetry(RealSymmetry *symmetry);
	//Returns the symmetry with the given name in this Tree if it exists.
	RealSymmetry *getSymmetry(std::string symmName);

	//Activate a symmetry from this symmetryTree if it is from this tree.
	void activateSymmetry(RealSymmetry *symmetry);
	//Deactivate a symmetry from this symmetryTree if it is from this tree.
	void deactivateSymmetry(RealSymmetry *symmetry);

	//Adds a WorldObject to the container.
	void addWorldObject(WorldObject *object);
	//Removes a WorldObject from the container if it is there.
	void removeWorldObject(WorldObject *object);
	//Returns the WorldObject with the given name in this Tree if it exists.
	WorldObject *getWorldObject(std::string objName);

	//Draws all the symmetrys in this Tree.
	//When implemented will probably only call all the deepest Symmetrys.
	//The ones without children.
	void draw(ShaderProgram *program);

public:
	//Construct an empty SymmetryTree with the given name.
	SymmetryTree(std::string name);
	//Destroy this SymmetryTree. Does not destroy any RealSymmetrys.
	//All RealSymmetrys have their parent removed.
	~SymmetryTree();

private:
	//Update the deepest symmetrys set/map.
	//FIXME currently assumes that all children of a symmetry
	//are RealSymmetrys and therefore can be casted to that type.
	void updateDeepestSymmetrys();
};