#include <LEO/LeoEngine.h>

class TestScene : public LEO::Scene
{
public:
	void OnCreate() override;
	void OnUpdate(f32 dt) override;
	void OnRender() override;
	void OnEvent(const LEO::Event& e) override;
private:
	void SetUpScene();
private:
	std::vector<LEO::Drawable> m_drawables;
};