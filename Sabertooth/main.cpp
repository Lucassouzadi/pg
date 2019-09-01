#include "System.h"
#include "AssetManager.h"

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

const char* vertex_shader =
"#version 330\n"

"layout(location = 0) in vec3 vp;"
"layout(location = 1) in vec3 vc;"
"layout(location = 2) in vec2 tm;"
"uniform mat4 matrix;" 
"out vec3 color;"
"out vec2 texture_coordinates;"
"void main () {"
"  texture_coordinates = tm;"
"  color = vc;"
"  gl_Position = matrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec2 texture_coordinates;"
"in vec3 color;"
"uniform sampler2D texture;"
"uniform float offsetx;"
"uniform float offsety;"
"out vec4 frag_color;"
"void main () {"
"	vec2 vt = vec2(texture_coordinates.x + offsetx, texture_coordinates.y + offsety);"
"   frag_color = texture (texture, vt) * vec4(color, 1.0);"
//"   frag_color = vec4(color, 1.0);"
"}";

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necess�rio, defini��es espec�ficas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	GLFWwindow *window = glfwCreateWindow(
		640, 480, "Teste de vers�o OpenGL", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// inicia manipulador da extens�o GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	// obten��o de vers�o suportada da OpenGL e renderizador
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL (vers�o suportada) %s\n", version);


	// ###############################################

//	#VBO

	GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// 1
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,		// 2
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	// 3
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// 1
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// 4
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f		// 3
	};

	GLfloat texMap[] = {
		0.0f, 0.0f,							// 1
		0.25f, 0.0f,							// 2
		0.25f, 0.25f,							// 3
		0.0f, 0.0f,							// 1
		0.0f, 0.25f,								// 4
		0.25f, 0.25f								// 3
	};

	GLuint verticeVBO = 0;
	GLuint texVBO = 0;

	glGenBuffers(1, &verticeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texMap), texMap, GL_STATIC_DRAW);

//	#VBO

//	#VAO

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, verticeVBO); // identifica vbo atual
	// habilitado primeiro atributo do vbo bound atual
	glEnableVertexAttribArray(0);
	// associa��o do vbo atual com primeiro atributo
	// 0 identifica que o primeiro atributo est� sendo definido
	// 3, GL_FLOAT identifica que dados s�o vec3 e est�o a cada 3 float.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	// � poss�vel associar outros atributos, como normais, mapeamento e cores
	// lembre-se: um por v�rtice!

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

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
		1.0f, 0.0f, 0.0f, 0.0f, // 1� coluna
		0.0f, 1.0f, 0.0f, 0.0f, // 2� coluna
		0.0f, 0.0f, 1.0f, 0.0f, // 3� coluna
		0.0f, 0.0f, 0.0f, 1.0f  // 4� coluna
	};
	//	#salva a matriz como uniform no program
	int matrixLocation = glGetUniformLocation(shader_program, "matrix");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

//	#SHADER PROGRAM

//	#TEXTURA

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int imgWidth, imgHeight;
	unsigned char *image;
	image = SOIL_load_image("bin/Images/carrito.png", &imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
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

	// Para desenhar com alfa, na inicializa��o da aplica��o, habilitar o alpha test :
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	int fps = 10;

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

		//transforma��o
		matrix[12] = positionX;
		matrix[13] = positionY;
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);

		//sprite
		frameAtual = currentSeconds*fps;
		acaoAtual = frameAtual / 4;
		textureOffsetX = frameAtual * frameWidth;
		textureOffsetY = acaoAtual * frameHeight;
		glUniform1f(textureOffsetXLocation, textureOffsetX);
		glUniform1f(textureOffsetYLocation, textureOffsetY);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Define vao como vertex array atual
		glBindVertexArray(VAO);
		// desenha pontos a partir do p0 e 3 no total do VAO atual com o shader
		// atualmente em uso
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
	}

	// ###############################################

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}