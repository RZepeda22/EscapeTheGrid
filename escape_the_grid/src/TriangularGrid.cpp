#include "TriangularGrid.hpp"
#include <algorithm>
#include <cmath>
#include "Config.hpp"

std::pair<int, int> TriangularGrid::getClickedCell(int mouseX, int mouseY) const
{
    // Aproximación inicial más precisa
    int col = static_cast<int>((mouseX - GridConfig::ORIGIN_X) / (GridConfig::CELL_SIZE * 0.5f));
    int row = static_cast<int>((mouseY - GridConfig::ORIGIN_Y) / GridConfig::CELL_SIZE);
    std::cout << "Celdas" << col << row  << std::endl;
    // Limitar a rangos válidos
    col = std::max(0, std::min(GridConfig::COLUMNS - 1, col));
    row = std::max(0, std::min(GridConfig::ROWS - 1, row));
    
    // Verificar la celda principal y adyacentes
    std::vector<std::pair<int, int>> candidates = {
        {col, row},           // Principal
        {col-1, row}, {col+1, row}, 
        {col, row-1}, {col, row+1}
    };
    
    for (const auto& candidate : candidates)
    {
        std::cout << "CeldasM" << col << row  << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        if (isValidCell(candidate.first, candidate.second))
        {
            if (isPointInTriangle(mouseX, mouseY, candidate.first, candidate.second))
            {
                return candidate;
            }
        }
    }
    
    return {-1, -1};
}

bool TriangularGrid::isPointInTriangle(int mouseX, int mouseY, int col, int row) const
{
    if (!isValidCell(col, row)) return false;
    
    // Calcular posición base del triángulo
    float x = static_cast<float>(GridConfig::ORIGIN_X) + (static_cast<float>(col) / 2.0f) * GridConfig::CELL_SIZE;
    float y = static_cast<float>(GridConfig::ORIGIN_Y) + static_cast<float>(row) * GridConfig::CELL_SIZE;
    
    sf::Vector2f mousePoint(static_cast<float>(mouseX), static_cast<float>(mouseY));
    std::vector<sf::Vector2f> vertices = getTriangleVertices(col, row);
    
    return isPointInTriangleBarycentric(mousePoint, vertices[0], vertices[1], vertices[2]);
}

std::vector<std::pair<int, int>> TriangularGrid::getAdjacentCells(int col, int row) const
{
    std::vector<std::pair<int, int>> adjacents;
    
    // 1. VERIFICAR SI LA CELDA ES VÁLIDA
    if (!isValidCell(col, row)) return adjacents;
    
    // 2. DETERMINAR ORIENTACIÓN DEL TRIÁNGULO
    if (isTrianglePointingUp(col, row)) {
        // Triángulo hacia ARRIBA (△)
        // Comparte aristas con:
        adjacents.push_back({col - 1, row});     // Izquierda (triángulo hacia abajo)
        adjacents.push_back({col + 1, row});     // Derecha (triángulo hacia abajo)
        adjacents.push_back({col, row + 1});     // Abajo (triángulo hacia abajo)
    } else {
        // Triángulo hacia ABAJO (▽)
        // Comparte aristas con:
        adjacents.push_back({col - 1, row});     // Izquierda (triángulo hacia arriba)
        adjacents.push_back({col + 1, row});     // Derecha (triángulo hacia arriba)
        adjacents.push_back({col, row - 1});     // Arriba (triángulo hacia arriba)
    }
    
    // Filtrar solo las celdas válidas
    std::vector<std::pair<int, int>> validAdjacents;
    for (const auto& cell : adjacents)
    {
        GameConfig::Cell cellType = GameConfig::gameMap[col][row];
        //std::cout << "Celda: " << cellType << " ";
        //std::cout << "col: " << col << " row: " << row << std::endl;
        if (isValidCell(cell.first, cell.second)  )  //&& cellType == GameConfig::Cell::Empty
        {
            validAdjacents.push_back(cell);
        }
    }
    
    return validAdjacents;
}

