#include "pch.h"
#include "Systems.h"

int main()
{
	SM * system_manager = SM::instance();

	system_manager->enter();
	system_manager->loop();
	system_manager->exit();

	return 0;
}