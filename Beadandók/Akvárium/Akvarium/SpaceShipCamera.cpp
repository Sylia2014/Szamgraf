#include "SpaceShipCamera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="SpaceShipCamera"/> class.
/// </summary>
SpaceShipCamera::SpaceShipCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(16.0f), m_goFw(0), m_goRight(0), m_slow(false)
{
	SetView(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_dist = glm::length(m_at - m_eye);

	SetProj(45.0f, 640 / 480.0f, 0.001f, 1000.0f);
}

SpaceShipCamera::SpaceShipCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_dist(10), m_slow(false)
{
	SetView(_eye, _at, _up);
}

SpaceShipCamera::~SpaceShipCamera(void)
{
}

void SpaceShipCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_eye = _eye;
	m_at = _at;
	m_up = _up;

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));

	m_dist = glm::length(m_at - m_eye);

	m_u = atan2f(m_fw.z, m_fw.x);
	m_v = acosf(m_fw.y);
}

void SpaceShipCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective(_angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 SpaceShipCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void SpaceShipCamera::Update(float _deltaTime)
{
	sebesseg_vektor += gyorsulas_vektor;

	m_eye += sebesseg_vektor;
	m_at += sebesseg_vektor;

	/*m_eye += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;
	m_at += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;*/
	glm::mat3 rotate = glm::mat3(glm::rotate<float>(glm::mat4(1.0), roll_irany, glm::vec3(0, 0, 1)));
	m_viewMatrix = glm::lookAt(m_eye, m_at, rotate * m_up);
	//m_viewMatrix = 
	m_matViewProj = m_matProj * m_viewMatrix;
}

void SpaceShipCamera::UpdateUV(float du, float dv)
{
	m_u += du;
	m_v = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);

	m_at = m_eye + m_dist * glm::vec3(cosf(m_u) * sinf(m_v),
		cosf(m_v),
		sinf(m_u) * sinf(m_v));

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));
}

void SpaceShipCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void SpaceShipCamera::Resize(int _w, int _h)
{
	m_matProj = glm::perspective(45.0f, _w / (float)_h, 0.01f, 1000.0f);

	m_matViewProj = m_matProj * m_viewMatrix;
}

void SpaceShipCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	/*case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (!m_slow)
		{
			m_slow = true;
			m_speed /= 4.0f;
		}
		break;*/
	case SDLK_w:
		gyorsulas_vektor.z -= gyorsulas_konstans;
		break;
	case SDLK_s:
		gyorsulas_vektor.z += gyorsulas_konstans;
		break;
	case SDLK_a:
		gyorsulas_vektor.x -= gyorsulas_konstans;
		break;
	case SDLK_d:
		gyorsulas_vektor.x += gyorsulas_konstans;
		break;
	case SDLK_SPACE:
		gyorsulas_vektor = glm::vec3(0);
		sebesseg_vektor = glm::vec3(0);
		break;
	case SDLK_q:
		roll_irany += -0.05;
		break;
	case SDLK_e:
		roll_irany += 0.05;
		break;
	}
}

void SpaceShipCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	float current_speed = m_speed;
	switch (key.keysym.sym)
	{
	/*case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (m_slow)
		{
			m_slow = false;
			m_speed *= 4.0f;
		}
		break;*/
	case SDLK_w:
	case SDLK_s:
		m_goFw = 0;
		break;
	case SDLK_a:
	case SDLK_d:
		m_goRight = 0;
		break;
	case SDLK_q:
	case SDLK_e:
		break;
	}
}

void SpaceShipCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	/*if (mouse.state & SDL_BUTTON_LMASK)
	{
		UpdateUV(mouse.xrel / 100.0f, mouse.yrel / 100.0f);
	}*/
}

void SpaceShipCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

