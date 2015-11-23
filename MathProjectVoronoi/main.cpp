#include <stdio.h>

//#include "LinkerVS.h"
#include "OpenGLRenderer.h"

int main(int argc, char **argv)
{
	OpenGLRenderer::Initialize(argc, argv);
	OpenGLRenderer::Start();
	//system("pause");
}