#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <array>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // -lSDL2_image

enum Event {
  EvNoInput = 0,
  EvMoveRight,
  EvMoveLeft
};

class state_base {
public:
  virtual void process_event(Event ev) = 0;
  virtual char const* to_string() const {
    return "[warn]not implemented";
  }
};

template <class Derived>
class state : public state_base {
public:
  static Derived* get_instance() {
    static Derived instance;
    return &instance;
  }
};

class stopped : public state<stopped> {
  friend class state<stopped>;
  stopped(){}
public:
  virtual void process_event(Event ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "stopped";
  }
};

class running_right : public state<running_right> {
  friend class state<running_right>;
  running_right(){}
public:
  virtual void process_event(Event ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "running_right";
  }
};

template <size_t NumEvents>
class state_machine {

  state_base* current_;
  
  /**
   * A hierarchy should be constructed with the
   * following two variables as a unit
   * so that it can deal with adding
   * states such as power on/off
   **/
  std::array<int, NumEvents> events_;
  std::map<state_base*, std::array<state_base*, NumEvents>> trans_;

  state_base* resolve_next(int ev) {
    return trans_[current_][events_[ev]];
  }
  void process_event(Event ev) {
    current_->process_event(ev);
  }
public:
  state_machine(
      state_base* initial,
      std::array<int, NumEvents> events,
      std::map<state_base*, std::array<state_base*, NumEvents>> trans)
      : current_(initial)
      , events_(events)
      , trans_(trans)
  { }
  void dispatch_event(Event ev) {
    state_base* prev = current_;
    current_ = resolve_next(ev);
    if (prev != current_) {
      std::cout << "switch state: "
                << prev->to_string()
                << " => "
                << current_->to_string()
                << std::endl;
    }
    process_event(ev);
  }
};

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

class sprite_renderer : public dknb::renderer {
  SDL_Texture* p_texture_;
  std::vector<SDL_Rect> clips_;
public:
  sprite_renderer(SDL_Window* window, char const* imgfile, std::vector<SDL_Rect> clips)
    : dknb::renderer(window)
    , p_texture_(load_texture(imgfile))
    , clips_(clips)
  { }
  virtual void render(int x, int y, int seq_no) override {
    if (seq_no < 0 || seq_no > clips_.size() - 1) return;
    render_clipped_texture(p_texture_, x, y, &clips_[seq_no]);
  }
};

class window {
  SDL_Window* p_window_;
  dknb::renderer* p_renderer_;
public:
  window(const char* caption, int width, int height) {
    p_window_ = SDL_CreateWindow(caption, 100, 100, width, height, SDL_WINDOW_SHOWN);
    p_renderer_ = new dknb::sprite_renderer(p_window_,
        "./resource/i8aic_running.png",
        {
          // x, y,  w,  h
          { 72 * 0, 0, 72, 72 },
          { 72 * 1, 0, 72, 72 },
          { 72 * 2, 0, 72, 72 },
          { 72 * 3, 0, 72, 72 },
          { 72 * 4, 0, 72, 72 },
          { 72 * 5, 0, 72, 72 }
        });
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
        seq++;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        logSDLInfo(std::cout, "mousebuttondown");
      }
      w.render_sprite(320, 240, seq % 6);
      w.update();
    }
    SDL_Delay(10);
  }
  SDL_Quit();
}

