#ifndef FRACTAL_H
#define FRACTAL_H

#include "pixel.h"
#include <filesystem>
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
  long double xMin, xMax, yMin, yMax, xResolution, yResolution;
  int width, height;
  int screenshotCount = 0;
  void clearPixels();

public:
  void update(int startIndex, int stopIndex);
  void initialize(int width, int height, sf::Color bgColor);
  void zoom(long double scale, long double xPosition, long double yPosition);
  void draw(sf::RenderWindow &window);
  std::string getDataString();
  void screenshot();
  unsigned int getPixelCount();
  ~Fractal();
};

#endif