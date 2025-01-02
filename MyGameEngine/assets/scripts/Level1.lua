-- Define a table with the values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
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
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = "tilemap-texture",
        --map_file = "./assets/tilemaps/mountain_landscape.map",
        --texture_asset_id = "tilemap-texture_2",
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
                    rotation = 45.0, -- degrees
                    velocity = { x = 50.0, y = 50.0 },
                    acceleration = 100.0,
                    friction = 10.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32,
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
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 200, y = 497 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false
                }
            }
        },
        {
            -- SU-27 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 20, y = 20 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = -100.0 }
                },
                sprite = {
                    texture_asset_id = "su27-texture",
                    width = 32,
                    height = 32,
                    z_index = 4
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -100 },
                    projectile_duration = 5, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        -- print("Executing the SU-27 fighter jet Lua script!")

                        -- this function makes the fighter jet move up and down the map shooting projectiles
                        local current_position_x, current_position_y = get_position(entity)
                        local current_velocity_x, current_velocity_y = get_velocity(entity)

                        -- if it reaches the top or the bottom of the map
                        if current_position_y < 10  or current_position_y > map_height - 32 then
                            set_velocity(entity, 0, current_velocity_y * -1); -- flip the entity y-velocity
                        else
                            set_velocity(entity, 0, current_velocity_y); -- do not flip y-velocity
                        end

                        -- set the transform rotation to match going up or down
                        if (current_velocity_y < 0) then
                            set_rotation(entity, 0) -- point up
                            set_projectile_velocity(entity, 0, -200) -- shoot projectiles up
                        else
                            set_rotation(entity, 180) -- point down
                            set_projectile_velocity(entity, 0, 200) -- shoot projectiles down
                        end
                    end
                }
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 90.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 4
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    projectile_duration = 1, -- secondsm
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        -- change the position of the the airplane to follow a sine wave movement
                        local new_x = ellapsed_time * 0.09
                        local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                        set_position(entity, new_x, new_y) -- set the new position
                    end
                }
            }
        },
        {
            -- Bomber airplane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 50, y = 50 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = -50.0 }
                },
                sprite = {
                    texture_asset_id = "bomber-texture",
                    width = 32,
                    height = 24,
                    z_index = 8
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 24
                },
                health = {
                    health_percentage = 100
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        -- change the position of the the airplane to follow a circular path
                        local radius = 170
                        local distance_from_origin = 300

                        local angle = ellapsed_time * 0.0009

                        -- calculate the new x-y cartesian position using polar coordinates
                        local new_x = (math.cos(angle) * radius) + distance_from_origin
                        local new_y = (math.sin(angle) * radius) + distance_from_origin
                        set_position(entity, new_x, new_y)

                        -- change the rotation of the sprite to match the circular motion
                        local angle_in_degrees = 180 + angle * 180 / math.pi
                        set_rotation(entity, angle_in_degrees)
                    end
                }
            }
        }
    }
}

-- Define some useful global variables
map_width = Level.tilemap.num_cols * Level.tilemap.tile_size * Level.tilemap.scale
map_height = Level.tilemap.num_rows * Level.tilemap.tile_size * Level.tilemap.scale
