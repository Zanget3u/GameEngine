#pragma once

#include <assimp/Importer.hpp>      //C++ importer interface
#include <assimp/scene.h>           //Output data structure
#include <assimp/postprocess.h>     //Post processing flags
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Data.hpp"

class MeshCreator
{
public:
	static Data* loadFromFile(const char* filepath)
	{
		Data *data = new Data();

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate);
		if (!scene)
		{
			spdlog::error("Failed to load file: {} ", filepath);
			spdlog::error("Error: {}", importer.GetErrorString());
			getchar();
		}
		else
		{
			spdlog::info("File loaded: {}", filepath);
			const aiMesh* mesh = scene->mMeshes[0];

			//Fill vertices positions
			if (mesh->HasPositions())
			{
				data->_vertices.reserve(mesh->mNumVertices);
				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					aiVector3D pos = mesh->mVertices[i];
					data->_vertices.emplace_back(glm::vec3(pos.x, pos.y, pos.z));
					data->_normals.emplace_back(0, 0, 0);
				}
				spdlog::info("Positions retrieved: {}", filepath);
			}
			else
			{
				spdlog::warn("Failed to retrieve positions {}", filepath);
			}

			//Fill vertices texture coordinates
			if (mesh->HasTextureCoords(0))
			{
				data->_texCoords.reserve(mesh->mNumVertices);
				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					//Assume only 1 set of UV coords; AssImp supports 8 UV sets
					aiVector3D UVW = mesh->mTextureCoords[0][i];
					data->_texCoords.emplace_back(glm::vec2(UVW.x, UVW.y));
				}
				spdlog::info("TextureCoords retrieved: {}", filepath);
			}
			else
			{
				spdlog::warn("Failed to retrieve textureCoords {}", filepath);
			}

			//Fill face indices
			if (mesh->HasFaces())
			{
				data->_indices.reserve(3 * mesh->mNumFaces);
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					//Assume the model has only triangles
					data->_indices.emplace_back(glm::uvec3(mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]));
				}
				spdlog::info("Indices retrieved: {}", filepath);
			}
			else
			{
				spdlog::warn("Failed to retrieve indices {}", filepath);
			}

			//Fill vertices normals
			if (mesh->HasNormals())
			{
				data->_normals.clear();
				data->_normals.reserve(mesh->mNumVertices);
				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					aiVector3D n = mesh->mNormals[i];
					data->_normals.emplace_back(glm::vec3(n.x, n.y, n.z));
				}
				spdlog::info("Normals retrieved: {}", filepath);
			}
			else
			{
				spdlog::warn("Failed to retrieve normals {}", filepath);
			}
		}

		return data;
	}
	
	static Data* createPlane(const unsigned int& x, const unsigned int& z, const float& tileSize)
	{
		Data *data = new Data();		
		
		for (int j = 0; j <= z; ++j)
		{
			for (int i = 0; i <= x; ++i)
			{
				float x_pos = (float)i * tileSize;
				float y_pos = 0.0f;
				float z_pos = (float)j * tileSize;
				
				data->_vertices.emplace_back(glm::vec3(x_pos, y_pos, z_pos));
				data->_texCoords.emplace_back(glm::vec2(x_pos / tileSize, z_pos / tileSize));

				if ((j != z) && (i != x))
				{
					const int row1 = j * (x + 1);
					const int row2 = (j + 1) * (x + 1);

					//Triangle 1
					data->_indices.emplace_back(glm::uvec3(row1 + i, row1 + i + 1, row2 + i + 1));

					//Triangle 2
					data->_indices.emplace_back(glm::uvec3(row1 + i, row2 + i + 1, row2 + i));
				}
			}
		}
		
		return data;
	}
};