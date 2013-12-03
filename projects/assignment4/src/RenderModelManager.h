#pragma once

#include <string>
#include <map>

#include "RenderModel.h"

//A singleton responsible for keeping all created RenderModels.
class RenderModelManager {
	static RenderModelManager *_instance;

	typedef std::map<std::string, RenderModel *>  rendermodels_type;
	rendermodels_type _renderModels;

	void destroyModels();
public:
	static RenderModelManager *instance();

	//if there was already a rendermodel with that name it might be lost.
	void addRenderModel(std::string name, RenderModel *model);
	RenderModel *getRenderModel(const std::string &name);

	~RenderModelManager();
};