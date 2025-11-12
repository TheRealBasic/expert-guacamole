#pragma once
#include <deque>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace shadow {

struct Position {
    int x{0};
    int y{0};

    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Position& other) const { return !(*this == other); }
};

struct PositionHash {
    std::size_t operator()(const Position& pos) const noexcept {
        return static_cast<std::size_t>(pos.x) ^ (static_cast<std::size_t>(pos.y) << 16);
    }
};

enum class TileType { Wall, Floor, Exit };

enum class ItemType { Battery, Key };

struct Item {
    Position pos{};
    ItemType type{ItemType::Battery};
    bool collected{false};
};

struct Generator {
    Position pos{};
    bool activated{false};
};

class GameMap {
public:
    GameMap();

    int width() const { return width_; }
    int height() const { return height_; }
    TileType tile_at(const Position& pos) const;
    bool in_bounds(const Position& pos) const;
    bool is_walkable(const Position& pos) const;

    const Position& player_start() const { return playerStart_; }
    const Position& monster_start() const { return monsterStart_; }
    const Position& exit_position() const { return exitPos_; }
    const Position& door_position() const { return doorPos_; }
    const std::vector<Item>& items() const { return items_; }
    const std::vector<Generator>& generators() const { return generators_; }

    std::vector<Item>& items() { return items_; }
    std::vector<Generator>& generators() { return generators_; }

private:
    void build_from_blueprint();

    int width_{0};
    int height_{0};
    std::vector<TileType> tiles_{};
    Position playerStart_{};
    Position monsterStart_{};
    Position exitPos_{};
    Position doorPos_{};
    std::vector<Item> items_{};
    std::vector<Generator> generators_{};
};

class Game {
public:
    Game();
    void run();

private:
    enum class CommandResult { Invalid, NoTurn, ConsumeTurn };

    void render() const;
    void render_header() const;
    void render_map() const;
    void render_log() const;
    CommandResult handle_command(const std::string& line);
    CommandResult handle_move(const std::string& direction);
    CommandResult handle_take();
    CommandResult handle_charge();
    CommandResult handle_scan();
    CommandResult handle_toggle();
    CommandResult handle_wait();

    void advance_turn();
    void update_monster();
    void drain_battery(int amount, const std::string& reason);
    void apply_fear(int amount, const std::string& reason);
    bool is_passable(const Position& pos) const;
    std::vector<Position> neighbours(const Position& pos) const;
    bool find_step_toward(const Position& goal, Position& outStep) const;
    int manhattan(const Position& a, const Position& b) const;

    void push_log(const std::string& entry);
    void check_exit();

    GameMap map_{};
    Position playerPos_{};
    Position monsterPos_{};
    bool doorLocked_{true};
    bool lanternOn_{true};
    int battery_{70};
    int fear_{0};
    int turn_{0};
    int spareBatteries_{0};
    int keys_{0};
    int activatedGenerators_{0};
    mutable std::vector<bool> visited_{};
    int monsterRevealTimer_{0};
    bool gameOver_{false};
    bool playerWon_{false};
    std::deque<std::string> log_{};
    std::mt19937 rng_;
};

} // namespace shadow
