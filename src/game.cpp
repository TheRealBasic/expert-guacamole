#include "shadow/game.hpp"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>

namespace shadow {
namespace {
const std::vector<std::string> kBlueprint = {
    "###########",
    "#P..#...GM#",
    "#..##.#..##",
    "#..G..B...#",
    "###.##.####",
    "#...#..G..#",
    "#K#.#.##E##",
    "#...D.....#",
    "###########"};

std::string lowercase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

bool is_direction(const std::string& token) {
    static const std::vector<std::string> dirs = {"north", "south", "east", "west", "n", "s", "e", "w"};
    return std::find(dirs.begin(), dirs.end(), token) != dirs.end();
}

Position direction_offset(const std::string& direction) {
    const std::string dir = lowercase(direction);
    if (dir == "north" || dir == "n") return {0, -1};
    if (dir == "south" || dir == "s") return {0, 1};
    if (dir == "west" || dir == "w") return {-1, 0};
    return {1, 0};
}

char draw_item_char(ItemType type) {
    return type == ItemType::Battery ? 'B' : 'K';
}

} // namespace

GameMap::GameMap() { build_from_blueprint(); }

void GameMap::build_from_blueprint() {
    height_ = static_cast<int>(kBlueprint.size());
    width_ = static_cast<int>(kBlueprint.empty() ? 0 : kBlueprint.front().size());
    tiles_.assign(width_ * height_, TileType::Floor);

    for (int y = 0; y < height_; ++y) {
        const auto& row = kBlueprint[y];
        for (int x = 0; x < width_; ++x) {
            const char ch = row[x];
            Position pos{x, y};
            switch (ch) {
            case '#':
                tiles_[y * width_ + x] = TileType::Wall;
                break;
            case 'P':
                playerStart_ = pos;
                break;
            case 'M':
                monsterStart_ = pos;
                break;
            case 'E':
                tiles_[y * width_ + x] = TileType::Exit;
                exitPos_ = pos;
                break;
            case 'D':
                doorPos_ = pos;
                break;
            case 'G':
                generators_.push_back({pos, false});
                break;
            case 'B':
                items_.push_back({pos, ItemType::Battery, false});
                break;
            case 'K':
                items_.push_back({pos, ItemType::Key, false});
                break;
            default:
                break;
            }
        }
    }
}

TileType GameMap::tile_at(const Position& pos) const {
    if (!in_bounds(pos)) {
        return TileType::Wall;
    }
    return tiles_[pos.y * width_ + pos.x];
}

bool GameMap::in_bounds(const Position& pos) const {
    return pos.x >= 0 && pos.y >= 0 && pos.x < width_ && pos.y < height_;
}

bool GameMap::is_walkable(const Position& pos) const {
    if (!in_bounds(pos)) {
        return false;
    }
    auto tile = tile_at(pos);
    return tile != TileType::Wall;
}

Game::Game() : playerPos_(map_.player_start()), monsterPos_(map_.monster_start()), rng_(static_cast<unsigned>(
                                std::chrono::steady_clock::now().time_since_epoch().count())) {
    const std::size_t cellCount = static_cast<std::size_t>(map_.width() * map_.height());
    visited_.assign(cellCount, false);
    push_log("Find and power every generator, unlock the gate, then reach the exit.");
    push_log("Toggle your lantern wisely—the dark feeds the monster.");
}

void Game::run() {
    render();
    while (!gameOver_) {
        std::cout << "\n> " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        auto result = handle_command(line);
        if (gameOver_) {
            break;
        }
        if (result == CommandResult::ConsumeTurn) {
            advance_turn();
        }
        if (!gameOver_) {
            render();
        }
    }
    if (playerWon_) {
        std::cout << "\nYou slip through the unlocked gate and the night air feels warm again.\n";
    } else if (gameOver_) {
        std::cout << "\nThe shadows claim you. Game over.\n";
    } else {
        std::cout << "\nYou left the mansion behind unfinished.\n";
    }
}

void Game::render() const {
    render_header();
    render_map();
    render_log();
}

void Game::render_header() const {
    std::cout << "\n=== Shadow Lantern ===\n";
    std::cout << "Turn: " << turn_ << "  Battery: " << battery_ << "%  Fear: " << fear_
              << "  Keys: " << keys_ << "  Spare batteries: " << spareBatteries_ << "\n";
    std::cout << "Generators: " << activatedGenerators_ << "/" << map_.generators().size()
              << "  Lantern: " << (lanternOn_ ? "ON" : "OFF")
              << "  Door: " << (doorLocked_ ? "locked" : "open") << "\n";
}

void Game::render_map() const {
    const int radius = lanternOn_ ? 4 : 2;
    for (int y = 0; y < map_.height(); ++y) {
        for (int x = 0; x < map_.width(); ++x) {
            Position pos{x, y};
            const int dist = manhattan(pos, playerPos_);
            const bool visible = dist <= radius;
            const std::size_t index = static_cast<std::size_t>(y * map_.width() + x);
            if (visible) {
                visited_[index] = true;
            }
            char glyph = ' ';
            if (pos == playerPos_) {
                glyph = 'P';
            } else if (pos == monsterPos_ && (visible || monsterRevealTimer_ > 0)) {
                glyph = 'M';
            } else {
                bool drawn = false;
                if (visible || visited_[index]) {
                    if (pos == map_.door_position()) {
                        glyph = doorLocked_ ? 'D' : 'd';
                        drawn = true;
                    }
                    if (!drawn && map_.tile_at(pos) == TileType::Wall) {
                        glyph = '#';
                        drawn = true;
                    }
                    if (!drawn && map_.tile_at(pos) == TileType::Exit) {
                        glyph = 'E';
                        drawn = true;
                    }
                    if (!drawn) {
                        for (const auto& generator : map_.generators()) {
                            if (generator.pos == pos && !generator.activated) {
                                glyph = 'G';
                                drawn = true;
                                break;
                            }
                        }
                    }
                    if (!drawn) {
                        for (const auto& item : map_.items()) {
                            if (item.pos == pos && !item.collected) {
                                glyph = draw_item_char(item.type);
                                drawn = true;
                                break;
                            }
                        }
                    }
                    if (!drawn) {
                        glyph = visible ? '.' : ':';
                    }
                }
            }
            std::cout << glyph;
        }
        std::cout << '\n';
    }
}

void Game::render_log() const {
    std::cout << "\n";
    for (const auto& entry : log_) {
        std::cout << "- " << entry << '\n';
    }
    std::cout << "Commands: move <n/s/e/w>, take, toggle, charge, scan, wait, inventory, help, quit\n";
}

void Game::push_log(const std::string& entry) {
    log_.push_back(entry);
    while (log_.size() > 5) {
        log_.pop_front();
    }
}

Game::CommandResult Game::handle_command(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    if (command.empty()) {
        return CommandResult::NoTurn;
    }
    command = lowercase(command);
    if (command == "move" || is_direction(command)) {
        std::string direction = command;
        if (command == "move") {
            if (!(iss >> direction)) {
                push_log("Specify a direction: north, south, east, or west.");
                return CommandResult::NoTurn;
            }
        }
        return handle_move(direction);
    }
    if (command == "go" || command == "walk") {
        std::string direction;
        if (!(iss >> direction)) {
            push_log("Where do you want to go?");
            return CommandResult::NoTurn;
        }
        return handle_move(direction);
    }
    if (command == "take" || command == "grab" || command == "activate") {
        return handle_take();
    }
    if (command == "toggle" || command == "lantern") {
        return handle_toggle();
    }
    if (command == "charge") {
        return handle_charge();
    }
    if (command == "scan" || command == "ping") {
        return handle_scan();
    }
    if (command == "wait" || command == "rest") {
        return handle_wait();
    }
    if (command == "inventory" || command == "inv") {
        push_log("Inventory -> keys: " + std::to_string(keys_) + ", spare batteries: " + std::to_string(spareBatteries_));
        return CommandResult::NoTurn;
    }
    if (command == "status") {
        push_log("You feel " + std::to_string(fear_) + "% afraid but still determined.");
        return CommandResult::NoTurn;
    }
    if (command == "help") {
        push_log("Find generators (G), collect keys (K) and batteries (B), unlock the door (D), reach the exit (E).");
        return CommandResult::NoTurn;
    }
    if (command == "quit" || command == "exit") {
        gameOver_ = true;
        playerWon_ = false;
        return CommandResult::NoTurn;
    }
    push_log("Unknown command.");
    return CommandResult::Invalid;
}

Game::CommandResult Game::handle_move(const std::string& direction) {
    if (!is_direction(lowercase(direction))) {
        push_log("That is not a direction.");
        return CommandResult::NoTurn;
    }
    const Position offset = direction_offset(direction);
    Position target{playerPos_.x + offset.x, playerPos_.y + offset.y};
    if (!map_.in_bounds(target)) {
        push_log("A collapsed wall blocks that way.");
        return CommandResult::NoTurn;
    }
    if (target == map_.door_position() && doorLocked_) {
        if (keys_ > 0) {
            --keys_;
            doorLocked_ = false;
            push_log("You unlock the heavy gate. It creaks open.");
            playerPos_ = target;
            return CommandResult::ConsumeTurn;
        }
        push_log("The gate is locked tight. You need a key.");
        return CommandResult::NoTurn;
    }
    if (!map_.is_walkable(target)) {
        push_log("You bump into crumbling stone.");
        return CommandResult::NoTurn;
    }
    playerPos_ = target;
    push_log("You move " + lowercase(direction) + ".");
    check_exit();
    return CommandResult::ConsumeTurn;
}

Game::CommandResult Game::handle_take() {
    bool foundSomething = false;
    for (auto& item : map_.items()) {
        if (!item.collected && item.pos == playerPos_) {
            foundSomething = true;
            item.collected = true;
            if (item.type == ItemType::Battery) {
                ++spareBatteries_;
                push_log("You pocket a fresh battery.");
            } else {
                ++keys_;
                push_log("You found an ornate key engraved with a lantern motif.");
            }
        }
    }
    for (auto& generator : map_.generators()) {
        if (!generator.activated && generator.pos == playerPos_) {
            generator.activated = true;
            ++activatedGenerators_;
            battery_ = std::min(100, battery_ + 20);
            push_log("The generator hums to life and feeds your lantern.");
            foundSomething = true;
        }
    }
    if (!foundSomething) {
        push_log("Your hands grasp at nothing of value here.");
        return CommandResult::NoTurn;
    }
    check_exit();
    return CommandResult::ConsumeTurn;
}

Game::CommandResult Game::handle_charge() {
    if (spareBatteries_ <= 0) {
        push_log("You have no spare batteries.");
        return CommandResult::NoTurn;
    }
    --spareBatteries_;
    battery_ = std::min(100, battery_ + 35);
    lanternOn_ = true;
    push_log("You swap in a new battery. The lantern burns bright.");
    return CommandResult::ConsumeTurn;
}

Game::CommandResult Game::handle_scan() {
    if (battery_ < 4) {
        push_log("The lantern sputters—there is not enough power to scan.");
        return CommandResult::NoTurn;
    }
    drain_battery(4, "You trigger a lantern pulse");
    int closestGenerator = 999;
    for (const auto& generator : map_.generators()) {
        if (!generator.activated) {
            closestGenerator = std::min(closestGenerator, manhattan(generator.pos, playerPos_));
        }
    }
    if (closestGenerator == 999) {
        push_log("All generators are humming already.");
    } else {
        push_log("A faint echo: the nearest generator is about " + std::to_string(closestGenerator) + " steps away.");
    }
    push_log("Monster signal locked. Stay alert.");
    monsterRevealTimer_ = 3;
    int monsterDistance = manhattan(monsterPos_, playerPos_);
    push_log("The monster is " + std::to_string(monsterDistance) + " steps from you.");
    return CommandResult::ConsumeTurn;
}

Game::CommandResult Game::handle_toggle() {
    lanternOn_ = !lanternOn_;
    push_log(lanternOn_ ? "You thumb the switch and light floods outward." : "You douse the lantern and step into shadow.");
    return CommandResult::ConsumeTurn;
}

Game::CommandResult Game::handle_wait() {
    push_log("You steady your breathing and listen.");
    return CommandResult::ConsumeTurn;
}

void Game::advance_turn() {
    ++turn_;
    if (monsterRevealTimer_ > 0) {
        --monsterRevealTimer_;
    }
    drain_battery(lanternOn_ ? 3 : 1, lanternOn_ ? "The lantern drinks power." : "Time passes in the dark.");
    update_monster();
    if (!gameOver_) {
        check_exit();
    }
}

void Game::drain_battery(int amount, const std::string& reason) {
    if (amount <= 0) {
        return;
    }
    const int prev = battery_;
    battery_ = std::max(0, battery_ - amount);
    if (battery_ != prev) {
        push_log(reason + " (-" + std::to_string(prev - battery_) + "%).");
    }
    if (battery_ == 0 && lanternOn_) {
        lanternOn_ = false;
        push_log("The light dies! Darkness engulfs you.");
        apply_fear(8, "the suffocating dark");
    }
}

void Game::apply_fear(int amount, const std::string& reason) {
    if (amount <= 0) return;
    fear_ = std::min(100, fear_ + amount);
    push_log("Fear rises (+" + std::to_string(amount) + "%) from " + reason + ".");
    if (fear_ >= 100) {
        gameOver_ = true;
        playerWon_ = false;
    }
}

bool Game::is_passable(const Position& pos) const {
    if (!map_.in_bounds(pos)) {
        return false;
    }
    if (pos == map_.door_position() && doorLocked_) {
        return false;
    }
    if (map_.tile_at(pos) == TileType::Wall) {
        return false;
    }
    return true;
}

std::vector<Position> Game::neighbours(const Position& pos) const {
    static const Position deltas[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::vector<Position> result;
    result.reserve(4);
    for (const auto& delta : deltas) {
        Position next{pos.x + delta.x, pos.y + delta.y};
        if (is_passable(next)) {
            result.push_back(next);
        }
    }
    return result;
}

bool Game::find_step_toward(const Position& goal, Position& outStep) const {
    std::queue<Position> frontier;
    std::unordered_map<Position, Position, PositionHash> cameFrom;
    frontier.push(monsterPos_);
    cameFrom[monsterPos_] = monsterPos_;

    bool found = false;
    while (!frontier.empty()) {
        Position current = frontier.front();
        frontier.pop();
        if (current == goal) {
            found = true;
            break;
        }
        for (const auto& next : neighbours(current)) {
            if (!cameFrom.count(next)) {
                cameFrom[next] = current;
                frontier.push(next);
            }
        }
    }
    if (!found) {
        return false;
    }
    Position current = goal;
    Position previous = goal;
    while (current != monsterPos_) {
        previous = current;
        current = cameFrom[current];
    }
    outStep = previous;
    return true;
}

int Game::manhattan(const Position& a, const Position& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

void Game::update_monster() {
    if (gameOver_) return;
    const int distance = manhattan(monsterPos_, playerPos_);
    Position nextStep = monsterPos_;
    bool chasing = false;
    if (distance <= 6) {
        Position candidate;
        if (find_step_toward(playerPos_, candidate)) {
            nextStep = candidate;
            chasing = true;
        }
    }
    if (!chasing) {
        auto options = neighbours(monsterPos_);
        if (!options.empty()) {
            std::uniform_int_distribution<std::size_t> dist(0, options.size() - 1);
            nextStep = options[dist(rng_)];
        }
    }
    monsterPos_ = nextStep;
    if (monsterPos_ == playerPos_) {
        gameOver_ = true;
        playerWon_ = false;
        push_log("The monster collides with you in the dark!");
        return;
    }
    if (!lanternOn_ && manhattan(monsterPos_, playerPos_) <= 1) {
        apply_fear(12, "the monster prowling nearby");
    } else if (chasing && lanternOn_) {
        push_log("You hear claws scraping as the creature hunts you.");
    }
}

void Game::check_exit() {
    if (playerPos_ == map_.exit_position()) {
        if (activatedGenerators_ == static_cast<int>(map_.generators().size()) && !doorLocked_) {
            gameOver_ = true;
            playerWon_ = true;
            push_log("The exit gate slides aside. Freedom!");
        } else {
            push_log("The exit is sealed. Power the generators and unlock the door first.");
        }
    }
}

} // namespace shadow
