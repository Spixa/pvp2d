#include "logger.h"

logger::Channel logger::log(std::string const& chan) {
    return logger::Channel(chan);
}