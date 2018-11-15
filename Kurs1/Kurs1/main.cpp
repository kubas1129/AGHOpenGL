/// GLAD odpowiada za przypisanie wskaŸników do funkcji do odpowiednich nazw
///     Jest to potrzebne bo implementacja funkcji znajduje siê na karcie graficznej
#include <glad/glad.h>

/// GLFW u³atwia wszystko zwi¹zane z systemem operacyjnym
///     np. tworzenie okna czy zbieranie inputu.
/// Wykorzystuje define-y zawarte w glad.h wiêc musi byæ zaincludowany po nim.
#include <GLFW/glfw3.h>

#include <iostream>

/// deklaracje funkcji zbieraj¹cych input
/// 1.) Zwyk³a funkcja która bêdzie wywo³ywana wewn¹trz g³ównej pêtli programu
void ProcessInput(GLFWwindow* window);

/// 2.) Callback przypisany do konkretnego zdarzenia wywo³ywany w ramach funkcji glfwPollEvents()
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main()
{
	/// Inicjalizacja GLFW
	glfwInit();
	/// Wybieramy wersjê 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Dodatkowy hint który podobno potrzebny jest dla Os X (nie mam jak przetestowaæ czy to faktycznie coœ zmienia)
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/// Tworzenie okna - glfwCreateWindow
	/** Parametry:
	* width     -   szerokoœæ
	* height    -   wysokoœæ
	* title     -   tytu³
	* monitor   -   Je¿eli wybierzemy monitor to gra w³¹czy siê w pe³nym ekranie
					w takich sytuacjach dobrze mieæ wysokoœæ i szerokoœæ ekranu
	* share     -   WskaŸnik na parent window gdybyœmy potrzebowali kilku.
	*/
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ShaderOpenGL", nullptr, nullptr);
	/// Poprawny FullScreen:
	/*
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "ShaderOpenGL", monitor, nullptr);
	*/
	/// Sprawdzamy czy okno zosta³o utworzone poprawnie
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


	/// Wybieranie koloru t³a wykorzystywanego przez glClear
	glClearColor(0.4f, 0.4f, 0.6f, 1.0f);

	/// main loop
	while (!glfwWindowShouldClose(window))
	{
		/// Czyszczenie ekranu wybranym kolorem
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessInput(window);

		/// Podmienienie przedniego i tylnego bufora
		/// Bez tego nic nie pojawi siê na ekranie jako ¿e wszystkie funkcje rysuj¹ce (w tym glClear)
		/// updateuja buffer który nie jest aktualnie wyœwietlany.
		glfwSwapBuffers(window);
		/// Przechwytywanie zdarzeñ takich jak wciœniêcie przycisku lub ruch kursora
		glfwPollEvents();
	}

	/// Zwolnienie pamiêci zaalokowanej przez GLFW
	glfwTerminate();
	return 0;
}

/// Zamiast tej funkcji mo¿na zarejestrowaæ callback reaguj¹cy na wciœniêcie klawisza (glfwSetKeyCallback)
void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	glClearColor(float(xPos / WIDTH), float(yPos / HEIGHT), 0.0f, 1.0f);
	/// Tu oczywiœcie je¿eli przeskalujemy okno podczas dzia³ania programu to nie bêdzie dzia³aæ poprawnie
	/// Je¿eli po podzieleniu dostaniemy wartoœæ > 1.0 to zostanie automatycznie zmieniona na 1.0.
	/// Mo¿na ewentualnie zarejestrowaæ callback na event zmiany rozmiaru okna u¿ywaj¹c glfwSetWindowSizeCallback()
	/// i updateowaæ zmienne WIDTH i HEIGHT (nie musz¹ byæ const)
	/// Innym rozwi¹zaniem jest stworzenie okna którego nie da siê przeskalowaæ
	/// u¿ywaj¹c glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}
