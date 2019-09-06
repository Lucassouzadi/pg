#include "System.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

const char* vertex_shader =
"#version 330\n"

"layout(location = 0) in vec3 vp;"
//"layout(location = 1) in vec3 vc;"
"layout(location = 1) in vec2 tm;"
"uniform mat4 matrix;" 
//"out vec3 color;"
"out vec2 texture_coordinates;"
"void main () {"
"  texture_coordinates = tm;"
//"  color = vc;"
"  gl_Position = matrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec2 texture_coordinates;"
//"in vec3 color;"
"uniform sampler2D texture;"
"uniform float offsetx;"
"uniform float offsety;"
"out vec4 frag_color;"
"void main () {"
"	vec2 vt = vec2(texture_coordinates.x + offsetx, texture_coordinates.y + offsety);"
"   frag_color = texture (texture, vt);"						//	textura
//"   frag_color = texture (texture, vt) * vec4(color, 1.0);"	//	cor + textura
//"   frag_color = vec4(color, 1.0);"							//	cor	
"   if (frag_color.a < 1.0f) "
"     discard;"
"}";

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	//glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(640, 480, "Teste de versão OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	// obtenção de versão suportada da OpenGL e renderizador
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versão suportada) %s\n", version);


	// ###############################################

//	#VBO

	GLfloat verticesFundo[] = {
		-1.0f, 0.6f, 0.0f,	// 1
		1.0f, 0.6f, 0.0f,	// 2
		1.0f, -0.6f, 0.0f,	// 3
		-1.0f, 0.6f, 0.0f,	// 1
		-1.0f, -0.6f, 0.0f,	// 4
		1.0f, -0.6f, 0.0f	// 3
	};

	GLfloat texMapFundo[] = { // 1x1
		0.0f, 0.0f,							// 1
		1.0f, 0.0f,							// 2
		1.0f, 1.0f,							// 3
		0.0f, 0.0f,							// 1
		0.0f, 1.0f,							// 4
		1.0f, 1.0f							// 3
	};

	GLuint verticeFundoVBO = 0;
	glGenBuffers(1, &verticeFundoVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticeFundoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFundo), verticesFundo, GL_STATIC_DRAW);

	GLuint texFundoVBO = 0;
	glGenBuffers(1, &texFundoVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texFundoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapFundo), texMapFundo, GL_STATIC_DRAW);



	GLfloat verticesAviao[] = {
		-0.15f, 0.15f, 0.0f,	// 1
		0.15f, 0.15f, 0.0f,		// 2
		0.15f, -0.15f, 0.0f,	// 3
		-0.15f, 0.15f, 0.0f,	// 1
		-0.15f, -0.15f, 0.0f,	// 4
		0.15f, -0.15f, 0.0f		// 3
	};

	GLfloat texMapAviao[] = { // 1x1
		0.0f, 0.0f,							// 1
		1.0f, 0.0f,							// 2
		1.0f, 1.0f,							// 3
		0.0f, 0.0f,							// 1
		0.0f, 1.0f,							// 4
		1.0f, 1.0f							// 3
	};

	GLuint verticeAviaoVBO = 0;
	glGenBuffers(1, &verticeAviaoVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticeAviaoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAviao), verticesAviao, GL_STATIC_DRAW);

	GLuint texAviaoVBO = 0;
	glGenBuffers(1, &texAviaoVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texAviaoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMapAviao), texMapAviao, GL_STATIC_DRAW);

//	#VBO

//	#VAO

	GLuint fundoVAO = 0;
	glGenVertexArrays(1, &fundoVAO);
	glBindVertexArray(fundoVAO);

	glBindBuffer(GL_ARRAY_BUFFER, verticeFundoVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texFundoVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


	GLuint aviaoVAO = 0;
	glGenVertexArrays(1, &aviaoVAO);
	glBindVertexArray(aviaoVAO);

	glBindBuffer(GL_ARRAY_BUFFER, verticeAviaoVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texAviaoVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

//	#VAO

//	#SHADER PROGRAM

	// identifica vs e o associa com vertex_shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);

	// identifica fs e o associa com fragment_shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
	glCompileShader(fragmentShader);

	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fragmentShader);
	glAttachShader(shader_program, vertexShader);
	glLinkProgram(shader_program);

	
	//	#passar o shader program ao OpenGL.
	glUseProgram(shader_program);

	float matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // 1ª coluna
		0.0f, 1.0f, 0.0f, 0.0f, // 2ª coluna
		0.0f, 0.0f, 1.0f, 0.0f, // 3ª coluna
		0.0f, 0.0f, 0.0f, 1.0f  // 4ª coluna
	};
	//	#salva a matriz como uniform no program
	int matrixLocation = glGetUniformLocation(shader_program, "matrix");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

//	#SHADER PROGRAM

//	#TEXTURA
	int imgWidth, imgHeight;
	unsigned char *image;

	GLuint texturaAviao = 0;
	glGenTextures(1, &texturaAviao);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturaAviao);
	image = SOIL_load_image("bin/Images/aviao.png", &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	GLuint texturaFundo = 0;
	glGenTextures(1, &texturaFundo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturaFundo);
	image = SOIL_load_image("bin/Images/ceu2.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);



	// filtragem de pixels 
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR); // ou GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR); // ou GL_NEAREST);

	// habilitar texturas
	glEnable(GL_TEXTURE_2D);

	// Para desenhar com alfa, na inicialização da aplicação, habilitar o alpha test :
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int textureLocation = glGetUniformLocation(shader_program, "texture");
	glUniform1i(textureLocation, 0);

	int textureOffsetXLocation = glGetUniformLocation(shader_program, "offsetx");
	int textureOffsetYLocation = glGetUniformLocation(shader_program, "offsety");
//	#TEXTURA */

//	#GAME LOOP

	int numFrames = 4;
	int numAcoes = 4;
	float frameWidth = 1.0f / numFrames;
	float frameHeight = 1.0f / numAcoes;
	int frameAtual = 4;
	int acaoAtual = 0;
	float textureOffsetX = 0.0f;
	float textureOffsetY = 0.0f;
	int fps = 8;

	static float speed = 1.0f;
	static float positionX = 0.0f;
	static float positionY = 0.0f;
	static double previousSeconds = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {

		double currentSeconds = glfwGetTime();
		double elapsedSeconds = currentSeconds - previousSeconds;
		previousSeconds = currentSeconds;
		double deslocamento = elapsedSeconds * speed;

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  /* Esc */
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { /* W */
			positionY += deslocamento;
			if (positionY > 1.0f) {
				positionY = 1.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { /* A */
			positionX -= deslocamento;
			if (positionX < -1.0f) {
				positionX = -1.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { /* S */
			positionY -= deslocamento;
			if (positionY < -1.0f) {
				positionY = -1.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { /* D */
			positionX += deslocamento;
			if (positionX > 1.0f) {
				positionX = 1.0f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) { /* + */
			speed += 0.005f;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) { /* - */
			speed -= 0.005f;
		}

		//transformação
		matrix[12] = positionX;
		matrix[13] = positionY;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
		glUniform1i(textureLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturaFundo);

		glBindVertexArray(fundoVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		matrix[12] = positionX;
		matrix[13] = positionY;
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
		glUniform1i(textureLocation, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturaAviao);

		glBindVertexArray(aviaoVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);
	}

	// ###############################################

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}