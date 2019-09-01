#include "System.h"
#include "AssetManager.h"

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vc;"
"out vec3 color;"
"uniform mat4 matrix;" 
"void main () {"
"  color = vc;"
"  gl_Position = matrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main () {"
"  frag_color = vec4 (color, 1.0);"
"}";

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	GLFWwindow *window = glfwCreateWindow(
		640, 480, "Teste de versão OpenGL", NULL, NULL);
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

	GLfloat vertices[] = {
		// positions		// colors		   // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f // top left
	};

	GLuint VBO = 0;
	GLuint colorVBO = 0;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//	#VBO

//	#EBO

	GLuint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//	#EBO

//	#VAO

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


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

	/*
	int imgWidth, imgHeight;
	unsigned char *image;

	image = SOIL_load_image("bin/Images/woodTexture.jpg", &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
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
	glEnable(GL_TEXTURE_2D);	// Para desenhar com alfa, na inicialização da aplicação, habilitar o alpha test :
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	*/

//	#TEXTURA

//	#GAME LOOP
	static float speed = 0.3f;
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

		matrix[12] = positionX;
		matrix[13] = positionY;
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Define vao como vertex array atual
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	// ###############################################

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}