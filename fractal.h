#ifndef FRACTAL_H
#define FRACTAL_H

#include "pixel.h"
#include <iostream>
#include <unordered_map>
#include <vector>

class Fractal {
private:
  std::unordered_map<long double, Pixel *> map;
  std::vector<Pixel *> pixels;
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
  long double xMin, xMax, yMin, yMax, resolution;
  int width, height;
  void clearPixels();

public:
  void update(int startIndex, int stopIndex);
  void initialize(int width, int height, sf::Color bgColor);
  void zoom(long double scale, sf::Vector2f position);
  void draw(sf::RenderWindow &window);
  ~Fractal();
};

#endif