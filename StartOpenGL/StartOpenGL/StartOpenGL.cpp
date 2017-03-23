#include "CreateWindow.h"

int main() {
	CreateWindow* window =new CreateWindow();
	window->InitialWindow();

	delete window;
}