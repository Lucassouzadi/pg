#include "System.h"

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

const char* vertex_shader =
"#version 410\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vc;"
"out vec3 color;"
"uniform mat4 matrix;" 
"void main () {"
"  color = vc;"
"  gl_Position = matrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 410\n"
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

	// ###############################################

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (versão suportada) %s\n", version);

	//	#VBO

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,	// top
		0.5f, -0.5f, 0.0f,	// right
		-0.5f, -0.5f, 0.0f	// left
	};
	GLuint pointVBO = 0;
	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, // red
		0.0f, 1.0f, 0.0f, // green
		0.0f, 0.0f, 1.0f  // blue
	};
	GLuint colorVBO = 0;
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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

	//	#SHADER PROGRAM

	// identifica vs e o associa com vertex_shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	//	#passar o shader programme ao OpenGL.
	float matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // 1ª coluna
		0.0f, 1.0f, 0.0f, 0.0f, // 2ª coluna
		0.0f, 0.0f, 1.0f, 0.0f, // 3ª coluna
		0.25f, 0.25f, 0.0f, 1.0f // 4ª coluna
	};
	int matrixLocation = glGetUniformLocation(shader_programme, "matrix");
	glUseProgram(shader_programme);
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

	//	#GAME LOOP
	float speed = 1.0f;
	float lastPosition = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}


		static double previousSeconds = glfwGetTime();
		double currentSeconds = glfwGetTime();
		double elapsedSeconds =
			currentSeconds - previousSeconds;
		previousSeconds = currentSeconds;
		if (fabs(lastPosition) > 1.0f) {
			speed = -speed;
		}
		matrix[12] = elapsedSeconds * speed +
			lastPosition;
		lastPosition = matrix[12];
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