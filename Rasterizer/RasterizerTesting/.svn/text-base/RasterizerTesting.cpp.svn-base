#include <stdio.h>
#include <tchar.h>
#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Q3/Q3BSP.h"
#include "Math3D.h"
#include "Renderer.h"
#include "Timer.h"

Camera camera;

void UpdateCameraRotation(int x, int y)
{
	camera.RotateY(x * 0.1f);
	camera.RotateX(y * -0.1f);
}

int _tmain(int argc, _TCHAR* argv[])
{	
	//0 -0.67766052 0.73537487
	//-0.18665533 -0.67766052 -0.71129179
	//0.36108494 -0.67766052 0.64061987
	//-0.51186395 -0.67766052 -0.52798820
	//0.62911642 -0.67766052 0.38077384

	//0.00000000  0.98328865 0.18205334
	//-0.046209391  0.98328865  -0.17609121
	//0.089392126  0.98328865  0.15859529
	//-0.12671977  0.98328865 -0.13071159
	//0.15574743  0.98328865  0.094266415

	//0.72932923  0.67766052 -0.094101280
	//-0.68155903  0.67766052  0.27614030
	//0.58914751  0.67766052  -0.44009250
	//-0.45814750  0.67766052  0.57521915
	//0.29713941  0.67766052  -0.67266959

	//0.18055665 -0.98328865 -0.023296218
	//-0.16873041  -0.98328865  0.068362772
	//0.14585252  -0.98328865  -0.10895166
	//-0.11342145 -0.98328865 0.14240433
	//0.073561423 -0.98328865 -0.16652969
	
	ostringstream caption;

	sf::VideoMode videoMode(resX, resY, 32);
	const char *name = "Rasterizer";
	sf::RenderWindow renderWindow(videoMode, name);
	renderWindow.UseVerticalSync(false);
	renderWindow.SetFramerateLimit(0);

	Renderer renderer;
	Q3BSP bsp;

	bsp.LoadBSP("..\\Content\\shadertest.bsp", &renderer);
	//bsp.LoadBSP("C:\\Documents and Settings\\Administrator\\My Documents\\Visual Studio 2008\\Projects\\RasterizerTesting\\Content\\shadertest.bsp", &renderer);

	sf::Font MyFont;
    if (!MyFont.LoadFromFile("..\\Content\\arial.ttf"))
        return EXIT_FAILURE;

	sf::Clock clock;

	sf::String fpsInfo;
    /*fpsInfo.SetText("FPS INFO");
    fpsInfo.SetFont(MyFont);
	fpsInfo.SetSize(32);
    fpsInfo.SetColor(sf::Color(255, 255, 255));
    fpsInfo.SetPosition(0.f, 0.f);*/

    const sf::Input& Input = renderWindow.GetInput();

	renderer.SetCamera(&camera);

	bool isMouseDown = false;
	int oldMouseX = 0;
	int oldMouseY = 0;

	sf::Image img;
	sf::Sprite sprite;

	const sf::Color black(0,0,0);
	sf::Event Event;

	while (renderWindow.IsOpened())
    {
        while (renderWindow.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                renderWindow.Close();

            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                renderWindow.Close();
        }

		if (Input.IsKeyDown(sf::Key::W)) camera.MoveForward();
		if (Input.IsKeyDown(sf::Key::S)) camera.MoveBackward();
		if (Input.IsKeyDown(sf::Key::D)) camera.MoveRight();
		if (Input.IsKeyDown(sf::Key::A)) camera.MoveLeft();

		if(Input.IsMouseButtonDown(sf::Mouse::Left)) 
		{ 
			if (!isMouseDown)
			{
				isMouseDown = true;

				oldMouseX = Input.GetMouseX();
				oldMouseY = Input.GetMouseY();
			}
		}

		if(!Input.IsMouseButtonDown(sf::Mouse::Left)) 
		{ 
			isMouseDown = false;
		}

		if ( isMouseDown )
		{
			UpdateCameraRotation(oldMouseX - Input.GetMouseX(), oldMouseY - Input.GetMouseY());

			oldMouseX = Input.GetMouseX();
			oldMouseY = Input.GetMouseY(); 
		}

		camera.Update();

		renderer.ClearFrame();
		renderWindow.Clear(black);

		caption.str("");
		caption.clear();
		caption << (int)(clock.GetElapsedTime() * 1000.0f) << "ms"; 
		clock.Reset();
		//fpsInfo.SetText(caption.str());

		bsp.RenderLevel(&camera.position, &renderer, camera.GetViewMatrix());

		img.LoadFromPixels(resX, resY, renderer.GetScreen());
		sprite.SetImage(img);
		renderWindow.Draw(sprite);

		//renderWindow.Draw(fpsInfo);
		renderWindow.Display();
	}

	return EXIT_SUCCESS;
}

