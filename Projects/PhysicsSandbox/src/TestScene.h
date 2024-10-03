#include <LEO/LeoEngine.h>
#include <LEO/Physics/RigidBody2D.h>

class TestScene : public LEO::Scene
{
public:
	void OnCreate() override;
	void OnUpdate(f32 dt) override;
	void OnRender() override;
	void OnEvent(const LEO::Event& e) override;
private:
	void SetUpScene();
	void CreateSphere(const glm::vec3 pos);
private:
	std::vector<LEO::Drawable> m_drawables;
	std::vector<LEO::RigidBody2D> m_bodies;
private:
	u32 sphere;
	bool up    = false;
	bool down  = false;
	bool right = false;
	bool left  = false;
};