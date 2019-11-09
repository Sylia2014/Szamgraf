#pragma once
// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <string>

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�

	// transzform�ci�s m�trixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// m�trixok helye a shaderekben
	GLuint	m_loc_mvp; // a h�rom m�trixunk szorzat�t adjuk �t a hat�konys�g �rdek�ben
	GLuint	c_loc_intp_end; //shadernek a szinErtek
	GLuint	f_loc_intp_val; //shadernek interpol�ci�s �rt�k

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 c;
	};

	glm::vec3 GetRandomGombPont();			//kiad a g�mb besej�b�l egy pontot
	glm::vec3 gombPontok[7];				//t�mb a 7 random pontnak, ami alapj�n ki lehet rajzolni 7 okta�dert
	glm::vec3 szinErtek;					//a kiv�lasztott sz�n lesz
	unsigned int elozoIdo = 0;
	int szinInterpolaciosErtek;				//az aktu�lis interpol�ci�s �rt�k, [0-szinvaltasPeriodus] k�z�tti 
	const int szinvaltasPeriodus = 5000;	//5 mp-es peri�dus
	bool gombLenyomva = false;				//1,2,3 gombok
};

