#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>

namespace starr {

  struct Particle {
    double m_x;
    double m_y;

  private:
    double m_speed;
    double m_direction;
    void init();

  public:

    Particle();
    void update(int interval);
  };

}

#endif
