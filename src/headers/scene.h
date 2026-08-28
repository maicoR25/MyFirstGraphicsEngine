#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "scene_object.h"

class Scene {
public:
	void addObject(std::unique_ptr<SceneObject> object, Shader* shader) {
		SceneObject* objPtr = object.get();
		Shader* targetShader = shader;

		renderBatches[targetShader].push_back(objPtr);
		sceneObjects.push_back(std::move(object));
	}

	void drawScene() {
		for (auto& [currentShader, shaderObjects] : renderBatches) {
			currentShader->use();

			for (SceneObject* obj : shaderObjects) {
				obj->Draw(*currentShader);
			}
		}
	}

	const std::vector<std::unique_ptr<SceneObject>>& getObjects() const {
		return sceneObjects;
	}

	SceneObject* getObjectByID(unsigned int objectID) {
		std::cout << "Reading ID" << std::endl;
		for (auto& obj : sceneObjects) {
			if (objectID == obj->id) {
				return obj.get();
			}
		}
		return nullptr;
	}
private:
	std::unordered_map<Shader*, std::vector<SceneObject*>> renderBatches;
	std::vector<std::unique_ptr<SceneObject>> sceneObjects;
};