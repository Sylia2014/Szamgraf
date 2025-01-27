#include "MyApp.h"
#include "GLUtils.hpp"
#include <ctime>

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_ibID = 0;

	m_programID = 0;
}


CMyApp::~CMyApp(void)
{
}

int get_random_int(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

glm::vec3 CMyApp::get_random_pont() {
	int x = get_random_int(0, 7);
	int z = get_random_int(0, 7);
	float y = (x / 2) ^ 3 - x * z / 4 + (z / 2) ^ 2;
	return glm::vec3(hasznalhato_pontok[x], y, hasznalhato_pontok[z]);
}

bool CMyApp::Init()
{
	srand(time(0));
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		//          x,  y, z             R, G, B
		{glm::vec3(0,0,0), glm::vec3(0,0,0)},		//0
		{glm::vec3(0,1,0), glm::vec3(0,1,0)},		//1
		{glm::vec3(1,0,0), glm::vec3(1,0,0)},		//2
		{glm::vec3(1,1,0), glm::vec3(1,1,0)},		//3
		{glm::vec3(1,0,1), glm::vec3(1,0,1)},		//4
		{glm::vec3(1,1,1), glm::vec3(1,1,1)},		//5
		{glm::vec3(0,0,1), glm::vec3(0,0,1)},		//6
		{glm::vec3(0,1,1), glm::vec3(0,1,1)}		//7
	};

	// indexpuffer adatai
    GLushort indices[]=
    {
		//FRONT
        0,3,2,
        3,0,1,
		//RIGHT
		4,2,5,
		2,3,5,
		//BACK
		4,5,6,
		5,7,6,
		//LEFT
		6,7,0,
		7,1,0,
		//TOP
		1,7,3,
		7,5,3,
		//BOTTOM
		4,6,0,
		4,0,2
    };

	for (int i = 0; i < 9; i++) {
		pontok[i] = get_random_pont();

		std::cout << pontok[i].x << ", " << pontok[i].y << ", " << pontok[i].z << std::endl;
	}

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  sizeof(vert),		// ennyi b�jt nagys�gban
				  vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
				  GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat
	

	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	); 

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
							0,				// a VAO-beli azonos�t� index
							"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egy�b inicializ�l�s
	//

	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// n�zeti transzform�ci� be�ll�t�sa
	float t = SDL_GetTicks() / 1000.0f;
	//m_matView = glm::lookAt(glm::vec3(5 * cosf(t), 5, 5 * sinf(t)),	// honnan n�zz�k a sz�nteret
	m_matView = glm::lookAt(glm::vec3( 20,  20,  20),		// honnan n�zz�k a sz�nteret
							glm::vec3( 0,  0,  0),		// a sz�nt�r melyik pontj�t n�zz�k
							glm::vec3( 0,  1,  0));		// felfel� mutat� ir�ny a vil�gban
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	// shader parameterek be�ll�t�sa
	/*

	GLM transzform�ci�s m�trixokra p�ld�k:
		glm::rotate<float>( sz�g, glm::vec3(tengely_x, tengely_y, tengely_z) ) <- tengely_{xyz} k�r�li elforgat�s
		glm::translate<float>( glm::vec3(eltol_x, eltol_y, eltol_z) ) <- eltol�s
		glm::scale<float>( glm::vec3(s_x, s_y, s_z) ) <- l�pt�kez�s

	*/

	float time;

	std::vector<glm::vec3> kocka_eltolas = std::vector<glm::vec3>{
		glm::vec3(-1,0,0),
		glm::vec3(0,0,0),
		glm::vec3(1,0,0),
		glm::vec3(0,-1,0),
		glm::vec3(0,-2,0)
	};

	if (space_lenyomva % 2 == 1) {
		//forgat�s
		time = SDL_GetTicks() / 1000.0f * 2 * float(M_PI) / 10;
		rotate_valtozo = glm::vec3(1, 0, 0);
	}
	else {
		//meg�ll�t�s
		time = 0;
		rotate_valtozo = glm::vec3(1, 0, 0);
	}

	for(int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 5; j++) {
			m_matWorld =
				glm::translate<float>(pontok[i])*
				glm::translate<float>(kocka_eltolas[j]) *
				glm::rotate<float>(time, rotate_valtozo);

			glm::mat4 mvp = m_matProj * m_matView * m_matWorld;

			// majd k�ldj�k �t a megfelel� m�trixot!
			glUniformMatrix4fv(m_loc_mvp,// erre a helyre t�lts�nk �t adatot
				1,			// egy darab m�trixot
				GL_FALSE,	// NEM transzpon�lva
				&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

			// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
			glBindVertexArray(m_vaoID);

			// kirajzol�s
			glDrawElements(GL_TRIANGLES,		// primit�v t�pus
				36,					// hany csucspontot hasznalunk a kirajzolashoz
				GL_UNSIGNED_SHORT,	// indexek tipusa
				0);					// indexek cime
		}
	}
	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) {
		space_lenyomva++;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}