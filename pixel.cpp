#include "pixel.h"

Pixel::Pixel(std::complex<long double> mathematicalPosition,
             sf::Vector2u screenPosition) {
  c = mathematicalPosition;
  this->screenPosition = screenPosition;
}

void Pixel::iterate(sf::Image &image) {
  if (!escaped) {
    iterations += 1;
    z = pow(z, 2) + c;
    if (abs(z) >= 2) {
      escaped = true;
      color = sf::Color((iterations * 8), (iterations * 6), (iterations * 12));
    }
  }
  image.setPixel(screenPosition.x, screenPosition.y, color);
}