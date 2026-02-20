#ifndef PIXEL_H
#define PIXEL_H

#include <SFML\Graphics.hpp>
#include <complex>
#include <iostream>

class Pixel {
public:
  Pixel(std::complex<long double> mathematicalPosition,
        sf::Vector2u screenPosition, long double distanceBetweenPixels);
  void iterate(sf::Image &image);
  // ~Pixel();

private:
  struct Point {
    Point(std::complex<long double> position);
    std::complex<long double> c;
    std::complex<long double> z = std::complex<long double>(0.0, 0.0);
    int iterations = 0;
    bool escaped = false;
    void iterate();
  };
  const bool SSAA = false;
  const int sampleCount = 1;
  sf::Vector2u screenPosition;
  sf::Color averageColor = sf::Color::White;
  std::vector<Point> points;
  Point point = Point(std::complex<long double>(0.0, 0.0));
};

#endif