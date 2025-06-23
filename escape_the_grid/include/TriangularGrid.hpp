#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class TriangularGrid
{
public:

    // Constructor
    TriangularGrid() = default;

    // Métodos para manejo de clicks y posiciones
    std::pair<int, int> getClickedCell(int mouseX, int mouseY) const;
    bool isPointInTriangle(int mouseX, int mouseY, int col, int row) const;
    
    // Métodos para celdas adyacentes
    std::vector<std::pair<int, int>> getAdjacentCells(int col, int row) const;
    bool isValidCell(int col, int row) const;
    bool isAdjacent(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2) const;
    
    // Métodos para información del grid
    bool isTrianglePointingUp(int col, int row) const;
    sf::Vector2f getCellCenter(int col, int row) const;
    std::vector<sf::Vector2f> getTriangleVertices(int col, int row) const;

private:
    // Función auxiliar para coordenadas baricéntricas
    bool isPointInTriangleBarycentric(const sf::Vector2f& point, 
                                     const sf::Vector2f& a, 
                                     const sf::Vector2f& b, 
                                     const sf::Vector2f& c) const;
};