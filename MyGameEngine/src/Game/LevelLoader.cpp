#include "LevelLoader.h"
#include <fstream>
#include "Game.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyBoardControlledComponent_v1.h"
#include "../Components/KeyBoardControlledComponent_v2.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/ScriptComponent.h"

LevelLoader::LevelLoader()
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int levelNumber)
{
	// Checks the syntax of the script, but doesnt execute it
	sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the lua script: " + errorMessage);
		return;
	}

	lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

	sol::table level = lua["Level"];

	////////////////////////////////////////////////////////////////////////////
	// Read the level assets
	////////////////////////////////////////////////////////////////////////////
	sol::table assets = level["assets"];

	int i = 0;
	while (true)
	{
		sol::optional<sol::table> hasAsset = assets[i];
		if (hasAsset == sol::nullopt)
			break;

		sol::table asset = assets[i];
		std::string assetType = asset["type"];
		if (assetType == "texture")
			assetManager->AddTexture(renderer, asset["id"], asset["file"]);
		if (assetType == "font")
			assetManager->AddFont(asset["id"], asset["file"], asset["font_size"]);
		i++;
	}


	////////////////////////////////////////////////////////////////////////////
	// Read the level tilemap information
	////////////////////////////////////////////////////////////////////////////
	sol::table map = level["tilemap"];
	std::string mapFilePath = map["map_file"];
	std::string mapTextureAssetId = map["texture_asset_ids"];
	std::vector<std::string> textureAssetIds = map["texture_asset_ids"].get<std::vector<std::string>>();
	int mapNumCols = map["num_cols"];
	int mapNumRows = map["num_rows"];
	int tileSize = map["tile_size"];
	double tileScale = map["scale"];
	std::fstream mapFile;

	mapFile.open(mapFilePath);
	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.setGroup("tiles");
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>(textureAssetIds, tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();

	Game::mapWidth = mapNumCols * tileSize * tileScale;
	Game::mapHeight = mapNumRows * tileSize * tileScale;

	////////////////////////////////////////////////////////////////////////////
	// Read the level entities and their components
	////////////////////////////////////////////////////////////////////////////
	sol::table entities = level["entities"];
	i = 0;
	while (true)
	{
		sol::optional<sol::table> hasEntity = entities[i];
		if (hasEntity == sol::nullopt) {
			break;
		}

		sol::table entity = entities[i];

		Entity newEntity = registry->CreateEntity();

		// Tag
		sol::optional<std::string> tag = entity["tag"];
		if (tag != sol::nullopt) {
			newEntity.HasTag(entity["tag"]);
		}

		// Group
		sol::optional<std::string> group = entity["group"];
		if (group != sol::nullopt) {
			newEntity.BelongsToGroup(entity["group"]);
		}

		// Components
		sol::optional<sol::table> hasComponents = entity["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform
			sol::optional<sol::table> transform = entity["components"]["transform"];
			if (transform != sol::nullopt)
			{
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						entity["components"]["transform"]["scale"]["x"].get_or(1.0),
						entity["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					entity["components"]["transform"]["rotation"].get_or(0.0),
					glm::vec2(
						entity["components"]["transform"]["velocity"]["x"].get_or(0.0),
						entity["components"]["transform"]["velocity"]["y"].get_or(0.0)
					),
					static_cast<double>(entity["components"]["transform"]["acceleration"].get_or(0.0)),
					static_cast<double>(entity["components"]["transform"]["friction"].get_or(0.0))
					);
			}


			// RigidBody
			sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
			if (rigidbody != sol::nullopt) 
			{
				newEntity.AddComponent<RigidBodyComponent>(
					glm::vec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
					)
					);
			}

			// Sprite
			sol::optional<sol::table> sprite = entity["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				std::vector<std::string> textureAssetIds = entity["components"]["sprite"]["texture_asset_ids"].get<std::vector<std::string>>();
				if (std::find(textureAssetIds.begin(), textureAssetIds.end(), "l_right-texture") != textureAssetIds.end()) {
					newEntity.setTag("player");
				}

				newEntity.AddComponent<SpriteComponent>(
					textureAssetIds,
					entity["components"]["sprite"]["width"],
					entity["components"]["sprite"]["height"],
					entity["components"]["sprite"]["z_index"].get_or(1),
					entity["components"]["sprite"]["fixed"].get_or(false),
					entity["components"]["sprite"]["src_rect_x"].get_or(0),
					entity["components"]["sprite"]["src_rect_y"].get_or(0)
					);
			}

			// Animation
			sol::optional<sol::table> animation = entity["components"]["animation"];
			if (animation != sol::nullopt) {
				newEntity.AddComponent<AnimationComponent>(
					entity["components"]["animation"]["num_frames"].get_or(1),
					entity["components"]["animation"]["speed_rate"].get_or(1)
					);
			}

			// BoxCollider
			sol::optional<sol::table> collider = entity["components"]["boxcollider"];
			if (collider != sol::nullopt) 
			{
				newEntity.AddComponent<BoxColliderComponent>(
					entity["components"]["boxcollider"]["width"],
					entity["components"]["boxcollider"]["height"],
					glm::vec2(
						entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
						entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
					)
					);
			}

			// Health
			sol::optional<sol::table> health = entity["components"]["health"];
			if (health != sol::nullopt) 
			{
				newEntity.AddComponent<HealthComponent>(
					static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
					);
			}

			// ProjectileEmitter
			sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
			if (projectileEmitter != sol::nullopt) 
			{
				newEntity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(
						entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
						entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
					),
					static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
					static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
					static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
					entity["components"]["projectile_emitter"]["friendly"].get_or(false)
					);
			}

			// CameraFollow
			sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
			if (cameraFollow != sol::nullopt)
			{
				newEntity.AddComponent<CameraFollowComponent>();
			}

			// KeyboardControlled v1
			//sol::optional<sol::table> keyboardControlled_v1 = entity["components"]["keyboard_controller"];
			//if (keyboardControlled_v1 != sol::nullopt)
			//{
			//	newEntity.AddComponent<KeyBoardControlledComponent_v1>(
			//		/*static_cast<float>(entity["components"]["keyboard_controller"]["rotationAngle"].get_or(0.0)),
			//		static_cast<float>(entity["components"]["keyboard_controller"]["walkSpeed"].get_or(0.0)),
			//		static_cast<float>(entity["components"]["keyboard_controller"]["turnSpeed"].get_or(0.0))*/
			//		);
			//}

			// KeyboardControlled v2
			sol::optional<sol::table> keyboardControlled_v2 = entity["components"]["keyboard_controller"];
			if (keyboardControlled_v2 != sol::nullopt)
			{
				newEntity.AddComponent<KeyBoardControlledComponent_v2>(
					/*static_cast<float>(entity["components"]["keyboard_controller"]["rotationAngle"].get_or(0.0)),
					static_cast<float>(entity["components"]["keyboard_controller"]["walkSpeed"].get_or(0.0)),
					static_cast<float>(entity["components"]["keyboard_controller"]["turnSpeed"].get_or(0.0))*/
				);
			}

			// Script
			sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt)
			{
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}
		}
		i++;
	}
}
