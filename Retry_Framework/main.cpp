//include standard headers
#include <iostream>

//include opengl headers
#include <GL\glew.h>
#include <GLFW\glfw3.h>

/////////////
/*Variables*/
/////////////

GLFWwindow* window;

bool CreateWindow();
bool CreateShader();
void RenderTriangle();

//Shader variables
GLchar* vertexShaderSource =  "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shader;
void wait(int forTicks) {
	for (int i = 0; i < forTicks; i++);
}

//Triangle variables
GLuint VBO, VAO, EBO;


float vertices[]{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

int main(void)
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "FAILED TO INITIALIZE GLFW" << std::endl;
		return -1;
	}
	if (!CreateWindow())
	{
		glfwTerminate();
		std::cout << "failed to create window" << std::endl;
		return -1;
	}
	GLenum initEnum = glewInit();
	if (initEnum != GLEW_OK) {
		std::cout << "FAILED TO INITIALIZE GLEW:" << glewGetErrorString(initEnum) << std::endl;
		glfwTerminate();
		return -1;
	}
	

	//setup triangle
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	do
	{
		glClearColor(0.2f, 0.4f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		RenderTriangle();
		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));
	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	wait(100000);
	return 0;

}

bool CreateShader()
{
	int succes;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetProgramiv(vertexShader, GL_LINK_STATUS, &succes);
	if(!succes)
	{
		GLchar* logInfo = "";
		glGetProgramInfoLog(vertexShader, 512, NULL, logInfo);
		std::cout << "failed to link shader" << logInfo << std::endl;
		return false;
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetProgramiv(fragmentShader, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		GLchar* logInfo = "";
		glGetProgramInfoLog(vertexShader, 512, NULL, logInfo);
		std::cout << "failed to link shader" << logInfo << std::endl;
		return false;
	}
	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		GLchar* logInfo = "";
		glGetProgramInfoLog(shader, 512, NULL, logInfo);
		std::cout << "failed to link shader" << logInfo << std::endl;
		return false;
	}
	glDeleteProgram(vertexShader);
	glDeleteProgram(fragmentShader);
	std::cout << "Shader created" << std::endl;
	return true;
}

void RenderTriangle() {
	glUseProgram(shader);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


bool CreateWindow()
{
	window = glfwCreateWindow(1280, 720, "Retry", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	return true;
}