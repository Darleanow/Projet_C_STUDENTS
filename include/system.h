#pragma once
#include <string.h>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>


//Go to an XY location on the console screen
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

//Outputs White Background and Black text to emulate a selection
#define selected() printf("\033[30;47m")

//Resets the Background and text colors to default
#define reset() printf("\033[0;0m")

//Sets Font to black, and background to a nice pale purple
#define purp_back() printf("\033[30;48;5;146m")

//Get Console data 
typedef struct DEFAULT_CONSOLE {
	SHORT Left;
	SHORT Top;
	SHORT Right;
	SHORT Bottom;
} DEFAULT_CONSOLE;

//Enable / Disable cursor
void cursor(int show_hide)
{
	//Get handle from stdout
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//Get cursor info
	CONSOLE_CURSOR_INFO info;

	//Size of the structure (bytes), Process32First fails if not initialized
	info.dwSize = 100;

	if (show_hide == 1)
	{
		info.bVisible = TRUE;
	}
	else
	{
		info.bVisible = FALSE;
	}

	//Update &info from CONSOLE_CURSOR_INFO
	SetConsoleCursorInfo(consoleHandle, &info);
}


//Adds an option box based on the size of the screen, its value, its index, and if its selected
//Basically some maths, index makes it possible to not overflow the boxes
/*
MATHS:
get the number of lines each box should be			: number_of_rows / number_of_options + 10
get the starting line from each box based on index	: ((box_width * index) / 10) + 1
*/
void optionBox(char** array_Values[], int *columns, int *rows, int is_selected, int index)
{
	//Evaluate each box width base on the rows available
	int box_Width = (int)(*rows / (sizeof(*array_Values) / sizeof(*array_Values[0]))) +10;

	//Evaluate when to start drawing outer box
	//We use the index provided from our array, as well as the array length
	int draw_Start = ((box_Width * index) / 10)+1;
	
	//Top Box Draw
	gotoxy(1, draw_Start);

	//Top outer line of each box
	printf("+");
	for (int i = 0; i < *columns-2; i++)
	{
		printf("-");
	}
	printf("+");	
	
	//Fill stdout (console screen) with spaces and draw left/right borders
	for (int actual_line = 1; actual_line < (box_Width/10); actual_line++)
	{
		gotoxy(0, draw_Start+actual_line);
		printf("|");

		if (is_selected == 1)
			purp_back();

		for (int line_fill = 0; line_fill < *columns - 2; line_fill++)
		{
			printf(" ");
		}

		if (is_selected == 1)
			reset();

		printf("|");
	}

	//Display the text at the center of each box on XY axis
	gotoxy((int)(*columns) / 2 - (int)(strlen(array_Values[index]) / 2), draw_Start + 2);
	if (is_selected == 1)
		purp_back();
	printf("%s", array_Values[index]);

	reset();
	gotoxy(0, draw_Start + ((box_Width / 10)));
	printf("+");
	for (int i = 0; i < *columns - 2; i++)
	{
		printf("-");
	}
	printf("+");
	
}


void display_options(int *selected, int *columns, int *rows)
{
	//Main Menu Values

	char* arr_values[] = { "Add a student", "Delete student(s)", "View students", "Add a module",
		"Delete module(s)", "View modules", "Help" };

	///////////////////////////////////////////////////////////////////////////////////////
	
	int arr_size = (int) (sizeof(arr_values) / sizeof(arr_values[0]));

	for (int check_vals = 0; check_vals < arr_size; check_vals++)
	{
		if (check_vals == *selected)
		{
			optionBox(&arr_values, columns, rows, 1, check_vals);
		}
		else
		{
			optionBox(&arr_values, columns, rows, 0, check_vals);
		}
	}
}
