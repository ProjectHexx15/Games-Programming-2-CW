#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <chrono>
#include <string>


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearDisplay(float r, float g, float b, float a);

	float getWidth();
	float getHeight();

	long getCurrentTime();

private:

	void returnError(std::string errorString);
	
	SDL_GLContext glContext; 
	SDL_Window* sdlWindow; 
	float screenWidth;
	float screenHeight;

	// used to calculate the elapsed timne
	std::chrono::high_resolution_clock::time_point startTime;

};

