// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <vector>
#include <list>
#include <map>
#include <time.h>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>

#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <process.h>
using namespace std;

#pragma comment (lib, "fmodex_vc.lib")
#include "SoundMgr.h"

#ifdef	_DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif