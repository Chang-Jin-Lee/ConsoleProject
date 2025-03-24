#include "ConsoleRenderer.h"
#include <stdio.h>
#include <string>

namespace ConsoleRenderer
{
    HANDLE hConsoleHandle;      // 초기 화면 콘솔의 핸들

    int nScreenWidth = 0; // 콘솔창의 너비
    int nScreenHeight = 0; // 콘솔창의 높이
    int nScreenBufferSize = 0; // 콘솔창의 스크린버퍼 크기
    int nScreenBufferIndex = 0; // 콘솔창이 사용할 스크린버퍼의 인덱스
    HANDLE hScreenBuffer[2]; // 콘솔창이 사용할 스크린버퍼의 핸들

    void ScreenInit()
    {
        // 현재 화면크기에 맞는 화면 콘솔스크린버퍼 2개를 만든다.    
        hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        hScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        hScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        // 글씨 크기 작게 설정
        SetSmallFont(hConsoleHandle, 4);
        SetSmallFont(hScreenBuffer[0], 4);
        SetSmallFont(hScreenBuffer[1], 4);

        // 기본 콘솔,생성된 콘솔스크린 모두 커서 안보이게 설정
        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.bVisible = FALSE;
        cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
        SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
        SetConsoleCursorInfo(hScreenBuffer[0], &cursorInfo);
        SetConsoleCursorInfo(hScreenBuffer[1], &cursorInfo);

        //기본 콘솔의 화면 크기 정보를 얻는다.
        CONSOLE_SCREEN_BUFFER_INFO Info; // 초기 화면 콘솔의 화면 정보
        GetConsoleScreenBufferInfo(hConsoleHandle, &Info);

        nScreenHeight = Info.srWindow.Bottom - Info.srWindow.Top + 1;
        nScreenWidth = Info.srWindow.Right - Info.srWindow.Left + 1;
        nScreenBufferSize = nScreenWidth * nScreenHeight;

        // 콘솔 출력 UTF8로 변경
        SetConsoleOutputCP(CP_UTF8);
    }

    void SetSmallFont(HANDLE hConsole, SHORT fontSize, const wchar_t* fontName)
    {
        CONSOLE_FONT_INFOEX cfi = { 0 };
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0; // Width: 0 = auto
        cfi.dwFontSize.Y = fontSize; // Height: 줄일수록 작아짐
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcsncpy_s(cfi.FaceName, fontName, LF_FACESIZE);
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    }

    void ScreenFlipping()
    {
        // 실제 콘솔이 사용할 스크린버퍼의 Handle을 설정하여 화면에 보여준다.
        SetConsoleActiveScreenBuffer(hScreenBuffer[nScreenBufferIndex]);
        // 다음에 사용할 스크린 버퍼의 인덱스를 증가시켜 준비한다.
        nScreenBufferIndex++;
        nScreenBufferIndex = nScreenBufferIndex % 2;  // 0,1,0,1,0,1,0,1....
    }

    void ScreenClear()
    {
        COORD Coor = { 0, 0 };
        DWORD dw;
        FillConsoleOutputCharacter(hScreenBuffer[nScreenBufferIndex], ' ', nScreenBufferSize, Coor, &dw);
    }

    void ScreenRelease()
    {
        CloseHandle(hScreenBuffer[0]);
        CloseHandle(hScreenBuffer[1]);
    }

    /*
        FOREGROUND_BLUE	텍스트 색에 파란색이 포함됩니다.
        COMMON_LVB_LEADING_BYTE	선행 바이트입니다.

        https://learn.microsoft.com/ko-kr/windows/console/console-screen-buffers#character-attributes
    */

