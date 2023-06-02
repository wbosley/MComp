#pragma once
#include "OBJLoader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "openvr.h"
class Model3D
{

	//constructor and destructor
	public:
		Model3D();
		~Model3D();

		enum RENDER_TYPE
		{
			NO_TEXTURES, TEXTURES, COLOUR, TEXTURES_PRECOMPILED
		};

		//will return 1 or zero depending on failure or success
		int createQuadColour(glm::vec3 colour);
		int createQuad(GLuint textureId, bool is3D);
		int createLine(glm::vec3 start, glm::vec3 end);
		int loadOpenVRModel(vr::RenderModel_t* vr_model, vr::RenderModel_TextureMap_t* vr_texture);
		int loadModelFromObj(OBJLoader obj);
		int addModelToMesh(Model3D model);
		int compileModel();
		int render(GLuint myShader);
		int changeColour(glm::vec3 colour);
		std::vector<glm::vec3>* getVertices();
		glm::mat4 ModelMatrix;

		bool isModelCompiled;
		

	protected:
		//these are in instance variables for a threedeeobjects
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		std::vector<unsigned int> vertices_indices;
		std::vector<unsigned int> uvs_indices;
		std::vector<unsigned int> normals_indices;

		std::vector<glm::vec3> colours;

		const uint8_t * textureData;
		uint16_t width;
		uint16_t height;

		GLuint texture;

		GLuint VAO;
		GLuint* VBOs;

		RENDER_TYPE render_type;

		void updateBufferData();

		int putModelDataInVbosAndVaos(RENDER_TYPE renderMode);
		

};

