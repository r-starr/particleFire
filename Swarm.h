#ifndef SWARM_H
#define SWARM_H

#include <SDL2/SDL.h>
#include "Particle.h"

namespace starr {

  class Swarm {
  private:
    Particle *m_pParticles;
    int lastTime;
  public:

    const static int NPARTICLES = 5000;

    Swarm();
    ~Swarm();
    const Particle * const getParticles() { return m_pParticles; };
    void update(int elapsed);
  };
}

#endif
