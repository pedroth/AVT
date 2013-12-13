#include "SymmetryExt.h"

#include <vector>

void SymmetryTree::addSymmetry(RealSymmetry *symmetry)
{
	symmetry->setParent(_container);
	_symmetrys[symmetry->name()] = symmetry;
	updateDeepestSymmetrys();
}
void SymmetryTree::addSymmetry(RealSymmetry *symmetry, RealSymmetry *parent)
{
	symm_map_type::iterator it = _symmetrys.find(parent->name());
	if (it == _symmetrys.end())
		return;
	symmetry->setParent(it->second);
	_symmetrys[symmetry->name()] = symmetry;
	updateDeepestSymmetrys();
}
void SymmetryTree::addSymmetryBetweenParentAndChildren(RealSymmetry *symmetry, RealSymmetry *parent)
{
	symm_map_type::iterator it = _symmetrys.find(parent->name());
	if (it == _symmetrys.end())
		return;
	
	SymmetryI::children_type parentChildren = it->second->getChildren();
	for (SymmetryI::children_type::iterator childIt = parentChildren.begin();
		childIt != parentChildren.end();
		++childIt)
		childIt->second->setParent(symmetry);

	symmetry->setParent(it->second);

	_symmetrys[symmetry->name()] = symmetry;
	updateDeepestSymmetrys();
}
void SymmetryTree::removeSymmetry(RealSymmetry *symmetry)
{
	symm_map_type::iterator it = _symmetrys.find(symmetry->name());
	if (it == _symmetrys.end())
		return;
	it->second->detach();
	_symmetrys.erase(it);
	updateDeepestSymmetrys();
}
RealSymmetry *SymmetryTree::getSymmetry(std::string name)
{
	symm_map_type::iterator it = _symmetrys.find(name);
	if (it == _symmetrys.end())
		return nullptr;
	return it->second;
}
void SymmetryTree::addWorldObject(WorldObject *object)
{
	_container->addWorldObject(object);
}
void SymmetryTree::removeWorldObject(WorldObject *object)
{
	_container->removeWorldObject(object);
}
WorldObject *SymmetryTree::getWorldObject(std::string name)
{
	return _container->getWorldObject(name);
}
void SymmetryTree::draw(ShaderProgram *program)
{
	for (symm_map_type::iterator it = _deepesSymmetrys.begin();
		it != _deepesSymmetrys.end();
		++it)
	{
		it->second->draw(program);
	}
}
SymmetryTree::SymmetryTree(std::string name)
: _name(name), _container(new SymmWorldObjContainer(name + ".Container")),
_symmetrys(), _deepesSymmetrys()
{}
SymmetryTree::~SymmetryTree()
{
	delete _container;
	for (symm_map_type::iterator it = _symmetrys.begin();
		it != _symmetrys.end();
		++it)
	{
		it->second->detach();
	}
}
void SymmetryTree::updateDeepestSymmetrys()
{
	_deepesSymmetrys.clear();
	
	std::vector<RealSymmetry*> _openStack;
	
	const SymmetryI::children_type &containerChildren = _container->getChildren();
	for (SymmetryI::children_type::const_iterator it = containerChildren.begin();
		it != containerChildren.end();
		++it)
	{
		RealSymmetry *child = dynamic_cast<RealSymmetry*>(it->second);
		_openStack.push_back(child);
	}

	while (!_openStack.empty())
	{
		RealSymmetry *current = _openStack.back();
		_openStack.pop_back();
		
		const SymmetryI::children_type &currentChildren = current->getChildren();
		for (SymmetryI::children_type::const_iterator it = currentChildren.begin();
			it != currentChildren.end();
			++it)
		{
			RealSymmetry *child = dynamic_cast<RealSymmetry*>(it->second);
			_openStack.push_back(child);
		}

		if (currentChildren.empty())
			_deepesSymmetrys[current->name()] = current;
	}
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

SymmetryI::SymmetryI(std::string name)
:_name(name), _children(), _parent(nullptr)
{}
SymmetryI::~SymmetryI()
{
	children_type oldChildren = _children;
	for (children_type::iterator it = oldChildren.begin();
		it != oldChildren.end();
		++it)
	{
		it->second->setParent(_parent);
	}
}
void SymmetryI::addChild(SymmetryI *child)
{
	_children[child->name()] = child;
	if (!child->isParent(this))
		child->setParent(this);
}
void SymmetryI::removeChild(SymmetryI *child)
{
	if (!isChild(child))
		return; //nothing to be done

	_children.erase(child->name());
	if (child->isParent(this))
		child->setParent(nullptr);
}
void SymmetryI::setParent(SymmetryI *parent)
{
	SymmetryI *oldParent = _parent;
	_parent = parent;

	if (oldParent != nullptr)
	if (oldParent->isChild(this))
		oldParent->removeChild(this);//this is not a child of old parent anymore
	
	if (parent != nullptr)
	if (!parent->isChild(this))
		parent->addChild(this);
}
void SymmetryI::detach()
{
	SymmetryI *oldParent = _parent;
	//set parent to no one
	_parent = nullptr;
	//remove this from old parent children
	if (oldParent != nullptr)
	if (oldParent->isChild(this))
		oldParent->removeChild(this);
	//reassign children's parent
	children_type oldChildren = _children;
	for (children_type::iterator it = oldChildren.begin();
		it != oldChildren.end();
		++it)
	{
		it->second->setParent(oldParent);
	}
}
const SymmetryI::children_type &SymmetryI::getChildren()
{
	return _children;
}
bool SymmetryI::isChild(SymmetryI *child)
{
	children_type::iterator it = _children.find(child->name());
	return it != _children.end();
}
bool SymmetryI::isParent(SymmetryI *parent)
{
	if (_parent == nullptr)
		return false;
	return _parent->name().compare(parent->name()) == 0;
}
SymmetryI *SymmetryI::parent()
{
	return _parent;
}
std::string SymmetryI::name() const
{
	return _name;
}

/////////////////////////////
/////////////////////////////

SymmWorldObjContainer::SymmWorldObjContainer(std::string name)
: SymmetryI(name), _worldObjects()
{}
void SymmWorldObjContainer::draw(ShaderProgram *program)
{
	for (world_obj_type::iterator it = _worldObjects.begin();
		it != _worldObjects.end();
		++it)
	{
		it->second->draw(program);
	}
}
void SymmWorldObjContainer::addWorldObject(WorldObject *object)
{
	_worldObjects[object->getName()] = object;
}
void SymmWorldObjContainer::removeWorldObject(WorldObject *object)
{
	world_obj_type::iterator it = _worldObjects.find(object->getName());
	if (it == _worldObjects.end())
		return; //nothing to be done
	_worldObjects.erase(it);
}
WorldObject *SymmWorldObjContainer::getWorldObject(std::string name)
{
	world_obj_type::iterator it = _worldObjects.find(name);
	if (it == _worldObjects.end())
		return nullptr;
	return it->second;
}

/////////////////////////////
/////////////////////////////

RealSymmetry::RealSymmetry(std::string name)
: SymmetryI(name)
{}