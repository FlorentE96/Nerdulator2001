#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <stdint.h>
#include <iostream>

//EnableWindow()  //disable buttons when in BIN mode

HINSTANCE hInst;
int16_t disp_num;
int16_t result;
bool flag;
int16_t operand1 = 0;
int16_t operand2 = 0;
int16_t * operand_ptr = &operand1;

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
        *operand_ptr = disp_num;
    }
}

void makeCalculation() {
    if(operand_ptr == &operand1)
        operand_ptr = &operand2;
    else
        operand_ptr = &operand1;

    switch(currentOp) {
    case SUM:
        std::cout << operand1 << "+" << operand2 << std::endl;
        result = operand1 + operand2;
        break;
    case SUB:
        std::cout << operand1 << "-" << operand2 << std::endl;
        result = operand1 - operand2;
        break;
    case DIV:
        result = disp_num / result;
        break;
    case MUL:
        result = disp_num * result;
        break;
    case NOT:
        result = ~disp_num;
        break;
    case AND:
        result = disp_num & result;
        break;
    case OR:
        result = disp_num | result;
        break;
    case XOR:
        result = disp_num ^ result;
        break;
    }
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
            switch(LOWORD(wParam))
            {
            case BUT_QUIT:
                if(MessageBox(hwndDlg, "Do you really want to quit?", "Confirmation", MB_OKCANCEL) != IDCANCEL)
                    EndDialog(hwndDlg,0);
                break;
            case BUT_ABOUT:
                MessageBox(hwndDlg, "blablabla", "About", MB_OK);
                break;
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
            case OP_SUM:
                currentOp = SUM;
                makeCalculation();
                break;
            case OP_SUB:
                currentOp = SUB;
                makeCalculation();
                break;
            case OP_DIV:
                currentOp = DIV;
                makeCalculation();
                break;
            case OP_MUL:
                currentOp = MUL;
                makeCalculation();
                break;
            case OP_AND:
                currentOp = AND;
                makeCalculation();
                break;
            case OP_OR:
                currentOp = OR;
                makeCalculation();
                break;
            case OP_NOT:
                currentOp = NOT;
                makeCalculation();
                break;
            case OP_XOR:
                currentOp = XOR;
                makeCalculation();
                break;
            case OP_EQ:
                makeCalculation();
                break;
            case BUT_C:
                currentOp = NOOP;
                disp_num = 0;
                result=0;
                break;
            }
            SetDlgItemInt(hwndDlg, RESULT_BOX,disp_num,0);
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
