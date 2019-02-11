# ifndef DKNB_WINDOW_BASE
# define DKNB_WINDOW_BASE

#include <string>
#include <windows.h>
#include <vector>

namespace dknb {

  struct screen_buffer {

    const int width_;
    const int height_;

    /* DIB用変数 */
    BITMAPINFO biDIB_;
    LPDWORD lppxDIB_;

    screen_buffer(int width, int height)
      : width_(width), height_(height)
    {
      /* DIB用BITMAPINFO設定 */
      biDIB_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      biDIB_.bmiHeader.biWidth = width;
      biDIB_.bmiHeader.biHeight = height;
      biDIB_.bmiHeader.biPlanes = 1;
      biDIB_.bmiHeader.biBitCount = 32;
      biDIB_.bmiHeader.biCompression = BI_RGB;

      /* DIB用ピクセル列確保 */
      lppxDIB_ = (LPDWORD) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, width * height * 4);
    }

    void setPixel(int x, int y, COLORREF& color) {
      if (x < 0 || x > width_ || y < 0 || y > height_) return;
      lppxDIB_[x + y * width_] = color;
    }

    void clear(COLORREF c = 0) {
      for (int i = 0; i < width_ * height_; ++i)
        lppxDIB_[i] = c;
    }

    void draw(HDC hdc)
    {
      /* DIB描画 */
      StretchDIBits(hdc, 0, 0, width_, height_,
                  0, 0, width_, height_, lppxDIB_,
                  &biDIB_, DIB_RGB_COLORS, SRCCOPY);
    }
  };

  template <int Width, int Height>
  class window_base {
    std::string _caption;
  protected:
    screen_buffer screen_;

    HWND _hwnd = nullptr;
    HDC _hdc = nullptr;

  public:
    window_base(std::string caption)
      : _caption(caption)
      , screen_(Width, Height)
    { }

    ~window_base()
    { }

    void start(HINSTANCE hInstance) {
      if (_hwnd != nullptr) return;
      _hwnd = createWindow(hInstance);
      msgLoop();
    }

    void setPixel(int x, int y, COLORREF c)
    {
      screen_.setPixel(x, y, c);
    }

    void setLine(int x1, int y1, int x2, int y2, COLORREF color)
    {
      int dx = abs(x1 - x2), dy = abs(y1 - y2);
      int s, step;

      if (dx > dy) {
        if (x1 > x2) {
          step = y1 > y2 ? 1 : -1;
          s = x1; x1 = x2; x2 = s;
          s = y1; y1 = y2; y2 = s;
        } else {
          step = y1 < y2 ? 1 : -1;
        }
        setPixel(x1, y1, color);
        s = dx >> 1;
        while (++x1 <= x2) {
          if ((s = s - dy) < 0) {
            s = s + dx;
            y1 = y1 + step;
          }
          setPixel(x1, y1, color);
        }
      } else { // dx <= dy
        if (y1 > y2) {
          step = x1 > x2 ? 1 : -1;
          s = y1; y1 = y2; y2 = s;
          s = x1; x1 = x2; x2 = s;
        } else {
          step = x1 < x2 ? 1 : -1;
        }
        setPixel(x1, y1, color);
        s = dy >> 1;
        while (++y1 <= y2) {
          if ((s = s - dx) < 0) {
            s = s + dy;
            x1 = x1 + step;
          }
          setPixel(x1, y1, color);
        }
      }
    }

    void draw(HDC hdc) { screen_.draw(hdc); }

    void clear() {
      screen_.clear();
    }

  private:
    // client should override this
    virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

    HWND createWindow(HINSTANCE hInstance) {

      WNDCLASS wcl;
      wcl.hInstance = hInstance;
      wcl.lpszClassName = _caption.c_str();
      wcl.lpfnWndProc = window_base::wndProcWrapper;
      wcl.style = 0;
      wcl.hIcon = NULL;
      wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
      wcl.lpszMenuName = NULL;
      wcl.cbClsExtra = 0;
      wcl.cbWndExtra = 0;
      wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
      if (!RegisterClass(&wcl)) return FALSE;

      RECT _rect;
      _rect.top = _rect.left = 200;
      _rect.right = 200 + Width;
      _rect.bottom = 200 + Height;

      AdjustWindowRect(&_rect, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE | WS_CAPTION, false);

      return CreateWindow(
        _caption.c_str(),      // LPCTSTR lpClassName   クラス名
        _caption.c_str(),      // LPCTSTR lpWindowName ウィンドウ名
        WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE | WS_CAPTION,  // DWORD dwStyle        ウィンドウスタイル
        CW_USEDEFAULT, CW_USEDEFAULT, // ウィンドウ初期表示位置(x, y)
        _rect.right - _rect.left, _rect.bottom - _rect.top,       // int n320, n200  ウィンドウの幅,高さ
        NULL,         // HWND hWndParent      親またはオーナーウィンドウのハンドル
        NULL,         // HMENU hMenu          メニューハンドルまたは子識別子
        hInstance,       // HINSTANCE hInstance  アプリケーションインスタンスのハンドル
        this        // LPVOID lpParam       ウィンドウ作成データ
      );
    }

    WPARAM msgLoop() {
      MSG msg;
      while (GetMessage(&msg,NULL,0,0) > 0){
          TranslateMessage(&msg);
          DispatchMessage(&msg);
      }
      return msg.wParam;
    }

#define ID_MYTIMER 32767

    static LRESULT CALLBACK wndProcWrapper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      window_base* thisPtr = (window_base*)GetWindowLongPtr( hWnd, GWLP_USERDATA );
      if(!thisPtr){
        if(msg == WM_CREATE){
          if(SetTimer(hWnd, ID_MYTIMER, 16, NULL) == 0) { // clock resolution: 4 ms (240fps)
              MessageBox(hWnd,
                (LPCSTR)"failed to SetTimer",
                (LPCSTR)"failed",
                MB_OK);
          }
          thisPtr = (window_base*)((LPCREATESTRUCT)lParam)->lpCreateParams;
          if(thisPtr){
            SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)thisPtr );
          }
        }
      } else {
        return thisPtr->WndProc(  hWnd,  msg,  wParam,  lParam);
      }
      return DefWindowProc( hWnd, msg, wParam, lParam );
    }

#undef ID_MYTIMER

  };

}

# endif
