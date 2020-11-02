#include "Mesh.h"

Mesh::Mesh(std::string objFilename)
{
	// parsing vertex, vertex normal and faces
	std::ifstream objFile(objFilename);

	if (objFile.is_open()) {
		std::string line;
		while (std::getline(objFile, line)) {
                  std::stringstream ss;
                  ss << line;

			// get the label
                  std::string label;
                  ss >> label;

			// line is vertex
                  if (label == "v") {
				// write position to a vec3 and add to points vector
                        glm::vec3 vertex;
                        ss >> vertex.x >> vertex.y >> vertex.z;
                        points.push_back(vertex);
			}
			// line is vertex normal
			else if (label == "vn") {
				// write normal data to a vec3 and push to vnormals vector
				glm::vec3 vnormal;
				ss >> vnormal.x >> vnormal.y >> vnormal.z;
				vnormals.push_back(vnormal);
			}
			// lien is face
			else if (label == "f") {
				glm::ivec3 face;
				string index1;
				string index2;
				string index3;
				ss >> index1 >> index2 >> index3;
				index1 = index1.substr(0, index1.find('/'));
				index2 = index2.substr(0, index2.find('/'));
				index3 = index3.substr(0, index3.find('/'));
				face.x = std::stoi(index1) - 1;
				face.y = std::stoi(index2) - 1;
				face.z = std::stoi(index3) - 1;
				faces.push_back(face);
			}
		}
	}
	else {
		std::cerr << "Can't open the file: " << objFilename << std::endl;
	}

	// record max and min on 3 dimensions
	GLfloat xMax = points[0].x;
	GLfloat xMin = points[0].x;
	GLfloat yMax = points[0].y;
	GLfloat yMin = points[0].y;
	GLfloat zMax = points[0].z;
	GLfloat zMin = points[0].z;

	// iterate through all points, record max and min of 3 dimensions
	for (const auto &vertex : points) {
		if (vertex.x > xMax) {
			xMax = vertex.x;
		}
		if (vertex.x < xMin) {
			xMin = vertex.x;
		}
		if (vertex.y > yMax) {
			yMax = vertex.y;
		}
		if (vertex.y < yMin) {
			yMin = vertex.y;
		}
		if (vertex.z > zMax) {
			zMax = vertex.z;
		}
		if (vertex.z < zMin) {
			zMin = vertex.z;
		}
	}
	
	// calculate center coordinate
	auto center = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);

	// record the maximum distance from the center
	GLfloat distMax = 0;
	for (const auto &vertex : points) {
		GLfloat dist = glm::length(vertex - center);
		if (dist > distMax) {
			distMax = dist;
		}
	}
	// calculate the scale factor, normalizing maximum distance to 9
	GLfloat scaleFactor = 9.0 / distMax;

	// create the transformation matrix
	auto transform = glm::mat4(1);

	// translate by scale factor times the negative value of the center coordinate
	// scale by scale factor
	transform = glm::translate(transform, - scaleFactor * center);
	transform = glm::scale(transform, glm::vec3(scaleFactor));

	// Set the model matrix to the transformation matrix
	model = transform;

	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cerr << "Finish loading " << objFilename << std::endl;
}

Mesh::~Mesh() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::update() {} 

void  Mesh::scale(double factor) {
	// scale bigger if factor > 0, otherwise sacle smaller
	double scaleFactor = factor > 0 ? 1.1 : 0.9;
	model = glm::scale(glm::mat4(1), glm::vec3(scaleFactor)) * model;
}
