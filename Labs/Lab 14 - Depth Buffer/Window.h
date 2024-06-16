#ifndef LEARNOPENGL_WINDOW_H
#define LEARNOPENGL_WINDOW_H

class Window
{
public:
	// Returns a reference for the window
	static inline Window& GetInstance() { static Window s_Instance; return s_Instance; }

	// Creates the window
	bool Create();

	// Closes the window
	bool Close(bool bForce = false);

	// Process all window's events
	inline void ProcessEvents() { glfwPollEvents(); if (glfwGetKey(m_Id, GLFW_KEY_ESCAPE) == GLFW_PRESS) { Close(true); } }

	// Clear all window's buffers
	void ClearBuffers();

	// Swaps window's buffers
	inline void SwapBuffers() { glfwSwapInterval(m_Synchronization); glfwSwapBuffers(m_Id); }

	// Returns window's ID
	inline GLFWwindow*& GetId() { return m_Id; }

	// Returns monitor's work area's size
	inline std::array<int, 2> GetScreen() const { return m_Screen; }

	// Returns window's size
	inline std::array<unsigned short, 2> GetSize() const { return m_Size; }

	// Returns window's background colour
	inline std::array<GLfloat, 3> GetBackgroundColour() const { return m_BackgroundColour; }

	// Returns window's center
	inline std::array<unsigned short, 2> GetCenter() const { return m_Center; }

	// Returns window's title
	inline std::string GetTitle() const { return m_Title; }

	// Sets window's title
	inline void SetTitle(const std::string& title) { m_Title = title; }

	// Sets window's size
	inline void SetSize(unsigned short width, unsigned short height) { m_Size = {width, height}; }

	// Sets window's background colour
	inline void SetBackgroundColour(GLfloat red, GLfloat green, GLfloat blue) { m_BackgroundColour = {red, green, blue}; }

	// Sets window's vertical synchronization (V-Sync)
	inline void SetVerticalSynchronization(bool enable) { m_Synchronization = enable; }

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
