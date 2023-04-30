#include "zeus.h"
#include "phyzicsall.h"
#include "zerializer/zerializer.hpp"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <random>
#include <iostream>
#include <random>
#include <stdexcept>
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
		Water = 0,
		Rock,
		Tree,
		None
	};

	struct Tile
	{
		int idx = -1;
		glm::vec3 Position;
		glm::ivec2 TexCoords;
		TileType Type = TileType::None;
	};

private:
	std::string m_LevelPath;

	uint32_t m_LevelCols{ 0u };
	uint32_t m_LevelRows{ 0u };
	uint32_t m_CellSize	{ 32u };
	float m_CellGap = 2.0f;

	std::vector<Tile> m_Map;
	std::shared_ptr<zeus::Zerializer> m_Serializer;
	std::unordered_map<int, bool> m_Selected;

public:
	void OnStart() override
	{
		m_Serializer = std::make_shared<zeus::Zerializer>();
		LoadLevel("assets/levels/level.data");
	}

	std::string GetLevelPath() const
	{
		return m_LevelPath;
	}

	std::string GetLevelName() const
	{
		size_t begin = m_LevelPath.find_last_of('/');
		size_t end = m_LevelPath.find_last_of('.');

		return m_LevelPath.substr(begin + 1, end - begin - 1);
	}

	zeus::QuadData quad;
	void Draw() override
	{
		float gap = m_CellSize * m_CellGap;
		static float leftBoundary	= 0.0f;
		static float rightBoundary	= WIDTH - m_CellSize;
		static float topBoundary	= HEIGHT - m_CellSize;
		static float bottomBoundary = 0.0f;

		for (uint32_t y = 0; y < m_LevelRows; y++)
		{
			for (uint32_t x = 0; x < m_LevelCols; x++)
			{
				int idx = x + y * m_LevelCols;
				float xx = x * gap - spritePos.x + 32.0f;
				float yy = y * gap - spritePos.y - 32.0f;

				if (xx < leftBoundary || xx > rightBoundary || yy < bottomBoundary || yy > topBoundary)
					continue;

				if (idx < (int)m_Map.size())
				{
					if (m_Selected[idx])
					{
						float dx = m_CellSize * 0.2f;
						quad.SetSize({ m_CellSize + dx, m_CellSize + dx, 1.0f });
					}
					else
					{
						quad.SetSize({ m_CellSize, m_CellSize, 0.0f });
					}

					quad.SetPosition({ xx, yy, m_Map[idx].Position.z });
					quad.SetRotation(0.0f);
					quad.SetSubTexture(textureManager->GetSubTexture("building_sheet", m_Map[idx].TexCoords.x, m_Map[idx].TexCoords.y));
					quad.SetEntityID(idx);

					zeus::Renderer::DrawTexturedQuad(quad);
				}
			}
		}

		quad.SetPosition(spritePos);
		quad.SetSize(scaler);
		quad.SetRotation(angle);
		quad.SetEntityID(2546);
		quad.SetTexture(textureManager->GetTexture("wood_tex"));

		zeus::Renderer::DrawTexturedQuad(quad);
	}

	// @brief This function will increase the map size always (cannot reduce map size)
	// @param size - side length of the map and size always +ve (entire map is a square)
	void ResizeMap(uint32_t size)
	{
		uint32_t remainMapSize = size * size - (uint32_t)m_Map.size();
		m_LevelCols += size;
		m_LevelRows += size;
		Tile defaultTile;
		defaultTile.TexCoords = { -1, -1 };
		for (uint32_t i = 0; i < remainMapSize; i++)
		{
			m_Map.emplace_back(defaultTile);
		}
	}

	// void AddTile(const Tile& tile)
	// {
	// 	if (tile.Position.x > m_LevelCols * m_CellSize)
	// 	{
	// 		m_LevelCols++;
	// 		QUICK_LOG(Trace, "columns increased");
	// 	}
	// 	if (tile.Position.y > m_LevelRows * m_CellSize)
	// 	{
	// 		m_LevelRows++;
	// 		QUICK_LOG(Trace, "columns increased");
	// 	}
	// 	m_Map.emplace_back(tile);
	// }

	std::vector<Tile>& GetLevelMap()
	{
		return m_Map;
	}

	void ChangeAllTiles(glm::ivec2 oldTex, TileType newType)
	{
		for (Tile& tile : m_Map)
		{
			if (tile.TexCoords == oldTex)
			{
				tile.Type = newType;
			}
		}
	}

	void ClearLevelMap()
	{
		m_LevelCols = 0;
		m_LevelRows = 0;
		m_Map.clear();
	}

	void SelectCell(int id)
	{
		m_Selected[id] = true;
	}

	void DeselectAll()
	{
		m_Selected.clear();
	}

	void SetCellsize(uint32_t size)
	{
		m_CellSize = size;
	}

	void SetCellgap(float gap)
	{
		m_CellGap = gap;
	}

	float GetCellgap()
	{
		return m_CellGap;
	}

	uint32_t GetCellsize()
	{
		return m_CellSize;
	}

	// String related utility functions for removing white spaces of a string(trailing/leading)
	static inline void left_trim(std::string& s)
	{
		s.erase(s.begin(), std::find_if(
				s.begin(),
				s.end(),
				[](unsigned char c) { return !std::isspace(c); }
			)
		);
	}

	static inline void right_trim(std::string& s)
	{
		s.erase(std::find_if(
				s.rbegin(),
				s.rend(),
				[](unsigned char c) { return !std::isspace(c); }
			).base(),
		s.end());
	}

	static inline void trim(std::string& s)
	{
		left_trim(s);
		right_trim(s);
	}

	void SaveLevel()
	{
		zeus::SerialInfo info;
		info.SerializationType = zeus::SerialType::PROPERTIES;
		m_Serializer->StartSerialize(m_LevelPath, info);

		// Tile(posX, posY, type, index)
	    const std::function<std::string(const Tile&)> ivToString = [](const Tile& tile) {
			std::stringstream ss;
			ss << "(" << tile.TexCoords.x << ", ";
			ss << tile.TexCoords.y << ", ";
			ss << (int)tile.Type << ", ";
			ss << tile.idx << ") ";
			return ss.str();
	    };

	    m_Serializer->Serialize("#", m_LevelPath);
		m_Serializer->Serialize("cols", (int)m_LevelCols);
		m_Serializer->Serialize("rows", (int)m_LevelRows);
		m_Serializer->Serialize("cellsize", (int)m_CellSize);
		m_Serializer->Serialize("cellgap", (double)m_CellGap);
		m_Serializer->Serialize("levelmap", m_Map, ivToString);
		m_Serializer->Flush();
	}

	void LoadLevel(const std::string& filepath)
	{
		m_LevelPath = filepath;

		zeus::SerialInfo info;
		info.SerializationType = zeus::SerialType::PROPERTIES;
		m_Serializer->StartDeserialize(filepath, info);

		m_LevelCols = m_Serializer->DeserializeInt("cols");
		m_LevelRows = m_Serializer->DeserializeInt("rows");
		m_CellSize = m_Serializer->DeserializeInt("cellsize");
		m_CellGap = m_Serializer->DeserializeDbl("cellgap");

	    const std::function<Tile(std::string&)> sToTile = [](std::string& s) {
			Tile tile;
	        std::vector<int> res;
			std::stringstream ss;
			ss.str(s);
	        std::string token;
	        size_t pos;

	        while (ss >> token)
	        {
	            pos = token.find(",");
	            if (pos != std::string::npos)
	                token.erase(pos, 1);
	            res.push_back(std::stoi(token));
	        }

	        tile.TexCoords = { res[0], res[1] };
	        tile.Type = (TileType)res[2];
	        tile.idx = res[3];

	        return tile;
	    };
		std::vector<Tile> levelmap = m_Serializer->DeserializeVec<Tile>("levelmap", sToTile);

		// Reading map
		for (const auto& tile : levelmap)
		{
			m_Map.push_back(tile);
		}

		// Reading enemy
		// if (m_Serializer->ReadHeader("enemy1"))
		// {
		// 	std::string enemyType = m_Serializer->DeserializeStr("type");
		// 	EnemyType type = EnemyType::None;
		// 	if (enemyType == "human")
		// 	{
		// 		type = EnemyType::Human;
		// 	}
		// 	else if (enemyType == "animal")
		// 	{
		// 		type = EnemyType::Animal;
		// 	}
		// 	else
		// 	{
		// 		type = EnemyType::None;
		// 	}

		// 	if (m_EnemyRegistry == nullptr)
		// 	{
		// 		LOG(Error, "Runtime Error: Enemy registry is not initialized for operation!");
		// 	}

		// 	// Reading enemy position
		// 	auto enemy = m_EnemyRegistry->CreateEnemy(type);

		// 	const std::function<int(const std::string&)> sToInt = [](const std::string& s) {
		// 		return std::stoi(s);
		// 	};
		// 	std::vector<int> pos = m_Serializer->DeserializeVec<int>("pos", sToInt);
		// 	enemy->SetPosition({ pos[0], pos[1], 0.1f });
		// 	m_Enemies.push_back(enemy);
			
		// 	// Reading enemy weapon
		// 	std::string weaponName = m_Serializer->DeserializeStr("weapon");
		// 	if (weaponName == "gun")
		// 	{
		// 		enemy->SetWeapon(WeaponType::Gun);
		// 	}
		// 	m_Serializer->EndHeader();
		// }
	}
};

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
		const char* fileName = activeLevel->GetLevelName().c_str();
		ImGui::Begin(fileName, &p_open, flags);
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
						activeLevel->GetLevelMap()[id].TexCoords = {-1, -1};
					}
					else if (m_DragTile.IsActive)
					{
						m_DragTile.IsActive = false;
					}
				}

				if (mouseButtons[MOUSE_LEFT] && m_DragTile.IsActive)
				{
					auto& levelMap = activeLevel->GetLevelMap();
#if 0
					if (id >= levelMap.size())
					{
						SandboxLevel::Tile _tile;
						_tile.Position = { m_CursorPos.x, m_CursorPos.y, 0 };
						_tile.TexCoords = m_DragTile.Sprite.Coordinates;
						_tile.Type = SandboxLevel::TileType::None;
						activeLevel->AddTile(_tile);
					}
#endif

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
			static int additionSize = 0;
			ImGui::InputInt("Map size NxN", &additionSize);
			if (ImGui::Button("Resize##Resize-button"))
			{
				activeLevel->ResizeMap(additionSize);
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("This amount will add to the existing level map");
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
				ImGui::SetWindowSize(ImVec2 { 200, 200 });
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
	auto app = new zeus::Application({ "Sandbox", WIDTH, HEIGHT, false });
	app->Init();
	app->PushLayer(new SandboxLayer("sandbox"));
	app->PushLayer(new EngineInfoLayer("engine"));
	app->Run();

	return 0;
}
