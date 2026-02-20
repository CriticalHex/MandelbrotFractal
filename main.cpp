#include "fractal.h"
#include <list>
#include <thread>

typedef const unsigned int cuint;

void updateSection(Fractal &fractal, int start, int stop) {
  fractal.update(start, stop);
}

void updateFractal(cuint threadCount, std::__cxx11::list<std::thread> &threads,
                   Fractal &fractal) {
  cuint pixelCount = fractal.getPixelCount();
  cuint baseSectionSize = pixelCount / threadCount;
  cuint remainingSectionSize = pixelCount % threadCount;
  for (int i = 0; i < threadCount - 1; i++) {
    threads.emplace_back(updateSection, std::ref(fractal), baseSectionSize * i,
                         baseSectionSize * (1 + i));
  }
  threads.emplace_back(updateSection, std::ref(fractal),
                       baseSectionSize * (threadCount - 1),
                       baseSectionSize * threadCount + remainingSectionSize);

  for (auto &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  threads.clear();
}

int main() {
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrot Set",
                          sf::Style::None);
  // window.setPosition(sf::Vector2i(460, 20));
  sf::Event event;
  sf::Color bgColor = sf::Color(8, 6, 12);
  cuint windowWidth = window.getSize().x;
  cuint windowHeight = window.getSize().y;

  Fractal fractal;
  fractal.initialize(windowWidth, windowHeight, bgColor);
  std::list<std::thread> threads;
  cuint threadCount = 12;

  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        window.close();
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        std::cout << "To get here again: " << fractal.getDataString()
                  << std::endl;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        fractal.screenshot();
      }

      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
          fractal.zoom(.5, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
        } else if (event.mouseWheelScroll.delta < 0) {
          fractal.zoom(2., event.mouseWheelScroll.x, event.mouseWheelScroll.y);
        }
        // 60.5379, 495.55110165
        // 567.844982197, 498.716377632
        // 590.060110859, 339.671734734
        // 62.5003972516, 500
        // all above invalidated by fullscreen :(
        // .000000000000000625042552437893, 120.000758171, 540, screenshot 6
        // (best location)
      }
    }

    updateFractal(threadCount, threads, fractal);

    window.clear(bgColor);

    fractal.draw(window);

    window.display();
  }

  return 0;
}
