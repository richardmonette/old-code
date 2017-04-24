#include <stdio.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <Windows.h>

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

int main(int argc, char* argv[])
{		
	HWND hWnd = GetConsoleWindow();
	ShowWindow( hWnd, SW_HIDE );

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

	sf::Text fpsInfo;
    fpsInfo.SetString("FPS INFO");
    fpsInfo.SetFont(MyFont);
    fpsInfo.SetColor(sf::Color(255, 255, 255));
    fpsInfo.SetPosition(0.f, 0.f);

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
		fpsInfo.SetString(caption.str());

		bsp.RenderLevel(&camera.position, &renderer, camera.GetViewMatrix());

		img.LoadFromPixels(resX, resY, renderer.GetScreen());
		sprite.SetImage(img);
		renderWindow.Draw(sprite);

		renderWindow.Draw(fpsInfo);
		renderWindow.Display();
	}

	return EXIT_SUCCESS;
}

