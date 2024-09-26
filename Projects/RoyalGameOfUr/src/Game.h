#pragma once
#include "GameRenderer.h"

class Game : public LEO::Scene
{
public:
	void OnCreate() override;
	void OnUpdate(f32 dt) override;
	void OnRender() override;
	void OnEvent(const LEO::Event& e) override;
private:
	void CreateRoom();
private:
	LEO::Camera m_camera;
	GameRenderer m_renderer;
};