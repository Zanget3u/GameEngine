#pragma once

#include <map>
#include "Texture.hpp"
#include "Shader.hpp"
#include "Data.hpp"
#include "DataLoader.hpp"

namespace ResourceManager
{
	//------------------------ Texture ------------------------ 
	
	//Storing
	static std::map<std::string, Texture*> _textures;

	//Loading
	static void LoadTexture(const char* Filepath, const std::string& name)
	{
		_textures[name] = new Texture(Filepath);		
	}

	//Retrieving
	static Texture* GetTexture(const std::string& name)
	{
		return _textures[name];
	}

	//Deleting
	static void DeleteTextures()
	{
		for (auto const& tex : _textures)
			delete tex.second;
	}

	//------------------------ Shader ------------------------ 
	
	//Storing
	static std::map<std::string, Shader*> _shaders;
	
	//Loading
	static void LoadShader(const std::string& vs_Filepath, const std::string& fs_Filepath, const std::string& name)
	{
		_shaders[name] = new Shader(vs_Filepath, fs_Filepath);
	}
	
	//Retrieving
	static Shader* GetShader(const std::string& name)
	{
		return _shaders[name];
	}
		
	//Deleting
	static void DeleteShaders()
	{
		for (auto const& shader : _shaders)
			delete shader.second;
	}
	
	//------------------------ Data ------------------------ 

	//Storing
	static std::map<std::string, Data*> _data;
	
	//Loading
	static DataLoader Loader;
	static void LoadData(const std::string& data_Filepath, const std::string& name)
	{
		Data* d = new Data();
		Loader.retrieve(data_Filepath.c_str(), d);
		_data[name] = d;
	}
	
	//Retrieving
	static Data* GetData(const std::string& name)
	{
		return _data[name];
	}
	
	//Deleting
	static void DeleteData()
	{
		for (auto const& data : _data)
			delete data.second;
	}
}