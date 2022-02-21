#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class GameOfLife : public olc::PixelGameEngine
{
public:

    uint16_t nWidth;
    uint16_t nHeight;
    uint16_t nPixelW;

    float updateTime;

    bool running;

    GameOfLife(uint16_t width, uint16_t height, uint16_t pixelW, uint16_t cellSize)
    {
        nWidth = width;
        nHeight = height;
        nPixelW = pixelW;
        nCellSize = cellSize;

        running = false;
        updateTime = 0.5f;

        sAppName = "GameOfLife (Paused)";
        showFPS = true;
    }

public:

    bool OnUserCreate() override
    {
        offSetGrid = 10;
        nGridWidth = nWidth / nCellSize / nPixelW + offSetGrid * 2;
        nGridHeight = nHeight / nCellSize / nPixelW + offSetGrid * 2;
        cells.resize(nGridWidth);
        for (int x = 0; x < nGridWidth; x++)
        {
            cells[x].resize(nGridHeight);
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        HandleInputs();
        DrawGrid();
        DrawCells();
        if (running) {
            timer += fElapsedTime;
            if (timer >= updateTime) {
                timer = 0.0f;
                GameCycle();
            }
        }
        return true;
    }

private:
    uint16_t offSetGrid;
    std::vector<std::vector<bool>> cells;

// Logic stuff
private:

    float timer = 0.0f;

    void GameCycle() {

        std::vector<std::vector<bool>> newCells(cells);
        for (int x = 0; x < nGridWidth; x++)
        {
            for (int y = 0; y < nGridHeight; y++)
            {
                uint16_t nNeighbours = GetNeighbours(x, y, newCells);
                cells[x][y] = (nNeighbours == 3 || (cells[x][y] && nNeighbours == 2));
            }
        }
    }

    uint16_t GetNeighbours(uint16_t x, uint16_t y, std::vector<std::vector<bool>>& cellsCopy) const {
        uint16_t nNeighbours = 0;
        int nX, nY;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                nX = x + i;
                nY = y + j;
                if (nX >= 0 && nX < nGridWidth && nY >= 0 && nY < nGridHeight && !(x == nX && y == nY)) {
                    if (cellsCopy[nX][nY]) {
                        nNeighbours++;
                    }
                }
            }
        }
        return nNeighbours;
    }

// Input stuff
private:

    void HandleInputs() {
        HandleKeyBoardInputs();
        HandleMouseInputs();
    }

    void HandleKeyBoardInputs() {
        if (GetKey(olc::Key::SPACE).bPressed) {
            running = !running;
            sAppName = running ? "GameOfLife (Running)" : "GameOfLife (Paused)";
        }

        if (GetKey(olc::Key::R).bPressed) {
            for (int x = 0; x < nGridWidth; x++) {
                for (int y = 0; y < nGridHeight; y++) {
                    cells[x][y] = false;
                }
            }
            running = false;
            sAppName = running ? "GameOfLife (Running)" : "GameOfLife (Paused)";
        }
    }

    void HandleMouseInputs() {

        if (GetMouse(0).bHeld) {
            int x = GetMouseX() / nCellSize;
            int y = GetMouseY() / nCellSize;
            cells[x+offSetGrid][y+offSetGrid] = true;
        }
        if (GetMouse(1).bHeld) {
            int x = GetMouseX() / nCellSize;
            int y = GetMouseY() / nCellSize;
            cells[x+offSetGrid][y+offSetGrid] = false;
        }

        if (GetMouseWheel() > 0) {
            updateTime += 0.05f;
        } else if (GetMouseWheel() < 0 && updateTime > 0.0f) {
            updateTime -= 0.05f;
        }

    }

// Drawing stuff
private:

    uint16_t nCellSize;
    uint16_t nGridWidth;
    uint16_t nGridHeight;

    void DrawGrid() {
        // draw columns
        for (int x = 0; x <= nWidth; x += nCellSize)
        {
            DrawLine(x, 0, x, nHeight, olc::Pixel(70, 70, 70));
        }
        // draw row
        for (int y = 0; y <= nHeight; y += nCellSize)
        {
            DrawLine(0, y, nWidth, y, olc::Pixel(70, 70, 70));
        }
    }

    void DrawCells() {
        for (int x = offSetGrid; x < nGridWidth - offSetGrid; x++) {
            for (int y = offSetGrid; y < nGridHeight - offSetGrid; y++) {
                if (cells[x][y]) {
                    FillRect((x-offSetGrid) * nCellSize + 1, (y-offSetGrid) * nCellSize + 1, nCellSize - 1, nCellSize - 1, olc::WHITE);
                }
            }
        }
    };

};

int main(int argc, char** argv)
{
    if (argc < 4 || argc > 5) {
        std::cout << "Usage: " << argv[0] << " <width> <height> <pixelW> [<cellSize>]" << std::endl;
        exit(-1);
    }
    uint16_t nWidth = std::stoi(argv[1]);
    uint16_t nHeight = std::stoi(argv[2]);
    uint16_t nPixelW = std::stoi(argv[3]);
    uint16_t nCellSize = 10;
    if (argc == 5)
        nCellSize = std::stoi(argv[4]);
    GameOfLife demo(nWidth, nHeight, nPixelW, nCellSize);
    if (demo.Construct(nWidth / nPixelW, nHeight / nPixelW, nPixelW, nPixelW))
        demo.Start();
    return 0;
}
