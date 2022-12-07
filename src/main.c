#include "../include/system.h"

int main(int argc, char* argv[])
{
    //Default console init
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    //MoveWindow(console, r.left, r.top, 360, 360, TRUE); // 360 width, 360 height

    //Init of struct
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    int rows;
    
    //to get width and height of the console
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    //Unsigned for security // NOT necessary
    unsigned char key_user_pressed;

    //Selection follower for the indexes
    int selected = 0;

    //Cursor hide
    cursor(0);
    
    //Menu init by display
    display_options(&selected, &columns, &rows);

    while (!(GetKeyState(VK_F4) < 0))
    {
        //Get current width/height of console 16*35
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        //Cursor hide (resizing window resets this)
        cursor(0);

        //if height or width changes, we clear screen and update it with the new height
        if (columns != csbi.srWindow.Right - csbi.srWindow.Left + 1 || rows != csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
        {
            //Windows screen clear
            system("cls");
            //Width of the console
            columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            //Height of the console
            rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

            //Display menu borders
            
            //Display options for the menu
            display_options(&selected, &columns, &rows);
        }

        //So screen will always refresh, _khbit checks if a key is awaiting in stdin
        if (_kbhit())
        {
            /*
                We couldn't use GetKeyState() function from Win api because of that its behaviour is different from getch()
                thus, when you press a key, it will return a value as long as key is being pressed.
                Would be useful for games but not our case, also the enter press detection was unecessary complex, thus i used keystate
            */
            //First getch
            key_user_pressed = getch();
            //for detect the function\arrow keys 
            //we must call the getch() again
            //testing if a is '0' or '0xE0'
            if (key_user_pressed == 0 || key_user_pressed == 0xE0) key_user_pressed = getch();

            if (key_user_pressed == 27) //ESC -> we don't need the esc sequence so we break
                break;
            else if (key_user_pressed == 72) //KEYUP
            {
                if (selected == 0)
                    selected = 6;
                else
                    selected--;
                //Display options for the menu
                display_options(&selected, &columns, &rows);
            }
            else if (key_user_pressed == 80) //KEYDOWN
            {
                if (selected == 6)
                    selected = 0;
                else
                    selected++;
                //Display options for the menu
                display_options(&selected, &columns, &rows);
            }
            //Enter key press
            if (GetKeyState(VK_RETURN) < 0)
            {
                exit(0);
            }
        }
    }
    //Clear screen when exiting
    system("cls");
    
    //Setting back console Height and Width
    MoveWindow(console, r.left, r.top, r.right, r.bottom, TRUE); 
}
