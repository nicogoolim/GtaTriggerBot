#pragma once
#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <thread>
struct RGB {
    int r;
    int g;
    int b;
};
class   CaptureScreen
{
public: CaptureScreen()
{   // Контексты:
    ScreenDC = GetDC(0);
    MemoryDC = CreateCompatibleDC(ScreenDC);

    // Размер экрана:
    screen_x = GetSystemMetrics(SM_CXSCREEN);
    screen_y = GetSystemMetrics(SM_CYSCREEN);

    // Заполнение структуры формата                         :
    BMI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BMI.bmiHeader.biWidth = screen_x;
    // Отрицательное, чтобы изображение не было перевёрнутым:
    BMI.bmiHeader.biHeight = -screen_y;
    BMI.bmiHeader.biSizeImage = screen_x * screen_y * 3;
    BMI.bmiHeader.biCompression = BI_RGB;
    BMI.bmiHeader.biBitCount = 24;
    BMI.bmiHeader.biPlanes = 1;

    hBitmap = CreateDIBSection(ScreenDC,
        &BMI,
        DIB_RGB_COLORS,
        (void**)&buf, 0, 0);
}
      ~CaptureScreen()
      {
          DeleteDC(MemoryDC);
          ReleaseDC(NULL, ScreenDC);
          DeleteObject(hBitmap);
      }

      HDC     ScreenDC;
      HDC     MemoryDC;
      HBITMAP  hBitmap;

      // Размер экрана:
      int     screen_x;
      int     screen_y;

      BITMAPINFO   BMI;

      // Указатель на блок данных BGR, управляемый HBITMAP
      unsigned char* buf;

      RGB getRGB(int x, int y)
      {
          RGB rgb;
          rgb.r = int(buf[(y * screen_x + x) * 3 + 2]);
          rgb.g = int(buf[(y * screen_x + x) * 3 + 1]);
          rgb.b = int(buf[(y * screen_x + x) * 3]);
          return rgb;
      }

      void get()
      {
          SelectObject(MemoryDC, hBitmap);
          BitBlt(MemoryDC, 0, 0, screen_x, screen_y, ScreenDC, 0, 0, SRCCOPY);
      }
};