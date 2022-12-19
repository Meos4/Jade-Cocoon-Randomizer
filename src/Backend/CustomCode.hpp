#pragma once

#include "Backend/Game.hpp"

#include <memory>

class CustomCode final
{
public:
    CustomCode(std::shared_ptr<Game> game);

    void write() const;
    bool isVanilla() const;
private:
    std::shared_ptr<Game> m_game;
};