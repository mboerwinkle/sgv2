typedef struct controlMap{
	double accel;//0 - 1
	double yaw;//-1 - 1
	double roll;//-1 - 1
	double pitch;//-1 - 1
	int fire;//this limits to 32 modules. doesnt really seem like a limit.
}controlMap;
