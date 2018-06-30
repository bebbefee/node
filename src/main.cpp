#include <csignal>
#include "master/master.h"

int main(void)
{
	signal(SIGINT, [](int){}); 

	Master& master = Master::GetInstance(); 

	master.Init(); 
	master.Run(); 

	return 0; 
}
