/*
STÓŁ TROCHĘ DO GÓRY I PODŁOGA TEŻ BO NIE DOTYKA AKWARIUM 
czy da się uzależnić wszystko od akwarium? przesuniecie akwarium = przesuniecie wszystkiego 
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "Camera.h"
#include <ctime>   


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

float speed_x = 0;
float speed_y = 0;
float speed = 1.5;

float aspectRatio = 1;

ShaderProgram* sp;


ShaderProgram* light;
ShaderProgram* glass;
ShaderProgram* water;
ShaderProgram* sand;
ShaderProgram* sp_floor;
ShaderProgram* light_cube;
ShaderProgram* outside;


//Odkomentuj, żeby rysować kostkę
float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;





#define FISH 0
#define TANK 1
#define ROCK1 2
#define ROCK2 3
#define ROCK3 4
#define ROCK4 5
#define ROCK5 6
#define ROCK6 7
#define AKWAR 8
#define FISH1 9
#define NEMO 10
#define PLANTS 11
#define PLANTS1 12	
#define TABLE 13 
#define PUSTYNIA 14
#define NEW_ROCK 15
#define NEW_ROCK2 16
#define GRASS_1 17
#define LEAF 18

#define TEX_FISH 0
#define TEX_TANK 1
#define TEX_ROCK1 2 
#define TEX_ROCK2 3 
#define TEX_ROCK3 4 
#define TEX_ROCK4 5 
#define TEX_ROCK5 6 
#define TEX_ROCK6 7 
#define TEX_ROCK7 8 
#define TEX_BOTTOM 9
#define TEX_FISH2 10
#define TEX_FISH3 11
#define TEX_PLANT 12
#define TEX_FISH4 13
#define TEX_ALUMINIUM 14
#define TEX_TIGER 15
#define TEX_FUR 16
#define TEX_SAND 17
#define TEX_SAND_LIGHT 18
#define TEX_GRASS_GREEN 19
#define TEX_GRASS_BLUE 20
#define TEX_GRASS_RED 21
#define TEX_FLOOR 22
#define TEX_LEAF 23




// ------------- CAMERA ---------------

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;


// camera


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float  lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;




// ---------------- Dekleracja funkcji  ---------------- 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void error_callback(int error, const char* description);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void loadModel(std::string filename, int model_i);
void readTexture(const char* filename, int tex_i);
void drawGlass(glm::mat4 P, glm::mat4 V, glm::mat4 M); void drawButtom(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture);
void drawRocks(glm::mat4 P, glm::mat4 V);
void drawPlants(glm::mat4 P, glm::mat4 V);
void drawSand(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2);
void drawModelNew(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2, glm::vec3 color, float shiness = 32.0f);
void drawLightCube(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTable(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2);
void drawTableNew(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2, float shiness);

void texKostka(glm::mat4 P2, glm::mat4 V2, glm::mat4 M2);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_callback1(GLFWwindow* window, double xposIn, double yposIn);
void initOpenGLProgram(GLFWwindow* window);
void freeOpenGLProgram(GLFWwindow* window);
void drawTank(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawGreenPlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle);
void drawRedPlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle);
void drawStones(glm::mat4 P, glm::mat4 V, float shiness);
void drawFloor(glm::mat4 P2, glm::mat4 V2, glm::mat4 M2, float angle_x, float angle_y);
void drawBluePlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle);
void drawFish(glm::mat4 P, glm::mat4 V, glm::mat4 M, float angle_x, float angle_y, float angle);
void drawMultipleLeafs(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle);
void drawScene(GLFWwindow* window, float angle_x, float angle_y, float angle);

// Structy do wgyrwania modeli

struct MyVertex {
	std::vector<glm::vec4> Vertices;
	std::vector<glm::vec4> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<unsigned int> Indices;
};

std::vector<MyVertex> models;
vector<GLuint> texs;

//Odkomentuj, żeby rysować czajnik
float* cubeVertices = myCubeVertices;
float* cubeNormals = myCubeNormals;
float* cubeTexCoords = myCubeTexCoords;
float* cubeColors = myCubeColors;
int cubeVertexCount = myCubeVertexCount;

GLuint tex0;
GLuint tex1;
GLuint tex2;


glm::vec4 light1 = glm::vec4(-2.5, 6, 0, 1);
glm::vec4 light2 = glm::vec4(2.5, 6, 0, 1);





int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	//glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	glfwSetCursorPosCallback(window, mouse_callback1);
	glfwSetScrollCallback(window, scroll_callback);

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x = 0; //Aktualny kąt obrotu obiektu
	float angle_y = 0; //Aktualny kąt obrotu obiektu
	float angle = 0;

	float lastTime = 0;

	glfwSetTime(0); //Zeruj timer


	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float currentTime = static_cast<float>(glfwGetTime());

		// per-frame time logic
// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);


		angle += speed * deltaTime;
		angle_x += speed_x * deltaTime; //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		angle_y += speed_y * deltaTime; //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		drawScene(window, angle_x, angle_y, angle); //Wykonaj procedurę rysującą
		lastTime = static_cast<float>(glfwGetTime());
		//glfwSetTime(0); //Zeruj timer


		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}


// ----------------------- FUNKCJE OPENGL I INIT -----------------------


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_x = -PI / 2;
		if (key == GLFW_KEY_RIGHT) speed_x = PI / 2;
		if (key == GLFW_KEY_UP) speed_y = PI / 2;
		if (key == GLFW_KEY_DOWN) speed_y = -PI / 2;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_x = 0;
		if (key == GLFW_KEY_RIGHT) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_y = 0;
		if (key == GLFW_KEY_DOWN) speed_y = 0;
	}
}



void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void loadTextures(const char* filename, int tex_i) {
	glActiveTexture(GL_TEXTURE0);
	GLuint tex;
	texs.push_back(tex);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texs[tex_i] = tex;
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}


void loadModel(string filename, int model_i) {
	models.push_back(MyVertex());
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	//cout << importer.GetErrorString() << endl;

	aiMesh* mesh = scene->mMeshes[0];
	for (int i = 0; i < mesh->mNumVertices; i++) {

		aiVector3D vertex = mesh->mVertices[i];
		models[model_i].Vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		models[model_i].Normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		unsigned int liczba_zest = mesh->GetNumUVChannels();
		unsigned int wymiar_wsp_tex = mesh->mNumUVComponents[0];

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		models[model_i].TexCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			models[model_i].Indices.push_back(face.mIndices[j]);
		}
	}
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.65f, 0.25f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	//glfwSetKeyCallback(window,keyCallback);


	//glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	loadTextures("./img/fish.png", TEX_FISH);
	loadTextures("./img/tank2.png", TEX_TANK);
	loadTextures("./img/rock0.png", TEX_ROCK1);
	loadTextures("./img/rock1.png", TEX_ROCK2);
	loadTextures("./img/rock2.png", TEX_ROCK3);
	loadTextures("./img/rock3.png", TEX_ROCK4);
	loadTextures("./img/rock4.png", TEX_ROCK5);
	loadTextures("./img/rock5.png", TEX_ROCK6);
	loadTextures("./img/rock6.png", TEX_ROCK7);
	loadTextures("./img/sand.png", TEX_BOTTOM);
	loadTextures("./img/fish2.png", TEX_FISH2);
	loadTextures("./img/fish3.png", TEX_FISH3);
	loadTextures("./img/lisc.png", TEX_PLANT);
	loadTextures("./img/Helena.png", TEX_FISH4);
	loadTextures("./img/metal2.png", TEX_ALUMINIUM);
	loadTextures("./img/tiger.png", TEX_TIGER);
	loadTextures("./img/fur.png", TEX_FUR);
	loadTextures("./img/sand_new.png", TEX_SAND);
	loadTextures("./img/sand_ligth.png", TEX_SAND_LIGHT);
	loadTextures("./img/Grass.png", TEX_GRASS_GREEN);
	loadTextures("./img/Grass_blue.png", TEX_GRASS_BLUE);
	loadTextures("./img/Grass_red.png", TEX_GRASS_RED);
	loadTextures("./img/gepart.png", TEX_FLOOR);
	loadTextures("./img/gepart.png", TEX_FLOOR);
	loadTextures("./img/leaf.png", TEX_LEAF);

	loadModel(std::string("models/fish.obj"), FISH);
	loadModel(std::string("models/tank.fbx"), TANK);
	loadModel(std::string("models/Rock0.obj"), ROCK1);
	loadModel(std::string("models/Rock1.fbx"), ROCK2);
	loadModel(std::string("models/Rock2.fbx"), ROCK3);
	loadModel(std::string("models/Rock3.fbx"), ROCK4);
	loadModel(std::string("models/Rock4.fbx"), ROCK5);
	loadModel(std::string("models/Rock5.fbx"), ROCK6);
	loadModel(std::string("models/akwarium.obj"), AKWAR);
	loadModel(std::string("models/fish_h.obj"), FISH1);
	loadModel(std::string("models/nemo.obj"), NEMO);
	loadModel(std::string("models/plants.obj"), PLANTS);
	loadModel(std::string("models/plants1.obj"), PLANTS1);
	loadModel(std::string("models/Table.obj"), TABLE);
	loadModel(std::string("models/pustynia.obj"), PUSTYNIA);
	loadModel(std::string("models/rock_new1.obj"), NEW_ROCK);
	loadModel(std::string("models/Free rock/Rock_1.obj"), NEW_ROCK2);
	loadModel(std::string("models/Grass/High Grass.obj"), GRASS_1);
	loadModel(std::string("models/leaf.obj"), LEAF);

	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	glass = new ShaderProgram("v_glass.glsl", NULL, "f_glass.glsl");
	water = new ShaderProgram("v_water.glsl", NULL, "f_water.glsl");
	light = new ShaderProgram("v_phong.glsl", NULL, "f_phong.glsl");
	sand = new ShaderProgram("new_v.glsl", NULL, "new_f.glsl");
	sp_floor = new ShaderProgram("v_floor.glsl", NULL, "f_floor.glsl");
	light_cube = new ShaderProgram("light_cube_v.glsl", NULL, "light_cube_f.glsl");
	outside = new ShaderProgram("outside_v.glsl", NULL, "outside_f.glsl");

	tex0 = texs[TEX_FUR];
	tex2 = texs[TEX_FLOOR];
	tex1 = texs[TEX_PLANT];
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {

	delete sp;

	glDeleteTextures(1, &tex0);
}

void mouse_callback1(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
		cout << "trzymam W" << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}



// ----------- RYSOWANIE ----------------------

// GŁÓWNA FUNKCJA

void drawScene(GLFWwindow* window, float angle_x, float angle_y, float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 V = camera.GetViewMatrix();
	glm::mat4 P = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


	drawFish(P, V, M, angle_x, angle_y, angle);


	//stol
	glm::mat4 MT = glm::mat4(1.0f);
	MT = glm::translate(MT, glm::vec3(0.0f, -23.5f, 0.0f));
	MT = glm::scale(MT, glm::vec3(0.18f, 0.18f, 0.18f));
	drawTableNew(P, V, MT, TABLE, TEX_TANK, TEX_TANK, 5.0f);

	//drawModelNew(P, V, MT, TABLE, TANK, TANK, glm::vec3(0.01f, 0.00f, 1.00f));

	glm::mat4 Mb = M;
	Mb = glm::translate(M, glm::vec3(0.0f, -7.0f, 0.0f));
	Mb = glm::scale(Mb, glm::vec3(0.85f, 0.7f, 0.85f));

	drawModelNew(P, V, Mb, PUSTYNIA, TEX_SAND, TEX_SAND_LIGHT, glm::vec3(0.01f, 0.00f, 1.00f));

	//drawSand(P, V, Mb, PUSTYNIA, TEX_SAND, TEX_SAND_LIGHT);
	//drawButtom(P, V, M);

	// Kolorwe rolsinki
	drawRedPlants(P, V, angle_x, angle_y, angle);
	drawGreenPlants(P, V, angle_x, angle_y, angle);
	drawBluePlants(P, V, angle_x, angle_y, angle);
	drawMultipleLeafs(P, V, angle_x, angle_y, angle);
	// Kamienie
	drawStones(P, V, 8.0f);

	glm::mat4 light_pos = glm::mat4(1.0f);
	light_pos = glm::translate(light_pos, glm::vec3(10.0f, 0.0f, 5.0f));
	light_pos = glm::scale(light_pos, glm::vec3(0.25f, 0.25f, 0.25f));
	drawLightCube(P, V, light_pos);

	M = glm::rotate(M, -PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Mt = glm::rotate(M, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::scale(Mt, glm::vec3(6.0f, 6.0f, 6.0f));
	drawTableNew(P, V, Mt, TANK, TEX_ALUMINIUM, TEX_ALUMINIUM, 64.0f);

	//drawRocks(P, V);
	//drawPlants(P, V);

	//podloga 
	glm::mat4 M2 = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	drawFloor(P, V, M2, angle_x, angle_y);

	//szklo 
	glm::mat4 Mg = glm::scale(M, glm::vec3(8.0f, 6.0f, 6.0f));
	drawGlass(P, V, Mg);

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


// POMOCNICZE

void drawFish(glm::mat4 P, glm::mat4 V, glm::mat4 M, float angle_x, float angle_y, float angle) {

	//====================================================================================================================
		// Fish 1 - rybka zamulajaca na gorze 
	glm::mat4 Mf10 = glm::rotate(M, glm::radians(angle * 1.6f), glm::vec3(0.0f, 1.0f, 0.0f));
	Mf10 = glm::translate(Mf10, glm::vec3(-2.1f, -2.4f, -3.1f));
	Mf10 = glm::scale(Mf10, glm::vec3(0.7f, 0.7f, 0.7f));

	Mf10 = glm::rotate(Mf10, glm::radians(angle * 0.05f), glm::vec3(0.0f, 0.5f, 0.0f));

	float swimOffsetX = sin(angle * 0.75) * 1.0f;
	float swimOffsetY = cos(angle * 0.5f) * 0.5f;
	float swimOffsetZ = cos(angle * 0.84f) * 1.0f;
	Mf10 = glm::translate(Mf10, glm::vec3(swimOffsetX, swimOffsetY, swimOffsetZ));

	//drawModel(P, V, Mf10, NEMO, TEX_FISH2);
	drawModelNew(P, V, Mf10, NEMO, TEX_FISH2, TEX_FISH2, glm::vec3(0.01f, 0.00f, 1.00f));

	//====================================================================================================================
	// Fish 2 -  mniejsza rybka 
	glm::mat4 Mf11 = glm::rotate(M, -angle * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mf11 = glm::translate(Mf11, glm::vec3(0.0f, 0.0f, -3.5f));
	Mf11 = glm::scale(Mf11, glm::vec3(0.5f, 0.5f, 0.5f));

	float waveOffsetX = sin(angle * 0.5f) * 1.0f;
	float waveOffsetY = cos(angle * 0.4f) * 0.3f;
	float waveOffsetZ = sin(angle * 0.3f) * 0.5f;
	Mf11 = glm::translate(Mf11, glm::vec3(waveOffsetX, waveOffsetY, waveOffsetZ));
	Mf11 = glm::rotate(Mf11, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -3.5f));

	//drawModel(P, V, Mf11, FISH, TEX_FISH); 
	drawModelNew(P, V, Mf11, FISH, TEX_FISH, TEX_FISH, glm::vec3(0.01f, 0.00f, 1.00f));

	//====================================================================================================================
	// Fish 3
	/*
	glm::mat4 Mf12 = glm::rotate(M, angle * 0.4f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mf12 = glm::translate(Mf12, glm::vec3(-3.0f, 0.0f, -2.7f));
	Mf12 = glm::scale(Mf12, glm::vec3(0.8f, 0.8f, 0.8f));

	float circleRadius = 0.5f;
	float circleAngle = angle * 0.4f;

	float circleX = sin(circleAngle) * circleRadius;
	float circleY = cos(circleAngle) * circleRadius;
	float circleZ = 0.0f;  /

	Mf12 = glm::translate(Mf12, glm::vec3(circleX, circleY, circleZ));

	Mf12 = glm::rotate(Mf12, glm::radians(180.0f), glm::vec3(1.0f, -0.8f, 0.0f));

	drawModel(P, V, Mf12, FISH, TEX_FISH);
	*/

	//====================================================================================================================
	//Fish 4 - Nemo 2 shaking wolniejsza
	glm::mat4 Mf13 = glm::rotate(M, -angle * 0.4f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mf13 = glm::translate(Mf13, glm::vec3(-2.0f, -6.2f, -1.7f));
	Mf13 = glm::scale(Mf13, glm::vec3(0.5f, 0.5f, 0.5f));
	float ellipseRadiusX = 0.20f;
	float ellipseRadiusZ = 0.10f;
	float ellipseAngle = angle * 0.4f;
	float ellipseX = ellipseRadiusX * cos(ellipseAngle);
	float ellipseZ = ellipseRadiusZ * sin(ellipseAngle);
	Mf13 = glm::translate(Mf13, glm::vec3(ellipseX, 0.0f, ellipseZ));

	float verticalOffset = 0.1f * sin(angle);
	Mf13 = glm::translate(Mf13, glm::vec3(0.0f, verticalOffset, 0.0f));

	float bodyRotation = 10.0f * sin(angle);
	Mf13 = glm::rotate(Mf13, glm::radians(bodyRotation), glm::vec3(0.0f, 1.0f, 0.0f));

	float tailAngle = 20.0f * sin(angle);
	Mf13 = glm::rotate(Mf13, glm::radians(tailAngle), glm::vec3(0.0f, 0.0f, 1.0f));

	float finAngle = 5.0f * sin(3 * angle + 0.5);
	Mf13 = glm::rotate(Mf13, glm::radians(finAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	//drawModel(P, V, Mf13, NEMO, TEX_FISH2); 
	drawModelNew(P, V, Mf13, NEMO, TEX_FISH2, TEX_FISH2, glm::vec3(0.01f, 0.00f, 1.00f));

	//====================================================================================================================
	// Fish 5  szybsza
	glm::mat4 Mf14 = glm::rotate(M, angle * 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mf14 = glm::translate(Mf14, glm::vec3(-1.0f, -5.2f, -2.5f));
	Mf14 = glm::scale(Mf14, glm::vec3(0.6f, 0.6f, 0.6f));

	float ellipseRadiusX_2 = 0.15f;
	float ellipseRadiusZ_2 = 0.08f;
	float ellipseAngle_2 = angle * 0.6f;
	float ellipseX_2 = ellipseRadiusX_2 * cos(ellipseAngle_2);
	float ellipseZ_2 = ellipseRadiusZ_2 * sin(ellipseAngle_2);
	Mf14 = glm::translate(Mf14, glm::vec3(ellipseX_2, 0.0f, ellipseZ_2));

	float verticalOffset_2 = 0.15f * sin(2 * angle);
	Mf14 = glm::translate(Mf14, glm::vec3(0.0f, verticalOffset_2, 0.0f));

	float bodyRotation_2 = 15.0f * sin(1.5 * angle);
	Mf14 = glm::rotate(Mf14, glm::radians(bodyRotation_2), glm::vec3(0.0f, 1.0f, 0.0f));

	float tailAngle_2 = 30.0f * sin(1.2 * angle);
	Mf14 = glm::rotate(Mf14, glm::radians(tailAngle_2), glm::vec3(0.0f, 0.0f, 1.0f));

	float finAngle_2 = 7.0f * sin(2.5 * angle + 1.0);
	Mf14 = glm::rotate(Mf14, glm::radians(finAngle_2), glm::vec3(0.0f, 1.0f, 0.0f));

	//drawModel(P, V, Mf14, NEMO, TEX_FISH2);
	drawModelNew(P, V, Mf14, NEMO, TEX_FISH2, TEX_FISH2, glm::vec3(0.01f, 0.00f, 1.00f));

	//====================================================================================================================
	// Fish 6 - HELENA 
	glm::mat4 Mf15 = glm::rotate(M, -angle * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	Mf15 = glm::translate(Mf15, glm::vec3(-3.0f + 0.3f * sin(angle), -3.0f + 0.2f * cos(angle), -2.5f)); // Ruch po krzywej
	Mf15 = glm::scale(Mf15, glm::vec3(-0.8f, 0.8f, 0.8f));
	Mf15 = glm::rotate(Mf15, glm::radians(180.0f), glm::vec3(1.0f, -0.8f, 0.0f));

	float bodyRotation_3 = 5.0f * sin(1.5 * angle);
	Mf15 = glm::rotate(Mf15, glm::radians(bodyRotation_3), glm::vec3(0.0f, 1.0f, 0.0f));
	//drawModel(P, V, Mf15, FISH, TEX_FISH);
	drawModelNew(P, V, Mf15, FISH, TEX_FISH, TEX_FISH, glm::vec3(0.01f, 0.00f, 1.00f));

}


void drawGlass(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	glass->use();

	glUniformMatrix4fv(glass->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(glass->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(glass->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(glass->a("vertex"));
	glVertexAttribPointer(glass->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

	glEnableVertexAttribArray(glass->a("normal"));
	glVertexAttribPointer(glass->a("normal"), 4, GL_FLOAT, false, 0, myCubeVertexNormals);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(glass->a("vertex"));
	glDisableVertexAttribArray(glass->a("normal"));
}

void drawButtom(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	float bottomTexCoords[] = {
		4.0f, 4.0f,	  0.0f, 0.0f,    0.0f, 4.0f,
		4.0f, 4.0f,   4.0f, 0.0f,    0.0f, 0.0f,
	};

	float bottomVertices[] = {
		8.0f,-6.0f, 6.0f,1.0f,
		8.0f, 6.0f,-6.0f,1.0f,
		8.0f,-6.0f,-6.0f,1.0f,

		8.0f,-6.0f, 6.0f,1.0f,
		8.0f, 6.0f, 6.0f,1.0f,
		8.0f, 6.0f,-6.0f,1.0f,
	};

	float bottomVertexNormals[] = {
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 0.0f,

		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 0.0f,
	};

	int bottomVertexCount = 6;

	water->use();

	glUniformMatrix4fv(water->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(water->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(water->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(water->a("vertex"));
	glVertexAttribPointer(water->a("vertex"), 4, GL_FLOAT, false, 0, bottomVertices);

	glEnableVertexAttribArray(water->a("normal"));
	glVertexAttribPointer(water->a("normal"), 4, GL_FLOAT, false, 0, bottomVertexNormals);

	glEnableVertexAttribArray(water->a("texCoord"));
	glVertexAttribPointer(water->a("texCoord"), 2, GL_FLOAT, false, 0, bottomTexCoords);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[TEX_BOTTOM]);
	glUniform1i(water->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, bottomVertexCount);

	glDisableVertexAttribArray(water->a("vertex"));
	glDisableVertexAttribArray(water->a("normal"));
	glDisableVertexAttribArray(water->a("texCoord"));
}

void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture) {
	water->use();

	glUniformMatrix4fv(water->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(water->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(water->u("M"), 1, false, glm::value_ptr(M));
	glUniform4fv(water->u("light1"), 1, glm::value_ptr(light1));
	glUniform4fv(water->u("light2"), 1, glm::value_ptr(light2));

	glEnableVertexAttribArray(water->a("vertex"));
	glVertexAttribPointer(water->a("vertex"), 4, GL_FLOAT, false, 0, models[model_i].Vertices.data());

	glEnableVertexAttribArray(water->a("texCoord"));
	glVertexAttribPointer(water->a("texCoord"), 2, GL_FLOAT, false, 0, models[model_i].TexCoords.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[texture]);
	glUniform1i(water->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, models[model_i].Indices.size(), GL_UNSIGNED_INT, models[model_i].Indices.data());

	glDisableVertexAttribArray(water->a("vertex"));
	glDisableVertexAttribArray(water->a("normal"));
	glDisableVertexAttribArray(water->a("color"));
}

void drawRocks(glm::mat4 P, glm::mat4 V) {
	glm::mat4 rocks = glm::mat4(1.0f);
	rocks = glm::translate(rocks, glm::vec3(0.0f, -7.85f, 0.0f));
	//	rocks = glm::scale(rocks, glm::vec3(0.005f, 0.005f, 0.005f));
	rocks = glm::scale(rocks, glm::vec3(0.25f, 0.25f, 0.25f));
	float wsp = 400;



	glm::mat4 rock1 = glm::translate(rocks, glm::vec3(0.0f, -2.0f, 0.0f));
	drawModel(P, V, rock1, ROCK1, TEX_ROCK1);
}

void drawPlants(glm::mat4 P, glm::mat4 V) {
	float wsp_plant = 1.5f;
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(0.25f, 0.25f, 0.25f));


	glm::mat4 plant1 = glm::scale(M, glm::vec3(wsp_plant, wsp_plant, wsp_plant));
	plant1 = glm::translate(plant1, glm::vec3(10.0f, -30.3f / wsp_plant, 3.0f));
	drawModel(P, V, plant1, PLANTS, TEX_PLANT);
}

void drawSand(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2) {


	sand->use();

	glUniformMatrix4fv(sand->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sand->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sand->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(sand->a("vertex"));
	glVertexAttribPointer(sand->a("vertex"), 4, GL_FLOAT, false, 0, models[model_i].Vertices.data());

	glEnableVertexAttribArray(sand->a("texCoord"));
	glVertexAttribPointer(sand->a("texCoord"), 2, GL_FLOAT, false, 0, models[model_i].TexCoords.data());

	glEnableVertexAttribArray(sand->a("normal"));
	glVertexAttribPointer(sand->a("normal"), 4, GL_FLOAT, false, 0, models[model_i].Normals.data());


	//glUniform1i(sand->u("material.diffuse"), 0);
	//glUniform1i(sand->u("material.specular"), 1);

	glUniform3fv(sand->u("viewPos"), 1, glm::value_ptr(camera.Position));

	glUniform1f(sand->u("material.shininess"), 32.0f);

	//sand->use();
	//sand->setVec3("viewPos", cameraPos);
	//sand->setFloat("material.shininess", 32.0f);

	//sand->setFloat("material.shininess", 32.0f);
	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/


	//sand->setMat4("projection", P);
	//sand->setMat4("view", V);

	// world transformation
	//sand->setMat4("model", M);



	// directional light
	//sand->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//sand->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//sand->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//sand->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	//sand->setVec3("pointLights.position", glm::vec3(0.7f, 0.2f, 2.0f));
	//sand->setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
	//sand->setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
	//sand->setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);




	glUniform3fv(sand->u("dirLight.direction"), 1, glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
	glUniform3fv(sand->u("dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.62, 0.60, 0.27)));
	glUniform3fv(sand->u("dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.62, 0.60, 0.27)));
	glUniform3fv(sand->u("dirLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));


	glUniform3fv(sand->u("pointLights.position"), 1, glm::value_ptr(glm::vec3(0.7f, 0.2f, 2.0f)));
	glUniform3fv(sand->u("pointLights.ambient"), 1, glm::value_ptr(glm::vec3(0.62, 0.60, 0.27)));
	glUniform3fv(sand->u("pointLights.diffuse"), 1, glm::value_ptr(glm::vec3(0.62, 0.60, 0.27)));
	glUniform3fv(sand->u("pointLights.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(sand->u("pointLights.constant"), 1.0f);
	glUniform1f(sand->u("pointLights.linear"), 0.09f);
	glUniform1f(sand->u("pointLights.quadratic"), 0.032f);


	//sand->setFloat("pointLights.constant", 1.0f);
	//sand->setFloat("pointLights.linear", 0.09f);
	//sand->setFloat("pointLights.quadratic", 0.032f);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[texture]);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texs[texture2]);

	glDrawElements(GL_TRIANGLES, models[model_i].Indices.size(), GL_UNSIGNED_INT, models[model_i].Indices.data());
}

void drawModelNew(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2, glm::vec3 color, float shiness) {


	sand->use();

	glUniformMatrix4fv(sand->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sand->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sand->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(sand->a("vertex"));
	glVertexAttribPointer(sand->a("vertex"), 4, GL_FLOAT, false, 0, models[model_i].Vertices.data());

	glEnableVertexAttribArray(sand->a("texCoord"));
	glVertexAttribPointer(sand->a("texCoord"), 2, GL_FLOAT, false, 0, models[model_i].TexCoords.data());

	glEnableVertexAttribArray(sand->a("normal"));
	glVertexAttribPointer(sand->a("normal"), 4, GL_FLOAT, false, 0, models[model_i].Normals.data());


	//glUniform1i(sand->u("material.diffuse"), 0);
	//glUniform1i(sand->u("material.specular"), 1);

	glUniform3fv(sand->u("viewPos"), 1, glm::value_ptr(camera.Position));

	glUniform1f(sand->u("material.shininess"), 32.0f);

	//sand->use();
	//sand->setVec3("viewPos", cameraPos);
	//sand->setFloat("material.shininess", 32.0f);

	//sand->setFloat("material.shininess", 32.0f);
	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/


	//sand->setMat4("projection", P);
	//sand->setMat4("view", V);

	// world transformation
	//sand->setMat4("model", M);



	// directional light
	//sand->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//sand->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//sand->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//sand->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	//sand->setVec3("pointLights.position", glm::vec3(0.7f, 0.2f, 2.0f));
	//sand->setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
	//sand->setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
	//sand->setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);




	glUniform3fv(sand->u("dirLight.direction"), 1, glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
	glUniform3fv(sand->u("dirLight.ambient"), 1, glm::value_ptr(glm::vec3(color)));
	glUniform3fv(sand->u("dirLight.diffuse"),1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(sand->u("dirLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));


	glUniform3fv(sand->u("pointLights.position"), 1, glm::value_ptr(glm::vec3(10.0f, 0.0f, 5.0f)));
	glUniform3fv(sand->u("pointLights.ambient"), 1, glm::value_ptr(glm::vec3(color)));
	glUniform3fv(sand->u("pointLights.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	glUniform3fv(sand->u("pointLights.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(sand->u("pointLights.constant"), 1.0f);
	glUniform1f(sand->u("pointLights.linear"), 0.09f);
	glUniform1f(sand->u("pointLights.quadratic"), 0.032f);


	//sand->setFloat("pointLights.constant", 1.0f);
	//sand->setFloat("pointLights.linear", 0.09f);
	//sand->setFloat("pointLights.quadratic", 0.032f);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[texture]);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texs[texture2]);

	glDrawElements(GL_TRIANGLES, models[model_i].Indices.size(), GL_UNSIGNED_INT, models[model_i].Indices.data());
}

void drawLightCube(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

	light_cube->use();

	glUniformMatrix4fv(light_cube->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(light_cube->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(light_cube->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(light_cube->a("vertex"));
	glVertexAttribPointer(light_cube->a("vertex"), 4, GL_FLOAT, false, 0, cubeVertices);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(glass->a("vertex"));
}

void drawTableNew(glm::mat4 P, glm::mat4 V, glm::mat4 M, int model_i, int texture, int texture2, float shiness) {


	outside->use();

	glUniformMatrix4fv(outside->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(outside->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(outside->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(outside->a("vertex"));
	glVertexAttribPointer(outside->a("vertex"), 4, GL_FLOAT, false, 0, models[model_i].Vertices.data());

	glEnableVertexAttribArray(outside->a("texCoord"));
	glVertexAttribPointer(outside->a("texCoord"), 2, GL_FLOAT, false, 0, models[model_i].TexCoords.data());

	glEnableVertexAttribArray(outside->a("normal"));
	glVertexAttribPointer(outside->a("normal"), 4, GL_FLOAT, false, 0, models[model_i].Normals.data());


	glUniform3fv(outside->u("viewPos"), 1, glm::value_ptr(camera.Position));

	glUniform1f(outside->u("material.shininess"), shiness);

	glUniform3fv(outside->u("dirLight.direction"), 1, glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
	glUniform3fv(outside->u("dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
	glUniform3fv(outside->u("dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	glUniform3fv(outside->u("dirLight.specular"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));


	glUniform3fv(outside->u("pointLights[0].position"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	glUniform3fv(outside->u("pointLights[0].ambient"), 1, glm::value_ptr(glm::vec3(0.35f, 0.35f, 0.35f)));
	glUniform3fv(outside->u("pointLights[0].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	glUniform3fv(outside->u("pointLights[0].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(outside->u("pointLights[0].constant"), 1.0f);
	glUniform1f(outside->u("pointLights[0].linear"), 0.07f);
	glUniform1f(outside->u("pointLights[0].quadratic"), 0.017f);


	glUniform3fv(outside->u("pointLights[1].position"), 1, glm::value_ptr(glm::vec3(10.0f, 0.0f, 5.0f)));
	glUniform3fv(outside->u("pointLights[1].ambient"), 1, glm::value_ptr(glm::vec3(0.35f, 0.35f, 0.35f)));
	glUniform3fv(outside->u("pointLights[1].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	glUniform3fv(outside->u("pointLights[1].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(outside->u("pointLights[1].constant"), 1.0f);
	glUniform1f(outside->u("pointLights[1].linear"), 0.07f);
	glUniform1f(outside->u("pointLights[1].quadratic"), 0.017f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[texture]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texs[texture2]);

	glDrawElements(GL_TRIANGLES, models[model_i].Indices.size(), GL_UNSIGNED_INT, models[model_i].Indices.data());
}

void drawTank(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	light->use();

	glUniformMatrix4fv(light->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(light->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(light->u("M"), 1, false, glm::value_ptr(M));
	glUniform4fv(light->u("light1"), 1, glm::value_ptr(light1));
	glUniform4fv(light->u("light2"), 1, glm::value_ptr(light2));

	glEnableVertexAttribArray(light->a("vertex"));
	glVertexAttribPointer(light->a("vertex"), 4, GL_FLOAT, false, 0, models[TANK].Vertices.data());

	glEnableVertexAttribArray(light->a("texCoord"));
	glVertexAttribPointer(light->a("texCoord"), 2, GL_FLOAT, false, 0, models[TANK].TexCoords.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[TEX_ALUMINIUM]);
	glUniform1i(light->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, models[TANK].Indices.size(), GL_UNSIGNED_INT, models[TANK].Indices.data());

	glDisableVertexAttribArray(light->a("vertex"));
	glDisableVertexAttribArray(light->a("normal"));
	glDisableVertexAttribArray(light->a("color"));
}


void texKostka(glm::mat4 P2, glm::mat4 V2, glm::mat4 M2) {
	//Tablica ta raczej powinna znaleźć się w pliku myCube.h, ale umieściłem ją tutaj, żeby w tej procedurze zawrzeć (prawie) całe rozwiązanie zadania
	//Reszta to wczytanie tekstury - czyli kawałki kodu, które trzeba przekopiować ze slajdów
	float myCubeTexCoords[] = {
		//Ściana 1
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		//Ściana 2
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		//Ściana 3
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		//Ściana 4
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		//Ściana 5
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		//Ściana 6
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	};

	sp_floor->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp_floor->u("P"), 1, false, glm::value_ptr(P2));
	glUniformMatrix4fv(sp_floor->u("V"), 1, false, glm::value_ptr(V2));
	glUniformMatrix4fv(sp_floor->u("M"), 1, false, glm::value_ptr(M2));

	glEnableVertexAttribArray(sp_floor->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp_floor->a("vertex"), 4, GL_FLOAT, false, 0, cubeVertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp_floor->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp_floor->a("color"), 4, GL_FLOAT, false, 0, cubeColors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp_floor->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp_floor->a("normal"), 4, GL_FLOAT, false, 0, cubeNormals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp_floor->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord0
	glVertexAttribPointer(sp_floor->a("texCoord0"), 2, GL_FLOAT, false, 0, cubeTexCoords); //Wskaż tablicę z danymi dla atrybutu texCoord0

	glUniform1f(sp_floor->u("maxFurLength"), 0.01);
	glUniform1f(sp_floor->u("maxLayer"), 10);

	glUniform1i(sp_floor->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glUniform1i(sp_floor->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glDrawArraysInstanced(GL_TRIANGLES, 0, cubeVertexCount, 10); //Narysuj obiekt
	glDrawArrays(GL_TRIANGLES, 18, 6);

	glDisableVertexAttribArray(sp_floor->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp_floor->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp_floor->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp_floor->a("texCoord0"));  //Wyłącz przesyłanie danych do atrybutu texCoord0

}


void drawFloor(glm::mat4 P2, glm::mat4 V2, glm::mat4 M2, float angle_x, float angle_y) {

	M2 = glm::translate(M2, glm::vec3(0.0f, -122.0f, 0.0f));
	M2 = glm::scale(M2, glm::vec3(100.0f, 100.0f, 100.0f));
	M2 = glm::rotate(M2, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	M2 = glm::rotate(M2, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
	texKostka(P2, V2, M2);
}


void drawRedPlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle) {

	// 1 roślinka
	glm::mat4 MG_1_1 = glm::mat4(1.0f);
	MG_1_1 = glm::translate(MG_1_1, glm::vec3(-5.0f, -7.2f + 0.1f * sin(0.5f * angle), -3.0f));
	MG_1_1 = glm::scale(MG_1_1, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_1_1, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));

	glm::mat4 MG_1_2 = glm::mat4(1.0f);
	MG_1_2 = glm::translate(MG_1_2, glm::vec3(-5.0f, -7.2f + 0.1f * cos(0.3f * angle), -3.0f));
	MG_1_2 = glm::scale(MG_1_2, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_1_2, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));

	// 2 roślinka
	glm::mat4 MG_2_1 = glm::mat4(1.0f);
	MG_2_1 = glm::translate(MG_2_1, glm::vec3(-2.0f, -7.2f + 0.1f * sin(0.7f * angle), -5.0f));
	MG_2_1 = glm::scale(MG_2_1, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_1, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));

	glm::mat4 MG_2_2 = glm::mat4(1.0f);
	MG_2_2 = glm::translate(MG_2_2, glm::vec3(-2.0f, -7.2f + 0.1f * sin(0.4f * angle), -5.0f));
	MG_2_2 = glm::scale(MG_2_2, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_2, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));

	//3 roślinka 
	glm::mat4 MG_3_1 = glm::mat4(1.0f);
	MG_3_1 = glm::translate(MG_3_1, glm::vec3(0.0f, -7.2f + 0.1f * sin(0.4f * angle), -3.0f));
	MG_3_1 = glm::scale(MG_3_1, glm::vec3(12.0f, 12.0f, 12.0f));
	drawModelNew(P, V, MG_3_1, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));

	glm::mat4 MG_3_2 = glm::mat4(1.0f);
	MG_3_2 = glm::translate(MG_3_2, glm::vec3(0.0f, -7.2f + 0.1f * sin(0.3f * angle), -3.0f));
	MG_3_2 = glm::scale(MG_3_2, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_3_2, GRASS_1, TEX_GRASS_RED, TEX_GRASS_RED, glm::vec3(1.00f, 0.00f, 0.00f));
}
void drawGreenPlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle) {
	// Roślinaka 1 
	glm::mat4 MG_2_1 = glm::mat4(1.0f);
	float horizontalOffset1 = 0.05f * sin(0.2f * angle);
	MG_2_1 = glm::translate(MG_2_1, glm::vec3(-5.0f + horizontalOffset1, -7.2f, -5.5f));
	MG_2_1 = glm::scale(MG_2_1, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_1, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	glm::mat4 MG_2_2 = glm::mat4(1.0f);
	float horizontalOffset2 = 0.05f * cos(0.15f * angle);
	MG_2_2 = glm::translate(MG_2_2, glm::vec3(-5.0f + horizontalOffset2, -7.2f, -5.5f));
	MG_2_2 = glm::scale(MG_2_2, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_2, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));
	// Roślinaka 2
	glm::mat4 MG_2_3 = glm::mat4(1.0f);
	float horizontalOffset3 = 0.05f * cos(0.4f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_3 = glm::translate(MG_2_3, glm::vec3(-3.0f + horizontalOffset3, -7.2f, -0.5f));  // Nowa pozycja roślinki
	MG_2_3 = glm::scale(MG_2_3, glm::vec3(8.0f, 8.0f, 8.0f));  // Większa skala
	drawModelNew(P, V, MG_2_3, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	glm::mat4 MG_2_4 = glm::mat4(1.0f);
	float horizontalOffset4 = 0.05f * sin(0.4f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_4 = glm::translate(MG_2_4, glm::vec3(-3.0f + horizontalOffset4, -7.2f, -0.5f));  // Nowa pozycja roślinki
	MG_2_4 = glm::scale(MG_2_4, glm::vec3(8.0f, 8.0f, 8.0f));  // Większa skala
	drawModelNew(P, V, MG_2_4, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	// Roślinaka 3
	glm::mat4 MG_2_5 = glm::mat4(1.0f);
	float horizontalOffset5 = 0.05f * cos(0.3f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_5 = glm::translate(MG_2_5, glm::vec3(-3.0f + horizontalOffset5, -7.2f, -0.5f));  // Nowa pozycja roślinki
	MG_2_5 = glm::scale(MG_2_5, glm::vec3(15.0f, 15.0f, 15.0f));  // Większa skala
	drawModelNew(P, V, MG_2_5, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	glm::mat4 MG_2_6 = glm::mat4(1.0f);
	float horizontalOffset6 = 0.05f * sin(0.5f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_6 = glm::translate(MG_2_6, glm::vec3(-3.0f + horizontalOffset6, -7.2f, -0.5f));  // Nowa pozycja roślinki
	MG_2_6 = glm::scale(MG_2_6, glm::vec3(15.0f, 15.0f, 15.0f));  // Większa skala
	drawModelNew(P, V, MG_2_6, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	// Roślinaka 4
	glm::mat4 MG_2_7 = glm::mat4(1.0f);
	float horizontalOffset7 = 0.05f * cos(0.3f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_7 = glm::translate(MG_2_7, glm::vec3(3.0f + horizontalOffset7, -7.2f, 1.5f));  // Nowa pozycja roślinki
	MG_2_7 = glm::scale(MG_2_7, glm::vec3(10.0f, 10.0f, 10.0f));  // Większa skala
	drawModelNew(P, V, MG_2_7, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));

	glm::mat4 MG_2_8 = glm::mat4(1.0f);
	float horizontalOffset8 = 0.05f * sin(0.5f * angle);  // Możemy również dodać lekkie oscylacje, jeśli chcemy
	MG_2_8 = glm::translate(MG_2_8, glm::vec3(3.0f + horizontalOffset8, -7.2f, 1.5f));  // Nowa pozycja roślinki
	MG_2_8 = glm::scale(MG_2_8, glm::vec3(10.0f, 10.0f, 10.0f));  // Większa skala
	drawModelNew(P, V, MG_2_8, GRASS_1, TEX_GRASS_GREEN, TEX_GRASS_GREEN, glm::vec3(0.00f, 1.00f, 0.39f));
}
void drawBluePlants(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle) {
	// Roślinaka 1 
	glm::mat4 MG_2_1 = glm::mat4(1.0f);
	float horizontalOffset1 = 0.05f * sin(0.2f * angle);
	MG_2_1 = glm::translate(MG_2_1, glm::vec3(5.0f + horizontalOffset1, -7.2f, 5.0f));
	MG_2_1 = glm::scale(MG_2_1, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_1, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

	glm::mat4 MG_2_2 = glm::mat4(1.0f);
	float horizontalOffset2 = 0.05f * cos(0.15f * angle);
	MG_2_2 = glm::translate(MG_2_2, glm::vec3(5.0f + horizontalOffset2, -7.2f, 5.0f));
	MG_2_2 = glm::scale(MG_2_2, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_2, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

	//Roślinka 2 
	glm::mat4 MG_2_3 = glm::mat4(1.0f);
	float horizontalOffset3 = 0.05f * sin(0.2f * angle);
	MG_2_3 = glm::translate(MG_2_3, glm::vec3(5.0f + horizontalOffset3, -7.2f, 5.0f));
	MG_2_3 = glm::scale(MG_2_3, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_3, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

	glm::mat4 MG_2_4 = glm::mat4(1.0f);
	float horizontalOffset4 = 0.05f * cos(0.15f * angle);
	MG_2_4 = glm::translate(MG_2_4, glm::vec3(5.0f + horizontalOffset4, -7.2f, 5.0f));
	MG_2_4 = glm::scale(MG_2_4, glm::vec3(6.0f, 6.0f, 6.0f));
	drawModelNew(P, V, MG_2_4, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

	//Roślinka 3
	glm::mat4 MG_2_5 = glm::mat4(1.0f);
	float horizontalOffset5 = 0.05f * sin(0.3f * angle);
	MG_2_5 = glm::translate(MG_2_5, glm::vec3(0.0f + horizontalOffset5, -7.2f, 3.0f));
	MG_2_5 = glm::scale(MG_2_5, glm::vec3(16.0f, 16.0f, 16.0f));
	drawModelNew(P, V, MG_2_5, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

	glm::mat4 MG_2_6 = glm::mat4(1.0f);
	float horizontalOffset6 = 0.05f * cos(0.15f * angle);
	MG_2_4 = glm::translate(MG_2_4, glm::vec3(0.0f + horizontalOffset6, -7.2f, 3.0f));
	MG_2_4 = glm::scale(MG_2_4, glm::vec3(16.0f, 16.0f, 16.0f));
	drawModelNew(P, V, MG_2_4, GRASS_1, TEX_GRASS_BLUE, TEX_GRASS_BLUE, glm::vec3(0.01f, 0.00f, 1.00f));

}


void drawLeafs(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle, float scale, glm::vec3 position) {
	glm::mat4 MR_2 = glm::mat4(1.0f);
	float waveHeight = 0.3f * std::sin(angle);
	float waveSpiral = 0.05f * std::sin(angle * 2.0f);
	float waveAngleX = 0.2f * std::cos(angle * 1.5f);
	float waveAngleZ = 0.1f * std::sin(angle * 1.2f);
	float waveRotationY = 15.0f * std::sin(angle * 0.8f);
	MR_2 = glm::translate(MR_2, position + glm::vec3(0.0f, -1.0f + waveHeight, waveSpiral));
	MR_2 = glm::rotate(MR_2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(angle_x + waveAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(angle + waveAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(waveRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	MR_2 = glm::rotate(MR_2, glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	MR_2 = glm::scale(MR_2, glm::vec3(scale, scale, scale));
	drawModelNew(P, V, MR_2, LEAF, TEX_LEAF, TEX_LEAF, glm::vec3(0.20f, 0.20f, 0.20f));
}


void drawMultipleLeafs(glm::mat4 P, glm::mat4 V, float angle_x, float angle_y, float angle) {
	drawLeafs(P, V, angle_x, angle_y, angle, 0.03f, glm::vec3(1.0f, 0.0f, 0.0f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.5f, 0.02f, glm::vec3(2.0f, 0.5f, 1.3f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.8f, 0.04f, glm::vec3(3.0f, -0.3f, 2.0f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.2f, 0.05f, glm::vec3(4.0f, 4.8f, 4.2f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.1f, 0.06f, glm::vec3(5.0f, 0.2f, 0.1f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.6f, 0.04f, glm::vec3(-1.1f, -1.7f, -1.2f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.4f, 0.01f, glm::vec3(-2.2f, 1.2f, -3.0f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.9f, 0.02f, glm::vec3(-3.4f, -0.5f, 0.9f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.7f, 0.03f, glm::vec3(-1.0f, -3.3f, 0.1f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.3f, 0.04f, glm::vec3(-2.8f, -3.8f, 1.4f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.0f, 0.05f, glm::vec3(-3.3f, -1.6f, -3.3f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.5f, 0.02f, glm::vec3(-4.7f, -0.1f, 0.1f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.7f, 0.03f, glm::vec3(-5.6f, 1.0f, 5.3f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.8f, 0.04f, glm::vec3(2.5f, 2.0f, -4.0f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.9f, 0.05f, glm::vec3(3.8f, 3.5f, 2.8f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.1f, 0.03f, glm::vec3(4.9f, 2.3f, -1.7f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.3f, 0.06f, glm::vec3(4.6f, 0.8f, 4.1f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.7f, 0.04f, glm::vec3(-3.5f, -3.0f, 0.5f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.2f, 0.05f, glm::vec3(-4.8f, 1.5f, -2.3f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.9f, 0.02f, glm::vec3(-5.9f, -2.0f, 3.7f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.6f, 0.04f, glm::vec3(2.1f, -4.5f, 0.3f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.5f, 0.05f, glm::vec3(3.7f, -3.1f, -2.7f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.8f, 0.03f, glm::vec3(4.8f, -1.8f, 3.2f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.2f, 0.06f, glm::vec3(6.0f, 3.4f, -1.5f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.1f, 0.04f, glm::vec3(-2.3f, -5.9f, 2.7f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.7f, 0.05f, glm::vec3(-4.1f, -4.2f, -3.0f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.4f, 0.03f, glm::vec3(-5.5f, -2.3f, 1.9f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.9f, 0.06f, glm::vec3(-1.9f, 3.4f, -4.4f));
	drawLeafs(P, V, angle_x, angle_y, angle * 0.6f, 0.04f, glm::vec3(3.2f, 4.9f, 0.7f));
	drawLeafs(P, V, angle_x, angle_y, angle * 1.3f, 0.05f, glm::vec3(5.1f, 2.0f, 3.3f));
}

void drawStones(glm::mat4 P, glm::mat4 V, float shiness) {

	glm::mat4 MR_2 = glm::mat4(1.0f);
	MR_2 = glm::translate(MR_2, glm::vec3(0.0f, -6.5f, 0.0f));
	MR_2 = glm::scale(MR_2, glm::vec3(0.1f, 0.1f, 0.1f));
	drawModelNew(P, V, MR_2, NEW_ROCK2, TEX_ROCK1, TEX_ROCK1, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

	glm::mat4 MR_3 = glm::mat4(1.0f);
	MR_3 = glm::translate(MR_3, glm::vec3(2.0f, -6.0f, -4.3f));
	MR_3 = glm::scale(MR_3, glm::vec3(0.1f, 0.1f, 0.1f));
	MR_3 = glm::rotate(MR_3, glm::radians(180.0f), glm::vec3(1.0f, -0.8f, 0.0f));
	drawModelNew(P, V, MR_3, NEW_ROCK2, TEX_ROCK1, TEX_ROCK1, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

	glm::mat4 MR_4 = glm::mat4(1.0f);
	MR_4 = glm::translate(MR_4, glm::vec3(1.1f, -4.8f, -4.7f));
	MR_4 = glm::scale(MR_4, glm::vec3(2.0f, 2.0f, 2.0f));
	MR_4 = glm::rotate(MR_4, glm::radians(180.0f), glm::vec3(1.0f, -0.8f, 0.0f));
	drawModelNew(P, V, MR_4, NEW_ROCK, TEX_ROCK5, TEX_ROCK5, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

	glm::mat4 MR_5 = glm::mat4(1.0f);
	MR_5 = glm::translate(MR_5, glm::vec3(-6.7f, -6.6f, -6.7f));
	MR_5 = glm::scale(MR_5, glm::vec3(0.1f, 0.1f, 0.1f));
	MR_5 = glm::rotate(MR_5, glm::radians(180.0f), glm::vec3(1.0f, -0.8f, 0.0f));
	drawModelNew(P, V, MR_5, NEW_ROCK, TEX_ROCK6, TEX_ROCK6, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

	glm::mat4 MR_6 = glm::mat4(1.0f);
	MR_6 = glm::translate(MR_6, glm::vec3(3.2f, -6.5f, 5.0f));
	MR_6 = glm::scale(MR_6, glm::vec3(0.1f, 0.1f, 0.1f));
	drawModelNew(P, V, MR_6, ROCK1, TEX_ROCK6, TEX_ROCK6, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

	glm::mat4 MR_7 = glm::mat4(1.0f);
	MR_7 = glm::translate(MR_7, glm::vec3(-1.5f, -6.5f, 4.6f));
	MR_7 = glm::scale(MR_7, glm::vec3(0.1f, 0.1f, 0.1f));
	drawModelNew(P, V, MR_7, ROCK1, TEX_ROCK6, TEX_ROCK6, glm::vec3(0.01f, 0.00f, 1.00f), shiness);

}