#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"

class SceneObject {
public:
	const unsigned int id;

	SceneObject(std::shared_ptr<Model> model) : id(nextID++) {
		objectModel = model;
	}

	struct Transform {
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation;
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	} transform;

	void Draw(Shader& shader) {
		shader.setMat4("model", modelMatrix());
		objectModel->Draw(shader);
	}

private:
	std::shared_ptr<Model> objectModel;
	inline static unsigned int nextID = 0;

	glm::mat4 modelMatrix() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.position);
		//model = model * glm::mat4_cast(transform.rotation);
		model = glm::scale(model, transform.scale);
		return model;
	}
};