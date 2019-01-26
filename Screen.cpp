#include "Screen.h"

namespace starr {

  Screen::Screen() :
      m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL), m_buffer2(NULL) {

  }
  bool Screen::init() {

      //checks to see if SDL initializes properly
      if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
      }

      //creates the object for the application window, produces a pointer to it
      m_window = SDL_CreateWindow("Particle Fire Explosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

      // checks to make sure that the window was created properly
      if(m_window == NULL) {
        SDL_GetError();
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
      }

      //creates the texture and the renderer, renderer is created first
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
      m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

      //checks to see if renderer was created correctly
      if(m_renderer == NULL) {
        SDL_GetError();
        SDL_Quit();
        return false;
      }

      //checks to see if texture was created correctly
      if(m_texture == NULL) {
        SDL_GetError();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
      }

      //allocates a memory block in which we will store the information about each color pixel
      m_buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
      m_buffer2 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];

      //sets the value of blocks of memory in the buffer to the RGB value for white, 3rd parameter specifies how many blocks
      memset(m_buffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));

      for(int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        m_buffer[i] = 0x000000ff;
      }

    return true;
  }

  void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

    if(x < 0 || x>=SCREEN_WIDTH || y < 0 || y>=SCREEN_HEIGHT) {
      return;
    }

    Uint32 color = 0;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xff;

    m_buffer[(y*SCREEN_WIDTH)+x] = color;
  }

  void Screen::update() {
    //updates the texture and the renderer so that it will appear onscreen
    SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH*sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
  }

  void Screen::boxBlur() {
    //swap the buffer locations
    Uint32 *temp = m_buffer;
    m_buffer = m_buffer2;
    m_buffer2 = temp;

    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      for(int x = 0; x < SCREEN_WIDTH; x++) {

        int redTotal = 0;
        int greenTotal = 0;
        int blueTotal = 0;

        for(int row = -1; row <= 1; row++){
          for(int col = -1; col <= 1; col++) {
            int currentX = x+col;
            int currentY = y+row;

            if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
              Uint32 color = m_buffer2[currentY*SCREEN_WIDTH + currentX];
              Uint8 red = color >> 24;
              Uint8 green = color >> 16;
              Uint8 blue = color >> 8;

              redTotal += red;
              greenTotal += green;
              blueTotal += blue;
            }
          }
        }

        Uint8 red = redTotal/9;
        Uint8 green = greenTotal/9;
        Uint8 blue = blueTotal/9;

        setPixel(x, y, red, green, blue);
      }
    }

  }

  bool Screen::processEvents() {
    //object that records events
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      //processes events
      if(event.type == SDL_QUIT) { //if user raises quit event
        return false;
      }

    }

    return true;
  }

  void Screen::close() {

      //destroy allocated memory before ending program
      delete [] m_buffer;
      delete [] m_buffer2;
      SDL_DestroyRenderer(m_renderer);
      SDL_DestroyTexture(m_texture);
      SDL_DestroyWindow(m_window);
      SDL_Quit();

  }

}
