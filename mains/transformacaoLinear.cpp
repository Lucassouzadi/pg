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

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,	// top
		0.5f, -0.5f, 0.0f,	// right
		-0.5f, -0.5f, 0.0f	// left
	};
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, // red
		0.0f, 1.0f, 0.0f, // green
		0.0f, 0.0f, 1.0f  // blue
	};

	GLuint pointVBO = 0;
	GLuint colorVBO = 0;

	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

//	#VBO

//	#VAO

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, pointVBO); // identifica vbo atual
	// habilitado primeiro atributo do vbo bound atual
	glEnableVertexAttribArray(0);
	// associação do vbo atual com primeiro atributo
	// 0 identifica que o primeiro atributo está sendo definido
	// 3, GL_FLOAT identifica que dados são vec3 e estão a cada 3 float.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// é possível associar outros atributos, como normais, mapeamento e cores
	// lembre-se: um por vértice!

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	// habilitado segundo atributo do vbo bound atual
	glEnableVertexAttribArray(1);
	// note que agora o atributo 1 está definido
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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

	/* bagulho da textura
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
	glEnable(GL_TEXTURE_2D);

	// Para desenhar com alfa, na inicialização da aplicação, habilitar o alpha test :
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	*/

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
		glBindVertexArray(vao);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

	// ###############################################

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}