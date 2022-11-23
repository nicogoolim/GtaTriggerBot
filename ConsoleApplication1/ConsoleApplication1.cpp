#include "Header.h"




bool isActive = false;

void stateIsActive() {
    while(true)
    {
        if (GetAsyncKeyState(VK_LMENU))
        {
            isActive = !isActive;
            if (isActive)
            {
                std::cout << "on" << std::endl;
            }
            else {
                std::cout << "off" << std::endl;
            }
            Sleep(300);
        }
    }
}

class ScreenAnalyzer {
public:
    COORD center;
    short offsetX;
    short offsetY;

    ScreenAnalyzer(short offsetX, short offsetY) {
        short cX = GetSystemMetrics(SM_CXSCREEN) / 2;
        short cY = GetSystemMetrics(SM_CYSCREEN) / 2;
        center = { cX, cY };
        this->offsetX = offsetX;
        this->offsetY = offsetY;
    };

    void checkArea(int ms) {
        CaptureScreen screen;
        RGB rgb;
        screen.get();
        for (int y = center.Y - offsetY; y <= center.Y + offsetY; y++) {
            for (int x = center.X - offsetX; x <= center.X + offsetX; x++) {
                rgb = screen.getRGB(x, y);
                std::cout <<"r: " << rgb.r << " g: "<< rgb.g<< " b: "<< rgb.b << std::endl;
                if ((rgb.r == 193) && rgb.b == 79 && rgb.g == 79) {
                    Sleep(ms);
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    std::cout << 1 << std::endl;
                    return;
                }
                else {
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                }
            }
        }
    }

private:
    RGB toRgb(COLORREF color) {
        RGB rgb;
        rgb.r = (int)GetRValue(color);
        rgb.g = (int)GetGValue(color);
        rgb.b = (int)GetBValue(color);

        return rgb;
    }
};

int main()
{
    short oX;
    short oY;
    int x;
    int ms;
    std::cout << "offset x: ";
    std::cin >> oX;
    std::cout << "offset y: ";
    std::cin >> oY;
    std::cout << "ms(recommend 30ms): ";
    std::cin >> ms;
    std::cout << "1.toggle alt 2.ctrl: ";
    std::cin >> x;
    ScreenAnalyzer screenAnalyzer(oX, oY);
    int start_time = clock();
    screenAnalyzer.checkArea(ms);
    unsigned long long int end_time = clock();
    std::cout << std::endl << end_time - start_time;
    
    if (x==1)
    {
        std::thread thr1(stateIsActive);
        thr1.detach();
        while (true) {
            while (isActive)
            {
                screenAnalyzer.checkArea(ms);
            }
        }

    }
    if (x==2)
    {
        while (true) {
            while (GetAsyncKeyState(VK_LCONTROL))
            {
                screenAnalyzer.checkArea(ms);
            }
        }
    }
    }
