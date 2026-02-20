#include "pixel.h"

Pixel::Point::Point(std::complex<long double> position) { c = position; }

void Pixel::Point::iterate() {
  for (int i = 0; !escaped && i < 5; i++) {
    iterations += 1;
    z = pow(z, 2) + c;
    if (abs(z) >= 2) {
      escaped = true;
    }
  }
}

Pixel::Pixel(std::complex<long double> mathematicalPosition,
             sf::Vector2u screenPosition, long double distanceBetweenPixels) {

  this->screenPosition = screenPosition;

  if (SSAA) {
    long double space = distanceBetweenPixels / 2.;
    long double xMin = mathematicalPosition.real() - space;
    long double xMax = mathematicalPosition.real() + space;
    long double yMin = mathematicalPosition.imag() - space;
    long double yMax = mathematicalPosition.imag() + space;
    long double xResolution = (xMax - xMin) / sampleCount;
    long double yResolution = (yMax - yMin) / sampleCount;

    for (long double x = xMin; x <= xMax; x += xResolution) {
      for (long double y = yMin; y <= yMax; y += yResolution) {
        points.push_back(Point(std::complex<long double>(x, y)));
      }
    }
  } else {
    point = Point(mathematicalPosition);
  }
}

void Pixel::iterate(sf::Image &image) {
  if (SSAA) {
    int iterations = 0;
    int escapedPoints = 0;
    for (auto &point : points) {
      point.iterate();
      if (point.escaped) {
        escapedPoints++;
        iterations += point.iterations;
      }
    }
    if (escapedPoints != 0) {
      int averageIterations = iterations / escapedPoints;
      averageColor.r = averageIterations * 8;
      averageColor.g = averageIterations * 6;
      averageColor.b = averageIterations * 12;
    }
  } else {
    point.iterate();
    if (point.escaped) {
      averageColor.r = point.iterations * 8;
      averageColor.g = point.iterations * 6;
      averageColor.b = point.iterations * 12;
    }
  }

  image.setPixel(screenPosition.x, screenPosition.y, averageColor);
}