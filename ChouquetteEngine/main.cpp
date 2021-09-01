#include <iostream>
#include <exception>

#include "ChouquetteEngine.h"



int main(int argc, char* argv[])
{
	ChouquetteEngine app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
	return 0;
}