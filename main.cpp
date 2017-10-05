#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <stdint.h>
#include <iostream>

#define IS_DIGIT_BUTTON(x) (x>=50000 && x<60000)
#define IS_OP_BUTTON(x) (x<50000 && x>=40000)
#define IS_SEL_BUTTON(x) (x>=60000)

using namespace std;

//EnableWindow()  //disable buttons when in BIN mode

/***************** TYPEDEFS *********************/
typedef enum NumMode {DEC,BIN} NumMode;
typedef enum OpCodes {NOOP = 0, DIV, MUL, SUM, SUB, AND, OR, XOR, NOT} OpCodes;
/************************************************/

/************** GLOBAL VARIABLES ****************/
HINSTANCE hInst;
HWND myHandle;
int16_t disp_num;
int16_t result;
bool flag, flagMake = true;

int16_t operand1 = 0;
int16_t operand2 = 0;

OpCodes currentOp;
NumMode currentMode;
/************************************************/

/************ FUNCTION PROTOTYPES ***************/
void makeCalculation();
void setResultValue(int inc);
/************************************************/

/**************** MY FUNCTIONS ******************/
void setResultValue(int inc)
{
    if(flag)
    {
        disp_num = inc;
        flag = false;
    }
    else if(disp_num < 3276)
    {
        if(currentMode == DEC){
            disp_num *= 10;
            disp_num += inc;
        }
        else
        {
            disp_num *= 2;
            disp_num += inc;
        }

    }
    operand2 = disp_num;
}

void makeCalculation() {
    switch(currentOp) {
    case SUM:
        result = operand1 + operand2;
        cout << operand1 << "+" << operand2 << "=" << result << endl;
        break;
    case SUB:
        result = operand1 - operand2;
        cout << operand1 << "-" << operand2 << "=" << result << endl;
        break;
    case DIV:
        if (!(operand2==0)){
        result = operand1 / operand2;
        } else {
        MessageBox(myHandle, "Don't divide by zero YO", "Fuck you", MB_OK);
        }
        cout << operand1 << "/" << operand2 << "=" << result << endl;
        break;
    case MUL:
        result = operand1 * operand2;
        cout << operand1 << "*" << operand2 << "=" << result << endl;
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

void toBinary(int16_t input , char * output){
    int16_t divider = input;
    int i = 0;
    if(input>=0){
        do {
            output[i++] = divider % 2 + '0';
            divider = (int)(divider / 2);
        } while(divider!=0);
        output[i]=0;
        int j,k = 0;
        for(j=i-1; j>=i/2; j--,k++){
            char val = output[j];
            output[j] = output[k];
            output[k] = val;
        }
    }
}
/************************************************/
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    myHandle = hwndDlg;
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        currentOp = NOOP;
        result = 0;
        CheckRadioButton(hwndDlg, SEL_DEC, SEL_BIN, SEL_DEC);
        SetDlgItemInt(hwndDlg, RESULT_BOX,0,0);
        cout << "::: Nerdulator2001 History :::" << endl;
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
            if(IS_DIGIT_BUTTON(LOWORD(wParam)))
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
                    cout << "updating operand2 from " << operand2 << " to " << disp_num << endl;
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

            } else if(IS_SEL_BUTTON(LOWORD(wParam)))
            {
                switch(LOWORD(wParam))
                {
                case SEL_DEC:
                    currentMode = DEC;
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT2),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT3),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT4),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT5),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT6),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT7),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT8),true);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT9),true);
                    break;
                case SEL_BIN:
                    currentMode = BIN;
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT2),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT3),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT4),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT5),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT6),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT7),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT8),false);
                    EnableWindow(GetDlgItem(hwndDlg, DIGIT9),false);
                    break;
                }

            } else

            {
                makeCalculation();
                flagMake = false;
            }
            char myString[1024];
            if(currentMode == BIN)
                toBinary(disp_num, myString);
            else
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
