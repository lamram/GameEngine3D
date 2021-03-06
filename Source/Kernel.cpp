#include "Kernel.h"
#include "FPSCounter.h"
#include "SceneManager.h"
#include "WindowManager.h"

namespace se {

	Kernel::Kernel(const std::string &title, bool centered, int x, int y, int width, int height, AbstractRenderer *renderer, AbstractInput *input) {
		m_isRunning = true;
		m_renderer = nullptr;
		m_input = nullptr;

		m_logger.SelectLogger("engine.log");

		if (!renderer) {
			m_logger.Log(ERRORTYPE_ERROR, __FILE__, __LINE__, "No renderer set");
			return;
		}
		m_renderer = renderer;
		m_input = input;

		WindowManager::GetInstance()->AddWindow(title, centered, x, y, width, height);
		HWND hWnd = WindowManager::GetInstance()->GetLastWindow().GetWindowHandle();
		renderer->Create(hWnd, width, height, true, CULL_CCW, false, true, FM_SOLID, true);
		input->Initialize(WindowManager::GetInstance()->GetLastWindow().GetWindowInstanceHandle(), hWnd, width, height);
		m_logger.Log(ERRORTYPE_INFO, __FILE__, __LINE__, "Initialized engine");
	}

	void Kernel::SetRunningState(bool running) {
		m_isRunning = running;
	}

	bool Kernel::IsRunning() {
		return m_isRunning;
	}

	int Kernel::EnterLoop() {
		if (!m_renderer) {
			m_logger.Log(ERRORTYPE_ERROR, __FILE__, __LINE__, "Failed to initialize renderer");
			SceneManager::GetInstance()->RemoveAll();
			WindowManager::GetInstance()->CloseAll();
			return 1;
		}

		FPSCounter fps;

		MSG msg;
		while (m_isRunning) {
			for (int i = 0; i < WindowManager::GetInstance()->GetWindowCount(); i++) {
				HWND windowHandle = WindowManager::GetInstance()->GetWindow(i).GetWindowHandle();
				while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			// Logic.
			m_input->Update();

			if (msg.message == WM_QUIT) {
				m_isRunning = false;
			}

			m_renderer->Update(fps.GetDelta());
			if (SceneManager::GetInstance()->GetSceneCount() > 0) {
				SceneManager::GetInstance()->GetCurrentScene()->Update(fps.GetDelta());
			}

			// Drawing.
			m_renderer->Clear(true, true);
			m_renderer->SetRenderTarget();

			m_renderer->BeginScene();

			// Sets the positions of the entities and renders them on the active scene.
			if (SceneManager::GetInstance()->GetSceneCount() > 0) {
				std::vector<Entity*> m_currentSceneEntities = SceneManager::GetInstance()->GetCurrentScene()->GetEntities();
				for (int i = 0; i < m_currentSceneEntities.size(); i++) {
					Entity* entity = m_currentSceneEntities[i];
					m_renderer->SetTransform(entity->GetPosition(), entity->GetScale(), entity->GetRotation());
					entity->Render(m_renderer);
				}
			}

			m_renderer->EndScene();
			m_renderer->Present();

			fps.Update();
		}

		SceneManager::GetInstance()->RemoveAll();
		m_renderer->Release();
		WindowManager::GetInstance()->CloseAll();

		return msg.wParam;
	}

}