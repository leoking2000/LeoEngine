#include "triliza.h"
#include <iostream>

void Triliza::OnActivate()
{
	m_table[0] = TileType::Empty;
	m_table[1] = TileType::Empty;
	m_table[2] = TileType::Empty;

	m_table[3] = TileType::Empty;
	m_table[4] = TileType::Empty;
	m_table[5] = TileType::Empty;

	m_table[6] = TileType::Empty;
	m_table[7] = TileType::Empty;
	m_table[8] = TileType::Empty;

	m_X_isPlaying = true;
	m_win = false;
}

void Triliza::OnUpdate(f32 dt)
{
	RenderUI();

	if (m_win) 
	{
		CloseApplication();
		return;
	}

	// get user input
	std::cout << "\nPlay: ";
	u32 userInput = 0;
	std::cin >> userInput;

	// check user input
	if (userInput == 0 || userInput > 9)
	{
		std::cin.get();
		system("cls");
		return;
	}

	// check if the tile is not empty
	if (m_table[userInput - 1] != TileType::Empty)
	{
		std::cin.get();
		system("cls");
		return;
	}

	// place the symbol in the tile
	m_table[userInput - 1] = (m_X_isPlaying ? TileType::X : TileType::O);

	// check for win
	if (CheckForWin())
	{	
		m_win = true;

		std::cin.get();
		system("cls");
		return;
	}

	m_X_isPlaying = !m_X_isPlaying;
	std::cin.get();
	system("cls");
}

void Triliza::RenderUI()
{
	std::cout << "Triliza: " << (m_X_isPlaying ? "X" : "O") << " is Playing\n\n";

	for (u32 col = 0; col < 3; col++)
	{
		for (u32 row = 0; row < 3; row++)
		{
			TileType t = m_table[row + col * 3];

			switch (t)
			{
			case TileType::Empty:
				std::cout << "| |";
				break;
			case TileType::O:
				std::cout << "|O|";
				break;
			case TileType::X:
				std::cout << "|X|";
				break;
			}
		}
		std::cout << "\n";
	}

	if (m_win)
	{
		std::cout << "\nPlayer " << (m_X_isPlaying ? "X" : "O") << " WON!!!";

		std::cin.get();
	}
}

bool Triliza::CheckForWin()
{
	TileType t = (m_X_isPlaying ? TileType::X : TileType::O);

	// check row wins
	for (u32 col = 0; col < 3; col++)
	{
		if (m_table[0 + 3 * col] == t                    && 
			m_table[0 + 3 * col] == m_table[1 + 3 * col] && 
			m_table[1 + 3 * col] == m_table[2 + 3 * col])
		{
			return true;
		}
	}

	// check col wins
	for (u32 row = 0; row < 3; row++)
	{
		if (m_table[row + 3 * 0] == t &&
			m_table[row + 3 * 0] == m_table[row + 3 * 1] &&
			m_table[row + 3 * 1] == m_table[row + 3 * 2])
		{
			return true;
		}
	}

	// check diag
	if (m_table[0] == t &&
		m_table[0] == m_table[4] &&
		m_table[4] == m_table[8])
	{
		return true;
	}

	// check rev diag
	if (m_table[2] == t &&
		m_table[2] == m_table[4] &&
		m_table[4] == m_table[6])
	{
		return true;
	}


	return false;
}
