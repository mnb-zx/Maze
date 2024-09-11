
// MazeGame.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include <vector>
#include <random>
#include <chrono>
#include "sqlite3.h"


// CMazeGameApp:
// 有关此类的实现，请参阅 MazeGame.cpp
//

class Maze {
public:
    Maze(int width, int height);
    void generate();
    bool isWall(int x, int y) const;
    std::pair<int, int> getStart() const;
    std::pair<int, int> getEnd() const;
    std::vector<std::vector<int>> GetMazeData() const;

private:
    void generateStartAndEnd();
    void dfs(int x, int y);

    int width;
    int height;
    std::vector<std::vector<bool>> grid;
    std::pair<int, int> start;
    std::pair<int, int> end;
    std::mt19937 rng;
};

class CMazeGameApp : public CWinApp
{
public:
	CMazeGameApp() noexcept;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMazeGameApp theApp;
