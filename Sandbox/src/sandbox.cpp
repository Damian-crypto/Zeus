#include "zeus.h"
#include "phyzicsall.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <random>
#include <iostream>
#include <random>
#include <exception>
#include <fstream>

static constexpr int WIDTH	= 1280;
static constexpr int HEIGHT = 720;

const glm::vec4 RED		= { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 BLUE	= { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 GREEN	= { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 GREY	= { 0.3f, 0.3f, 0.3f, 1.0f };

static const struct DefaultOrthographicCamera
{
	glm::vec3 Position	= { 0.0f, 0.0f, 0.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right		= { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 30.0f;
} defaultOrthoCamera;

static const struct DefaultPerspectiveCamera
{
	glm::vec3 Position	= { 0.0f, 0.0f, 600.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right		= { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 0.3f;
} defaultPerspectiveCamera;

struct TileEntity
{
	bool IsActive = false;

	zeus::SpriteComponent Sprite;
	zeus::TransformComponent Transform;
};

glm::vec3 spritePos	{ 0.0f, 0.0f, 0.1f };
glm::vec3 scaler	{ 50.0f, 50.0f, 0.0f };
glm::bvec3 mouseButtons{ false };
float zVal	= 0.0f;
float angle = 0.0f;
std::shared_ptr<zeus::TextureManager> textureManager;
class SandboxLevel : public zeus::Level
{
public:
	enum class TileType
	{
		None = 0,
		Water,
		Rock,
		Tree
	};

	struct Tile
	{
		glm::vec3 Position;
		glm::ivec2 TexCoords;
		TileType Type;
	};

private:
	std::string m_LevelPath;

	uint32_t m_LevelCols{ 0u };
	uint32_t m_LevelRows{ 0u };
	uint32_t m_CellSize	{ 0u };

	std::vector<Tile> m_Map;

public:
	void OnStart() override
	{
		LoadLevel("assets/levels/testlevel.data");
	}

	zeus::QuadData quad;
	void Draw() override
	{
		static float gap			= m_CellSize * 2;
		static float leftBoundary	= 0.0f;
		static float rightBoundary	= WIDTH - m_CellSize;
		static float topBoundary	= HEIGHT - m_CellSize;
		static float bottomBoundary = 0.0f;

		for (int y = 0; y < m_LevelRows; y++)
		{
			for (int x = 0; x < m_LevelCols; x++)
			{
				int idx = x + y * m_LevelCols;
				float xx = x * gap - spritePos.x + 32.0f;
				float yy = y * gap - spritePos.y - 32.0f;

				if (xx < leftBoundary || xx > rightBoundary || yy < bottomBoundary || yy > topBoundary)
					continue;

				if (idx < m_Map.size())
				{
					quad.SetPosition({ xx, yy, 0.0f });
					quad.SetSize({ m_CellSize, m_CellSize, 0.0f });
					quad.SetRotation(0.0f);
					quad.SetSubTexture(textureManager->GetSubTexture("building_sheet", m_Map[idx].TexCoords.x, m_Map[idx].TexCoords.y));
					quad.SetEntityID(idx);

					zeus::Renderer::DrawTexturedQuad(quad);
				}
				//zeus::Renderer::DrawQuad(quad);
			}
		}

		quad.SetPosition(spritePos);
		quad.SetSize(scaler);
		quad.SetRotation(angle);
		quad.SetEntityID(2546);
		quad.SetTexture(textureManager->GetTexture("wood_tex"));

		zeus::Renderer::DrawTexturedQuad(quad);
	}

	void AddTile(const Tile& tile)
	{
		if (tile.Position.x > m_LevelCols * m_CellSize)
		{
			m_LevelCols++;
			QUICK_LOG(Trace, "columns increased");
		}
		if (tile.Position.y > m_LevelRows * m_CellSize)
		{
			m_LevelRows++;
			QUICK_LOG(Trace, "columns increased");
		}
		m_Map.emplace_back(tile);
	}

	std::vector<Tile>& GetLevelMap()
	{
		return m_Map;
	}

	void SaveLevel()
	{
		std::ofstream file(m_LevelPath, std::ios_base::out);

		std::string line;

		// Writing columns count
		line = "cols: " + std::to_string(m_LevelCols);
		file << line << '\n';

		// Reading rows count
		line = "rows: " + std::to_string(m_LevelRows);
		file << line << '\n';

		// Reading cell size
		line = "cellsize: " + std::to_string(m_CellSize);
		file << line << '\n';

		// Escape blank line
		file << '\n';

		// Reading map
		int idx = 0;
		for (int y = 0; y < m_LevelRows; y++)
		{
			for (int x = 0; x < m_LevelCols; x++)
			{
				idx = x + y * m_LevelCols;
				if (idx < m_Map.size())
				{
					file << "(" << m_Map[idx].TexCoords.x << ", " << m_Map[idx].TexCoords.y << ", " << (int)m_Map[idx].Type << ") ";
				}
			}
			file << '\n';
		}
		file << '\n';
	}

	void LoadLevel(const std::string& filepath)
	{
		m_LevelPath = filepath;

		std::ifstream file(m_LevelPath);


		if (!file.is_open())
		{
			LOG(Error, "Level file couldn't be found!");
			return;
		}

		std::string line;

		// Reading columns count
		getline(file, line);
		size_t beginPos = line.find(':');
		size_t endPos = line.size();
		m_LevelCols = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

		// Reading rows count
		getline(file, line);
		beginPos = line.find(':');
		endPos = line.size();
		m_LevelRows = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

		// Reading cell size
		getline(file, line);
		beginPos = line.find(':');
		endPos = line.size();
		m_CellSize = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

		// Escape blank line
		std::getline(file, line);

		// Reading map
		while (std::getline(file, line))
		{
			size_t lastPos = 0u;
			for (size_t k = 0; k < line.size() - 10; k++)
			{
				beginPos = line.find('(', lastPos);
				size_t midPos1 = line.find(',', beginPos);
				size_t midPos2 = line.find(',', midPos1 + 1);
				endPos = line.find(')', beginPos + 1);

				int x = stoi(line.substr(beginPos + 1, midPos1 - beginPos - 1));
				int y = stoi(line.substr(midPos1 + 1, midPos2 - midPos1 - 1));
				int mode = stoi(line.substr(midPos1 + 1, endPos - midPos1 - 1));


				Tile tile;
				tile.TexCoords = { x, y };
				tile.Type = (TileType)mode;


				m_Map.emplace_back(tile);

				lastPos = endPos + 1;
				k = lastPos;
			}
		}
	}
};

class SandboxLayer : public zeus::Layer
{
private:
	std::string m_Name;

	bool m_Keys[65536] = { false };
	bool m_CameraMode = false;
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
		m_Camera = std::make_shared<zeus::Camera>();
		m_Camera->GetProperties().Position = defaultOrthoCamera.Position;
		m_Camera->GetProperties().MovementSpeed = defaultOrthoCamera.MovementSpeed;

		textureManager = zeus::TextureManager::GetInstance();
		textureManager->PutTexture({ "wood_tex", "assets/textures/wood.png" });
		textureManager->PutTexture({ "window_tex", "assets/textures/window.png" });
		textureManager->PutSpritesheet({ "person_sheet", "assets/textures/tilemap_packed.png", 16, true });
		textureManager->PutSpritesheet({ "building_sheet", "assets/textures/roguelike_spritesheet.png", 17, true });

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
		const auto& activeLevel = (SandboxLevel*)m_LevelManager.GetActiveLevel().get();
		static bool p_open = false;
		ImGui::Begin("Viewport", &p_open, flags);
		{
			const auto& windowOffset = ImGui::GetWindowContentRegionMin();
			const auto& windowMax = ImGui::GetContentRegionAvail();
			const auto& [sX, sY] = ImGui::GetWindowPos();
			m_CursorPos.x = pos.x - (sX + windowOffset.x);
			m_CursorPos.y = pos.y - (sY + windowOffset.y);

			if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered())
			{
				ImGuiIO& io = ImGui::GetIO();
				io.WantCaptureKeyboard = false;
				io.WantCaptureMouse = false;

				int id = m_Framebuffer->ReadID(1, m_CursorPos.x, HEIGHT - m_CursorPos.y);

				if (mouseButtons[MOUSE_RIGHT] && m_DragTile.IsActive)
				{
					activeLevel->GetLevelMap()[id].TexCoords = {-1, -1};
				}

				if (mouseButtons[MOUSE_LEFT] && m_DragTile.IsActive)
				{
					auto& levelMap = activeLevel->GetLevelMap();
					if (id >= levelMap.size())
					{
						SandboxLevel::Tile _tile;
						_tile.Position = { m_CursorPos.x, m_CursorPos.y, 0 };
						_tile.TexCoords = m_DragTile.Sprite.Coordinates;
						_tile.Type = SandboxLevel::TileType::None;
						activeLevel->AddTile(_tile);
					}
					else
					{
						levelMap[id].TexCoords = m_DragTile.Sprite.Coordinates;
					}
					m_DragTile.IsActive = false;
				}
			}

			const auto& tex = m_Framebuffer->GetAttachedTextures()[0];
			ImGui::Image(reinterpret_cast<void*>(tex->GetTextureID()), ImVec2{ WIDTH, HEIGHT }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
		}

		ImGui::Begin("Entity");
		{
			int id = m_Framebuffer->ReadID(1, m_CursorPos.x, HEIGHT - m_CursorPos.y);
			
			static int savedID = -1;
			if (id >= 0 && id < activeLevel->GetLevelMap().size())
			{
				
				if (mouseButtons[MOUSE_LEFT])
				{
					savedID = id;
				}
			}

			if (savedID != -1)
			{
				auto& tile = activeLevel->GetLevelMap()[savedID];
				const auto& tex = textureManager->GetSubTexture("building_sheet", tile.TexCoords.x, tile.TexCoords.y);
				ImVec2 bottomRight = *(ImVec2*)&tex->GetTexCoords()[1];
				ImVec2 topLeft = *(ImVec2*)&tex->GetTexCoords()[3];

				ImGui::Image((ImTextureID)tex->GetTextureID(), ImVec2(32, 32), topLeft, bottomRight);

				static const char* items[] = { "None", "Water", "Rock", "Tree" };
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

				ImGui::Text("Selected entity id: %d", savedID);
			}

			ImGui::Text("%f %f: %d", m_CursorPos.x, m_CursorPos.y, id);

			if (ImGui::Button("Save Level"))
			{
				activeLevel->SaveLevel();
			}

			ImGui::Checkbox("Show grid", &showGrid);
			ImGui::DragInt("Grid square size", &m_GridCellSize, 1, 0);
			ImGui::DragInt("Line width", &m_LineWidth, 1, 0, 31);
			if (ImGui::Button("Add New Entity"))
				ImGui::OpenPopup("Add New Entity");

			if (ImGui::BeginPopupModal("Add New Entity", NULL))
			{
				ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

				// Testing behavior of widgets stacking their own regular popups over the modal.
				static int item = 1;
				static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
				ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
				ImGui::ColorEdit4("color", color);

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
#if 1
		static bool p_open = false;
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		ImGui::Begin("Tiles", &p_open, flags);
		{
			ImTextureID id = 0;
			ImVec2 bottomRight, topLeft, tileSize = { 32, 32 };
			int windowWidth = (int)ImGui::GetContentRegionAvail().x;
			int sentinel = windowWidth / (tileSize.x + 14);

			uint32_t x = 1, y = 1;
			static int tilesCount = 57 * 31;
			for (int i = 0; i < tilesCount; i++)
			{
				const auto& tex = textureManager->GetSubTexture("building_sheet", x, y);
				id = (ImTextureID)tex->GetTextureID();
				bottomRight = *(ImVec2*)&tex->GetTexCoords()[1];
				topLeft = *(ImVec2*)&tex->GetTexCoords()[3];
				int texID = x * 57 + y;
				ImGui::PushID(texID);
				if (ImGui::ImageButton(id, tileSize, topLeft, bottomRight))
				{
					m_DragTile.IsActive = true;
					m_DragTile.Sprite.TextureName = "building_sheet";
					m_DragTile.Sprite.Coordinates = { x, y };
				}
				ImGui::PopID();

				if (x == 0 || x % sentinel != 0)
				{
					ImGui::SameLine();
					x++;
				}
				else
				{
					x = 1;
					y++;
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
			auto [x, y] = zeus::Input::GetMousePosition();
			auto mouseEvent = (zeus::MousePressedEvent&)evt;

			mouseButtons[mouseEvent.GetMouseButton()] = true;

			return true;
		};

		std::function<bool()> mouseReleased = [&]() -> bool {
			auto [x, y] = zeus::Input::GetMousePosition();
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
			auto mouseEvent = (zeus::MouseMovedEvent&)evt;
			auto pos = mouseEvent.GetMousePosition();

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

int main()
{
	try
	{
		auto app = new zeus::Application({ "Sandbox", WIDTH, HEIGHT });
		app->Init();
		app->PushLayer(new SandboxLayer("sandbox"));
		app->PushLayer(new EngineInfoLayer("engine"));
		app->Run();
	}
	catch (std::exception e)
	{
		LOG(Error, "%s", e.what());
	}

	return 0;
}
