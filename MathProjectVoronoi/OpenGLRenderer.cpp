#include "OpenGLRenderer.h"
#include "Landmark.h"
#include "Algorithm.h"

std::vector<std::shared_ptr<DrawableItem>> OpenGLRenderer::DrawableElements;

//MainLoop
bool OpenGLRenderer::ContinueMainLoop;

//Window
GLFWwindow*	OpenGLRenderer::window;
int	OpenGLRenderer::windowWidth = 800;
int	OpenGLRenderer::windowHeight = 600;

//Camera
Camera OpenGLRenderer::MainCamera;
float OpenGLRenderer::CameraSpeed;
float OpenGLRenderer::CameraSensitivity;
float OpenGLRenderer::Depth;

//Mouse
bool OpenGLRenderer::mouseDown;
double OpenGLRenderer::previousMousePosX;
double OpenGLRenderer::previousMousePosY;

//keyboard
std::map<int, bool> OpenGLRenderer::Keys;

//Point manager
PointPool* OpenGLRenderer::PointManager;

void OpenGLRenderer::Initialize(int argc, char* argv[])
{
	OpenGLRenderer::MainCamera = Camera();
	OpenGLRenderer::Depth = 10.0f;
	OpenGLRenderer::MainCamera.setPosition(glm::vec3(0.0f, 0.0f, OpenGLRenderer::Depth));
	OpenGLRenderer::MainCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	OpenGLRenderer::MainCamera.setViewportAspectRatio(OpenGLRenderer::windowWidth / OpenGLRenderer::windowHeight);
	OpenGLRenderer::CameraSpeed = 10.0f;
	OpenGLRenderer::CameraSensitivity = 0.5f;

	glfwInit();

	OpenGLRenderer::window = glfwCreateWindow(OpenGLRenderer::windowWidth, OpenGLRenderer::windowHeight, "Sound Analyzer", NULL, NULL);
	glfwMakeContextCurrent(OpenGLRenderer::window);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	glfwSetKeyCallback(window, KeyboardFunc);
	glfwSetMouseButtonCallback(window, MouseFunc);
	glfwSetCursorPosCallback(window, MotionFunc);
}

void OpenGLRenderer::Start()
{
	DrawableElements.clear();

	GLenum error = glewInit();
	if (error != GL_NO_ERROR) {
		// TODO
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	PointManager = new PointPool;
	OpenGLRenderer::PointManager->Initialize();
	OpenGLRenderer::RegisterElement(std::shared_ptr<DrawableItem>(OpenGLRenderer::PointManager));
	OpenGLRenderer::RegisterElement(std::shared_ptr<DrawableItem>(new Landmark()));

	Time::Start();

	OpenGLRenderer::EnterMainLoop();
}

void OpenGLRenderer::EnterMainLoop()
{
	OpenGLRenderer::ContinueMainLoop = true;
	while (!glfwWindowShouldClose(OpenGLRenderer::window) && OpenGLRenderer::ContinueMainLoop)
	{
		OpenGLRenderer::DisplayFunc();
		glfwSwapBuffers(window);
		glfwPollEvents();
		OpenGLRenderer::UpdateKeyboard();
	}

	for (auto element : OpenGLRenderer::DrawableElements)
	{
		element.get()->Destroy();
	}

	OpenGLRenderer::DrawableElements.clear();

	glfwTerminate();
}

void OpenGLRenderer::ExitMainLoop()
{
	OpenGLRenderer::ContinueMainLoop = false;
}

void OpenGLRenderer::RegisterElement(const std::shared_ptr<DrawableItem> item)
{
	OpenGLRenderer::DrawableElements.push_back(item);
}

void OpenGLRenderer::Close()
{
	OpenGLRenderer::ExitMainLoop();
}

void OpenGLRenderer::DisplayFunc()
{
	int widthWindow;
	int heightWindow;

	Time::Update();

	glfwGetWindowSize(window, &widthWindow, &heightWindow);

	glViewport(0, 0, widthWindow, heightWindow);
	glClearColor(0.f, 0.f, 0.0f, 1.0f);
	glClearDepth(1.F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = MainCamera.projection();
	glm::mat4 World = MainCamera.view();
	glm::mat4 View = glm::mat4(1.0f);


	for (auto element : OpenGLRenderer::DrawableElements)
	{
		element.get()->Draw(Projection,View,World,0);
	}
}

void OpenGLRenderer::MouseFunc(GLFWwindow* window, int button, int state, int mods)
{
	int widthWindow;
	int heightWindow;
	OpenGLRenderer::mouseDown = state != GLFW_RELEASE;
	if (OpenGLRenderer::mouseDown)
	{
		glfwGetCursorPos(window, &OpenGLRenderer::previousMousePosX, &OpenGLRenderer::previousMousePosY);
		glfwGetWindowSize(window, &widthWindow, &heightWindow);
		OpenGLRenderer::PointManager->AddPoint(OpenGLRenderer::previousMousePosX, OpenGLRenderer::previousMousePosY,MainCamera.view() * MainCamera.projection(), glm::vec4(0, 0, widthWindow, heightWindow));
	}
}

void OpenGLRenderer::MotionFunc(GLFWwindow* window, double x, double y)
{
	if (OpenGLRenderer::mouseDown)
	{
		
	}
}

void OpenGLRenderer::KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool Instantkey = false;
	if (action != GLFW_RELEASE)
	{
		switch (key) //touche sans prédiction (juste une frame)
		{
		}
	}
	if(!Instantkey) //Touche avec prédiction
		OpenGLRenderer::Keys[key] = action != GLFW_RELEASE;
}

void OpenGLRenderer::UpdateKeyboard()
{
	for (std::pair<int,bool> keyState : OpenGLRenderer::Keys)
	{
		if (keyState.second)
		{
			switch (keyState.first) //Attention clavier QWERTY
			{
				case(GLFW_KEY_ESCAPE):
					OpenGLRenderer::Close();
					break;
				case(GLFW_KEY_W):
					MainCamera.offsetPosition((float)Time::GetSecondElapsed() * CameraSpeed * MainCamera.forward());
					break;
				case(GLFW_KEY_S):
					MainCamera.offsetPosition((float)Time::GetSecondElapsed() * CameraSpeed * -MainCamera.forward());
					break;
				case(GLFW_KEY_1) :
					Algorithm::MarcheDeJarvis(*PointManager);
					break;
			}
		}
	}
}