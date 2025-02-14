#include <game.h>
#include <thread>
#include <server.h>

#define NDEBUG

int main() {
    std::cout << "Host/Join/Local? (h/j/l): ";
    std::string y;
    std::getline(std::cin, y);

    if (y == "h" || y == "H")
    {
        // Run local server on seperate thread
        Server s{37549};
        s.run();

    } else if (y == "j" || y == "J") {
        std::cout << "Username: ";
        std::string uname;
        std::getline(std::cin, uname);
        Game::getInstance()->run(uname, "127.0.0.1", 37549);
    } else if (y== "l" || y == "L") {
        Server s{37549};
        std::thread serverThr{&Server::run, &s};

        // Start game and connect to local server
        std::this_thread::sleep_for((std::chrono::milliseconds)50);
        Game::getInstance()->run("main", "127.0.0.1", 37549);
    } else {
        std::cout << "Invalid response \"" << y << "\"" << std::endl;
    }
}