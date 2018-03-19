#include <iostream>
#include <vector>
#include <deque>

#include "src/utils.h"
#include "src/uid.h"
#include "src/order.h"
#include "src/buffer/buffer.h"
#include "src/allocator/allocator.h"
#include "src/container/container.h"
#include "src/entity/entity.h"


// COMPONENTS
struct position {
    float x, y;
};


// SYSTEMS
void update_position(lithe::entity& self, float x, float y) {
    auto& pos = self.get<position>();

    pos.x += x;
    pos.y += y;
}





















/*struct world {
    // holds systems, entities and components...

    void new_system(system sys);
    void rem_system();

    void update();

    Entity& new_entity();
    void rem_entity(entity&);
    void get_entity(int id);
};
*/








constexpr unsigned ENTITIES = 1000;





int main(int argc, const char* argv[]) {
    auto info      = lithe::get_info<position>();
    auto buff      = lithe::setup_buffer(info, ENTITIES);
    auto alloc     = lithe::setup_allocator(info, buff);
    auto container = lithe::setup_container(alloc);

    lithe::entity ent(0, container);

    ent.insert(position{3, 5});

    std::cout << ent.has<position>() << std::endl;

    ent.remove<position>();

    std::cout << ent.has<position>() << std::endl;

    return 0;
}