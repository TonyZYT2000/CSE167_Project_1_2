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

	GLuint VAO, VBO, EBO;

public:
	Mesh(std::string objFilename);
	~Mesh();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void scale(double factor);
	void rotate(glm::vec3 axis, double angle);
};

#endif
