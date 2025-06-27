#pragma once
#include <vector>
#include <iostream>
// Constantes compartidas del grid triangular
/*namespace GridConfig {
    constexpr unsigned short ORIGIN_X = 8 + 50;
    constexpr unsigned short ORIGIN_Y = 8 + 50;
    constexpr unsigned short COLUMNS = 10;
    constexpr unsigned short ROWS = 5;
    constexpr float CELL_SIZE = 70.f;
}*/

// Variables modificables en runtime
namespace GridConfig {
    inline unsigned short ORIGIN_X = 0;
    inline unsigned short ORIGIN_Y = 0;
    inline unsigned short COLUMNS = 10;
    inline unsigned short ROWS = 5;
    inline float CELL_SIZE = 70.0f;
    
    // Funciones para modificar valores
    // Funciones inline
    inline void updateGridSize(unsigned short cols, unsigned short rows) {
        COLUMNS = cols;
        ROWS = rows;
        //std::cout << "Grid size updated to: " << cols << "x" << rows << std::endl;
    }

    
    inline void updateOrigins(unsigned short origin_x, unsigned short origin_y) {
        ORIGIN_X = origin_x;
        ORIGIN_Y = origin_y;
        //std::cout << "Origins updated to: (" << origin_x << ", " << origin_y << ")" << std::endl;
    }
    
    inline void updateCellSize(float size) {
        CELL_SIZE = size;
        //std::cout << "Cell size updated to: " << size << std::endl;
    }
}



// Constantes compartidas del juego
namespace GameConfig {

    enum Cell { Empty, Invalid, Path, Visited, Wall, Item, Start, Finish };

    inline std::pair<unsigned short, unsigned short> START_POSITION = {1, 2};
    inline std::pair<unsigned short, unsigned short> FINISH_POSITION = {8, 3};
    inline   std::vector<std::vector<Cell>> gameMap(GridConfig::COLUMNS, std::vector<Cell>(GridConfig::ROWS, Cell::Empty));
    
    inline void updateMap( std::vector<std::vector<Cell>> newMap) {
        gameMap = newMap;
    }

    inline void updateStart(unsigned short col, unsigned short row ) {
        START_POSITION  = {col, row};
    }

    inline void updateFinish(unsigned short col, unsigned short row ) {
        FINISH_POSITION  = {col, row};
    }

}