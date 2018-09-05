# ifndef DKNB_WINDOW_BASE
# define DKNB_WINDOW_BASE

#include <array> // delete

#include <string>
#include <windows.h>

#include "util.hpp"

namespace dknb {
namespace graphics {

  template <int _width, int _height>
  class window {
    std::string _caption;
    std::array<std::array<COLORREF, _height>, _width> _data;

    HWND _hwnd = nullptr;
    HDC _hdc = nullptr;

  public:
    window(std::string caption)
      : _caption(caption)
    {
      for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
          _data[x][y] = RGB(0, 0, 0x50);
        }
      }
    }

    ~window()
    {
    }

    void start(HINSTANCE hInstance) {
      if (_hwnd != nullptr) return;
      _hwnd = createWindow(hInstance);
      msgLoop();
    }

    void draw() {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(_hwnd, &ps);
      // 描画処理の実装ここから
      for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
          gr_dot(hdc, x, y, _data[x][y]);
        }
      }
      // 描画処理の実装ここまで
      EndPaint(_hwnd, &ps);
    }

  private:

    HWND createWindow(HINSTANCE hInstance) {

      WNDCLASS wcl;
      wcl.hInstance = hInstance;
      wcl.lpszClassName = _caption.c_str();
      wcl.lpfnWndProc = window::wndProcWrapper;
      wcl.style = 0;
      wcl.hIcon = NULL;
      wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
      wcl.lpszMenuName = NULL;
      wcl.cbClsExtra = 0;
      wcl.cbWndExtra = 0;
      wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
      if (!RegisterClass(&wcl)) return FALSE;

      RECT _rect;
      _rect.top = _rect.left = 0;
      _rect.right = _width;
      _rect.bottom = _height;

      AdjustWindowRectEx(&_rect, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, false, 0);

      return CreateWindow(
        _caption.c_str(),      // LPCTSTR lpClassName   クラス名
        _caption.c_str(),      // LPCTSTR lpWindowName ウィンドウ名
        WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,  // DWORD dwStyle        ウィンドウスタイル
        _width, _height,       // int n320, n200  ウィンドウの幅,高さ
        _rect.right - _rect.left,
        _rect.bottom - _rect.top,      // int x, y             ウィンドウの横,縦方向の位置
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

    static LRESULT CALLBACK wndProcWrapper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      window* thisPtr = (window*)GetWindowLongPtr( hWnd, GWLP_USERDATA );
      if(!thisPtr){
        if(msg==WM_CREATE){
          thisPtr = (window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
          if(thisPtr){
            SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)thisPtr );
          }
        }
      } else {
        return thisPtr->WndProc(  hWnd,  msg,  wParam,  lParam);
      }
      return DefWindowProc( hWnd, msg, wParam, lParam );
    }

    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      switch( msg )
      {
        case WM_PAINT:
          draw();
          break;
        case WM_DESTROY:
          PostQuitMessage(0);
          break;
        default:
          return DefWindowProc(hWnd, msg, wParam, lParam);
      }
      return DefWindowProc( hWnd, msg, wParam, lParam );
    }

  };

}
}

# endif
