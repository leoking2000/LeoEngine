#pragma once
#include <LEO/LeoEngine.h>

enum class TileType
{
	Empty = 0,
	O = 1,
	X = 2
};

class Triliza : public LEO::Scene
{
public:
	Triliza() = default;
public:
	void OnCreate() override {}
	void OnActivate() override;
	void OnEvent(const LEO::Event& e) {}
	void OnUpdate(f32 dt) override;
	void OnRender() override {}
public:
	void RenderUI();
	bool CheckForWin();
private:
	bool m_X_isPlaying = false;
	bool m_win = false;

	TileType m_table[9];
};