    bool ScreenDrawChar(int x, int y, char ch, WORD attr)
    {
        COORD	cdPos;
        BOOL	bRval = FALSE;
        DWORD	dwCharsWritten;
        cdPos.X = x;
        cdPos.Y = y;

        bRval = FillConsoleOutputCharacterA(hScreenBuffer[nScreenBufferIndex], ch, 1, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputCharacter()\n");

        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, 1, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    bool ScreenDrawString(int x, int y, const char* pStr, WORD attr)
    {
        COORD	cdPos;
        BOOL	bRval = FALSE;
        DWORD	dwCharsWritten;
        cdPos.X = x;
        cdPos.Y = y;

        DWORD nNumberOfBytesToWrite = (DWORD)strlen(pStr);
        //특정 위치에 문자열을 출력한다.
        WriteConsoleOutputCharacterA(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    bool ScreenDrawString(int x, int y, const wchar_t* pStr, WORD attr)
    {
        COORD	cdPos;
        BOOL	bRval = FALSE;
        DWORD	dwCharsWritten;
        cdPos.X = x;
        cdPos.Y = y;

        DWORD nNumberOfBytesToWrite = (DWORD)wcslen(pStr);
        //특정 위치에 문자열을 출력한다.
        WriteConsoleOutputCharacterW(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    bool ScreenDrawStringFromFile(int x, int y, const char* pStr, WORD attr)
    {
        COORD	cdPos;
        BOOL	bRval = FALSE;
        DWORD	dwCharsWritten;
        cdPos.X = x;
        cdPos.Y = y;

        DWORD nNumberOfBytesToWrite = (DWORD)(strlen(pStr) - 1);
        //특정 위치에 문자열을 출력한다.
        WriteConsoleOutputCharacterA(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    bool ScreenDrawStringFromFile(int x, int y, const wchar_t* pStr, WORD attr)
    {
        COORD	cdPos;
        BOOL	bRval = FALSE;
        DWORD	dwCharsWritten;
        cdPos.X = x;
        cdPos.Y = y;

        DWORD nNumberOfBytesToWrite = (DWORD)(wcslen(pStr) - 1);
        //특정 위치에 문자열을 출력한다.
        WriteConsoleOutputCharacterW(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    void ScreenDrawUI(UI::FUI ui, WORD attr)
    {
        ScreenDrawString(ui.m_fAxis.X, ui.m_fAxis.Y, ui.m_pcontent, attr);
    }

    void ScreenDrawUIFromFile(UI::FUI ui, WORD attr)
    {
        for (int i = 0; i < ui.m_icontentSize; i++)
        {
            ScreenDrawStringFromFile(ui.m_fAxis.X, ui.m_fAxis.Y + i, ui.m_ppcontent[i], attr);
        }
    }

    void ScreenDrawFileStrings(int x, int y, char** str, int str_size, WORD attr)
    {
        for (int i = 0; i < str_size; i++)
        {
            ScreenDrawStringFromFile(x, y + i, str[i], attr);
        }
    }

    void ScreenDrawFileStrings(int x, int y, wchar_t** str, int str_size, WORD attr)
    {
        for (int i = 0; i < str_size; i++)
        {
            ScreenDrawStringFromFile(x, y + i, str[i], attr);
        }
    }

    bool ScreenSetAttributes(WORD attr)
    {
        COORD	cdPos;
        bool	bRval = FALSE;
        DWORD	dwCharsWritten;
        //	int x,y;	

        cdPos.X = 0;
        cdPos.Y = 0;
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nScreenBufferSize, cdPos, &dwCharsWritten);
        if (bRval == false)
        {
            printf("Error, FillConsoleOutputCharacter()\n");
            return bRval;
        }

        return bRval;
    }

    void print(char* value)
    {
        OutputDebugStringA(value);
    }

    int ScreenWidth()
    {
        return nScreenWidth;
    }
    int ScreenHeight()
    {
        return nScreenHeight;
    }

    int ScreenCenter(const char* ch)
    {
        return nScreenWidth / 2 - strlen(ch) / 2;
    }

    int ScreenCenterW(const wchar_t* ch)
    {
        return nScreenWidth / 2 - wcslen(ch) / 2;
    }
};



