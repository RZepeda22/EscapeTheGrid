#pragma once

void draw_map(  sf::RenderWindow& i_window,
                const std::pair<int, int>& playerPos,
                const std::vector<std::pair<int, int>>& highlightedCells,
                float pulseIntensity);