#include "fractal.h"

using namespace std;

void Fractal::draw(sf::RenderWindow &window) {
  texture.loadFromImage(image);
  window.draw(sprite);
}

void Fractal::initialize(int width, int height, sf::Color bgColor) {
  this->width = width;
  this->height = height;
  map.reserve(width * height);
  pixels.reserve(width * height);
  image.create(width, height, bgColor);
  texture.loadFromImage(image);
  sprite.setTexture(texture, true);

  xMin = -2.;
  xMax = -xMin;
  yMin = xMin;
  yMax = -xMin;
  resolution = 4. / width;

  zoom(1., sf::Vector2f(width / 2.f, height / 2.f));
}

void Fractal::zoom(long double scale, sf::Vector2f position) {
  long double xCenter = (position.x / width) * (xMax - xMin) + xMin;
  long double yCenter = (position.y / height) * (yMax - yMin) + yMin;

  long double newWidth = (xMax - xMin) * scale;
  long double newHeight = (yMax - yMin) * scale;

  xMin = xCenter - newWidth / 2.0;
  xMax = xCenter + newWidth / 2.0;
  yMin = yCenter - newHeight / 2.0;
  yMax = yCenter + newHeight / 2.0;
  resolution = (xMax - xMin) / width;

  unsigned int screenX, screenY;
  clearPixels();
  for (long double x = xMin; x <= xMax; x += resolution) {
    for (long double y = yMin; y <= yMax; y += resolution) {
      screenX = (x - xMin) / (xMax - xMin) * width;
      screenY = (y - yMin) / (yMax - yMin) * height;
      pixels.push_back(new Pixel(complex<long double>(x, y),
                                 sf::Vector2u(screenX, screenY)));
    }
  }
}

void Fractal::update(int startIndex, int stopIndex) {
  for (int i = startIndex; i < stopIndex; i++) {
    pixels[i]->iterate(image);
  }
}

Fractal::~Fractal() {
  clearPixels();
  for (auto &iter : map) {
    free(iter.second);
  }
  map.clear();
}

void Fractal::clearPixels() {
  for (Pixel *p : pixels) {
    free(p);
  }
  pixels.clear();
}