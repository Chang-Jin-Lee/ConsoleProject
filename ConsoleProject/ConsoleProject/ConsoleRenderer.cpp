#include "ConsoleRenderer.h"
#include <stdio.h>
#include <string>

namespace ConsoleRenderer
{
    HANDLE hConsoleHandle;      // �ʱ� ȭ�� �ܼ��� �ڵ�

    int nScreenWidth = 0; // �ܼ�â�� �ʺ�
    int nScreenHeight = 0; // �ܼ�â�� ����
    int nScreenBufferSize = 0; // �ܼ�â�� ��ũ������ ũ��
    int nScreenBufferIndex = 0; // �ܼ�â�� ����� ��ũ�������� �ε���
    HANDLE hScreenBuffer[2]; // �ܼ�â�� ����� ��ũ�������� �ڵ�

    void ScreenInit()
    {
        // ���� ȭ��ũ�⿡ �´� ȭ�� �ֽܼ�ũ������ 2���� �����.    
        hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        hScreenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        hScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        // �۾� ũ�� �۰� ����
        SetSmallFont(hConsoleHandle, 4);
        SetSmallFont(hScreenBuffer[0], 4);
        SetSmallFont(hScreenBuffer[1], 4);

        // �⺻ �ܼ�,������ �ֽܼ�ũ�� ��� Ŀ�� �Ⱥ��̰� ����
        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.bVisible = FALSE;
        cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)
        SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
        SetConsoleCursorInfo(hScreenBuffer[0], &cursorInfo);
        SetConsoleCursorInfo(hScreenBuffer[1], &cursorInfo);

        //�⺻ �ܼ��� ȭ�� ũ�� ������ ��´�.
        CONSOLE_SCREEN_BUFFER_INFO Info; // �ʱ� ȭ�� �ܼ��� ȭ�� ����
        GetConsoleScreenBufferInfo(hConsoleHandle, &Info);

        nScreenHeight = Info.srWindow.Bottom - Info.srWindow.Top + 1;
        nScreenWidth = Info.srWindow.Right - Info.srWindow.Left + 1;
        nScreenBufferSize = nScreenWidth * nScreenHeight;

        // �ܼ� ��� UTF8�� ����
        SetConsoleOutputCP(CP_UTF8);
        system("mode con cols=120 lines=30");

    }

    void SetSmallFont(HANDLE hConsole, SHORT fontSize, const wchar_t* fontName)
    {
        CONSOLE_FONT_INFOEX cfi = { 0 };
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0; // Width: 0 = auto
        cfi.dwFontSize.Y = fontSize; // Height: ���ϼ��� �۾���
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcsncpy_s(cfi.FaceName, fontName, LF_FACESIZE);
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    }

    void ScreenFlipping()
    {
        // ���� �ܼ��� ����� ��ũ�������� Handle�� �����Ͽ� ȭ�鿡 �����ش�.
        SetConsoleActiveScreenBuffer(hScreenBuffer[nScreenBufferIndex]);
        // ������ ����� ��ũ�� ������ �ε����� �������� �غ��Ѵ�.
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
        FOREGROUND_BLUE	�ؽ�Ʈ ���� �Ķ����� ���Ե˴ϴ�.
        COMMON_LVB_LEADING_BYTE	���� ����Ʈ�Դϴ�.

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
        //Ư�� ��ġ�� ���ڿ��� ����Ѵ�.
        WriteConsoleOutputCharacterA(hScreenBuffer[nScreenBufferIndex], pStr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        bRval = FillConsoleOutputAttribute(hScreenBuffer[nScreenBufferIndex], attr, nNumberOfBytesToWrite, cdPos, &dwCharsWritten);
        if (bRval == false) printf("Error, FillConsoleOutputAttribute()\n");
        return bRval;
    }

    void ConsoleRenderer::ScreenDrawMultilineString(int x, int y, const char* str, WORD attr)
    {
        int offsetY = 0;
        const char* lineStart = str;
        while (lineStart)
        {
            const char* lineEnd = strchr(lineStart, '\n');
            std::string line;
            if (lineEnd)
            {
                line = std::string(lineStart, lineEnd);
                lineStart = lineEnd + 1;
            }
            else
            {
                line = std::string(lineStart);
                lineStart = nullptr;
            }
            ScreenDrawString(x, y + offsetY, line.c_str(), attr);
            offsetY++;
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
};



