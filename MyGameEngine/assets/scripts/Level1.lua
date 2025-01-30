-- Define a table with the values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        --{ type = "texture", id = "player-texture", file = "./assets/sprites/characters/player.png" },
        { type = "texture", id = "l_up-texture", file = "./assets/sprites/characters/l_up.png" },
        { type = "texture", id = "l_right-texture", file = "./assets/sprites/characters/l_right.png" },
        { type = "texture", id = "l_down-texture", file = "./assets/sprites/characters/l_down.png" },
        { type = "texture", id = "l_left-texture", file = "./assets/sprites/characters/l_left.png" },
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/grass.png" },
        { type = "texture", id = "tilemap-texture_2", file = "./assets/tilemaps/mountain_landscape.png" },
        { type = "texture", id = "chopper-texture", file = "./assets/images/chopper.png" },
        { type = "texture", id = "f22-texture", file = "./assets/images/fw190-spritesheet.png" },
        { type = "texture", id = "su27-texture", file = "./assets/images/su27-spritesheet.png" },
        { type = "texture", id = "chopper-spritesheet-texture", file = "./assets/images/chopper-spritesheet.png" },
        { type = "texture", id = "tank-texture", file = "./assets/images/tank-tiger-up.png" },
        { type = "texture", id = "bomber-texture", file = "./assets/images/bomber-spritesheet.png" },
        { type = "texture", id = "bullet-texture", file = "./assets/images/bullet.png" },
        { type = "font"   , id = "arial-font", file = "./assets/fonts/arial.ttf", font_size = 10 },
        { type = "font"   , id = "charriot-font", file = "./assets/fonts/charriot.ttf", font_size = 20 }
    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        map_file = "./assets/tilemaps/grass.map",
        texture_asset_ids =
        {
            "tilemap-texture"
        },
        num_rows = 15,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] =
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 242, y = 110 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                    velocity = { x = 50.0, y = 50.0 },
                    acceleration = 100.0,
                    friction = 10.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_ids =
                    {
                        "l_up-texture",
                        "l_right-texture",
                        "l_down-texture",
                        "l_left-texture"
                    },
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 12,
                    speed_rate = 30 -- fps
                },
                boxcollider = {
                    width = 192,
                    height = 192,
                    -- offset = { x = 0, y = 5 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true
                },
                keyboard_controller = {
                    --rotationAngle = 0.0,
                    --walkSpeed = 0.0,
                    --turnSpeed = 180.0
                    --up_velocity = { x = 0, y = -50 },
                    --right_velocity = { x = 50, y = 0 },
                    --down_velocity = { x = 0, y = 50 },
                    --left_velocity = { x = -50, y = 0 }
                },
                camera_follow = {
                    follow = true
                }
            }
        }
    }
}

-- Define some useful global variables
map_width = Level.tilemap.num_cols * Level.tilemap.tile_size * Level.tilemap.scale
map_height = Level.tilemap.num_rows * Level.tilemap.tile_size * Level.tilemap.scale
