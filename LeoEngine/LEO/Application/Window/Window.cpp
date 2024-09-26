#include "../../Utilities/Log.h"
#include "Window.h"
#include <GLFW/glfw3.h>

#define M_GLFW_WINDOW reinterpret_cast<GLFWwindow*>(m_window)
 
namespace LEO
{
	static bool g_glfwInnit = false;

	void glfw_error_callback(int code, const char* description)
	{
		LEOCHECKF(false, "GLFW error {} : {}", code, description);
	}

	void WINInitialization()
	{
		LEOASSERT(g_glfwInnit == false, "WIN system is already Initialized.");

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			LEOASSERT(false ,"GLFW init failed.");
		}

		// Hits
		// OpenGL version 4.6
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, LEODEBUG == 1);

		g_glfwInnit = true;
	}

	void WINTerminate()
	{
		g_glfwInnit = false;
		glfwTerminate();
	}

	//-----------------------------------------------------------

	void glfw_window_size_callback(GLFWwindow* glfw_window, int width, int height)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));

		LEOASSERT(data->windowResizeCallback, "KeyBoard Callback not set");
		data->windowResizeCallback(width, height);
	}

	void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));
		LEOASSERT(data->keyboardCallback, "KeyBoard Callback not set");
		data->keyboardCallback(key, action);
	}

	void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));
		LEOASSERT(data->mouseKeyCallback, "Mouse Button Callback not set");
		data->mouseKeyCallback(button, action);
	}

	static void glfw_cursor_position_callback(GLFWwindow* glfw_window, double xpos, double ypos)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));
		LEOASSERT(data->mouseMoveCallback, "Mouse Move Callback not set");
		data->mouseMoveCallback((float)xpos, (float)ypos);
	}

	//-----------------------------------------------------------

	Window::Window(u32 width, u32 height, bool isResizable, bool vsync)
	{
		LEOASSERT(g_glfwInnit == true, "WIN system needs to be Initialized. call LEO::WINInitialization()");

		// window hits
		glfwWindowHint(GLFW_RESIZABLE, isResizable ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// create the window
		m_window = glfwCreateWindow(width, height, "", NULL, NULL);
		LEOASSERT(m_window != nullptr, "Window creation failed.");

		// set user pointer (used by the callbacks)
		glfwSetWindowUserPointer(M_GLFW_WINDOW, &m_data);

		// set the glfw callbacks
		glfwSetWindowSizeCallback(M_GLFW_WINDOW, glfw_window_size_callback);
		glfwSetKeyCallback(M_GLFW_WINDOW, glfw_key_callback);
		glfwSetMouseButtonCallback(M_GLFW_WINDOW, glfw_mouse_button_callback);
		glfwSetCursorPosCallback(M_GLFW_WINDOW, glfw_cursor_position_callback);

		glfwMakeContextCurrent(M_GLFW_WINDOW);
		glfwSwapInterval(vsync ? 1 : 0);
	}

	Window::~Window()
	{
		glfwDestroyWindow(M_GLFW_WINDOW);
	}

	//-----------------------------------------------------------

	void Window::SetWindowTitle(const std::string& title)
	{
		glfwSetWindowTitle(M_GLFW_WINDOW, title.c_str());
	}

	void Window::SetVsync(bool vsync)
	{
		glfwSwapInterval(vsync ? 1 : 0);
	}

	//-----------------------------------------------------------

	glm::uvec2 Window::WindowSize() const
	{
		int width, height;
		glfwGetWindowSize(M_GLFW_WINDOW, &width, &height);

		return glm::uvec2((u32)width, (u32)height);
	}

	glm::vec2 Window::MousePos() const
	{
		double xpos, ypos;
		glfwGetCursorPos(M_GLFW_WINDOW, &xpos, &ypos);

		return glm::vec2((float)xpos, (float)ypos);
	}

	f32 Window::Time() const
	{
		return (f32)glfwGetTime();
	}

	//-----------------------------------------------------------

	bool Window::ShouldWindowClose() const
	{
		return glfwWindowShouldClose(M_GLFW_WINDOW);
	}

	void Window::CloseWindow()
	{
		glfwSetWindowShouldClose(M_GLFW_WINDOW, true);
	}

	void Window::NewFrame()
	{

	}

	void Window::EndFrame()
	{
		glfwPollEvents();
		glfwSwapBuffers(M_GLFW_WINDOW);
	}

	void Window::SetResizeCallback(WindowResizeCallback resize_callback)
	{
		m_data.windowResizeCallback = resize_callback;
	}

	void Window::SetKeyboardCallback(ButtonEventCallback key_callback)
	{
		m_data.keyboardCallback = key_callback;
	}

	void Window::SetMouseButtonCallBack(ButtonEventCallback mouse_key_callback)
	{
		m_data.mouseKeyCallback = mouse_key_callback;
	}

	void Window::SetMouseMoveCallback(MouseMoveCallback mouse_move_callback)
	{
		m_data.mouseMoveCallback = mouse_move_callback;
	}
}


