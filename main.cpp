#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <stdint.h>
#include <iostream>

#define IS_DIGIT_BUTTON(x) (x>=50000)
#define IS_OP_BUTTON(x) (x<50000 && x>=40000)

//EnableWindow()  //disable buttons when in BIN mode

HINSTANCE hInst;
int16_t disp_num;
int16_t result;
bool flag, flagMake = true;

int16_t operand1 = 0;
int16_t operand2 = 0;

typedef enum OpCodes {NOOP = 0, DIV, MUL, SUM, SUB, AND, OR, XOR, NOT} OpCodes;
OpCodes currentOp;

void setResultValue(int inc)
{
    if(flag)
    {
        disp_num = inc;
        flag = false;
    }
    else if(disp_num < 3276)
    {
        disp_num *= 10;
        disp_num += inc;
    }
}

void makeCalculation() {
    switch(currentOp) {
    case SUM:
        result = operand1 + operand2;
        std::cout << operand1 << "+" << operand2 << "=" << result << std::endl;
        break;
    case SUB:
        result = operand1 - operand2;
        std::cout << operand1 << "-" << operand2 << "=" << result << std::endl;
        break;
    case DIV:
        result = operand1 / operand2;
        std::cout << operand1 << "/" << operand2 << "=" << result << std::endl;
        break;
    case MUL:
        result = operand1 * operand2;
        std::cout << operand1 << "*" << operand2 << "=" << result << std::endl;
        break;
    case NOT:
        result = ~disp_num;
        break;
    case AND:
        result = operand1 & operand2;
        break;
    case OR:
        result = operand1 | operand2;
        break;
    case XOR:
        result = operand1 ^ operand2;
        break;
    default:
        operand1 = disp_num;
        flag = true;
        return;
        break;
    }
    operand1 = result;
    disp_num = result;
    flag = true;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        currentOp = NOOP;
        result = 0;
        CheckRadioButton(hwndDlg, SEL_DEC, SEL_BIN, SEL_DEC);
        SetDlgItemInt(hwndDlg, RESULT_BOX,0,0);
        std::cout << "::: Nerdulator2001 History :::" << std::endl;
        break;
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        if(HIWORD(wParam) == BN_CLICKED)
        {
            if(LOWORD(wParam)>=50000)
            {
                switch(LOWORD(wParam))
                {
                case DIGIT1:
                    setResultValue(1);
                    break;
                case DIGIT2:
                    setResultValue(2);
                    break;
                case DIGIT3:
                    setResultValue(3);
                    break;
                case DIGIT4:
                    setResultValue(4);
                    break;
                case DIGIT5:
                    setResultValue(5);
                    break;
                case DIGIT6:
                    setResultValue(6);
                    break;
                case DIGIT7:
                    setResultValue(7);
                    break;
                case DIGIT8:
                    setResultValue(8);
                    break;
                case DIGIT9:
                    setResultValue(9);
                    break;
                case DIGIT0:
                    setResultValue(0);
                    break;
                }
                flagMake = true;
            } else if(IS_OP_BUTTON(LOWORD(wParam)))
            {
                if(flagMake) {
                    operand2 = disp_num;
                    makeCalculation();
                }
                switch(LOWORD(wParam))
                {
                case BUT_QUIT:
                    if(MessageBox(hwndDlg, "Do you really want to quit?", "Confirmation", MB_OKCANCEL) != IDCANCEL)
                        EndDialog(hwndDlg,0);
                    break;
                case BUT_ABOUT:
                    MessageBox(hwndDlg, "We are the coolest.", "About", MB_OK);
                    break;
                case OP_SUM:
                    currentOp = SUM;
                    break;
                case OP_SUB:
                    currentOp = SUB;
                    break;
                case OP_DIV:
                    currentOp = DIV;
                    break;
                case OP_MUL:
                    currentOp = MUL;
                    break;
                case OP_AND:
                    currentOp = AND;
                    break;
                case OP_OR:
                    currentOp = OR;
                    break;
                case OP_NOT:
                    currentOp = NOT;
                    break;
                case OP_XOR:
                    currentOp = XOR;
                    break;
                case BUT_C:
                    currentOp = NOOP;
                    disp_num = 0;
                    operand1 = 0;
                    operand2 = 0;
                    result=0;
                    break;
                }
                flagMake = false;
            } else
            {
                makeCalculation();
                flagMake = false;
            }
            char myString[1024];
            sprintf(myString, "%d", disp_num);
            SetDlgItemText(hwndDlg, RESULT_BOX,myString);
        }

    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgMain);
}
