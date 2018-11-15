/// GLAD odpowiada za przypisanie wska�nik�w do funkcji do odpowiednich nazw
///     Jest to potrzebne bo implementacja funkcji znajduje si� na karcie graficznej
#include <glad/glad.h>

/// GLFW u�atwia wszystko zwi�zane z systemem operacyjnym
///     np. tworzenie okna czy zbieranie inputu.
/// Wykorzystuje define-y zawarte w glad.h wi�c musi by� zaincludowany po nim.
#include <GLFW/glfw3.h>

#include <iostream>

/// deklaracje funkcji zbieraj�cych input
/// 1.) Zwyk�a funkcja kt�ra b�dzie wywo�ywana wewn�trz g��wnej p�tli programu
void ProcessInput(GLFWwindow* window);

/// 2.) Callback przypisany do konkretnego zdarzenia wywo�ywany w ramach funkcji glfwPollEvents()
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main()
{
	/// Inicjalizacja GLFW
	glfwInit();
	/// Wybieramy wersj� 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Dodatkowy hint kt�ry podobno potrzebny jest dla Os X (nie mam jak przetestowa� czy to faktycznie co� zmienia)
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/// Tworzenie okna - glfwCreateWindow
	/** Parametry:
	* width     -   szeroko��
	* height    -   wysoko��
	* title     -   tytu�
	* monitor   -   Je�eli wybierzemy monitor to gra w��czy si� w pe�nym ekranie
					w takich sytuacjach dobrze mie� wysoko�� i szeroko�� ekranu
	* share     -   Wska�nik na parent window gdyby�my potrzebowali kilku.
	*/
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ShaderOpenGL", nullptr, nullptr);
	/// Poprawny FullScreen:
	/*
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "ShaderOpenGL", monitor, nullptr);
	*/
	/// Sprawdzamy czy okno zosta�o utworzone poprawnie
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/// Ustawianie stworzonego okna jako aktywny kontekst
	glfwMakeContextCurrent(window);

	/// Inicjalizacja GLADa
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/// Przypisywanie funkcji reagujacej na zdarzenia
	glfwSetCursorPosCallback(window, cursorPosCallback);


	/// Wybieranie koloru t�a wykorzystywanego przez glClear
	glClearColor(0.4f, 0.4f, 0.6f, 1.0f);

	/// main loop
	while (!glfwWindowShouldClose(window))
	{
		/// Czyszczenie ekranu wybranym kolorem
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessInput(window);

		/// Podmienienie przedniego i tylnego bufora
		/// Bez tego nic nie pojawi si� na ekranie jako �e wszystkie funkcje rysuj�ce (w tym glClear)
		/// updateuja buffer kt�ry nie jest aktualnie wy�wietlany.
		glfwSwapBuffers(window);
		/// Przechwytywanie zdarze� takich jak wci�ni�cie przycisku lub ruch kursora
		glfwPollEvents();
	}

	/// Zwolnienie pami�ci zaalokowanej przez GLFW
	glfwTerminate();
	return 0;
}

/// Zamiast tej funkcji mo�na zarejestrowa� callback reaguj�cy na wci�ni�cie klawisza (glfwSetKeyCallback)
void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	glClearColor(float(xPos / WIDTH), float(yPos / HEIGHT), 0.0f, 1.0f);
	/// Tu oczywi�cie je�eli przeskalujemy okno podczas dzia�ania programu to nie b�dzie dzia�a� poprawnie
	/// Je�eli po podzieleniu dostaniemy warto�� > 1.0 to zostanie automatycznie zmieniona na 1.0.
	/// Mo�na ewentualnie zarejestrowa� callback na event zmiany rozmiaru okna u�ywaj�c glfwSetWindowSizeCallback()
	/// i updateowa� zmienne WIDTH i HEIGHT (nie musz� by� const)
	/// Innym rozwi�zaniem jest stworzenie okna kt�rego nie da si� przeskalowa�
	/// u�ywaj�c glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}
