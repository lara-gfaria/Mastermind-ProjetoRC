#include "debug.hpp"

bool DebugCommand::check() {
    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    if (std::find(colors.begin(), colors.end(), this->C1) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C2) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C3) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C4) == colors.end()) {
        return false;
    }
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    if (!std::all_of(this->max_playtime.begin(), this->max_playtime.end(), ::isdigit) ||
        std::stoi(this->max_playtime) < 1 || std::stoi(this->max_playtime) > GAME_TIMEOUT) {
        return false;
    }
    return true;
}

std::string DebugCommand::exec() {

    // playerInfo: plid mode colors time date hour timestamp
    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    // active game
    if (playerInfo != "") {
        std::istringstream iss(playerInfo);
        std::string arg;
        std::vector<std::string> args;
        while (iss >> arg) { args.push_back(arg); }

        time_t now = std::time(nullptr);
        // game hasn't timed out but has tries
        if (now - std::stoi(args[6]) <= std::stoi(args[3])) {
            if (getPlayerTries(this->plid).size() > 0)
                return "RDB NOK\n";
        }
        else { // game timeout - log game and make new one
            this->logGameG("T", now, std::stoi(args[6]));
        }
    }

    std::ofstream file((std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }

    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime = *std::localtime(&currentTime);
    std::ostringstream timeStream;
    timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    file << this->plid + " D " << this->C1 + this->C2 + this->C3 + this->C4 + " " + this->max_playtime + 
        " " + timeStream.str() + " " + std::to_string(std::time(nullptr)) << std::endl;

    file.close();

    return "RDB OK\n";
}