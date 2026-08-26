#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"

class SceneObject {
public: 
	std::shared_ptr<Model> objectModel;

	struct Transform {
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	} transform;

	void Draw(Shader* shader) {

	}

private:
	glm::mat4 modelMatrix() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.position);
		model = model * glm::mat4_cast(transform.rotation);
		model = glm::scale(model, transform.scale);
		return model;
	}
};