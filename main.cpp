#include <iostream>
#include <thread>
#include <string>
#include <iomanip>
#include <algorithm> // for transform

struct vector2d{
	int x , y;
};

int num_of_rows, num_of_cols;

vector2d player_pos;
vector2d new_pos;

bool isRunning = true;
bool hasInput = false;
std::string isInvalidMsg = "";
std::string infoMsg = "";

char map[10][10] = {
		{'#','#','#','#','D','#','#','#','#','G'},
		{'#','.','#','#','.','#','#','#','#','.'},
		{'#','.','#','#','.','.','.','#','.','.'},
		{'#','.','#','#','#','#','.','.','.','#'},
		{'#','.','.','#','#','#','.','#','.','#'},
		{'#','.','.','.','.','.','.','#','.','#'},
		{'#','.','.','#','#','.','#','#','.','#'},
		{'#','.','.','#','#','.','#','#','.','D'},
		{'#','#','.','.','#','D','#','#','#','#'},
		{'#','#','P','.','#','#','#','#','#','#'},
};

void swap_player_pos(vector2d aPlayerPos, vector2d aNewPos)
{
	if (map[aNewPos.x][aNewPos.y] == 'D')
	{
		infoMsg = "You died!";
	}
	else
	{
		if (map[aNewPos.x][aNewPos.y] == 'G')
		{
			infoMsg = "You successfully reached the GOAL!!!";
		}

		map[aPlayerPos.x][aPlayerPos.y] = '.';
		map[aNewPos.x][aNewPos.y] = 'P';
		player_pos = aNewPos;
	}
	if (infoMsg.size() > 0)
		isRunning = false;
}

std::string process_input(std::string aString)
{
	std::transform(aString.begin(), aString.end(), aString.begin(), ::tolower);

	if (aString.empty())
	{
		return "The input inserted is invalid. Please try again.";
	}


	if (aString.size() == 1)
	{
		new_pos = {-1,-1}; //set invalid input
		if (aString[0] == 'n')
		{
			new_pos = { player_pos.x - 1, player_pos.y};
		}
		if (aString[0] == 's')
		{
			new_pos = { player_pos.x + 1, player_pos.y};
		}
		if (aString[0] == 'e')
		{
			new_pos = { player_pos.x, player_pos.y + 1};
		}
		if (aString[0] == 'w')
		{
			new_pos = { player_pos.x, player_pos.y - 1};
		}

		if (new_pos.x == -1 && new_pos.y == -1)
		{
			return "The input inserted is invalid. Please try again.";
		}

		if (new_pos.x > num_of_cols || new_pos.y > num_of_rows)
		{
			return "Cannot move like this.";
		}

		if (map[new_pos.x][new_pos.y] == '#')
		{
			return "Cannot move through a wall";
		}
	}

	if (aString.size() == 2)
	{

	}

	return "";
}

void get_input()
{
	std::string lString;
	while (isRunning)
	{
		if (!hasInput)
		{
			std::getline(std::cin,lString, '\n');
			isInvalidMsg = process_input(lString);
			hasInput = true;
		}
	}
}

void update()
{
	if(isInvalidMsg.size() == 0)
	{
		swap_player_pos(player_pos, new_pos);
	}
}

void draw_compass(int aIndex)
{
	static int lCompassCount = 0;
	int lmiddle = num_of_rows/2 - 3;
	if (aIndex > lmiddle && lCompassCount < 5)
	{
		//Draw mini compass
		switch (lCompassCount) {
		case 0:
			std::cout << std::setfill(' ') << std::setw(10) << "" << "^";//		^
			break;
		case 1:
			std::cout << std::setfill(' ') << std::setw(10) << "" << "N";//		N
			break;
		case 2:
			std::cout << std::setfill(' ') << std::setw(6) << "" << "< W" << std::setfill(' ') << std::setw(6) << "E >";//< W		E >
			break;
		case 3:
			std::cout << std::setfill(' ') << std::setw(10) << "" << "S";//		S
			break;
		case 4:
			std::cout << std::setfill(' ') << std::setw(10) << "" << "v";//      v
			break;
		default:
			break;
		}
		lCompassCount++;
	}
}

void draw_map()
{
	std::cout << std::setfill('*') << std::setw(8) << "" << "The GridWorld Map" << std::setfill('*') << std::setw(8) << "\n";
	std::cout << std::endl;
	for (int i = 0; i < num_of_rows; ++i) {
		std::cout << std::setfill(' ') << std::setw(12) << ""; // need to front space width manually
		for (int j = 0; j < num_of_cols; ++j) {
			std::cout << map[i][j];
		}
		draw_compass(i);
		std::cout << "\n";
	}
	std::cout << "\n";
}

void display_player_pos()
{
	std::cout << "Player location is " << player_pos.x + 1 << ", " << player_pos.y + 1 << std::endl;
}

void display_movement()
{
	//Movement allow are : N, S, W, E
	std::cout << "You can move N, S, W, E" << std::endl;
}

void display()
{
	if (isInvalidMsg.size() == 0)
	{
		draw_map();
		display_player_pos();
	}
	else{
		std::cout << isInvalidMsg << std::endl;
	}
	display_movement();
}

void init()
{
	num_of_rows = sizeof(map)/sizeof(map[0]);
	num_of_cols = sizeof(map[0])/sizeof(char);
	for (int i = 0; i < num_of_rows; ++i) {
		for (int j = 0; j < num_of_cols; ++j) {
			if (map[i][j] == 'P')
			{
				player_pos = {i, j};
			}
		}
	}
	display();
}

void handle_end()
{
	std::cout << infoMsg << std::endl;
}

void run_gameloop()
{
	std::thread inputThread(get_input);
	while(isRunning)
	{
		if (hasInput)
		{
			hasInput = false;
			update();

			// TODO: Better way to handle death
			if (!isRunning)
			{
				inputThread.detach(); // need to remove thread from main thread as it already point to cin when the map renders
				break;
			}
			display();
		}
	}
	//TODO: Make thread accessible by other method to terminate them
}

int main()
{
	init();
	run_gameloop();
	handle_end();
	return 0;
}
