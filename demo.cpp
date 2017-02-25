/*
 * Aidan Shafran <zelbrium@gmail.com>, 2017.
 * 
 * Demo code for GLLabel. Depends on GLFW3, GLEW, GLM, FreeType2, and C++11.
 * Currently requires Noto Sans and Droid Sans fonts installed.
 * Only tested on Arch Linux.
 */

#include "label.hpp"
#include <glfw3.h>
#include <glm/gtx/transform.hpp>
#include <codecvt>
#include <iomanip>
#include <fstream>

static uint32_t width = 1280;
static uint32_t height = 800;

GLuint fbTex, warpShader, uWarpAmount;
static GLLabel *Label;
static bool spin = false;
static FT_Face defaultFace;
static FT_Face boldFace;
float horizontalTransform = 0.0;
float verticalTransform = 0.8;
float scale = 1;
static float warpAmount = 0.0f;

void onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mods);
void onCharTyped(GLFWwindow *window, unsigned int codePoint, int mods);
void onScroll(GLFWwindow *window, double deltaX, double deltaY);
void onResize(GLFWwindow *window, int width, int height);
std::u32string toUTF32(const std::string &s);
static glm::vec3 pt(float pt);
static GLuint loadShaderProgramFile(const char *vertexShaderPath, const char *fragmentShaderPath);

