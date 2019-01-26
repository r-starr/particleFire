#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Screen.cpp"
#include "Particle.cpp"
#include "Swarm.cpp"

using namespace std;
using namespace starr;

int main() {

  srand(time(NULL));

  Screen screen;

  if(!screen.init()) {
    cout << "Error initializing SDL." << endl;
  }

  Swarm swarm;

  //main game loop
  bool quit = false;
  while(!quit) {

    const Particle * const pParticles = swarm.getParticles();
    int elapsed = SDL_GetTicks();

    swarm.update(elapsed);

    unsigned char red = (unsigned char)((1 + sin(elapsed * 0.0008))*128);
    unsigned char green = (unsigned char)((1 + cos(elapsed * 0.0008))*128);
    unsigned char blue = (unsigned char)((1 + -sin(elapsed * 0.0008))*128);

    for(int i=0; i < Swarm::NPARTICLES; i++) {
      Particle particle = pParticles[i];

      int x = (particle.m_x + 1)*Screen::SCREEN_WIDTH/2;
      int y = (particle.m_y*Screen::SCREEN_WIDTH/2) + Screen::SCREEN_HEIGHT/2;

      screen.setPixel(x, y, red, green, blue);

    }

    screen.boxBlur();

    screen.update(); //updates the texture and the renderer, draws the screen

    if(!screen.processEvents()) {
      quit = true;
    }

  }
  screen.close();

  return 0;
}
