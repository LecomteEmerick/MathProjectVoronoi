#include "PointPool.h"



PointPool::PointPool() : CurrentColor(1.0f,1.0f,1.0f)
{
}

void PointPool::Initialize()
{
	this->Shader.LoadVertexShader("basic.vs");
	this->Shader.LoadFragmentShader("basic.fs");
	this->Shader.Create();

	this->ShaderProgram = this->Shader.GetProgram();
}

void PointPool::AddPoint(double x, double y, const glm::mat4& cameraMatrix, const glm::vec4& viewport)
{
	glm::mat4 matInverse = glm::inverse(cameraMatrix);

	glm::vec4 in;

	in[0] = (2.0f*((float)(x - 0) / (viewport[2] - 0))) - 1.0f; //viewport x
	in[1] = 1.0f - (2.0f*((float)(y - 0) / (viewport[3] - 0))); //viewport y
	in[2] = 2.0* 1.0 - 1.0;
	in[3] = 1.0;

	glm::vec4 pos = in * matInverse;
	
	float tmp = pos.w / pos.z;

	Point2D p(pos.x * tmp, pos.y * tmp, this->CurrentColor);

	this->cloudPoint.push_back(p);

	this->ConstructVBO();
}

void PointPool::ConstructEBO()
{
}

void PointPool::ConstructVBO()
{
	glGenBuffers(1, &this->VBO);

	std::vector<float> vertex;
	vertex.reserve(this->cloudPoint.size() * 6);

	for (Point2D p : this->cloudPoint)
	{
		vertex.push_back(p.x);
		vertex.push_back(p.y);
		vertex.push_back(p.z);

		vertex.push_back(p.color.Red);
		vertex.push_back(p.color.Green);
		vertex.push_back(p.color.Blue);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), &vertex.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointPool::Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::mat4& worldTransform, const float& time) const
{
	if (this->cloudPoint.size() < 1)
		return;

	glPointSize(2.5f);

	glUseProgram(this->ShaderProgram);

	GLint projLocation = glGetUniformLocation(this->ShaderProgram, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLint viewLocation = glGetUniformLocation(this->ShaderProgram, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	GLint worldLocation = glGetUniformLocation(this->ShaderProgram, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(worldTransform));

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	GLint positionLocation = glGetAttribLocation(this->ShaderProgram, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	GLint colorLocation = glGetAttribLocation(this->ShaderProgram, "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

	glDrawArrays(GL_POINTS, 0, this->cloudPoint.size());

	glPointSize(1.0f);

	glUseProgram(0);
}

void PointPool::Destroy()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);

	this->Shader.Destroy();
}


PointPool::~PointPool()
{
}
