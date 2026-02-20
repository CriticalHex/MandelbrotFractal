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
  xResolution = 4. / width;
  yResolution = 4. / height;

  // zoom(1., width / 2.f, height / 2.f);
  zoom(.000000000000000625042552437893, 120.000758178092576, 540.);
}

string Fractal::getDataString() {
  long double mathWidth = (xMax - xMin);
  long double mathHeight = (yMax - yMin);

  long double mathCenterX = xMin + (mathWidth / 2);
  long double mathCenterY = yMin + (mathHeight / 2);

  long double screenCenterX = (mathCenterX + 2) * (width / 4);
  long double screenCenterY = (mathCenterY + 2) * (height / 4);

  long double zoomLevel = mathWidth / 4;

  stringstream str;
  str << setprecision(15) << zoomLevel << ", " << screenCenterX << ", "
      << screenCenterY;
  return str.str();
}

void Fractal::screenshot() {
  string fileName = "screenshot" + to_string(screenshotCount) + ".png";
  while (filesystem::exists(fileName)) {
    screenshotCount++;
    fileName = "screenshot" + to_string(screenshotCount) + ".png";
  }
  image.saveToFile(fileName);

  // TODO: Write to a file pairing screenshot name to location (since cannot
  // name file after location for some reason)
}

unsigned int Fractal::getPixelCount() { return pixels.size(); }

void Fractal::zoom(long double scale, long double xPosition,
                   long double yPosition) {
  long double xCenter = (xPosition / width) * (xMax - xMin) + xMin;
  long double yCenter = (yPosition / height) * (yMax - yMin) + yMin;

  long double newWidth = (xMax - xMin) * scale;
  long double newHeight = (yMax - yMin) * scale;

  xMin = xCenter - newWidth / 2.0;
  xMax = xCenter + newWidth / 2.0;
  yMin = yCenter - newHeight / 2.0;
  yMax = yCenter + newHeight / 2.0;
  xResolution = (xMax - xMin) / width;
  yResolution = (yMax - yMin) / height;

  unsigned int screenX, screenY;
  clearPixels();
  for (long double x = xMin; x <= xMax; x += xResolution) {
    for (long double y = yMin; y <= yMax; y += yResolution) {
      screenX = (x - xMin) / (xMax - xMin) * width;
      screenY = (y - yMin) / (yMax - yMin) * height;
      pixels.push_back(new Pixel(complex<long double>(x, y),
                                 sf::Vector2u(screenX, screenY), xResolution));
    }
  }
  cout << pixels.size() << endl;
}

void Fractal::update(int startIndex, int stopIndex) {
  for (int i = startIndex; i < stopIndex; i++) {
    pixels[i]->iterate(image);
  }
}

Fractal::~Fractal() {
  clearPixels();
  // for (auto &iter : map) {
  //   delete iter.second;
  // }
  map.clear();
}

void Fractal::clearPixels() {
  for (Pixel *p : pixels) {
    delete p;
  }
  pixels.clear();
}