// Función para verificar si una celda está dentro de los límites del grid
bool TriangularGrid::isValidCell(int col, int row) const
{
    // Verificar que la columna esté entre 0 y COLUMNS-1 (inclusive)
    // Y que la fila esté entre 0 y ROWS-1 (inclusive)
    
    return col >= 0 && col < GridConfig::COLUMNS && row >= 0 && row < GridConfig::ROWS;
    
    //return cellType != GameConfig::Cell::Invalid && cellType != GameConfig::Cell

}

// Función para verificar si dos celdas son adyacentes entre sí
bool TriangularGrid::isAdjacent(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2) const
{
    // Obtener todas las celdas adyacentes a la primera celda
    std::vector<std::pair<int, int>> adjacents = getAdjacentCells(cell1.first, cell1.second);
    
    // Buscar si la segunda celda está en la lista de adyacentes de la primera
    for (const auto& adj : adjacents)
    {
        if (adj.first == cell2.first && adj.second == cell2.second)
        {
            return true;
        }
    }
    return false;
}

bool TriangularGrid::isTrianglePointingUp(int col, int row) const
{
    return (col + row) % 2 == 0;
}

sf::Vector2f TriangularGrid::getCellCenter(int col, int row) const
{
    float x = static_cast<float>(GridConfig::ORIGIN_X) + (static_cast<float>(col) / 2.0f) * GridConfig::CELL_SIZE;
    float y = static_cast<float>(GridConfig::ORIGIN_Y) + static_cast<float>(row) * GridConfig::CELL_SIZE;
    
    if (isTrianglePointingUp(col, row)) {
        return sf::Vector2f(x + GridConfig::CELL_SIZE/2.0f, y + GridConfig::CELL_SIZE * 0.66f);
    } else {
        return sf::Vector2f(x + GridConfig::CELL_SIZE/2.0f, y + GridConfig::CELL_SIZE * 0.33f);
    }
}

std::vector<sf::Vector2f> TriangularGrid::getTriangleVertices(int col, int row) const
{
    float x = static_cast<float>(GridConfig::ORIGIN_X) + (static_cast<float>(col) / 2.0f) * GridConfig::CELL_SIZE;
    float y = static_cast<float>(GridConfig::ORIGIN_Y) + static_cast<float>(row) * GridConfig::CELL_SIZE;
    
    std::vector<sf::Vector2f> vertices(3);
    
    if (isTrianglePointingUp(col, row)) {
        // Triángulo hacia arriba
        vertices[0] = {x + GridConfig::CELL_SIZE/2.0f, y};
        vertices[1] = {x, y + GridConfig::CELL_SIZE};
        vertices[2] = {x + GridConfig::CELL_SIZE, y + GridConfig::CELL_SIZE};
    } else {
        // Triángulo hacia abajo  
        vertices[0] = {x, y};
        vertices[1] = {x + GridConfig::CELL_SIZE, y};
        vertices[2] = {x + GridConfig::CELL_SIZE/2.0f, y + GridConfig::CELL_SIZE};
    }
    
    return vertices;
}

bool TriangularGrid::isPointInTriangleBarycentric(const sf::Vector2f& point, 
                                                 const sf::Vector2f& a, 
                                                 const sf::Vector2f& b, 
                                                 const sf::Vector2f& c) const
{
    sf::Vector2f v0 = c - a;
    sf::Vector2f v1 = b - a;
    sf::Vector2f v2 = point - a;

    float dot00 = v0.x * v0.x + v0.y * v0.y;
    float dot01 = v0.x * v1.x + v0.y * v1.y;
    float dot02 = v0.x * v2.x + v0.y * v2.y;
    float dot11 = v1.x * v1.x + v1.y * v1.y;
    float dot12 = v1.x * v2.x + v1.y * v2.y;

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}