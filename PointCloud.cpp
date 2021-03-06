#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	std::ifstream objFile(objFilename);

	if (objFile.is_open()) {
		std::string line;
		while (std::getline(objFile, line)) {
                  std::stringstream ss;
                  ss << line;

                  std::string label;
                  ss >> label;

                  if (label == "v") {
				// write position to a vec3 and add to points vector
                        glm::vec3 vertex;
                        ss >> vertex.x >> vertex.y >> vertex.z;
                        points.push_back(vertex);
                  }
		}
	}
	else {
		std::cerr << "Can't open the file: " << objFilename << std::endl;
	}

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
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

	// Set point size
	glPointSize(pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, points.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree
	spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	pointSize = (pointSize + size) > 0 ? (pointSize + size) : 0;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
