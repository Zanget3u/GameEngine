#pragma once

#include "Standardmodel.hpp"
#include "GroundData.hpp"
#include "AudioManager.hpp"

enum Player_Movement
{
	P_FORWARD,
	P_BACKWARD,
	P_LEFT,
	P_RIGHT
};

//Default Playervalues
const float P_SPEED = 10.0f;
const float P_TURNSPEED = 200.0f;

class Player
{
private:	
	float _currentSpeed = 0;
	float _currentTurnSpeed = 0;	
	float new_height = 0, old_height = 0;
	GroundData *_ground_data = nullptr;
	AudioManager *_audioManager = nullptr;
	
public:
	Standardmodel* _playerModel;
	float _yaw = 0.0f;
	glm::vec3 _front, _playerPosition;

	Player(Standardmodel* player, glm::vec3 player_position, GroundData* ground_data, AudioManager* audio_manager)
		: _playerModel(player), _playerPosition(player_position), _ground_data(ground_data), _audioManager(audio_manager)
	{
		_playerModel->translate(_playerPosition);
	}

	void ProcessKeyboard(Player_Movement direction, float deltaTime)
	{		
		if (direction == P_FORWARD) 
		{
			_currentSpeed = P_SPEED;
		}
		else if (direction == P_BACKWARD) 
		{
			_currentSpeed = -P_SPEED;
		}
		else
		{
			_currentSpeed = 0;
		}

		if (direction == P_LEFT) 
		{
			_currentTurnSpeed = P_TURNSPEED;
		}
		else if (direction == P_RIGHT) 
		{
			_currentTurnSpeed = -P_TURNSPEED;			
		}
		else
		{
			_currentTurnSpeed = 0;
		}

		//Calculate direction and rotate player
		float angle = _currentTurnSpeed * deltaTime;		
		_playerModel->rotate(angle, glm::vec3(0, 1, 0));
		_yaw += angle;
		if(_yaw >= 360.0f)
		{
			_yaw = 0.0f;
		}
		if(_yaw <= -360.0f)
		{
			_yaw = 0.0f;
		}

		//Calculate dx/dz and keep track of player position
		float velocity = _currentSpeed * deltaTime;
		glm::vec3 front;
		front.x = glm::sin(glm::radians(_yaw));
		front.y = 0.0f;
		front.z = glm::cos(glm::radians(_yaw));				
		_front = glm::normalize(front) * velocity;
		_playerPosition += _front;

		//Calculate new height (y-value)
		old_height = _playerPosition.y;
		new_height = _ground_data->getHeightValueUnbuffered(_playerPosition.x, _playerPosition.z);
		_playerPosition.y = new_height;

		//Translate the player
		float dx = velocity * glm::sin(glm::radians(angle));
		float dz = velocity * glm::cos(glm::radians(angle));
		float dy = new_height - old_height;		
		_playerModel->translate(glm::vec3(dx, dy, dz));
		

		if(_audioManager->isPlaying("../res/audio/sounds/Step.wav"))
		{
			
		}
		else
		{
			_audioManager->playSound3D("../res/audio/sounds/Step.wav", &_playerPosition);
		}
	}
};