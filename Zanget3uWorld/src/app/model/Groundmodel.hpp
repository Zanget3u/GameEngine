#pragma once

#include "Basemodel.hpp"
#include "RawData.hpp"
#include <string>

class Groundmodel : public Basemodel
{
private:
	Shader* _shader = nullptr;
	RawData* _data = nullptr;
	unsigned int _texSlot0, _texSlot1, _texSlot2, _texSlot3;
	VertexBuffer *_vbo2 = nullptr, *_vbo3 = nullptr, *_vbo4 = nullptr, *_vbo5 = nullptr;

public:
	Groundmodel(RawData* dataToUse, Shader* shaderToUse, unsigned int textureSlot0, unsigned int textureSlot1, unsigned int textureSlot2, unsigned int textureSlot3)
		: _data(dataToUse), _shader(shaderToUse), _texSlot0(textureSlot0), _texSlot1(textureSlot1), _texSlot2(textureSlot2), _texSlot3(textureSlot3)
	{
		this->initialize();
	}

	~Groundmodel()
	{
		delete _vao;
		delete _vbo1;
		delete _vbo2;
		delete _vbo3;
		delete _vbo4;
		delete _vbo5;
		delete _ib;
	}

	void initialize() override
	{
		//Erstellt und bindet VAO
		_vao = new VertexArray();
		_vao->bind();

		//Erstellt VBO und konfiguriert VAO
		_vbo1 = new VertexBuffer(&_data->_vertices[0], _data->_verticeSize);
		_vao->DefineAttributes(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //Position attribute
		_vbo2 = new VertexBuffer(&_data->_texCoords[0], _data->_texCoordSize);
		_vao->DefineAttributes(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); //Texture attribute
		_vbo3 = new VertexBuffer(&_data->_blendmapCoords[0], _data->_blendmapCoordsSize);
		_vao->DefineAttributes(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); //Map coords
		_vbo4 = new VertexBuffer(&_data->_normals[0], _data->_normalSize);
		_vao->DefineAttributes(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //Normal attribute
		_vbo5 = new VertexBuffer(&_data->_isPicked[0], _data->_isPickedSize);
		_vao->DefineAttributes(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //isPicked attribute
				
		//Erstellt IB
		_ib = new IndexBuffer(&_data->_indices[0], _data->_indiceSize);

		//Unbindet VAO und VBO
		_vbo1->unbind();
		_vbo2->unbind();
		_vbo3->unbind();
		_vbo4->unbind();
		_vbo5->unbind();
		_vao->unbind();
	}

	void draw() override
	{
		_projection = glm::perspective(glm::radians(_camera->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
		_view = _camera->GetViewMatrix();
		_shader->bind();
		_shader->SetUniformMat4f("model", _model);
		_shader->SetUniformMat4f("projection", _projection);
		_shader->SetUniformMat4f("view", _view);
		_shader->SetUniform1i("grassTexture", _texSlot0);
		_shader->SetUniform1i("dirtTexture", _texSlot1);
		_shader->SetUniform1i("stoneTexture", _texSlot2);
		_shader->SetUniform1i("blendmap", _texSlot3);
		_shader->SetUniformVec3("fogColor", glm::vec3(0.611, 0.705, 0.752));
		_shader->SetUniformVec3("lightColor", _lightColor);
		_shader->SetUniformVec3("viewPosition", _camera->Position);

		for(int i = 0; i < numberOfPointlights; i++)
		{
			std::string uniformName = "lightPositions[";
			uniformName += std::to_string(i);
			uniformName += "]";
			_shader->SetUniformVec3(uniformName, _lightPositions[i]);
		}		
		
		_vao->bind();
	}

	void undraw() override
	{
		_shader->unbind();
		_vao->unbind();
	}

	void translate(const glm::vec3& position) override
	{
		_model = glm::translate(_model, position);
	}

	void rotate(const float& angle, const glm::vec3& axis) override
	{
		_model = glm::rotate(_model, glm::radians(angle), axis);
	}

	void scale(const glm::vec3& scalar) override
	{
		_model = glm::scale(_model, scalar);
	}

	unsigned int getNumberOfVertices() override
	{
		return _data->_verticesToRender;
	}

	void updateColorOfPickedVertice() const
	{
		//Binden
		_vao->bind();
		_vbo5->bind();

		//Updaten
		_vbo5->updateData(&_data->_isPicked[0], _data->_isPickedSize);

		//Unbinden
		_vbo5->unbind();
		_vao->unbind();
	}

	void updateHeightOfPickedVertice() const
	{
		//Binden
		_vao->bind();
		_vbo1->bind();

		//Updaten
		_vbo1->updateData(&_data->_vertices[0], _data->_verticeSize);

		//Unbinden
		_vbo1->unbind();
		_vao->unbind();
	}		
};