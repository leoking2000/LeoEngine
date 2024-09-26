#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "../../Utilities/Types.h"


namespace LEO
{
	void WINInitialization();
	void WINTerminate();

	class Window final
	{
	public:
		Window(u32 width, u32 height, bool isResizable, bool vsync = true);

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		~Window();
	public:
		void       SetWindowTitle(const std::string& title);
		void       SetVsync(bool vsync);
	public:
		glm::uvec2 WindowSize() const;
		glm::vec2  MousePos()   const;
		f32        Time()       const;
	public:
		bool       ShouldWindowClose() const;
		void       CloseWindow();
		void       NewFrame();
		void       EndFrame();
	public:
		using WindowResizeCallback = std::function<void(int, int)>;
		void SetResizeCallback(WindowResizeCallback resize_callback);

		// first input is the keycode, second is the action(press, relesed, repeat)
		using ButtonEventCallback = std::function<void(int, int)>;
		void SetKeyboardCallback(ButtonEventCallback key_callback);
		void SetMouseButtonCallBack(ButtonEventCallback mouse_key_callback);

		using MouseMoveCallback = std::function<void(float, float)>;
		void SetMouseMoveCallback(MouseMoveCallback mouse_move_callback);
	public:
		struct WinData
		{
			ButtonEventCallback  keyboardCallback;
			ButtonEventCallback  mouseKeyCallback;
			MouseMoveCallback    mouseMoveCallback;
			WindowResizeCallback windowResizeCallback;
		};
	private:
		void* m_window;
		WinData m_data;
	};

}