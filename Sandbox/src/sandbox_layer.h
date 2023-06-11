#pragma once

#include "sandbox.h"
#include "sandbox_level.h"

class SandboxLayer : public zeus::Layer
{
private:
	std::string m_Name;

	bool m_Keys[65536] = { false };
	bool m_CameraMode = false;
	bool m_ShowFileDialog = false;
	int m_GridCellSize = 64;
	int m_LineWidth = 1;

	std::mt19937 mt;
	std::uniform_int_distribution<int> dist{ 0, 255 };

	std::shared_ptr<zeus::Camera> m_Camera;

	zeus::LevelManager m_LevelManager;
	std::shared_ptr<zeus::Framebuffer> m_Framebuffer;
	std::shared_ptr<zeus::Phyzics> m_PhysicsEngine;

	glm::vec2 m_CursorPos{ 0, 0 };

	TileEntity m_DragTile;

public:
	SandboxLayer(const char* name)
		: m_Name(name)
	{
		// Initialize Menubar
		const auto& app = zeus::Application::GetInstance();
		const auto& ui = app->GetUI();
		ui->AddMenuItem({ "menu$$File" });
		ui->AddMenuItem({ "menu-item$$Open", [&]() -> bool {
				m_ShowFileDialog = true;
				return true;
			}
			});

		m_Camera = std::make_shared<zeus::Camera>();
		m_Camera->GetProperties().Position = defaultOrthoCamera.Position;
		m_Camera->GetProperties().MovementSpeed = defaultOrthoCamera.MovementSpeed;

		textureManager = zeus::TextureManager::GetInstance();
		textureManager->PutTexture({ "wood_tex", "assets/textures/wood.png" });
		textureManager->PutTexture({ "window_tex", "assets/textures/window.png" });
		textureManager->PutSpritesheet({ "person_sheet", "assets/textures/tilemap_packed.png", 16, true });
		textureManager->PutSpritesheet({ "building_sheet", "assets/textures/mytilemap.png", 16, true });

		const auto& lvl1 = std::make_shared<SandboxLevel>();
		m_LevelManager.AddLevel(1, lvl1);

		zeus::FramebufferInfo frameInfo;
		frameInfo.Attachments = { zeus::ColorAttachment::RGBA, zeus::ColorAttachment::INT };
		m_Framebuffer = zeus::Framebuffer::CreateFramebuffer(frameInfo);
		m_Framebuffer->Bind(textureManager);

		m_PhysicsEngine = std::make_shared<zeus::Phyzics>();

		zeus::Renderer::Init();
		zeus::Renderer::SetBackgroundColor(GREY);
		zeus::Renderer::SetColorBlending(true);
		zeus::Renderer::SetDepthTest(true);
	}

	~SandboxLayer()
	{
	}

	float speed = 2.5f, lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
	bool firstMove = true, lockZoom = true;
	void OnUpdate(float dt) override
	{
		auto const& cam = m_Camera->GetProperties();
		if (m_Keys[KEY_LEFT_CONTROL])
		{
			if (m_Keys[KEY_D])
			{
				m_Camera->ProcessKeyboard(zeus::CameraMovement::RIGHT, dt);
			}
			else if (m_Keys[KEY_A])
			{
				m_Camera->ProcessKeyboard(zeus::CameraMovement::LEFT, dt);
			}
			else if (m_Keys[KEY_W])
			{
				if (cam.Type == zeus::CameraType::Orthographic)
					m_Camera->ProcessKeyboard(zeus::CameraMovement::UP, dt);
				else
					m_Camera->ProcessKeyboard(zeus::CameraMovement::FORWARD, dt);
			}
			else if (m_Keys[KEY_S])
			{
				if (cam.Type == zeus::CameraType::Orthographic)
					m_Camera->ProcessKeyboard(zeus::CameraMovement::DOWN, dt);
				else
					m_Camera->ProcessKeyboard(zeus::CameraMovement::BACKWARD, dt);
			}

			if (cam.Type == zeus::CameraType::Perspective)
			{
				auto [mouseX, mouseY] = zeus::Input::GetMousePosition();
				if (firstMove)
				{
					lastX = mouseX;
					lastY = mouseY;
					firstMove = false;
				}

				float xoffset = mouseX - lastX;
				float yoffset = lastY - mouseY; // reversed since y-coordinates go from bottom to top

				lastX = mouseX;
				lastY = mouseY;

				m_Camera->ProcessMouse(xoffset, yoffset);
			}
		}
		else
		{
			if (m_Keys[KEY_A])
			{
				spritePos.x -= speed;
			}
			if (m_Keys[KEY_D])
			{
				spritePos.x += speed;
			}
			if (m_Keys[KEY_W])
			{
				spritePos.y += speed;
			}
			if (m_Keys[KEY_S])
			{
				spritePos.y -= speed;
			}
		}
	}