int main()
{
	// Create a window
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW.\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(width, height, "Vector-Based GPU Text Rendering", NULL, NULL);
	if(!window)
	{
		fprintf(stderr, "Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}
	
	glfwSetKeyCallback(window, onKeyPress);
	glfwSetCharModsCallback(window, onCharTyped);
	glfwSetScrollCallback(window, onScroll);
	glfwSetWindowSizeCallback(window, onResize);
	
	// Create OpenGL context
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW.\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	
	printf("GL Version: %s\n", glGetString(GL_VERSION));

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	// Create new label
	Label = new GLLabel();
	Label->ShowCaret(true);

	printf("Loading font files. If this crashes, you probably don't have Droid Sans and Noto Sans installed at /usr/share/fonts.\n");
	defaultFace = GLFontManager::GetFontManager()->GetDefaultFont();
	boldFace = GLFontManager::GetFontManager()->GetFontFromPath("/usr/share/fonts/TTF/DroidSans-Bold.ttf");
	
	Label->SetText(U"Welcome to vector-based GPU text rendering!\nType whatever you want!\n\nPress LEFT/RIGHT to move cursor.\nPress ESC to toggle rotate.\nScroll vertically/horizontally to move.\nScroll while holding shift to zoom.\nRight-shift for bold.\nHold ALT to type in ", 1, glm::vec4(0.5,0,0,1), defaultFace);
	Label->AppendText(U"r", 1, glm::vec4(0.58, 0, 0.83, 1), defaultFace);
	Label->AppendText(U"a", 1, glm::vec4(0.29, 0, 0.51, 1), defaultFace);
	Label->AppendText(U"i", 1, glm::vec4(0,    0, 1,    1), defaultFace);
	Label->AppendText(U"n", 1, glm::vec4(0,    1, 0,    1), defaultFace);
	Label->AppendText(U"b", 1, glm::vec4(1,    1, 0,    1), defaultFace);
	Label->AppendText(U"o", 1, glm::vec4(1,    0.5, 0,  1), defaultFace);
	Label->AppendText(U"w", 1, glm::vec4(1,    0, 0,    1), defaultFace);
	Label->AppendText(U"!\n", 1, glm::vec4(0.5,0,0,1), defaultFace);
	Label->SetCaretPosition(Label->GetText().size());
	
	GLLabel fpsLabel;

	// Framebuffer for two-pass rendering
	GLuint fb, fbDepth;
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glGenTextures(1, &fbTex);
	glBindTexture(GL_TEXTURE_2D, fbTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbTex, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	float fsQuad[] = {
	   //X, Y,  U, V
		 1, 1,  1, 1,
		 1,-1,  1, 0,
		-1,-1,  0, 0,
		 1, 1,  1, 1,
		-1,-1,  0, 0,
		-1, 1,  0, 1
	};

	GLuint fsQuadBuffer;
	glGenBuffers(1, &fsQuadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fsQuadBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fsQuad), fsQuad, GL_STATIC_DRAW);

	warpShader = loadShaderProgramFile("warpvs.glsl", "warpfs.glsl");
	glUseProgram(warpShader);
	
	GLuint uWarpSampler = glGetUniformLocation(warpShader, "uSampler");
	uWarpAmount = glGetUniformLocation(warpShader, "uAmt");
	GLuint uWarpTime = glGetUniformLocation(warpShader, "uT");
	glUniform1i(uWarpSampler, 0);
	glUniform2f(uWarpAmount, warpAmount, warpAmount);
	glUniform2f(uWarpTime, 0.0f, 0.0f);

	int fpsFrame = 0;
	double fpsStartTime = glfwGetTime();
	while(!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glClearColor(160/255.0, 169/255.0, 175/255.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		if(spin)
		{
			glm::mat4 mat = glm::scale(glm::mat4(),
				glm::vec3(sin(time)/6000.0, cos(time)/12000.0, 1.0));
			mat = glm::rotate(mat, time/3, glm::vec3(0.0,0.0,1.0));
			Label->Render(time, mat);
		}
		else
		{
			glm::mat4 kS = glm::scale(glm::mat4(), pt(8));
			glm::mat4 mat = glm::scale(glm::mat4(), glm::vec3(scale, scale, 1.0));
			mat = glm::translate(mat,
				glm::vec3(-0.9 + horizontalTransform, verticalTransform, 0));
			Label->Render(time, mat*kS);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbTex);
		glBindBuffer(GL_ARRAY_BUFFER, fsQuadBuffer);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
		glUseProgram(warpShader);
		glUniform2f(uWarpTime, time, time);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		// Window size might change, so recalculate this (and other pt() calls)
		glm::mat4 fpsTransform = glm::scale(glm::translate(glm::mat4(), glm::vec3(-1, 0.86, 0)), pt(10));
		fpsLabel.Render(time, fpsTransform);
		
		glfwPollEvents();
		glfwSwapBuffers(window);
		
		// FPS Counter
		fpsFrame ++;
		if(fpsFrame >= 20)
		{
			double endTime = glfwGetTime();
			double fps = fpsFrame / (endTime - fpsStartTime);
			fpsFrame = 0;
			fpsStartTime = endTime;
			
			std::ostringstream stream;
			stream << "FPS: ";
			stream << std::fixed << std::setprecision(1) << fps;
			fpsLabel.SetText(toUTF32(stream.str()), 1, glm::vec4(0,0,0,1), defaultFace);
		}
	}
	
	// Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

static bool leftShift = false;
static bool rightShift = false;

void onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
	if(action == GLFW_PRESS && key == GLFW_KEY_LEFT_SHIFT)
		leftShift = true;
	else if(action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT)
		leftShift = false;
	else if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT_SHIFT)
		rightShift = true;
	else if(action == GLFW_RELEASE && key == GLFW_KEY_RIGHT_SHIFT)
		rightShift = false;

	if(action == GLFW_RELEASE)
		return;
		
	if(key == GLFW_KEY_BACKSPACE)
	{
		std::u32string text = Label->GetText();
		if(text.size() > 0 && Label->GetCaretPosition() > 0)
		{
			Label->RemoveText(Label->GetCaretPosition()-1, 1);
			Label->SetCaretPosition(Label->GetCaretPosition() - 1);
		}
	}
	else if(key == GLFW_KEY_TAB)
	{
		glUseProgram(warpShader);
		if(warpAmount == 0)
			warpAmount = 0.005f;
		else
			warpAmount += 0.02f;
		if(warpAmount >= 0.04)
			warpAmount = 0;
		glUniform2f(uWarpAmount, warpAmount, warpAmount);
	}
	else if(key == GLFW_KEY_ENTER)
	{
		Label->InsertText(U"\n", Label->GetCaretPosition(), 1, glm::vec4(0,0,0,1), rightShift?boldFace:defaultFace);
		Label->SetCaretPosition(Label->GetCaretPosition() + 1);
	}
	else if(key == GLFW_KEY_ESCAPE)
	{
		spin = !spin;
	}
	else if(key == GLFW_KEY_LEFT)
	{
		Label->SetCaretPosition(Label->GetCaretPosition() - 1);
	}
	else if(key == GLFW_KEY_RIGHT)
	{
		Label->SetCaretPosition(Label->GetCaretPosition() + 1);
	}
	else if(key == GLFW_KEY_RIGHT_ALT)
	{	
		Label->SetText(U"Welcome to vector-based GPU text rendering!\nType whatever you want!\n\nPress LEFT/RIGHT to move cursor.\nPress ESC to toggle rotate.\nScroll vertically/horizontally to move.\nScroll while holding shift to zoom.\nRight-shift for bold.\nHold ALT to type in ", 1, glm::vec4(0.5,0,0,1), defaultFace);
		Label->AppendText(U"r", 1, glm::vec4(0.58, 0, 0.83, 1), defaultFace);
		Label->AppendText(U"a", 1, glm::vec4(0.29, 0, 0.51, 1), defaultFace);
		Label->AppendText(U"i", 1, glm::vec4(0,    0, 1,    1), defaultFace);
		Label->AppendText(U"n", 1, glm::vec4(0,    1, 0,    1), defaultFace);
		Label->AppendText(U"b", 1, glm::vec4(1,    1, 0,    1), defaultFace);
		Label->AppendText(U"o", 1, glm::vec4(1,    0.5, 0,  1), defaultFace);
		Label->AppendText(U"w", 1, glm::vec4(1,    0, 0,    1), defaultFace);
		Label->AppendText(U"!\n", 1, glm::vec4(0.5,0,0,1), defaultFace);
		Label->SetCaretPosition(Label->GetText().size());
		horizontalTransform = 0.1;
		verticalTransform = 0.7;
		scale = 1;
		spin = false;
		warpAmount = 0.005f;
		glUseProgram(warpShader);
		glUniform2f(uWarpAmount, warpAmount, warpAmount);
	}
}

void onCharTyped(GLFWwindow *window, unsigned int codePoint, int mods)
{
	double r0 = 0, r1 = 0, r2 = 0;

	if((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
	{
		r0 = ((double) rand() / (RAND_MAX-1));
		r1 = ((double) rand() / (RAND_MAX-1));
		r2 = ((double) rand() / (RAND_MAX-1));
	}

	Label->InsertText(std::u32string(1, codePoint), Label->GetCaretPosition(), 1, glm::vec4(r0,r1,r2,1), rightShift?boldFace:defaultFace);
	Label->SetCaretPosition(Label->GetCaretPosition() + 1);
}

void onScroll(GLFWwindow *window, double deltaX, double deltaY)
{
	if(leftShift)
	{
		scale += 0.1*deltaY;
		if(scale < 0.1)
			scale = 0.1;
	}
	else
	{
		horizontalTransform += 0.1*deltaX/scale;
		verticalTransform -= 0.1*deltaY/scale;
	}
}

void onResize(GLFWwindow *window, int w, int h)
{
	width = w;
	height = h;
	glBindTexture(GL_TEXTURE_2D, fbTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glViewport(0,0,width,height);
}

std::u32string toUTF32(const std::string &s)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(s);
}

// Converts font points into a glm::vec3 scalar.
static glm::vec3 pt(float pt)
{
	static const float dpiScale = 96.0 / 72.0;
	static const float emUnits = 1.0/2048.0;
	const float aspect = (float)height / (float)width;

	float scale = emUnits * pt / 72.0;
	return glm::vec3(scale * aspect, scale, 0);
}

static GLuint loadShaderProgramFile(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	// Compile vertex shader
	std::ifstream vsStream(vertexShaderPath);
	std::string vsCode((std::istreambuf_iterator<char>(vsStream)), (std::istreambuf_iterator<char>()));
	const char *vsCodeC = vsCode.c_str();
	
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vsCodeC, NULL);
	glCompileShader(vertexShaderId);
	
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 1)
	{
		std::vector<char> infoLog(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &infoLog[0]);
		printf("[Vertex] %s\n", &infoLog[0]);
	}
	if(!result)
		return 0;

	// Compile fragment shader
	std::ifstream fsStream(fragmentShaderPath);
	std::string fsCode((std::istreambuf_iterator<char>(fsStream)), (std::istreambuf_iterator<char>()));
	const char *fsCodeC = fsCode.c_str();
	
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fsCodeC, NULL);
	glCompileShader(fragmentShaderId);
	
	result = GL_FALSE, infoLogLength = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 1)
	{
		std::vector<char> infoLog(infoLogLength);
		glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &infoLog[0]);
		printf("[Fragment] %s\n", &infoLog[0]);
	}
	if(!result)
		return 0;
	
	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	result = GL_FALSE, infoLogLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 1)
	{
		std::vector<char> infoLog(infoLogLength+1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &infoLog[0]);
		printf("[Shader Linker] %s\n", &infoLog[0]);
	}
	if(!result)
		return 0;

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}
