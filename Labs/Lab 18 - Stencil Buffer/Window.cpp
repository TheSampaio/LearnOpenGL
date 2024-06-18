#include "PCH.h"
#include "Window.h"

#include "Debug.h"

Window::Window()
{
	// Initializes GLFW and log it if failed
	if (!glfwInit())
	{
		Debug::Log("[ERRO] Failed to initialize GLFW.", true);
		exit(EXIT_FAILURE);
	}

	// Initializes main attributes
	m_Id = nullptr;
	m_Synchronization = true;

	m_Title = "Window";
	m_Size = { 800, 600 };
	m_Center = { static_cast<unsigned short>(m_Size[0] / 2), static_cast<unsigned short>(m_Size[1] / 2) };
	m_BackgroundColour = { 0.0f, 0.0f, 0.0f};
	
	// Gets monitor's work area
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), 0, 0, &m_Screen[0], &m_Screen[1]);
}

Window::~Window()
{
	// Destroying window's process
	glfwDestroyWindow(m_Id);

	// Finalizes GLFW
	glfwTerminate();
}

bool Window::Create()
{
	if (!m_Id)
	{
		// Set-up OpenGL's version and compatibility mode
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Locks window's size
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Creates a window pointer
		m_Id = glfwCreateWindow(m_Size[0], m_Size[1], m_Title.c_str(), nullptr, nullptr);

		if (m_Id)
		{
			// Sets window's position
			glfwSetWindowPos(m_Id, (m_Screen[0] / 2) - (m_Size[0] / 2), (m_Screen[1] / 2) - (m_Size[1] / 2));

			// Creates an OpenGL's context
			glfwMakeContextCurrent(m_Id);

			// Loads GLAD
			gladLoadGL();

			// Creates a viewport for the window
			glViewport(0, 0, m_Size[0], m_Size[1]);
		}

		else
		{
			Debug::Log("[ERRO] Failed to create a window.", true);
		}
	}

	// Returns true if exists a pointer to the window's id
	return (m_Id) ? true : false;
}

bool Window::Close(bool bForce)
{
	if (!bForce)
	{
		return glfwWindowShouldClose(m_Id);
	}

	else
	{
		// Forces window to close
		glfwSetWindowShouldClose(m_Id, bForce);
		return false;
	}
}

void Window::ClearBuffers()
{
	// Enable OpenGL's features
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable OpenGL samples
	glEnable(GL_MULTISAMPLE);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Sets background's colour
	glClearColor(m_BackgroundColour[0], m_BackgroundColour[1], m_BackgroundColour[2], 1.0f);
}

void Window::SetTitle(const std::string& title)
{
	if (!m_Id)
		m_Title = title;

	else
		glfwSetWindowTitle(m_Id, title.c_str());
}

void Window::SetAntiAliasing(bool enable, unsigned short samples)
{
	if (enable)
		glfwWindowHint(GLFW_SAMPLES, samples);
}