	void ResetCamera(zeus::CameraType type)
	{
		auto& cam = m_Camera->GetProperties();
		if (type == zeus::CameraType::Orthographic)
		{
			cam.Position = defaultOrthoCamera.Position;
			cam.MovementSpeed = defaultOrthoCamera.MovementSpeed;
			cam.Direction = defaultOrthoCamera.Direction;
			cam.Right = defaultOrthoCamera.Right;
		}
		else if (type == zeus::CameraType::Perspective)
		{
			cam.Position = defaultPerspectiveCamera.Position;
			cam.MovementSpeed = defaultPerspectiveCamera.MovementSpeed;
			cam.Direction = defaultPerspectiveCamera.Direction;
			cam.Right = defaultPerspectiveCamera.Right;
		}
	}

	void ChangeCameraType(zeus::CameraType type)
	{
		static zeus::CameraType currentType = m_Camera->GetProperties().Type;
		auto& cam = m_Camera->GetProperties();
		if (type != currentType)
		{
			cam.Type = type;
			currentType = type;

			if (type == zeus::CameraType::Orthographic)
			{
				cam.Position = defaultOrthoCamera.Position;
			}
			else
			{
				cam.Position = defaultPerspectiveCamera.Position;
			}
		}
	}

	bool showGrid = true;
	zeus::QuadData quad;
	void OnRender() override
	{
		m_Framebuffer->Activate();
		zeus::Renderer::Start(m_Camera);

		m_LevelManager.SetActiveLevel(1);
		m_LevelManager.Draw();

		if (m_DragTile.IsActive)
		{
			const auto& sprite = m_DragTile.Sprite;
			const auto& transform = m_DragTile.Transform;
			quad.SetSubTexture(textureManager->GetSubTexture(sprite.TextureName, sprite.Coordinates.x, sprite.Coordinates.y));
			quad.SetSize(transform.Scale);
			quad.SetPosition({ m_CursorPos.x + 32.0f, HEIGHT - m_CursorPos.y - 32.0f, 0.0f });
			zeus::Renderer::DrawTexturedQuad(quad);
		}

		if (showGrid)
		{
			DrawGrid(m_GridCellSize, m_LineWidth);
		}

		zeus::Renderer::Flush(textureManager);

		m_Framebuffer->Unbind();

		zeus::Renderer::Refresh();

		RenderUI();

		ImVec2 pos = ImGui::GetMousePos();
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::SetNextWindowSize(ImVec2{ 100, 100 });
		const auto& activeLevel = (SandboxLevel*)m_LevelManager.GetActiveLevel().get();
		static bool p_open = false;
		std::string fileName = activeLevel->GetLevelName();
		ImGui::Begin(fileName.c_str(), &p_open, flags);
		{
			const auto& windowOffset = ImGui::GetWindowContentRegionMin();
			// const auto& windowMax = ImGui::GetContentRegionAvail();
			const auto& [sX, sY] = ImGui::GetWindowPos();
			m_CursorPos.x = pos.x - (sX + windowOffset.x);
			m_CursorPos.y = pos.y - (sY + windowOffset.y);

			if (ImGui::IsWindowHovered())
			{
				ImGuiIO& io = ImGui::GetIO();
				io.WantCaptureKeyboard = false;
				io.WantCaptureMouse = false;

				int id = m_Framebuffer->ReadID(1, m_CursorPos.x, HEIGHT - m_CursorPos.y);

				if (mouseButtons[MOUSE_RIGHT] && m_Keys[KEY_LEFT_CONTROL])
				{
					if (ImGui::IsWindowFocused())
					{
						activeLevel->GetLevelMap()[id].TexCoords = { -1, -1 };
					}
					else if (m_DragTile.IsActive)
					{
						m_DragTile.IsActive = false;
					}
				}

				if (mouseButtons[MOUSE_LEFT] && m_DragTile.IsActive)
				{
					auto& levelMap = activeLevel->GetLevelMap();
					if (id < (int)levelMap.size())
					{
						levelMap[id].TexCoords = m_DragTile.Sprite.Coordinates;
					}
					m_DragTile.IsActive = false;
				}
			}

			if (ImGui::BeginPopupContextItem("level_rclick"))
			{
				if (ImGui::Button("Level details"))
					ImGui::OpenPopup("level_info");

				if (ImGui::BeginPopup("level_info"))
				{
					ImGui::Text("Current File: %s", activeLevel->GetLevelPath().c_str());
					ImGui::EndPopup();
				}

				ImGui::EndPopup();
			}

			const auto& tex = m_Framebuffer->GetAttachedTextures()[0];
			ImGui::Image(reinterpret_cast<void*>(tex->GetTextureID()), ImVec2{ WIDTH, HEIGHT }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::OpenPopupOnItemClick("level_rclick", ImGuiPopupFlags_MouseButtonRight);

			ImGui::End();
		}

		ImGui::Begin("Entity");
		{
			int id = m_Framebuffer->ReadID(1, m_CursorPos.x, HEIGHT - m_CursorPos.y);

			if (mouseButtons[MOUSE_LEFT] && !m_Keys[KEY_LEFT_CONTROL])
			{
				activeLevel->DeselectAll();
			}

			static int savedID = -1;
			if (id >= 0 && id < (int)activeLevel->GetLevelMap().size())
			{
				if (mouseButtons[MOUSE_LEFT])
				{
					savedID = id;
					activeLevel->SelectCell(id);
				}
			}

			if (savedID != -1)
			{
				auto& tile = activeLevel->GetLevelMap().at(savedID);
				const auto& tex = textureManager->GetSubTexture("building_sheet", tile.TexCoords.x, tile.TexCoords.y);
				// Empty tiles may have null textures
				if (tex != nullptr)
				{
					ImVec2 bottomRight = *(ImVec2*)&tex->GetTexCoords().at(1);
					ImVec2 topLeft = *(ImVec2*)&tex->GetTexCoords().at(3);
					ImGui::Image((ImTextureID)(tex->GetTextureID()), ImVec2(32, 32), topLeft, bottomRight);
				}

				ImGui::DragFloat("z-index", &tile.Position.z, 0.01f, -1.0f, 1.0f);

				// This list order as original tile types order since index are corresponding
				static const char* items[] = { "Water", "Rock", "Tree", "None" };
				int item_current_idx = (int)tile.Type;
				const char* combo_preview_value = items[item_current_idx];
				if (ImGui::BeginCombo("Assigned Type", combo_preview_value, 0))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (tile.Type != (SandboxLevel::TileType)item_current_idx)
				{
					tile.Type = (SandboxLevel::TileType)item_current_idx;
				}

				if (ImGui::Button("Apply to all"))
				{
					activeLevel->ChangeAllTiles(tile.TexCoords, (SandboxLevel::TileType)item_current_idx);
				}
			}

			ImGui::Text("%f %f: %d", m_CursorPos.x, m_CursorPos.y, id);

			if (ImGui::Button("Save Level"))
			{
				activeLevel->SaveLevel();
			}

			static int cellsize = activeLevel->GetCellsize();
			ImGui::InputInt("Tile size", &cellsize);
			activeLevel->SetCellsize(cellsize);

			float cellgap = activeLevel->GetCellgap();
			ImGui::InputFloat("Tile gap", &cellgap, 0.1f);
			activeLevel->SetCellgap(cellgap);

			if (ImGui::Button("Add New Entity"))
				ImGui::OpenPopup("Add New Entity");

			if (ImGui::BeginPopupModal("Add New Entity", NULL))
			{
				ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

				// Testing behavior of widgets stacking their own regular popups over the modal.
				static int item = 1;
				// static int color[4] = { 1, 2, 3, 4 };
				ImGui::Combo("Combo", &item, "Human Enemy 1\0Human Enemy 2\0Animal Enemy 1\0Animal Enemy 2\0Animal Enemy 3\0\0");

				if (ImGui::Button("Add another modal.."))
					ImGui::OpenPopup("Stacked 2");

				// Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close button which
				// will close the popup. Note that the visibility state of popups is owned by imgui, so the input value
				// of the bool actually doesn't matter here.
				bool unused_open = true;
				if (ImGui::BeginPopupModal("Stacked 2", &unused_open))
				{
					ImGui::Text("Hello from Stacked The Second!");
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}

				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			ImGui::Checkbox("Show grid", &showGrid);
			ImGui::DragInt("Grid square size", &m_GridCellSize, 1, 0);
			ImGui::DragInt("Line width", &m_LineWidth, 1, 0, 31);
			ImGui::Text("Level map size: %d", (int)activeLevel->GetLevelMap().size());
			static int newSize = 0;
			auto [rows, cols] = activeLevel->GetDims();
			char str[256];
			sprintf_s(str, "Map size %ux%u", rows, cols);
			ImGui::InputInt(str, &newSize);
			if (ImGui::Button("Resize##Resize-button"))
			{
				activeLevel->ResizeMap(newSize);
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Map is a square, so this will be a side length!");
			}

			if (ImGui::Button("Clear level map"))
				ImGui::OpenPopup("Clear Level Map##level-clear-popup");

			if (ImGui::BeginPopupModal("Clear Level Map##level-clear-popup"))
			{
				ImGui::Text("Are you sure you want to clear the level map?");
				ImGui::Text("This will erase entire level map with tiles (you will have to draw this map again tile by tile)");

				if (ImGui::Button("Yes"))
				{
					activeLevel->ClearLevelMap();
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("No"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}

	void DrawGrid(int cellsize = 64, int lineWidth = 1)
	{
		if (cellsize <= 0)
		{
			cellsize = 1;
		}
		if (lineWidth <= 0)
		{
			lineWidth = 1;
		}

		static zeus::LineData line;
		static float z = 0.1f;
		line.SetLineWidth((float)lineWidth);
		line.SetColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		for (float x = 0.0f; x <= WIDTH; x += (float)cellsize)
		{
			line.SetPoint1({ x, HEIGHT, z });
			line.SetPoint2({ x, 0.0f, z });
			zeus::Renderer::DrawLine(line);
		}

		for (float y = 0.0f; y <= HEIGHT; y += (float)cellsize)
		{
			line.SetPoint1({ 0.0f, y, z });
			line.SetPoint2({ WIDTH, y, z });
			zeus::Renderer::DrawLine(line);
		}
	}

	void RenderUI()
	{
		if (m_ShowFileDialog)
		{
			LOG(Trace, "open dialog");
			ImGui::OpenPopup("Open File");
			m_ShowFileDialog = false;
		}

		if (ImGui::BeginPopupModal("Open File", NULL))
		{
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Open"))
			{
				ImGui::Text("Hello");
			}

			ImGui::EndPopup();
		}

#if 1
		static bool p_open = false;
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::Begin("Tiles", &p_open, flags);
		{
			ImTextureID id = 0;
			ImVec2 bottomRight, topLeft, tileSize = { 32, 32 };
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			if (contentRegion.x < 100)
			{
				ImGui::SetWindowSize(ImVec2{ 200, 200 });
				contentRegion = ImGui::GetWindowSize();
			}
			int sentinel = contentRegion.x / int(tileSize.x + 14);

			uint32_t x = 1, y = 1; // origin of x, y is bottom left corner of the sprite sheet
			static int tilesCount = 6 * 3;
			for (int i = 0; i < tilesCount; i++)
			{
				const auto& tex = textureManager->GetSubTexture("building_sheet", x, y);
				id = (ImTextureID)tex->GetTextureID();
				bottomRight = *(ImVec2*)&tex->GetTexCoords()[1];
				topLeft = *(ImVec2*)&tex->GetTexCoords()[3];
				int texID = x + y * 6;
				ImGui::PushID(texID);
				if (ImGui::ImageButton(id, tileSize, topLeft, bottomRight))
				{
					m_DragTile.IsActive = true;
					m_DragTile.Sprite.TextureName = "building_sheet";
					m_DragTile.Sprite.Coordinates = { x, y };
				}
				ImGui::PopID();

				if (x++ == 6) {
					x = 1;
					y++;
				}

				if (i == 0 || (sentinel > 1 && i % sentinel != 0))
				{
					ImGui::SameLine();
				}
			}

			ImGui::End();
		}
#endif

		// ImGui::ShowMetricsWindow();

		ImGui::Begin("Camera Properties");
		{
			auto& cam = m_Camera->GetProperties();
			ImGui::SameLine();
			ImGui::DragFloat("zoom", &cam.ZoomLevel);
			ImGui::SameLine();
			ImGui::Checkbox("lock zoom", &lockZoom);

			ImGui::DragFloat("speed", &cam.MovementSpeed);
			ImGui::DragFloat3("position", glm::value_ptr(cam.Position));
			ImGui::DragFloat3("looking at", glm::value_ptr(cam.Direction));
			ImGui::DragFloat3("right", glm::value_ptr(cam.Right));

			static const char* items[] = { "Orthographic", "Perspective" };
			static int item_current_idx = 0;
			const char* combo_preview_value = items[item_current_idx];
			if (ImGui::BeginCombo("Camera Type", combo_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (item_current_idx == 0)
				ChangeCameraType(zeus::CameraType::Orthographic);
			else
				ChangeCameraType(zeus::CameraType::Perspective);

			ImGui::Checkbox("camera mode", &m_CameraMode);
			zeus::Application::GetInstance()->GetWindow()->SetCameraMode(m_CameraMode);

			if (ImGui::Button("Reset"))
			{
				ResetCamera(cam.Type);
			}

			ImGui::End();
		}

		ImGui::Begin("Textured Quad Handle");
		{
			ImGui::DragFloat2("box position", glm::value_ptr(spritePos), 0.1f);
			ImGui::DragFloat("box depth value", &spritePos.z, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("angle", &angle);
			ImGui::DragFloat3("scale", glm::value_ptr(scaler));
			ImGui::End();
		}
	}

	void OnEvent(zeus::Event& evt) override
	{
		zeus::EventDispatcher dispatcher(evt);

		std::function<bool()> mousePressed = [&]() -> bool {
			// auto [x, y] = zeus::Input::GetMousePosition();
			auto mouseEvent = (zeus::MousePressedEvent&)evt;
			mouseButtons[mouseEvent.GetMouseButton()] = true;

			return true;
		};

		std::function<bool()> mouseReleased = [&]() -> bool {
			// auto [x, y] = zeus::Input::GetMousePosition();
			auto mouseEvent = (zeus::MouseReleasedEvent&)evt;
			mouseButtons[mouseEvent.GetMouseButton()] = false;

			return true;
		};

		std::function<bool()> keyPressed = [&]() -> bool {
			auto keyEvent = (zeus::KeyPressedEvent&)evt;
			int keyCode = keyEvent.GetKeycode();
			m_Keys[keyCode] = true;

			switch (keyCode)
			{
			case KEY_LEFT_ALT:
				m_CameraMode = !m_CameraMode;
				break;
			}

			return true;
		};

		std::function<bool()> keyReleased = [&]() -> bool {
			auto keyEvent = (zeus::KeyReleasedEvent&)evt;
			int keyCode = keyEvent.GetKeycode();
			m_Keys[keyCode] = false;

			switch (keyCode)
			{
			case KEY_ESCAPE:
				zeus::Application::GetInstance()->Stop();
				break;
			}

			return true;
		};

		std::function<bool()> mouseMoved = [&]() -> bool {
			// auto mouseEvent = (zeus::MouseMovedEvent&)evt;
			// auto pos = mouseEvent.GetMousePosition();

			return true;
		};

		std::function<bool()> mouseScrolled = [&]() -> bool {
			auto mouseEvent = (zeus::MouseScrolledEvent&)evt;
			auto scroll = mouseEvent.GetMouseScroll();
			m_Camera->ProcessMouseScroll(scroll.y, lockZoom);

			return true;
		};

		dispatcher.Dispatch(zeus::EventType::MousePressed, mousePressed);
		dispatcher.Dispatch(zeus::EventType::MouseReleased, mouseReleased);
		dispatcher.Dispatch(zeus::EventType::MouseScrolled, mouseScrolled);
		dispatcher.Dispatch(zeus::EventType::MouseMoved, mouseMoved);
		dispatcher.Dispatch(zeus::EventType::KeyPressed, keyPressed);
		dispatcher.Dispatch(zeus::EventType::KeyReleased, keyReleased);
	}
};
