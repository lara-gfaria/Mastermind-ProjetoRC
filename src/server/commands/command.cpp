#include "command.hpp"

std::string UDPCommand::execute() {
    if (!this->check())
        return this->cmd_response + " ERR\n";
    return this->exec();
}

std::string TCPCommand::execute() {
    if (!this->check())
        return this->cmd_response + " ERR\n";
    return this->exec();
}

std::string Command::findPlayerInfo(std::string plid) { 

    std::string fileName = "GAME_" + plid + ".txt";

    try {
        for (const auto &entry : std::filesystem::directory_iterator(DB_GAMES_PATH)) {
            if (entry.path().filename().string() == fileName) {

                std::ifstream file((std::string)DB_GAMES_PATH + "/" + fileName);
                if (!file.is_open()) {
                    perror("Unable to open file.");
                    exit(1);
                }
                std::string line;
                std::getline(file, line);
                file.close();
                return line;
            }
        }
    } catch (const std::filesystem::filesystem_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }
    return "";
}

std::vector<std::string> Command::getPlayerTries(std::string plid) {

    std::ifstream file((std::string)DB_GAMES_PATH + "/GAME_" + plid + ".txt");
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::string> tries;
    while (std::getline(file, line)) {
        if (line[0] == 'T') {
            std::istringstream iss(line);
            std::vector<std::string> args;
            std::string arg;
            while (iss >> arg) {
                args.push_back(arg);
            }
            tries.push_back(args[1]);
        }
    }
    file.close();
    return tries;
}

void Command::logGameG(std::string code, std::time_t now, std::time_t init) {

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    std::string srcFileName = (std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt";
    std::ifstream src(srcFileName);

    std::filesystem::create_directories((std::string)DB_GAMES_PATH + "/" + this->plid);
    std::ofstream dst((std::string)DB_GAMES_PATH + "/" + this->plid + "/" + timestamp.str() + code + ".txt");

    if (!src.is_open() || !dst.is_open()) {
        std::perror("Error opening file(s)");
        exit(1);
    }

    std::string firstLine;
    std::string line;
    if (std::getline(src, firstLine)) {
        dst << firstLine << std::endl;
    }

    while (std::getline(src, line)) {
        dst << line << std::endl;
    }

    timestamp.str("");
    timestamp.clear();
    if (code == "T") {
        // last line is the timestamp of the timeout
        std::istringstream iss(firstLine);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) { args.push_back(arg); }
        time_t timeout = std::stoi(args[3]) + std::stoi(args[6]);
        timestamp << std::put_time(std::localtime(&timeout), "%d-%m-%Y %H:%M:%S");
        dst << timestamp.str() + " " + std::to_string(timeout - init) << std::endl;
    }
    else {
        timestamp << std::put_time(std::localtime(&now), "%d-%m-%Y %H:%M:%S");
        dst << timestamp.str() + " " + std::to_string(now - init) << std::endl;
    }

    src.close();
    dst.close();

    if (std::remove(srcFileName.c_str()) == ERROR) {
        std::perror("Error deleting file");
        exit(1);
    }
}
