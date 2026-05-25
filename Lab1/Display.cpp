#include "Display.h"


Display::Display()
{
	startTime = std::chrono::high_resolution_clock::now();
	sdlWindow = nullptr; 
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

Display::~Display()
{
	// Cleans up the SLD/OpenGL resources 
	SDL_GL_DeleteContext(glContext); 
	SDL_DestroyWindow(sdlWindow); 
	SDL_Quit();
}

// Getters
float Display::getWidth() { return screenWidth; } 
float Display::getHeight() { return screenHeight; }

void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any escape to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Display::swapBuffer()
{
	SDL_GL_SwapWindow(sdlWindow); 
}

void Display::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); 

	// configure the framebuffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  

	// create an sdl window
	sdlWindow = SDL_CreateWindow("Amie's Super Awesome Coursework!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); 
	
	if (sdlWindow == nullptr)
	{
		returnError("window failed to create");
	}

	// attaches an opengl contect to the window
	glContext = SDL_GL_CreateContext(sdlWindow);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}


	glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	// enaable depth test & face culling
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE); 

}

long Display::getCurrentTime()
{
	// calculate current time, to determine the cameras movement
	auto now = std::chrono::high_resolution_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
	return static_cast<long>(sec);

}