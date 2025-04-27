#include "fractal.h"
#include <list>
#include <thread>

typedef const unsigned int cuint;

void updateSection(Fractal &fractal, int start, int stop) {
  fractal.update(start, stop);
}

void updateFractal(cuint threadCount, cuint baseSectionSize,
                   cuint remainingSectionSize,
                   std::__cxx11::list<std::thread> &threads, Fractal &fractal) {
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
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mandelbrot Set",
                          sf::Style::None);
  window.setPosition(sf::Vector2i(460, 20));
  sf::Event event;
  sf::Color bgColor = sf::Color(8, 6, 12);
  cuint windowWidth = window.getSize().x;
  cuint windowHeight = window.getSize().y;
  cuint pixelCount = windowWidth * windowHeight;

  Fractal fractal;
  fractal.initialize(windowWidth, windowHeight, bgColor);
  std::list<std::thread> threads;
  cuint threadCount = 12;
  cuint baseSectionSize = pixelCount / threadCount;
  cuint remainingSectionSize = pixelCount % threadCount;

  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        window.close();
      }

      if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2f mousePosition(event.mouseWheelScroll.x,
                                   event.mouseWheelScroll.y);
        if (event.mouseWheelScroll.delta > 0) {
          fractal.zoom(.5, mousePosition);
        } else if (event.mouseWheelScroll.delta < 0) {
          fractal.zoom(2., mousePosition);
        }
        // 60.5379, 495.55110165
        // 567.844982197, 498.716377632
        // 590.060110859, 339.671734734 from video
        // 62.5003972516, 500 other video, probably best zoom location i have
      }
    }

    updateFractal(threadCount, baseSectionSize, remainingSectionSize, threads,
                  fractal);

    window.clear(bgColor);

    fractal.draw(window);

    window.display();
  }

  return 0;
}
