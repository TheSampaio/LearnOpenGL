#ifndef LEARNOPENGL_WINDOW_H
#define LEARNOPENGL_WINDOW_H

class Window
{
public:
	// Static reference
	static inline Window& GetInstance() { static Window m_Instance; return m_Instance; }

	// Main methods
	bool Create();
	bool Close(bool bForce = false);

	inline void ProcessEvents() { glfwPollEvents(); if (glfwGetKey(m_Id, GLFW_KEY_ESCAPE) == GLFW_PRESS) { Close(true); } }
	inline void ClearBuffers()  { glClear(GL_COLOR_BUFFER_BIT); glClearColor(m_BackgroundColour[0], m_BackgroundColour[1], m_BackgroundColour[2], 1.0f); }
	inline void SwapBuffers()   { glfwSwapInterval(m_Synchronization); glfwSwapBuffers(m_Id); }

	// Get methods
	inline GLFWwindow*& GetId()                      { return m_Id; }
	inline std::array<int, 2> GetScreen()            { return m_Screen; }
	inline std::array<unsigned short, 2> GetSize()   { return m_Size; }
	inline std::array<unsigned short, 2> GetCenter() { return m_Center; }
	inline std::string GetTitle()                    { return m_Title; }

	// Set methods
	inline void SetTitle(std::string Title)                                   { m_Title = Title; }
	inline void SetSize(unsigned short Width, unsigned short Height)          { m_Size = {Width, Height}; }
	inline void SetBackgroundColour(GLfloat Red, GLfloat Green, GLfloat Blue) { m_BackgroundColour = {Red, Green, Blue}; }
	inline void SetVerticalSynchronization(bool Enable)                       { m_Synchronization = Enable; }

private:
	Window();
	~Window();

	// Deletes copy contructor and assigment operator
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	// Attributes
	GLFWwindow* m_Id;
	std::array<int, 2> m_Screen;
	std::array<unsigned short, 2> m_Size;
	std::array<unsigned short, 2> m_Center;
	std::array<GLfloat, 3> m_BackgroundColour;
	std::string m_Title;
	bool m_Synchronization;
};

#endif // !LEARNOPENGL_WINDOW_H
