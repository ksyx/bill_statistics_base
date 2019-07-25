#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<iostream>
#include<queue> 
#include<algorithm>
#include<cmath>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp" 
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/types_c.h"//Get consts from here
// some of the code was adapted from dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#undef min
#undef max

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
/*
// Main code
int main(int, char**) {
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd)) {
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
		//    ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		// 3. Show another simple window.
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}
*/
// Helper functions

bool CreateDeviceD3D(HWND hWnd) {
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

void CleanupDeviceD3D() {
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
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
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

struct data {
	int x, y;
	bool operator < (const data& b) const {
		return y > b.y;
	}
};
int op, WinWidth = 680, WinHeight = 480;std::string str;cv::Mat mat1, mat2, canv, textarea, org, pp, fetchshow, show, passby, bicolor;cv::Rect rect, fetch;std::string WindowName;bool confirmed;
int minx, miny, maxx, maxy, cntsa, cntsa_, trans, range[2][2];double positive, negative;bool fetched;
std::vector<std::vector<int> >imgdata;
std::vector<std::vector<int> >vis;
std::vector<std::vector<int> >imgdata_;
std::vector<std::vector<int> >vis_;
std::vector<std::priority_queue<data> >textinfo;//x: val,y: posx
std::vector<int>visca;
std::vector<int>visarea;
std::vector<int>emp;
std::vector<data>cainfo[2];
std::vector<data>areainfo[2];
std::queue<data>que;
std::priority_queue<data>pq_emp;
int xd[4] = { -1,1,0,0 }, yd[4] = { 0,0,-1,1 }, id[20];
void BFS_remove(int sx, int sy) {
	//printf("%d %d\n",sx,sy);
	data tmp, temp;
	while (!que.empty()) que.pop();
	temp.x = sx;temp.y = sy;
	que.push(temp);
	int cnt = 0;
	while (!que.empty()) {
		cnt++;
		temp = que.front();que.pop();
		show = bicolor(rect); //printf("%d %d\n",tmp.x,tmp.y);
		for (int i = 0;i < 4;i++) {
			tmp = temp;
			tmp.x += xd[i];tmp.y += yd[i];
			if (tmp.x < 0 || tmp.y < 0 || tmp.x >= bicolor.rows || tmp.y >= bicolor.cols || !imgdata[tmp.y][tmp.x]) continue;
			imgdata[tmp.y][tmp.x] = 0;
			cv::rectangle(bicolor, cv::Point(tmp.x, tmp.y), cv::Point(tmp.x, tmp.y), cv::Scalar(255, 255, 255), 1);
			que.push(tmp);
		}
	}
}
void on_remove_mouse(int event, int x, int y, int flags, void* ustc) {
	static cv::Point p0;
	static int lastx, lasty;
	//printf("%d %d %d %d\n",event,x+rect.x,rect.y+y,imgdata[rect.y+y][x+rect.x]);
	if (event == 1) {
		p0 = cv::Point(x, y);
		lastx = rect.x;lasty = rect.y;
	}
	if (event == 8) {
		y += rect.y;x += rect.x;
		if (imgdata[y][x]) {
			BFS_remove(x, y);
			show = bicolor(rect);
			cv::imshow(WindowName, show);
		}
		y -= rect.y;x -= rect.x;
	}
	if (!event && (flags & 1)) {
		passby = bicolor;
		if (x >= 0 && x < WinWidth && y >= 0 && y < WinHeight) {
			int deltax = x - p0.x, deltay = y - p0.y;
			int newx = lastx - deltax, newy = lasty - deltay;
			if (newx < 0) newx = 0;
			if (newy < 0) newy = 0;
			if (newx > passby.cols - rect.width - 1) newx = passby.cols - rect.width - 1;
			if (newy > passby.rows - rect.height - 1) newy = passby.rows - rect.height - 1;//���γ���ͼ�� 
			//printf("%d %d %d %d %d %d %d %d %d %d\n",x,y,p0.x,p0.y,deltax,deltay,newx,newy,passby.cols,passby.rows);
			//scanf("%d"); 
			rect = cv::Rect(newx, newy, rect.width, rect.height);
		}
		show = passby(rect);
		cv::imshow(WindowName, show);
	}
}
void on_mouse(int event, int x, int y, int flags, void* ustc) {
	static cv::Point p0;
	static int lastx, lasty;
	if (event == 1) {
		p0 = cv::Point(x, y);
		lastx = rect.x;lasty = rect.y;
	}
	if (!event && (flags & 1)) {
		if (x >= 0 && x < WinWidth && y >= 0 && y < WinHeight) {
			int deltax = x - p0.x, deltay = y - p0.y;
			int newx = lastx - deltax, newy = lasty - deltay;
			if (newx < 0) newx = 0;
			if (newy < 0) newy = 0;
			if (newx > passby.cols - rect.width - 1) newx = passby.cols - rect.width - 1;
			if (newy > passby.rows - rect.height - 1) newy = passby.rows - rect.height - 1;//���γ���ͼ�� 
			//printf("%d %d %d %d %d %d %d %d %d %d\n",x,y,p0.x,p0.y,deltax,deltay,newx,newy,passby.cols,passby.rows);
			//scanf("%d"); 
			rect = cv::Rect(newx, newy, rect.width, rect.height);
		}
		show = passby(rect);
		cv::imshow(WindowName, show);
	}
}
void onChangeTrackBar(int pos, void* data) {
	cv::Mat src = bicolor, dst;
	cv::threshold(src, dst, pos, 255, 0);
	passby = dst;
	show = dst(rect);
	cv::imshow(WindowName, show);
}
//TODO: ��������ͨ�������������С��ĳ��ֵ�ļ���һ�����򣬽����д��������������ͣ���򵥰汾�� 
void BFS_CA(int sx, int sy, int id) {
	data tmp, temp;
	tmp.x = sx;tmp.y = sy;
	que.push(tmp);
	//printf("1");
	while (!que.empty()) {
		temp = que.front();que.pop();
		//printf("%d ",que.size());
		minx = std::min(minx, temp.x);
		maxx = std::max(maxx, temp.x);
		miny = std::min(miny, temp.y);
		maxy = std::max(maxy, temp.y);
		//if(cntsa==535) printf("%d %d\n",temp.x,temp.y);
		for (int i = 0;i < 4;i++) {
			tmp = temp;
			tmp.x += xd[i];tmp.y += yd[i];
			//printf("%d %d %d\n",i,tmp.x,tmp.y);
			if (tmp.x < 0 || tmp.y < 0 || tmp.x >= bicolor.rows || tmp.y >= bicolor.cols || !imgdata[tmp.x][tmp.y] || vis[tmp.x][tmp.y]) continue;
			vis[tmp.x][tmp.y] = id;
			que.push(tmp);
		}
	}
	//printf("0");
}
void ScanCA() {
	data tmp;
	for (int i = 0;i < bicolor.rows;i++)
		for (int j = 0;j < bicolor.cols;j++)
			if (!vis[i][j] && imgdata[i][j]) {
				//printf("%d %d ",i,j);
				vis[i][j] = ++cntsa;
				minx = miny = 2147483647;
				maxy = maxx = 0;
				BFS_CA(i, j, cntsa);
				cv::rectangle(canv, cv::Point(miny, minx), cv::Point(maxy, maxx), cv::Scalar(255, 0, 0), 2);
				for (int ii = minx;ii <= maxx;ii++)
					for (int jj = miny;jj <= maxy;jj++)
						vis[ii][jj] = cntsa;
				//if(cntsa<=10)printf("%d %d %d %d %d\n",vis[i][j],minx,miny,maxx,maxy);
				tmp.x = minx;tmp.y = miny;
				cainfo[0].push_back(tmp);
				//printf("i1");
				tmp.x = maxx;tmp.y = maxy;
				cainfo[1].push_back(tmp);
				//printf("%d ",cainfo[0].size()); 
				//printf("%d\n",cainfo[1].size());
			}
	show = canv(rect);
	passby = canv;
	cv::imshow("Connective Area Scanning", show);
}
void BFS_CA_(int sx, int sy, int id) {
	data tmp, temp;
	tmp.x = sx;tmp.y = sy;
	que.push(tmp);
	//printf("1");
	while (!que.empty()) {
		temp = que.front();que.pop();
		//printf("%d ",que.size());
		minx = std::min(minx, temp.x);
		maxx = std::max(maxx, temp.x);
		miny = std::min(miny, temp.y);
		maxy = std::max(maxy, temp.y);
		//if(cntsa_==26) printf("%d %d\n",temp.x,temp.y);
		for (int i = 0;i < 4;i++) {
			tmp = temp;
			tmp.x += xd[i];tmp.y += yd[i];
			//printf("%d %d %d\n",i,tmp.x,tmp.y);
			if (tmp.x < 0 || tmp.y < 0 || tmp.x >= bicolor.rows || tmp.y >= bicolor.cols || !imgdata_[tmp.x][tmp.y] || vis_[tmp.x][tmp.y]) continue;
			vis_[tmp.x][tmp.y] = id;
			que.push(tmp);
		}
	}
	//printf("0");
}
void ScanCA_() {
	data tmp;
	for (int i = 0;i < bicolor.rows;i++)
		for (int j = 0;j < bicolor.cols;j++)
			if (!vis_[i][j] && imgdata_[i][j]) {
				//printf("%d %d ",i,j);
				vis_[i][j] = ++cntsa_;
				minx = miny = 2147483647;
				maxy = maxx = 0;
				BFS_CA_(i, j, cntsa_);
				cv::rectangle(canv, cv::Point(miny, minx), cv::Point(maxy, maxx), cv::Scalar(0, 0, 255), 2);
				cv::rectangle(canv, cv::Point(1344, 689), cv::Point(1344, 689), cv::Scalar(255, 0, 0), 2);

				//printf("%d %d %d %d %d\n",cntsa_,minx,maxx,miny,maxy);
				for (int ii = minx;ii <= maxx;ii++)
					for (int jj = miny;jj <= maxy;jj++) {
						vis_[ii][jj] = cntsa_;
						//if(ii==1344&&jj==689) printf("%d\n",cntsa_);
					}
				tmp.x = minx;tmp.y = miny;
				areainfo[0].push_back(tmp);
				//printf("i1");
				tmp.x = maxx;tmp.y = maxy;
				areainfo[1].push_back(tmp);
				//printf("%d ",cainfo[0].size()); 
				//printf("%d\n",cainfo[1].size());
			}
	show = canv(rect);
	passby = canv;
	cv::imshow(WindowName, show);
}
void on_pick_mouse(int event, int x, int y, int flags, void* ustc) {
	static cv::Point p0;
	static int lastx, lasty;
	if (event == 1) {
		p0 = cv::Point(x, y);
		lastx = rect.x;lasty = rect.y;
	}
	if (event == 7) {
		y += rect.y;x += rect.x;
		if (imgdata[y][x]) {
			//printf("Double Click Received\n");
			id[trans] = vis[y][x];
			//printf("Register %d->%d",id[trans],trans);
			std::swap(x, y);
			//printf("%d %d %d [%d,%d]-[%d,%d]\n",x,y,vis[x][y],cainfo[0][vis[x][y]-1].x,cainfo[0][vis[x][y]-1].y,cainfo[1][vis[x][y]-1].x,cainfo[1][vis[x][y]-1].y);
			cv::destroyWindow("Current Choice");
			fetch = cv::Rect(cainfo[0][vis[x][y] - 1].y, cainfo[0][vis[x][y] - 1].x, cainfo[1][vis[x][y] - 1].y - cainfo[0][vis[x][y] - 1].y, cainfo[1][vis[x][y] - 1].x - cainfo[0][vis[x][y] - 1].x);
			fetchshow = bicolor(fetch);
			cv::imshow("Current Choice", fetchshow);
			std::swap(x, y);
		}
		y -= rect.y;x -= rect.x;
	}
	if (event == 8) {
		cv::destroyWindow("Current Choice");
		cv::destroyWindow("Point Picking");
		id[trans] = 0;
	}
	if (event == 9) {
		cv::destroyWindow("Current Choice");
		cv::destroyWindow("Point Picking");
	}
	if (!event && (flags & 1)) {
		if (x >= 0 && x < WinWidth && y >= 0 && y < WinHeight) {
			int deltax = x - p0.x, deltay = y - p0.y;
			int newx = lastx - deltax, newy = lasty - deltay;
			if (newx < 0) newx = 0;
			if (newy < 0) newy = 0;
			if (newx > passby.cols - rect.width - 1) newx = passby.cols - rect.width - 1;
			if (newy > passby.rows - rect.height - 1) newy = passby.rows - rect.height - 1;//���γ���ͼ�� 
			//printf("%d %d %d %d %d %d %d %d %d %d\n",x,y,p0.x,p0.y,deltax,deltay,newx,newy,passby.cols,passby.rows);
			//scanf("%d"); 
			rect = cv::Rect(newx, newy, rect.width, rect.height);
		}
		show = passby(rect);
		cv::imshow(WindowName, show);
	}
}
void DoFetchSample(int now) {
	system("cls");
	printf("The followings are important, read it if you are not familiar with the process.\n");
	printf("Guide: double-left click on a black point to match sample and required information. If there is NO SUCH THING, double right-click on the image.\n");
	printf("Note: close both the window or double-click middle key to confirm your choice, we will treat you as double right-clicked if you closed window without any VAILD point choosing and the ONLY way to cancel your choice is to double right-click before the window was closed.\n");
	printf("Notice: once the window was closed, you can NOT go back, the ONLY way to change your choice is do the whole process again after completing all of the fetches THIS TIME.\n");
	printf("There will be a window showing your choice.\n");
	printf("Now Fetching: ");
	if (now <= 9) printf("%d", now); else if (now == 10) printf("."); else if (now == 11) printf("+"); else if (now == 12) printf("-");
	printf("\n");
	printf("\n");
	cv::namedWindow("Point Picking");
	trans = now;
	passby = bicolor;
	rect = cv::Rect(0, 0, WinWidth, WinHeight);
	show = bicolor(rect);
	WindowName = "Point Picking";
	cv::imshow(WindowName, show);
	fetch = cv::Rect(0, 0, 100, 100);
	cv::Mat tmp(100, 100, CV_8UC3, cv::Scalar(255, 255, 255));
	fetchshow = tmp(fetch);
	cv::imshow("Current Choice", fetchshow);
	cv::setMouseCallback("Point Picking", on_pick_mouse);
	cv::waitKey(0);
}
void on_recognize_mouse(int event, int x, int y, int flags, void* ustc) {
	static cv::Point p0;
	static int lastx, lasty;
	if (event == 1) {
		p0 = cv::Point(x, y);
		lastx = rect.x;lasty = rect.y;
	}
	if (event == 7) {
		printf("Point Selected. %d %d\n", y + rect.y, x + rect.x);
		range[trans][1] = x + rect.x;
		range[trans][0] = y + rect.y;
	}
	if (event == 8) {
		cv::destroyWindow("Select Point");
	}
	if (!event && (flags & 1)) {
		if (x >= 0 && x < WinWidth && y >= 0 && y < WinHeight) {
			int deltax = x - p0.x, deltay = y - p0.y;
			int newx = lastx - deltax, newy = lasty - deltay;
			if (newx < 0) newx = 0;
			if (newy < 0) newy = 0;
			if (newx > passby.cols - rect.width - 1) newx = passby.cols - rect.width - 1;
			if (newy > passby.rows - rect.height - 1) newy = passby.rows - rect.height - 1;//���γ���ͼ�� 
			//printf("%d %d %d %d %d %d %d %d %d %d\n",x,y,p0.x,p0.y,deltax,deltay,newx,newy,passby.cols,passby.rows);
			//scanf("%d"); 
			rect = cv::Rect(newx, newy, rect.width, rect.height);
		}
		show = passby(rect);
		cv::imshow(WindowName, show);
	}
}
double CalcSSIM() {
	double mu1 = 0, mu2 = 0, sig1 = 0, sig2 = 0, tosig = 0, k1, k2;//sig: sigma, tosig: together sigma
	k1 = (double)std::min(mat1.rows, mat2.rows) / std::max(mat1.rows, mat2.rows);
	k2 = (double)std::min(mat1.cols, mat2.cols) / std::max(mat1.cols, mat2.cols);
	k1 = k1 * k2;
	int a = std::max(mat1.rows, mat2.rows), b = std::max(mat1.cols, mat2.cols);
	//printf("%d %d %d %d\n",mat1.cols,mat1.rows,mat2.cols,mat2.rows);
	cv::resize(mat2, mat2, cv::Size(b, a), 0, 0, 1);
	cv::resize(mat1, mat1, cv::Size(b, a), 0, 0, 1);
	//cv::imshow("a",mat1);
	//cv::imshow("b",mat2);
	//cv::waitKey(0);
	//printf("%d %d %d %d\n",mat1.cols,mat2.cols,mat1.rows,mat2.rows);
	for (int i = 0;i < a;i++)
		for (int j = 0;j < b;j++)
			mu1 += (bool)(mat1.at<uchar>(i, j) == 0);
	mu1 /= (a * b);
	for (int i = 0;i < a;i++)
		for (int j = 0;j < b;j++)
			mu2 += (bool)(mat2.at<uchar>(i, j) == 0);
	mu2 /= (a * b);
	for (int i = 0;i < a;i++)
		for (int j = 0;j < b;j++)
			sig1 += (double)(((bool)(mat1.at<uchar>(i, j) == 0)) - mu1) * (((bool)(mat1.at<uchar>(i, j) == 0)) - mu1);
	sig1 /= (a * b - 1);
	sig1 = sqrt(sig1);
	for (int i = 0;i < a;i++)
		for (int j = 0;j < b;j++)
			sig2 += (double)(((bool)(mat2.at<uchar>(i, j) == 0)) - mu2) * (((bool)(mat2.at<uchar>(i, j) == 0)) - mu2);
	sig2 /= (a * b - 1);
	sig2 = sqrt(sig2);
	for (int i = 0;i < a;i++)
		for (int j = 0;j < b;j++)
			tosig += (double)(((bool)(mat2.at<uchar>(i, j) == 0)) - mu2) * (((bool)(mat1.at<uchar>(i, j) == 0)) - mu1);
	tosig /= (a * b - 1);
	//printf("%lf %lf\n",((tosig+32)/(sig1*sig2+32)*1)+(k1*0));
	//return tosig;
	tosig = sqrt(tosig);
	return ((tosig + 32) / (sig1 * sig2 + 32) * 1) + (k1 * 0);
	double c1 = 0.0004, c2 = 0.00001296, c3 = 0.00000648;
	//printf("<%lf>%lf or %lf\n",tosig,((2*mu1*mu2+c1)*(2*tosig+c2))/((mu1*mu1+mu2*mu2+c1)*(sig1*sig1+sig2*sig2+c2))*k1,((2*mu1*mu2+c1)*(2*tosig+c2))/((mu1*mu1+mu2*mu2+c1)*(sig1*sig1+sig2*sig2+c2)));
	return ((2 * mu1 * mu2 + c1) * (2 * tosig + c2)) / ((mu1 * mu1 + mu2 * mu2 + c1) * (sig1 * sig1 + sig2 * sig2 + c2)) * k1;
}
int GetResponse(int x1, int y1, int x2, int y2) {
	//printf("%d %d %d %d\n",x1,y1,x2,y2);
	double max = -1, ret;int maxid = -1;
	for (int i = 0;i <= 12;i++) {
		if (!id[i]) continue;
		/*
		DEBUGGING
		//[RectCurrent]rect=cv::Rect(y1,x1,y2-y1,x2-x1);
		//[RectSample] rect=cv::Rect(cainfo[0][id[i]-1].y,cainfo[0][id[i]-1].x,cainfo[1][id[i]-1].y-cainfo[0][id[i]-1].y,cainfo[1][id[i]-1].x-cainfo[0][id[i]-1].x);
		show=bicolor(rect);
		cv::imshow("aaa",show);
		cv::waitKey(0);*/
		//printf("%d %d %d %d %d\n",id[i],cainfo[0][id[i]-1].y,cainfo[0][id[i]-1].x,cainfo[1][id[i]-1].y-cainfo[0][id[i]-1].y,cainfo[1][id[i]-1].x-cainfo[0][id[i]-1].x); 
		rect = cv::Rect(y1, x1, y2 - y1 + 1, x2 - x1 + 1);
		mat1 = bicolor(rect);
		rect = cv::Rect(cainfo[0][id[i] - 1].y, cainfo[0][id[i] - 1].x, cainfo[1][id[i] - 1].y - cainfo[0][id[i] - 1].y + 1, cainfo[1][id[i] - 1].x - cainfo[0][id[i] - 1].x + 1);
		//printf("Chk: %d %d %d %d\n",cainfo[0][id[i]-1].y,cainfo[0][id[i]-1].x,cainfo[1][id[i]-1].y-cainfo[0][id[i]-1].y,cainfo[1][id[i]-1].x-cainfo[0][id[i]-1].x);
		mat2 = bicolor(rect);
		//printf("Possbility of %d: ",i);
		ret = CalcSSIM();
		if (ret > max) {
			max = ret;
			maxid = i;
		}
	}
	//printf("OK."); 
	//printf("This is %d\n",maxid);
	//rect=cv::Rect(y1,x1,y2-y1,x2-x1);
	//show=bicolor(rect); 
	//cv::imshow("aaa",show);
	//cv::waitKey(0);
	//printf("This is %d\n",maxid);
	return maxid;
}
char GetChar(int x) {
	if (x <= 9)return x + '0'; else if (x == 10) return '.'; else if (x == 11) return '+'; else if (x == 12) return '-'; else return '?';
}
void Accumulate(std::string str) {
	double ret = 0, awa = 0;int l = str.length(), p = -1;bool change = 0;
	for (int i = 0;i < l;i++) {
		if (str[i] == '+' || str[i] == '-') {
			if (i) return;
			if (str[i] == '+') change = 0; else change = 1;
			continue;
		}
		if (str[i] != '.') {
			ret = ret * 10 + str[i] - '0';
		}
		else { p = i + 1;break; }
	}
	if (p < 0) goto accu;
	for (int i = l - 1;i >= p;i--) {
		awa = (awa + str[i] - '0') / 10;
		if (str[i] == '.') return;
	}
	ret += awa;
accu:
	if (change) negative += ret; else positive += ret;
}
int main() {
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Bill Statistics GUI"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd)) {
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
		//    ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);
			
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		// 3. Show another simple window.
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
remenu:
	fflush(stdin);
	system("cls");
	printf("[Main Menu]\n");
	printf("Welcome to Bill Statistics Software! Choose what you want: \n");
	printf("* Tip: The IDs without [] should be done with the order they lists\n");
	printf(" 1. Load Image\n");
	printf(" 2. Binarize the Image\n");
	printf(" 3. Confirm Binarize\n");
	printf(" 4. Load Image Data\n");
	printf("[5]. Manual Removal Tool\n");
	printf(" 6. Scan Connective Area\n");
	printf(" 7. Automatic Text Areas Detecting\n");
	printf(" 8. Picking Samples\n");
	printf(" 9. Do Recognize\n");
	printf(" 99. Exit\n");
	printf(" 999. About\n");
	printf("Your Choice: ");
	scanf("%d", &op);
	system("cls");
	if (op == 1) {
reloadimg:
		system("cls");
		printf("[Load Image]\n");
		printf("Input exit to go back\n");
		printf("After the input of file path, the preview will be shown, you need to close the window in order to continue the operation.");
		printf("Input the RELATIVE path of the image: ");
		std::cin >> str;
		if (str == "exit") goto remenu;
		org = cv::imread(str);
		if (org.empty()) {
			printf("Failed to load the image!");
			system("pause");
			goto reloadimg;
		}
		long long p = (long long)(org.rows) * (long long)(org.cols);
		if (org.rows < WinHeight) WinHeight = org.rows - 1;
		if (org.cols < WinWidth) WinWidth = org.cols - 1;
		if (p > 10000 * 10000) {
			printf("The size of the image is too big!");
			system("pause");
			goto reloadimg;
		}
		rect = cv::Rect(0, 0, WinWidth, WinHeight);
		show = org(rect);
		passby = org;
		WindowName = "Original Image";
		cv::imshow("Original Image", show);
		cv::setMouseCallback("Original Image", on_mouse);
		cv::waitKey(0);
		cv::destroyWindow("Original Image");
		system("cls");
		printf("Image loaded, check it in the new window.\n");
		printf("Do you want to change a file? Y to confirm, others to not confirm.");
		std::cin >> str;
		if (str == "Y") goto reloadimg;
	}
	else if (op == 2) {
		system("cls");
		printf("[Binarize Image]\n");
		if (!org.data) { printf("ERROR: ORIGINAL IMAGE NOT LOADED\n");system("pause");goto remenu; }
		printf("Scroll the bar to the position where best shows the information on your image and most of the other things were removed\n");
		WindowName = "Binarized Image";
		cv::namedWindow("Binarized Image");
		cv::cvtColor(org, bicolor, 7);
		passby = bicolor;
		rect = cv::Rect(0, 0, WinWidth, WinHeight);
		show = bicolor(rect);
		cv::imshow(WindowName, show);
		cv::createTrackbar("threshold", "Binarized Image", 0, 255, onChangeTrackBar);
		cv::setMouseCallback("Binarized Image", on_mouse);
		cv::waitKey(0);
		cv::destroyWindow("Binarized Image");
		confirmed = 0;
		goto remenu;
	}
	else if (op == 3) {
		printf("[Confirm Binarize]\n");
		if (!bicolor.data) { printf("ERROR: IMAGE NOT BINARIZED!\n");system("pause");goto remenu; }
		if (confirmed) { printf("ERROR: THIS BINARIZE HAS BEEN CONFIRMED!\n");system("pause");goto remenu; }
		passby.copyTo(bicolor);
		printf("Confirmed.\n");
		system("pause");
		confirmed = 1;
		goto remenu;
	}
	else if (op == 4) {
reloadimg_:
		printf("[Load Image Data]\n");
		if (!confirmed) { printf("ERROR: BINARIZE NOT CONFIRMED\n");system("pause");goto remenu; }
		printf("Loading...\n");
		imgdata.clear();
		emp.clear();
		for (int i = 0;i < bicolor.rows;i++) {
			imgdata.push_back(emp);
			for (int j = 0;j < bicolor.cols;j++) {
				imgdata[i].push_back((bool)(bicolor.at<uchar>(i, j) == 0));
			}
		}
		printf("Done.\n");
		system("pause");
	}
	else if (op == 5) {
		printf("[Manual Removal Tool]\n");
		if (!imgdata.size()) { printf("ERROR: IMAGE DATA NOT LOADED!\n");system("pause");goto remenu; }
		printf("Use this tool to remove the unimportant details from the image, once you dobule right-clicked a point, all of the points that have a fully-black path to it will be removed, NOTICE THAT THIS OPERATION CAN NOT BE UNDID EXCEPT RELOADING THE IMAGE.\n");
		rect = cv::Rect(0, 0, WinWidth, WinHeight);
		show = bicolor(rect);
		cv::imshow("Removal Tool", show);
		WindowName = "Removal Tool";
		cv::setMouseCallback("Removal Tool", on_remove_mouse);
		cv::waitKey(0);
		cv::destroyWindow("Removal Tool");
		goto reloadimg_;
	}
	else if (op == 6) {
		printf("[Scan Connective Area]\n");
recalc:
		if (!imgdata.size()) { printf("ERROR: IMAGE DATA NOT LOADED!\n");system("pause");goto remenu; }
		if (cainfo[0].size()) {
			printf("Are you sure to calcuate the connective area AGAIN? Y to confirm, the others to not confirm.\n");
			std::cin >> str;
			if (str == "Y") { cainfo[0].clear();cainfo[1].clear();cntsa = 0;goto recalc; }
			else goto remenu;
		}
		emp.clear();vis.clear();
		for (int j = 0;j < bicolor.cols;j++)
			emp.push_back(0);
		for (int i = 0;i < bicolor.rows;i++)
			vis.push_back(emp);
		cvtColor(bicolor, canv, 8);
		rect = cv::Rect(0, 0, WinWidth, WinHeight);
		show = bicolor(rect);
		cv::imshow("Connective Area Scanning", show);
		WindowName = "Connective Area Scanning";
		passby = bicolor;
		cv::setMouseCallback("Connective Area Scanning", on_mouse);
		ScanCA();
		cv::waitKey(0);
		cv::destroyWindow("Connective Area Scanning");
		system("cls");
		printf("Completed.\n");
		system("pause");
		goto remenu;
	}
	else if (op == 7) {
		printf("[Automatic Text Areas Detecting]\n");
redetect:
		if (!cainfo[0].size()) { printf("ERROR: CONNECTIVE AREA NOT DETECTED\n");system("pause");goto remenu; }
		if (areainfo[0].size()) {
			printf("Are you sure to detect text areas AGAIN? Y to confirm, the others to not confirm.\n");
			std::cin >> str;
			if (str == "Y") { areainfo[0].clear();areainfo[1].clear();cntsa_ = 0;goto redetect; }
			else goto remenu;
		}
		cv::morphologyEx(bicolor, textarea, 2, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 1)));
		rect = cv::Rect(0, 0, WinWidth, WinHeight);
		cv::cvtColor(bicolor, canv, 8);
		show = canv(rect);
		cv::imshow("Automatic Text Areas Detecting", bicolor);
		passby = canv;
		WindowName = "Automatic Text Areas Detecting";
		cv::setMouseCallback(WindowName, on_mouse);
		printf("Loading data...\n");
		imgdata_.clear();
		emp.clear();
		for (int i = 0;i < bicolor.rows;i++) {
			imgdata_.push_back(emp);
			for (int j = 0;j < bicolor.cols;j++) {
				imgdata_[i].push_back((bool)(textarea.at<uchar>(i, j) == 0));
			}
		}
		printf("Calcuating Text Areas...\n");
		emp.clear();vis_.clear();
		for (int j = 0;j < bicolor.cols;j++)
			emp.push_back(0);
		for (int i = 0;i < bicolor.rows;i++)
			vis_.push_back(emp);
		ScanCA_();
		emp.clear();imgdata_.clear();
		cv::waitKey(0);
		cv::destroyWindow("Automatic Text Areas Detecting");
	}
	else if (op == 8) {
		printf("[Picking Samples]\n");
		if (!areainfo[0].size()) {
			printf("ERROR: AREA INFO NOT DETECTED!\n");
			system("pause");
			goto remenu;
		}
		for (int i = 0;i <= 12;i++)
			DoFetchSample(i);
		fetched = 1;
		cv::destroyWindow("Point Picking");
		cv::destroyWindow("Current Choice");
		goto remenu;
	}
	else if (op == 9) {
		printf("[Do Recognize]\n");
		positive = negative = 0;
		if (!fetched) {
			printf("ERROR: SAMPLE NOT FETCHED!\n");
			system("pause");
			goto remenu;
		}
		visca.clear();visarea.clear();textinfo.clear();
rechoose:
		for (int i = 0;i < 2;i++) {
			system("cls");
			printf("[Do Recognize]\n");
			printf("Double left-click to choose point, now choose %s point. Double right-click to confirm.\n", i ? "RIGHT-DOWN" : "LEFT-UP");
			passby = bicolor;
			trans = i;
			WindowName = "Select Point";
			rect = cv::Rect(0, 0, WinWidth, WinHeight);
			show = bicolor(rect);
			cv::imshow("Select Point", show);
			cv::setMouseCallback(WindowName, on_recognize_mouse);
			cv::waitKey(0);
			cv::destroyWindow("Select Point");
			if (i) {
				if (range[0][0] > range[1][0] || range[0][1] > range[1][1]) {
					printf("ERROR: INVAILD RANGE\n");
					system("pause");
					goto rechoose;
				}
			}
		}
		for (int i = 0;i <= cntsa;i++)
			visca.push_back(0);
		for (int i = 0;i <= cntsa_;i++) {
			visarea.push_back(0);
			textinfo.push_back(pq_emp);
		}
		//printf("%d %d %d %d\n",range[0][0],range[0][1],range[1][0],range[1][1]);
		for (int i = range[0][0];i <= range[1][0];i++)
			for (int j = range[0][1];j <= range[1][1];j++) {
				//printf("%d %d %d %d %d | %d %d\n",i,j,imgdata[i][j],visarea[vis_[i][j]],visca[vis[i][j]], vis_[i][j],vis[i][j]);
				if (imgdata[i][j] && !visarea[vis_[i][j]] && !visca[vis[i][j]]) {
					int pp = i, qq = j;
					int v = GetResponse(cainfo[0][vis[i][j] - 1].x, cainfo[0][vis[i][j] - 1].y, cainfo[1][vis[i][j] - 1].x, cainfo[1][vis[i][j] - 1].y);
					i = (cainfo[0][vis[i][j] - 1].x + cainfo[1][vis[i][j] - 1].x) / 2;j = (cainfo[0][vis[i][j] - 1].y + cainfo[1][vis[i][j] - 1].y) / 2;
					visca[vis[i][j]] = 1;
					if (v == -1) {
						visarea[vis_[i][j]] = 1;
						continue;
					}
					data tmp;
					tmp.x = v;tmp.y = cainfo[0][vis[i][j] - 1].y;
					textinfo[vis_[i][j]].push(tmp);
					//printf("%d %d At area %d, Add %d with Y=%d\n",i,j,vis_[i][j],v,cainfo[0][vis[i][j]-1].y);
					i = pp;j = qq;
				}
			}
		std::string tmp;
		printf("Original Entries:\n");
		for (int i = 1;i <= cntsa_;i++) {
			if (textinfo[i].size()) {
				tmp = "";
				while (!textinfo[i].empty()) { tmp += GetChar(textinfo[i].top().x);printf("%c", GetChar(textinfo[i].top().x));textinfo[i].pop(); }
				printf("\n");
				Accumulate(tmp);
			}
		}
		printf("result: income %.2lf | outgoing %.2lf\n", positive, negative);
		system("pause");
	}
	else if (op == 99) {
		return 0;
	}
	else if (op == 999) {
		printf("Version: 1.0.1 Beta\n");
		printf("Copyright (C) ksyx 2019, all rights reserved. This software is under MIT license.\n");
		printf("This product used OpenCV library, thanks OpenCV group for providing such a good library.\n");
		printf("Copyright (C) 2000-2019, Intel Corporation, all rights reserved.\n");
		printf("Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.\n");
		printf("Copyright (C) 2009-2016, NVIDIA Corporation, all rights reserved.\n");
		printf("Copyright (C) 2010-2013, Advanced Micro Devices, Inc., all rights reserved.\n");
		printf("Copyright (C) 2015-2016, OpenCV Foundation, all rights reserved.\n");
		printf("Copyright (C) 2015-2016, Itseez Inc., all rights reserved.\n");
		printf("Third party copyrights are property of their respective owners.\n");
		system("pause");
		goto remenu;
	}
	goto remenu;
	return 0;
}