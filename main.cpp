#include "main.h"
#include "style.h"
#include "implot.h"
#include <fstream>
#include <string>
bool debug = false;
int int1 = 0;
int click = 1;
int totalClicks;
int time1Total = 0;
int time2Total = 0;
float ms = 0;
float cps = 0;
float msArr[200];
float cpsArr[200];
int count = 0;

std::string averMS;
std::string stdMS;

std::string averCPS;
std::string stdsCPS;

void csnl() {
    if (debug == true) {
        AllocConsole();
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        FILE* fDummy;
        freopen_s(&fDummy, "CONIN$", "r", stdin);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
    }
    else {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        UpdateWindow(GetConsoleWindow());
    }
}

void RenderPlots() {
    if (int1 == 2) {
        ImGui::SetNextWindowSize({ 600.f,350.f });
        ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetCursorPos({ 0.f, 10.f });
        if (ImPlot::BeginPlot(" ")) {
            ImPlot::PlotLine("MS", msArr, 100);
            ImPlot::PlotLine("CPS", cpsArr, 100);
            ImPlot::EndPlot();
        }
        ImGui::SetCursorPos({ 0.f, 310.f });
        if (ImGui::Button("Close Graph", { 600.f, 40.f })) {
            int1 = 0;
            std::cout << "Graph closed\n";
        }
        ImGui::End();
    }
}

void Render() {
    ImGui::SetNextWindowSize({ 320.f,280.f });

    ImGui::Begin("Click Data Analyzer by Buxh", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::Button("", { 320.f,200.f })) {
        static auto captureTime2 = get_time();
        if (click == 2) {
            click = 1;
            time2Total = get_time() - captureTime2;
            float totalTime = time2Total - time1Total;
            ms = totalTime;
            cps = 1000 / totalTime;
            if (totalTime > 20 && totalTime < 300) {
                std::cout << "Delay (ms): " << totalTime << "  |   CPS: " << 1000 / totalTime << "\n";

                std::ofstream oMS("outputMS.txt", std::ios_base::app);
                if (300 > totalTime) {
                    oMS << ms << "\n";
                    oMS.close();
                }

                std::ofstream oCPS("outputCPS.txt", std::ios_base::app);
                if (300 > totalTime) {
                    oCPS << 1000 / totalTime << "\n";
                    oCPS.close();
                    totalClicks += 1;
                }
            }
        }

        static auto captureTime1 = get_time();
        if (click == 1) {
            click = 2;
            time1Total = get_time() - captureTime1;
            float totalTime = time1Total - time2Total;
            if (totalTime > 20) {
                std::cout << "Delay (ms): " << totalTime << "  |   CPS: " << 1000 / totalTime << "\n";
                ms = totalTime;
                cps = 1000 / totalTime;
            }
        }

        if (ms > 300) {
            std::cout << "You're clicking too slow\n";
        }

    }

    csnl();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.6f, 0.7f));
    ImGui::SetCursorPos({ 0.f, 200.f });
    if (ImGui::Button("Evaluate Data", { 320.f, 40.f })) {
        int1 = 2;

        std::ifstream ms;
        ms.open("outputMS.txt");

        int n = 0;
        while (ms >> msArr[n]) {
            n++;
            Sleep(0.1);
        }
        ms.close();

        std::ifstream cps;
        cps.open("outputCPS.txt");
        n = 0;
        while (cps >> cpsArr[n]) {
            n++;
            Sleep(0.1);
        }
        cps.close();

        remove("outputMS.txt");
        remove("outputCPS.txt");

        Sleep(20);

        averMS = "Average Delay (ms): " + std::to_string(average(msArr, totalClicks));
        stdMS = "Standard Deviation (ms): " + std::to_string(calculateSD(msArr, totalClicks));

        averCPS = "Average CPS: " + std::to_string(average(cpsArr, totalClicks));
        stdsCPS = "Standard Deviation CPS: " + std::to_string(calculateSD(cpsArr, totalClicks));

        std::cout << "\nCalculated\n";
        std::cout << totalClicks << " totalClicks\n";
        count = totalClicks;
        totalClicks = 0;
        std::cout << "totalClicks reset\n";
    }
    ImGui::PopStyleColor();

    ImGui::SetCursorPos({ 0.f, 240.f });
    if (ImGui::Button("Debug Info", { 320.f, 40.f })) {
        if (debug == true) {
            debug = false;
            std::cout << "Debug window closed\n";
        }
        else {
            debug = true;
            std::cout << "Debug window opened\n";
        }
    }

    RenderPlots();

    std::string cp = "CPS: " + std::to_string(cps);
    std::string m = "Delay (ms): " + std::to_string(ms);
    std::string currentCps = "Count: " + std::to_string(count);

    ImGui::SetCursorPos({ 10.f, 20.f });
    ImGui::Text(cp.c_str());
    ImGui::SetCursorPos({ 12.f, 30.f });
    ImGui::Text(m.c_str());
    ImGui::SetCursorPos({ 10, 40.f });
    ImGui::Text(currentCps.c_str());
    ImGui::SetCursorPos({ 10.f, 50.f });
    ImGui::Text(stdMS.c_str());
    ImGui::SetCursorPos({ 10.f, 60.f });
    ImGui::Text(averMS.c_str());
    ImGui::SetCursorPos({ 10.f, 70.f });
    ImGui::Text(averCPS.c_str());
    ImGui::SetCursorPos({ 10.f, 80.f });
    ImGui::Text(stdsCPS.c_str());
    ImGui::SetCursorPos({ 155.f, 110.f });
    ImGui::Text(std::to_string(totalClicks).c_str());

    if (ms > 300) {
        ImGui::SetCursorPos({ 10.f, 90.f });
        ImGui::Text("Please click faster");
    }

    ImGui::End();
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    LPCSTR wndwName = "  ";


    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, wndwName, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, wndwName, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        CustomStyle();
        ImGui::NewFrame();
        {
            Render();
        }
        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!globals.active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
