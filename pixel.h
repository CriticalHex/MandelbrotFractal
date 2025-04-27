#ifndef PIXEL_H
#define PIXEL_H

#include <SFML\Graphics.hpp>
#include <complex>
#include <iostream>

class Pixel {
public:
  Pixel(std::complex<long double> mathematicalPosition, sf::Vector2u screenPosition);
  void iterate(sf::Image &image);

private:
  sf::Vector2u screenPosition;
  std::complex<long double> c;
  std::complex<long double> z = std::complex<long double>(0.0, 0.0);
  int iterations = 0;
  bool escaped = false;
  sf::Color color = sf::Color(255, 255, 255);
};

#endif