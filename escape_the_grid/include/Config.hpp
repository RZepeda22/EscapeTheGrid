#pragma once

// Constantes compartidas del grid triangular
namespace GridConfig {
    constexpr unsigned short ORIGIN_X = 8 + 50;
    constexpr unsigned short ORIGIN_Y = 8 + 50;
    constexpr unsigned short COLUMNS = 10;
    constexpr unsigned short ROWS = 5;
    constexpr float CELL_SIZE = 70.f;
}

// Constantes compartidas del juego
namespace GameConfig {
    constexpr std::pair<unsigned short, unsigned short> START_POSITION = {1, 2};
    constexpr std::pair<unsigned short, unsigned short> FINISH_POSITION = {8, 3};
}