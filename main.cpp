#include <iostream>
#include <windows.h>
#include "tank.h"


int main()
{
	system("title Ì¹¿Ë´óÕ½");
	system("mode con cols=90 lines=45");
	
	startMenu();
	while (true)
	{
		if(interval[0]++%speed==0)
		{
			
			if (interval[10]++ %2 == 0)
			{
				updateWithInput();
			}
			updateWithoutInput();
			show();
			gameCheck();
			
		}
		Sleep(5);
	}

	std::cin.get();
	return 0;
}