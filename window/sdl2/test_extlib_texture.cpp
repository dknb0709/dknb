#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

void logSDLInfo(std::ostream &os, const std::string &msg){
  os << msg << " [info ] " << SDL_GetError() << std::endl;
}

void logSDLError(std::ostream &os, const std::string &msg){
  os << msg << " [error] " << SDL_GetError() << std::endl;
}

namespace dknb {

class renderer {
  SDL_Renderer* p_renderer_;
public:
  renderer(SDL_Window* window) {
    p_renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC);
  }
  virtual ~renderer() {
    SDL_DestroyRenderer(p_renderer_);
  }
  SDL_Texture* load_texture(const std::string& bmpfile) {
    SDL_Texture* p_texture = nullptr;
    SDL_Surface* loaded_image = SDL_LoadBMP(bmpfile.c_str());
    if (loaded_image == nullptr) {
      logSDLError(std::cout, "SDL_LoadBMP");
      return p_texture;
    }
    p_texture = SDL_CreateTextureFromSurface(p_renderer_, loaded_image);
    if (p_texture == nullptr)
      logSDLError(std::cout, "SDL_CreateTextureFromSurface");
    SDL_FreeSurface(loaded_image);
    return p_texture;
  }
  void upload_texture(SDL_Texture* p_texture, int x, int y) {
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    SDL_QueryTexture(p_texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(p_renderer_, p_texture, NULL, &dst);
  }
  void clear() { SDL_RenderClear(p_renderer_); } // clear renderer
  void update() { SDL_RenderPresent(p_renderer_); } // update the screen
};

class window {
  SDL_Window* p_window_;
  dknb::renderer* p_renderer_;
public:
  window(const char* caption, int width, int height) {
    p_window_ = SDL_CreateWindow(caption, 100, 100, width, height, SDL_WINDOW_SHOWN);
    p_renderer_ = new dknb::renderer(p_window_);
  }
  ~window() {
    delete p_renderer_;
    SDL_DestroyWindow(p_window_);
  }
  void upload_texture(const std::string& bmpfile, int x, int y) {
    SDL_Texture* image = p_renderer_->load_texture(bmpfile);
    p_renderer_->upload_texture(image, x, y);
  }
  void clear() { p_renderer_->clear(); }
  void update() { p_renderer_->update(); }
};

} // namespace dknb

int main () {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }
  dknb::window w("test window", 640, 480);
  for (int i = 0; i < 100; ++i) {
    w.clear();
    w.upload_texture("./resource/barbara.bmp", 50 + i, i);
    w.upload_texture("./resource/image.bmp", 150 - i, 100 - i);
    w.update();
    SDL_Delay(30);
  }
  SDL_Quit();
}

