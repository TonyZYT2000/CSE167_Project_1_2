#ifndef _MESH_H_
#define _MESH_H_

#include "Object.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Mesh : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> vnormals;
	std::vector<glm::ivec3> faces;
	glm::vec3 kAmbient;
	glm::vec3 kSpecular;

	GLuint VAO, VBO, NBO, EBO;

public:
	Mesh(std::string objFilename, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	~Mesh();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void scale(double factor);
	void scalebyScroll(double offset);
	void rotate(glm::vec3 axis, float angle);
	void translate(glm::vec3 trans);
};

#endif
