#pragma once

namespace server {

struct Chunk {
    int data[256]{0};
};

enum class Biome {
    Void = 0,
    Plains = 1,
};

// yet to be implemented
class ChunkBuilder {
public:
    static Chunk generate(Biome const& biome);
};

}