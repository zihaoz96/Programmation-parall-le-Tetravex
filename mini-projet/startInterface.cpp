#include <iostream>
#include <windows.h>


BOOL RegisterWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wnd;
    wnd.hbrBackground=(HBRUSH)COLOR_WINDOW;
    wnd.lpfnWndProc=WndProc;
    wnd.style=CS_HREDRAW|CS_VREDRAW;
    wnd.lpszClassName="WindowClass";
    wnd.hInstance=hInstance;
    wnd.hIcon=LoadIcon(hInstance,IDI_APPLICATION);
    wnd.hIconSm=LoadIcon(hInstance,IDI_APPLICATION);
    wnd.hCursor=LoadCursor(hInstance,IDC_ARROW);
    wnd.cbSize=sizeof(WNDCLASSEX);
    wnd.lpszMenuName=nullptr;
    wnd.cbClsExtra=0;
    wnd.cbWndExtra=0;return RegisterClassEx(&wnd);
}
BOOL DisplayWindow(HWND& hwnd,HINSTANCE hInstance,HMENU hMenu,int nCmdShow)
{
    hwnd=CreateWindow("WindowClass","TweeChaice",WS_OVERLAPPEDWINDOW,0,0,800,800,
            nullptr,nullptr,hInstance,nullptr
            );
    ShowWindow(hwnd,nCmdShow);
    return hwnd?TRUE:FALSE;
}

HWND hwnd;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    if(!RegisterWindow(hInstance))
    {
        MessageBox(nullptr,"Error register!","Error",MB_ICONERROR|MB_OK);
        return -1;
    }
    DisplayMenu();
    if(!DisplayWindow(hwnd,hInstance,nullptr,nCmdShow))
    {
        MessageBox(nullptr,"Error creating window!","Error",MB_ICONERROR|MB_OK);
        return -1;
    }
    MSG msg;
    while(GetMessage(&msg,0,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}