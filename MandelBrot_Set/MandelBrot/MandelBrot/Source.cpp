#include "SFML/Graphics.hpp"
#include<iostream>
#include<windows.h>
#include <tchar.h>
using namespace std;
//resolution of the window
const int width = 1280;
const int height = 720;

//used for complex numbers
struct complex_number
{
	long double real;
	long double imaginary;
};

void generate_mandelbrot_set(sf::VertexArray& vertexarray, int pixel_shift_x, int pixel_shift_y, int precision, float zoom)
//void generate_mandelbrot_set(sf::VertexArray& vertexarray, int pixel_shift_x, int pixel_shift_y, int precision)
{
#pragma omp parallel for
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//scale the pixel location to the complex plane for calculations
			//long double x = ((long double)j - pixel_shift_x);
			//long double y = ((long double)i - pixel_shift_y);
			long double x = ((long double)j - pixel_shift_x) / zoom;
			long double y = ((long double)i - pixel_shift_y) / zoom;
			complex_number c;
			c.real = x;
			c.imaginary = y;
			complex_number z = c;
			int iterations = 0; //keep track of the number of iterations
			for (int k = 0; k < precision; k++)
			{
				complex_number z2;
				z2.real = z.real * z.real - z.imaginary * z.imaginary;
				z2.imaginary = 2 * z.real * z.imaginary;
				z2.real += c.real;
				z2.imaginary += c.imaginary;
				z = z2;
				iterations++;
				if (z.real * z.real + z.imaginary * z.imaginary > 4)
					break;
			}
			//color pixel based on the number of iterations
			if (iterations < precision / 4.0f)
			{
				vertexarray[i*width + j].position = sf::Vector2f(j, i);
				sf::Color color(iterations * 255.0f / (precision / 4.0f), 0, 0);
				vertexarray[i*width + j].color = color;
			}
			else if (iterations < precision / 2.0f)
			{
				vertexarray[i*width + j].position = sf::Vector2f(j, i);
				sf::Color color(0, iterations * 255.0f / (precision / 2.0f), 0);
				vertexarray[i*width + j].color = color;
			}
			else if (iterations < precision)
			{
				vertexarray[i*width + j].position = sf::Vector2f(j, i);
				sf::Color color(0, 0, iterations * 255.0f / precision);
				vertexarray[i*width + j].color = color;
			}
		}
	}
}

int main()
{
	sf::String title_string = "Mandelbrot Set Plotter";
	sf::RenderWindow window(sf::VideoMode(width, height), title_string);
	window.setFramerateLimit(30);
	sf::VertexArray pointmap(sf::Points, width * height);

	float zoom = 300.0f;
	int precision = 100;
	int x_shift = width / 2;
	int y_shift = height / 2;

	_int64 ctr1 = 0, ctr2 = 0, freq = 0;
	if (QueryPerformanceCounter((LARGE_INTEGER *)&ctr1) != 0)
	{
		
		QueryPerformanceCounter((LARGE_INTEGER *)&ctr2);
		std::cout << "Start " << ctr1 << std::endl;
		std::cout << "End " << ctr2 << std::endl;
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		std::cout << "ctr1 - ctr2 = " << ctr1 - ctr2 << endl;
		std::cout << "QueryPerformanceCounter minimum resolution : 1/ " << freq << "seconds" << std::endl;
		std::cout << "Function takes time: " << ((ctr2 - ctr1) * 1.0 / freq) * 1000000 << " Microseconds." << std::endl;
		std::cout << endl;

		//generate_mandelbrot_set(pointmap, x_shift, y_shift, precision);
		generate_mandelbrot_set(pointmap, x_shift, y_shift, precision, zoom);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			//zoom into area that is left clicked
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i position = sf::Mouse::getPosition(window);
				x_shift -= position.x - x_shift;
				y_shift -= position.y - y_shift;
				zoom *= 2;
				precision += 200;
#pragma omp parallel for
				for (int i = 0; i < width*height; i++)
				{
					pointmap[i].color = sf::Color::Black;
				}
				generate_mandelbrot_set(pointmap, x_shift, y_shift, precision,zoom);
			}
			window.clear();
			window.draw(pointmap);
			window.display();
		}
	}
	else
	{
		DWORD dwError = GetLastError();
		std::cout << "Error vaflue = " << dwError << std::endl;
	}
	system("PAUSE");

	return 0;
}
