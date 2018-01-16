#include <iostream>
#include "Debug.h"
#include "Kernel.h"
#include "FPSCounter.h"
#include "SceneManager.h"

namespace se {

	Kernel::Kernel(const std::string &title, int width, int height, AbstractRenderer *renderer, Input *input) {
		m_logger.SelectLogger("engine.log");

		HWND hWnd;
		if (FAILED(hWnd = m_window.OpenWindow(title, width, height))) {
			m_logger.Log(0, __FILE__, __LINE__, "Failed to create window");
			return;
		}

		m_input = input;
		if (!m_input->Initialize(m_window.GetInstance(), hWnd, width, height)) {
			m_logger.Log(1, __FILE__, __LINE__, "Failed to initialize input");
			return;
		}

		if (!renderer) {
			m_logger.Log(2, __FILE__, __LINE__, "No renderer set");
			return;
		}
		m_renderer = renderer;
		m_renderer->Create(hWnd);

	}

	//TODO: work multiple windows out
	void Kernel::AddWindow(const std::string &title, int width, int height) {
		if (FAILED(m_window.OpenWindow(title, width, height))) {
			m_logger.Log(0, __FILE__, __LINE__, "Failed to create new window");
			return;
		}

	}

	void Kernel::AddCameraController(CameraController *cameraController) {
		m_cameraController = cameraController;
	}

	int Kernel::EnterLoop() {
		if (!m_renderer || !m_cameraController) {
			return 1;
		}

		bool isRunning = true;

		MSG msg;
		FPSCounter fps;

		while (isRunning) {
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			m_input->Update();

			if (msg.message == WM_QUIT || m_input->IsPressed(DIK_ESCAPE)) {
				isRunning = false;
			}

			// Logic.
			m_cameraController->HandleInput(fps.GetDelta());
			SceneManager::GetInstance()->GetCurrentScene()->Update(fps.GetDelta());

			// Drawing.
			m_renderer->Render();

			std::cout << fps.GetFPS() << std::endl;
			fps.Update();
		}

		m_renderer->Release();
		m_window.CloseAll();

		return msg.wParam;
	}

}