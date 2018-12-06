#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h> // -lSDL2_image

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
  renderer(SDL_Window* window)
    : p_renderer_( SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC) )
  { }
  virtual ~renderer() {
    SDL_DestroyRenderer(p_renderer_);
  }
  SDL_Texture* load_texture(const std::string& imgfile) {
    SDL_Texture* p_texture = IMG_LoadTexture(p_renderer_, imgfile.c_str());
    if (p_texture == nullptr) logSDLError(std::cout, "IMG_LoadTexture");
    return p_texture;
  }
  void render_clipped_texture(SDL_Texture* p_texture, SDL_Rect dst, SDL_Rect* clip = nullptr) {
    SDL_RenderCopy(p_renderer_, p_texture, clip, &dst);
  }
  void render_clipped_texture(SDL_Texture* p_texture, int x, int y, SDL_Rect* clip = nullptr) {
    SDL_Rect dst;
    dst.x = x; dst.y = y;
    if (clip != nullptr) {
      dst.w = clip->w;
      dst.h = clip->h;
    } else {
      SDL_QueryTexture(p_texture, NULL, NULL, &dst.w, &dst.h);
    }
    render_clipped_texture(p_texture, dst, clip);
  }
  void render_texture(SDL_Texture* p_texture, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x; dst.y = y; dst.w = w; dst.h = h;
    SDL_RenderCopy(p_renderer_, p_texture, NULL, &dst);
  }
  void render_texture(SDL_Texture* p_texture, int x, int y) {
    int w, h;
    SDL_QueryTexture(p_texture, NULL, NULL, &w, &h);
    render_texture(p_texture, x, y, w, h);
  }
  virtual void render(int x, int y, int seq_no) {}
  void clear() { SDL_RenderClear(p_renderer_); } // clear renderer
  void update() { SDL_RenderPresent(p_renderer_); } // update the screen
};

template <size_t N>
class sprite_renderer : public dknb::renderer {
  SDL_Texture* p_texture_;
  SDL_Rect clips_[N];
  struct sprite {
    int top, w, h;
  };
  sprite sprite_;
public:
  sprite_renderer(SDL_Window* window, char const* imgfile, sprite src)
    : dknb::renderer(window)
    , p_texture_(load_texture(imgfile))
    , sprite_(src)
  {
    for (int i = 0; i < N; ++i) {
      clips_[i].x = sprite_.w * i;
      clips_[i].y = sprite_.top;
      clips_[i].w = sprite_.w;
      clips_[i].h = sprite_.h;
    }
  }
  virtual void render(int x, int y, int seq_no) override {
    if (seq_no < 0 || seq_no > N - 1) return;
    render_clipped_texture(p_texture_, x, y, &clips_[seq_no]);
  }
};

class window {
  SDL_Window* p_window_;
  dknb::renderer* p_renderer_;
public:
  window(const char* caption, int width, int height) {
    p_window_ = SDL_CreateWindow(caption, 100, 100, width, height, SDL_WINDOW_SHOWN);
    p_renderer_ = new dknb::sprite_renderer<6>(p_window_, "./resource/i8aic.png", { 100, 72, 72 });
  }
  ~window() {
    delete p_renderer_;
    SDL_DestroyWindow(p_window_);
  }
  void render_sprite(int x, int y, int seq_no) {
    p_renderer_->render(x, y, seq_no);
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

  int seq = 0;

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {

      w.clear();

      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.type == SDL_KEYDOWN) {
        logSDLInfo(std::cout, "keydown");
        seq++;
        std::cout << seq << std::endl;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        logSDLInfo(std::cout, "mousebuttondown");
      }

      w.render_sprite(320, 240, seq % 6);
      w.update();
    }
    SDL_Delay(10);
  }
  // SDL_Delay(30);
  SDL_Quit();
}

