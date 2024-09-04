
// MazeGame.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "MazeGame.h"
#include "MazeGameDlg.h"
#include "MazeGame.h"
#include <algorithm>
#include <chrono>
#include "Cwlc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMazeGameApp

BEGIN_MESSAGE_MAP(CMazeGameApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMazeGameApp 构造

CMazeGameApp::CMazeGameApp() noexcept
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMazeGameApp 对象

CMazeGameApp theApp;


// CMazeGameApp 初始化

BOOL CMazeGameApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 创建并显示欢迎对话框
	Cwlc dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// 根据对话框的返回值处理结果
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

Maze::Maze(int width, int height) : width(width), height(height) {
	grid.resize(height, std::vector<bool>(width, true));
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	rng.seed(seed);
}

void Maze::generate() {
	// 初始化迷宫，所有格子都是墙
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			grid[y][x] = true;
		}
	}

	// 从(1,1)开始生成迷宫
	dfs(1, 1);

	// 设置起点和终点
	start = { 1, 1 };
	end = { width - 2, height - 2 };

	// 确保起点和终点是通路
	//grid[start.second][start.first] = false;
	//grid[end.second][end.first] = false;
}

bool Maze::isWall(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) return true;
	return grid[y][x];
}

void Maze::dfs(int x, int y) {
	grid[y][x] = false; // 当前格子设为通路

	// 定义四个方向：上、右、下、左
	std::vector<std::pair<int, int>> dirs = { {0, -2}, {2, 0}, {0, 2}, {-2, 0} };
	std::shuffle(dirs.begin(), dirs.end(), rng); // 随机打乱方向顺序

	for (const auto& dir : dirs) {
		int nx = x + dir.first, ny = y + dir.second;
		if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && grid[ny][nx]) {
			// 如果下一个格子在范围内且是墙，就把中间的墙打通
			grid[y + dir.second / 2][x + dir.first / 2] = false;
			dfs(nx, ny); // 递归处理下一个格子
		}
	}
}
