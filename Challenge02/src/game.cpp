#include "game.hpp"
#include <format>
#include <algorithm>
#include "collisions.hpp"
#include <iostream>


bool Game::report_game_is_valid(ErrorReport & report) const noexcept{
    for (auto &player : players)
    {
        for (auto shipit = player.ships.begin(); shipit!=player.ships.end(); ++shipit)
        {
            auto & ship = *shipit;
            if (!ship.is_valid()){
                report(std::format("Ship Id: {} is invalid.", ship.shiplength));
                return false;
            }
                
            if (ship.shiplength > maxShipSize || ship.shiplength < minShipSize){
                report(std::format("Player: {}'s Ship Id: {} is larger than game specified.", player.name, ship.id()));
                return false;
            }

            if( std::ranges::any_of( shipit+1, player.ships.end(), [&](auto & next_ship){  
                std::cout << std::format("{}'s comparing {} to {} collides = {}\n", player.name, ship.id(), next_ship.id(), aabb_collision( ship.location, next_ship.location ));
                
                if( aabb_collision( ship.location, next_ship.location ) ) {
                    report(std::format("{}'s ships collide:  {} collides with {}.", player.name, ship.id(), next_ship.id() ));    
                    return true;
                }
                
                return false;
                }
            )) 
            {

                return false;
            }
                
        }
    }




    return true;
}