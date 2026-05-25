#pragma once

// vector/matrice types
#include <glm/glm.hpp>

// rotation, translation and perspective helpetrs
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera() {}

	void initCamera(const glm::vec3& pos, float fovDegrees, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->yaw = -90.0f;
		this->pitch = 0.0f;

		updateVectors();

		float fovRadians = glm::radians(fovDegrees);
		projection = glm::perspective(fovRadians, aspect, nearClip, farClip);
	}

	// Getters
	glm::vec3 getPos() const { return this->pos; }

	inline glm::mat4 getViewProjection() const { return projection * glm::lookAt(pos, pos + forward, up); }
	inline glm::mat4 getProjection() const { return projection; }

	inline glm::mat4 getView() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	// Setters
	void setPos(glm::vec3& p)
	{
		pos = p;
	}

	// left and right looking
	void RotateY(float angle)
	{
		yaw -= angle;
		updateVectors();
	}

	// up and down looking
	void Pitch(float angle)
	{
		pitch += angle;

		// limit the pitch angle, prevent from fliping
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		updateVectors();
	}

	// Calculate the movement of the camera, excluding the y axis
	void MoveForward(float amount)
	{
		glm::vec3 flat = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
		pos += flat * amount;
	}

	void MoveBackward(float amount)
	{
		glm::vec3 flat = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
		pos -= flat * amount;
	}

	void MoveRight(float amount)
	{
		glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
		pos += flatRight * amount;
	}

	void MoveLeft(float amount)
	{
		glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
		pos -= flatRight * amount;
	}

private:
	glm::mat4 projection;

	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	float yaw;
	float pitch;

	// used to update the directional vectors
	void updateVectors()
	{
		glm::vec3 fw;
		fw.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		fw.y = sin(glm::radians(pitch));
		fw.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		forward = glm::normalize(fw);
		right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, forward));


	}


};



