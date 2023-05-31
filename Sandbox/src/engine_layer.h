#pragma once

#include "sandbox.h"

class EngineInfoLayer : public zeus::Layer
{
private:
	zeus::Application* app = nullptr;

public:
	EngineInfoLayer(const char* name)
	{
		app = zeus::Application::GetInstance();
	}

	bool showDemo = true;
	void OnUpdate(float dt) override
	{
		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Begin("Renderer Info");
		{
			const auto& stat = zeus::Renderer::GetRendererStat();
			ImGui::Text("Texture slots used: %d / %d", textureManager->GetTextureSlotsUsed(), zeus::TextureManager::GetTextureMaxSlotsCount());
			ImGui::Text("Quads drawn: %d / %d", stat.QuadsDrawn, zeus::Renderer::MaxQuads);
			ImGui::Text("Lines drawn: %d / %d", stat.LinesDrawn, zeus::Renderer::MaxLines);
			ImGui::Text("Draw calls: %d", stat.DrawCalls);
			ImGui::End();
		}

		ImGui::Begin("Mouse");
		{
			const auto& [mouseX, mouseY] = zeus::Input::GetMousePosition();
			ImGui::Text("cursor position: %f, %f", mouseX, mouseY);
			ImGui::End();
		}

		ImGui::Begin("Application");
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("ImGUI FPS: %f", io.Framerate);
			static bool vsyncOn = true;
			ImGui::Checkbox("v-sync enabled", &vsyncOn);
			app->SetVSync(vsyncOn);
			ImGui::End();
		}
	}

	void OnRender() override
	{
	}

	void OnEvent(zeus::Event& e) override
	{
	}
};
