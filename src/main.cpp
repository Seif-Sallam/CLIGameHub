#include <iostream>
#include <stdio.h>

#include <windows.h>

uint32_t windowWidth = 0;


void setWindowSize(uint32_t width, uint32_t height)
{
	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

	consolesize.cbSize=sizeof(consolesize);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfoEx(hConsole,&consolesize);

	COORD c;
	c.X = width;
	c.Y = height;
	consolesize.dwSize = c;

	consolesize.srWindow.Left = 0;
	consolesize.srWindow.Right = width;
	consolesize.srWindow.Top = 0;
	consolesize.srWindow.Bottom = height;

	SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
}

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
	hStdOut,
	(TCHAR) ' ',
	cellCount,
	homeCoords,
	&count
	)) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
	hStdOut,
	csbi.wAttributes,
	cellCount,
	homeCoords,
	&count
	)) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
struct Game;

void centerString(const std::string_view& string, uint32_t width)
{
	uint32_t oneSide = width/2 - string.size() /2;
	for (int i = 0; i < oneSide; i++)
		printf(" ");
	printf("%s", string.data());
	for (int i = 0; i < oneSide; i++)
		printf(" ");
}

void resetView(uint32_t rows, uint32_t columns)
{
	for(int j = 0; j < rows; j++)
	{
		printf("\r");
		for (int i = 0; i < columns; i++)
			printf("#");
		printf("\r\b");
	}
}
struct Game
{
	Game() = default;
	~Game() = default;

	virtual void update(const size_t& tick) {}
	virtual void render() {}

	virtual std::string_view toString() {
		return std::string_view("DefaultGame");
	}

	bool isRunning = true;
};

struct MainMenu : public Game
{
	MainMenu() = default;
	~MainMenu() = default;

	void update(const size_t& tick) override final {

	}

	void render() override final {

	}

	std::string_view toString() override final {
		return std::string_view("Main Menu");
	}
};

void renderHeader(Game* game)
{

	for (uint32_t i = 0; i < windowWidth; i++)
		printf("-");
	printf("\n");

	centerString(game->toString(), windowWidth);
	printf("\n");

	for (uint32_t i = 0; i < windowWidth; i++)
		printf("-");
	printf("\n");
}

int main()
{
	MainMenu mainMenu;

	Game* currentGame{&mainMenu};

	uint32_t rows = 10, columns = 10;

	windowWidth = 100;
	size_t tickCount = 0;
	while(currentGame->isRunning)
	{
		setWindowSize(100, 50);
		renderHeader(currentGame);
		currentGame->update(tickCount);
		currentGame->render();

		tickCount++;
		std::cin.get();
		ClearScreen();
	}


	return 0;
}