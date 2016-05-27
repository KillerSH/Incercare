#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<conio.h>
#include<limits.h>

#define BUTTON_ID      1001
#define NO_OF_CHARS 256

HWND button, button2, button3, button4, button_exit, button_cenzura1, button_back, button_exit2;
HWND TextBox;
HWND HwndLabel, explic;
HWND hWndButton, hWndButton2, hWndButton3;
HWND Box1, Box2;
HWND hsti, hsti2;
HBITMAP hBitmap;

int count = 0, numbs = 0;
char **v = NULL;
char replace[20], ch = '*', *retin;

// A utility function to get maximum of two integers
int maxim(int a, int b) { return (a > b) ? a : b; }

// The preprocessing function for Boyer Moore's bad character heuristic
void badCharHeuristic(char *str, int size, int badchar[NO_OF_CHARS])
{
	int i;

	// Initialize all occurrences as -1
	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	// Fill the actual value of last occurrence of a character
	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

/* A pattern searching function that uses Bad Character Heuristic of
Boyer Moore Algorithm */
int search(char *txt, char *pat, int s)
{
	int m = strlen(pat);
	int n = strlen(txt);

	int badchar[NO_OF_CHARS];

	/* Fill the bad character array by calling the preprocessing
	function badCharHeuristic() for given pattern */
	badCharHeuristic(pat, m, badchar);

	while (s <= (n - m))
	{
		int j = m - 1;

		/* Keep reducing index j of pattern while characters of
		pattern and text are matching at this shift s */
		while (j >= 0 && pat[j] == txt[s + j])
			j--;

		/* If the pattern is present at current shift, then index j
		will become -1 after the above loop */
		if (j < 0)
		{
			//printf("\n pattern occurs at shift = %d", s);

			/* Shift the pattern so that the next character in text
			aligns with the last occurrence of it in pattern.
			The condition s+m < n is necessary for the case when
			pattern occurs at the end of text */

			return s;

			s += (s + m < n) ? m - badchar[txt[s + m]] : 1;
		}

		else
		{
			/* Shift the pattern so that the bad character in text
			aligns with the last occurrence of it in pattern. The
			max function is used to make sure that we get a positive
			shift. We may get a negative shift if the last occurrence
			of bad character in pattern is on the right side of the
			current character. */
			s += max(1, j - badchar[txt[s + j]]);
		}
	}
	return 0;
}

void cenzura(char *v, int poz, int lungime, char c)
{
	for (int i = poz; i < poz + lungime; i++)
		v[i] = c;
}


void Inlocuire(int lungime)
{
	FILE *outstrem = fopen("Out.txt", "w");

	for (int j = 0; j < lungime; j++)
	{
		fprintf(outstrem, "%s", v[j]);
	}

	fclose(outstrem);
}

int verifica_first(char *v, char *pat)
{
	bool x = true;

	for (int i = 0; i < strlen(pat) && x == true; i++)
	{
		if (v[i] == pat[i])
			x = true;
		else
			x = false;
	}

	if (x)
		return 1;
	else
		return 0;
}

int citire(HWND hwnd, char *path)
{
	int i = 0, j = 0;
	FILE *stream = fopen(path, "r");

	if (stream == NULL)
	{
		return 0;
	}
	else
	{
		char cuvant[100];

		while (!feof(stream))
		{
			fgets(cuvant, 100, stream);

			v = (char**)realloc(v, (i + 1)*sizeof(char*));
			v[i] = (char*)malloc(strlen(cuvant)*sizeof(char));
			strcpy(v[i], cuvant);
			i++;
		}

		return i;
	}

	fclose(stream);
}

void LoadMyImage() {

	hBitmap = LoadImageW(NULL, L"C:\\Users\\stefa\\Desktop\\New\\New\\cenzura.bmp", IMAGE_BITMAP,
		825, 530, LR_LOADFROMFILE);
}

void LoadMyImage2()
{
	hBitmap = LoadImageW(NULL, L"C:\\Users\\stefa\\Desktop\\New\\New\\alb.bmp", IMAGE_BITMAP,
		1200, 720, LR_LOADFROMFILE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool chk = false;

	switch (message)
	{
	case WM_CREATE:
	{
		LoadMyImage();
		hsti = CreateWindow("Static", NULL,
			WS_CHILD | WS_VISIBLE | SS_BITMAP,
			125, 100, 625, 301, hwnd, NULL, NULL, NULL);

		SendMessage(hsti, STM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		LoadMyImage2();
		hsti2 = CreateWindow("Static", NULL,
			WS_CHILD | WS_VISIBLE | SS_BITMAP,
			0, 0, 625, 301, hwnd, NULL, NULL, NULL);


		button = CreateWindow("BUTTON", "Incaraca Text", WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 40, 120, 30, hwnd, (HMENU)1, NULL, NULL);
		button2 = CreateWindow("BUTTON", "Deschide fisier", WS_VISIBLE | WS_CHILD | WS_BORDER, 840, 140, 120, 30, hwnd, (HMENU)2, NULL, NULL);
		button3 = CreateWindow("BUTTON", "Aplic cenzura", WS_VISIBLE | WS_CHILD | WS_BORDER, 840, 250, 120, 30, hwnd, (HMENU)6, NULL, NULL);
		button_exit = CreateWindow("BUTTON", "EXIT", WS_VISIBLE | WS_CHILD | WS_BORDER, 840, 180, 120, 30, hwnd, (HMENU)3, NULL, NULL);
		TextBox = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 820, 40, 160, 20, hwnd, NULL, NULL, NULL);
		Box1 = CreateWindow("Button", "Verificari", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			800, 10, 200, 220, hwnd, 0, GetModuleHandle(0), 0);
		Box2 = CreateWindow("Button", "Corectari", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			800, 200, 200, 300, hwnd, 0, GetModuleHandle(0), 0);
		button4 = CreateWindow("BUTTON", "Scaneaza", WS_VISIBLE | WS_CHILD | WS_BORDER, 860, 75, 80, 25, hwnd, (HMENU)4, NULL, NULL);
		button_cenzura1 = CreateWindow("BUTTON", "CENZURA", WS_VISIBLE | WS_CHILD | WS_BORDER, 840, 290, 120, 30, hwnd, (HMENU)5, NULL, NULL);
		button_back = CreateWindow("BUTTON", "Back to 'Verificari'", WS_VISIBLE | WS_CHILD | WS_BORDER, 830, 120, 150, 30, hwnd, (HMENU)7, NULL, NULL);
		explic = CreateWindow("STATIC", TEXT("Alegeti modul de cenzura:"), WS_CHILD | WS_VISIBLE | SS_LEFT, 815, 305, 175, 20, hwnd, NULL, NULL, NULL);
		button_exit2 = CreateWindow("BUTTON", "EXIT", WS_VISIBLE | WS_CHILD | WS_BORDER, 830, 165, 150, 30, hwnd, (HMENU)3, NULL, NULL);
		

		// RadioButtons
		hWndButton = CreateWindowEx(0, "BUTTON", "*****", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			840, 350, 80, 20, hwnd, (HMENU)10, NULL, NULL);
		hWndButton2 = CreateWindowEx(0, "BUTTON", "#####", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			840, 375, 80, 20, hwnd, (HMENU)11, NULL, NULL);
		hWndButton3 = CreateWindowEx(0, "BUTTON", "@@@@@@", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
			840, 400, 90, 20, hwnd, (HMENU)12, NULL, NULL);

		SendDlgItemMessage(hwnd, 10, BM_SETCHECK, 1, 0);


		// Hide
		ShowWindow(hWndButton, SW_HIDE);
		ShowWindow(hWndButton2, SW_HIDE);
		ShowWindow(hWndButton3, SW_HIDE);
		ShowWindow(button_cenzura1, SW_HIDE);
		ShowWindow(button_back, SW_HIDE);

		ShowWindow(button2, SW_HIDE);
		ShowWindow(button3, SW_HIDE);
		ShowWindow(button4, SW_HIDE);
		ShowWindow(button_exit, SW_HIDE); 
		ShowWindow(button_exit2, SW_HIDE);
		ShowWindow(TextBox, SW_HIDE);

		ShowWindow(Box1, SW_HIDE);
		ShowWindow(Box2, SW_HIDE);
		ShowWindow(explic, SW_HIDE);

	}; break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1:
		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);			// SEE NOTE BELOW
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
			ofn.lpstrDefExt = "txt";

			if (GetOpenFileName(&ofn))
			{
				// Adresa se memoreaza in szFileName

				chk = true;

				count = citire(hwnd, szFileName);
				if (count)
				{
					int sep = 20;

					for (int u = 0; u < count; u++)
					{
						HwndLabel = CreateWindowW(L"STATIC", TEXT(v[u]),
							WS_CHILD | WS_VISIBLE | SS_LEFT,
							20, sep, 700, 30,
							hwnd, NULL, NULL, NULL);

						SendMessage(HwndLabel,     /*HWND*/        /*Label*/
							WM_SETTEXT,     /*UINT*/        /*Message*/
							NULL,           /*WPARAM*/      /*Unused*/
							(LPARAM)TEXT(v[u]));  /*LPARAM*/      /*Text*/

						sep = sep + 18;
					}
				}
				else
					MessageBox(hwnd, "Nu exista fisier de incarcat", "ATENTIUNE!!!", MB_OK);
			}
			else
				chk = false;

			
			// Interfata
			if (chk)
			{
				ShowWindow(hsti, SW_HIDE);
				ShowWindow(button, SW_HIDE);

				SendMessage(hsti2, STM_SETIMAGE,						// disparitia imaginii de la inceput
					(WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

				ShowWindow(button2, SW_SHOW);
				ShowWindow(button4, SW_SHOW);
				ShowWindow(TextBox, SW_SHOW);
				ShowWindow(button_exit, SW_SHOW);
				ShowWindow(Box1, SW_SHOW);
			}

			retin = (char *)malloc(sizeof(char)*strlen(szFileName));
			strcpy(retin, szFileName);

		}; break;


		case 2:
		{
			MessageBox(hwnd, "Se deschide fisierul original!", "Da bai", MB_OK);
			
			ShellExecute(hwnd, "open", retin, NULL, NULL, SW_SHOW);

		}; break;


		case 3:
		{
			int result = MessageBox(hwnd, "Esti sigur?", "Ce faci boss ?", MB_YESNO);

			if (result == IDYES)
			{
				ExitProcess(0);
			}
			else if (result == IDNO)
			{
				int x = 0;
			}

		}; break;


		case 4:
		{
			int gwstat = 0;
			char textul[20];
			char *t = &textul[0];

			strcpy(textul, "");
			gwstat = GetWindowText(TextBox, t, 20);
			strcpy(replace, textul);

			if (strcmp(textul, "") == 0)
			{
				MessageBox(hwnd, "Introdu un cuvant de scanat !!!", "ATENTIUNE", MB_OK);
			}
			else
			{
				for (int j = 0; j < count; j++)
				{
					if (verifica_first(v[j], textul))					// verific daca apare chiar pe prima pozitie
						numbs++;

					int x = 1;
					while (search(v[j], textul, x))
					{
						x = search(v[j], textul, x);
						x++;
						numbs++;
					}
				}

				char buffer[32];
				sprintf(buffer, "%d", numbs);
				strcat(buffer, " aparitii");
				MessageBox(hwnd, buffer, "ATENTIUNE", MB_OK);

				if (numbs)
					ShowWindow(button_cenzura1, SW_SHOW);
				else
					ShowWindow(button_cenzura1, SW_HIDE);

				numbs = 0;
			}

		}; break;


		case 5:
		{
			ShowWindow(button_cenzura1, SW_HIDE);

			ShowWindow(Box1, SW_HIDE);
			ShowWindow(TextBox, SW_HIDE);
			ShowWindow(button4, SW_HIDE);
			ShowWindow(button_exit, SW_HIDE);
			ShowWindow(button2, SW_HIDE);

			ShowWindow(Box2, SW_SHOW);
			ShowWindow(explic, SW_SHOW);
			ShowWindow(button3, SW_SHOW);
			ShowWindow(hWndButton, SW_SHOW);
			ShowWindow(hWndButton2, SW_SHOW);
			ShowWindow(hWndButton3, SW_SHOW);
			ShowWindow(button_back, SW_SHOW);
		}; break;


		case 6:
		{
			for (int j = 0; j < count; j++)
			{
				if (verifica_first(v[j], replace))					// cenzurez daca apare chiar pe prima pozitie
					cenzura(v[j], 0, strlen(replace), ch);

				while (search(v[j], replace, 0))
					cenzura(v[j], search(v[j], replace, 0), strlen(replace), ch);
			}

			Inlocuire(count);				// scriu in fisierul 'Out.txt' ceea ce am cenzurat anterior


			MessageBox(hwnd, "Cenzura efectuata!", "ATENTIUNE", MB_OK);
			MessageBox(hwnd, "Noul tau fisier cenzurat este 'Out.txt'", "ATENTIUNE", MB_OK);

			ShowWindow(button_exit2, SW_SHOW);
			ShowWindow(Box2, SW_HIDE);
			ShowWindow(explic, SW_HIDE);
			ShowWindow(button3, SW_HIDE);
			ShowWindow(hWndButton, SW_HIDE);
			ShowWindow(hWndButton2, SW_HIDE);
			ShowWindow(hWndButton3, SW_HIDE);

			ShellExecute(hwnd, "open", "Out.txt", NULL, NULL, SW_SHOW);

		}; break;


		case 7:
		{
			ShowWindow(Box1, SW_SHOW);
			ShowWindow(TextBox, SW_SHOW);
			ShowWindow(button4, SW_SHOW);
			ShowWindow(button_exit, SW_SHOW);
			ShowWindow(button2, SW_SHOW);

			ShowWindow(Box2, SW_HIDE);
			ShowWindow(explic, SW_HIDE);
			ShowWindow(button3, SW_HIDE);
			ShowWindow(hWndButton, SW_HIDE);
			ShowWindow(hWndButton2, SW_HIDE);
			ShowWindow(hWndButton3, SW_HIDE);
			ShowWindow(button_back, SW_HIDE);
			ShowWindow(button_exit2, SW_HIDE);
		}; break;


		case 10:
		{
			switch (HIWORD(wParam))
			{

			case BN_CLICKED:
			{
				if (SendDlgItemMessage(hwnd, 10, BM_GETCHECK, 0, 0) == 0)

				{
					SendDlgItemMessage(hwnd, 10, BM_SETCHECK, 1, 0);

					SendDlgItemMessage(hwnd, 11, BM_SETCHECK, 0, 0);

					SendDlgItemMessage(hwnd, 12, BM_SETCHECK, 0, 0);

					ch = '*';

				}

			}; break;

			}
		}; break;

		case 11:
		{
			switch (HIWORD(wParam))
			{

			case BN_CLICKED:
			{
				if (SendDlgItemMessage(hwnd, 11, BM_GETCHECK, 0, 0) == 0)

				{
					SendDlgItemMessage(hwnd, 11, BM_SETCHECK, 1, 0);

					SendDlgItemMessage(hwnd, 10, BM_SETCHECK, 0, 0);

					SendDlgItemMessage(hwnd, 12, BM_SETCHECK, 0, 0);

					ch = '#';
				}

			}; break;

			}
		}; break;

		case 12:
		{
			switch (HIWORD(wParam))
			{

			case BN_CLICKED:
			{
				if (SendDlgItemMessage(hwnd, 12, BM_GETCHECK, 0, 0) == 0)

				{
					SendDlgItemMessage(hwnd, 12, BM_SETCHECK, 1, 0);

					SendDlgItemMessage(hwnd, 11, BM_SETCHECK, 0, 0);

					SendDlgItemMessage(hwnd, 10, BM_SETCHECK, 0, 0);

					ch = '@';
				}

			}; break;
			}

		}; break;

		}
	}



	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		return 0;
	}; break;

	case WM_DESTROY:
	{	
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
	}; break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR lpszArgument, int nCmdShow)
{
	char szClassName[] = "Cenzura 1.4";
	WNDCLASSEX wc;
	MSG messages;
	HWND hwnd;

	wc.hInstance = hIns;
	wc.lpszClassName = szClassName, wc.lpfnWndProc = WndProc;
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION), wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW), wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0, wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetSysColorBrush(32);
	RegisterClassEx(&wc);



	hwnd = CreateWindow(szClassName, szClassName, WS_OVERLAPPEDWINDOW, 25, 10, 1100, 720, HWND_DESKTOP, NULL, hIns, NULL);
	ShowWindow(hwnd, nCmdShow);

	

	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}


	return messages.wParam;
}