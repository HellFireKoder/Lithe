#ifndef LITHE_CONTAINER_H
#define LITHE_CONTAINER_H


#include <utility>
#include "../constants.h"
#include "../assert.h"
#include "../types.h"
#include "../uid.h"
#include "../translate_index.h"
#include "../allocator/allocator.h"
#include "../component/component.h"


namespace lithe {
    // Component interface.
    struct container {
        lithe::allocator* alloc;


        container(lithe::allocator* alloc_);


        // Attaches a component to a specified entity.
        template <typename T>
        void insert(lithe::entity_id entity, const T& item) {
            // This is very hacky but it does the trick.
            // With debug enabled you get more verbose
            // errors but incur come runtime cost.
            #ifdef LITHE_DEBUG_FLAG
                LITHE_ASSERT(
                    (std::is_base_of<lithe::component<T>, T>::value),

                    std::string{"Component ("}
                    + typeid(T).name()
                    + ") must be derived from `lithe::component`."
                );

            #else
                // Fast compile time checking.
                LITHE_STATIC_ASSERT(
                    (std::is_base_of<lithe::component<T>, T>::value),
                    "Component must be derived from `lithe::component`."
                );
            #endif

            alloc->insert(lithe::get_type_uid<T>(), entity, item);
        }


        template<typename T, typename... Ts>
        void insert(lithe::entity_id entity, const T& t, const Ts&&... ts) {
            insert(entity, t);
            insert<Ts...>(entity, ts...);
        }


        // Returns a reference to a component within an entity.
        template <typename T>
        T& get(lithe::entity_id entity) const {
            return alloc->get<T>(lithe::get_type_uid<T>(), entity);
        }


        // Removes a component from an entity.
        template <typename T>//, typename... Ts>
        void remove(lithe::entity_id entity) {
            alloc->remove<T>(lithe::get_type_uid<T>(), entity);
            alloc->zero<T>(lithe::get_type_uid<T>(), entity);

            alloc->remove<Ts...>(lithe::get_type_uid<Ts...>(), entity);
            alloc->zero<Ts...>(lithe::get_type_uid<Ts...>(), entity);
        }


        template <typename T>
        void swap_component(lithe::entity_id a, lithe::entity_id b) {
            lithe::component_id tmp = lithe::get_type_uid<T>();
            alloc->swap_component<T>(tmp, a, tmp, b);
        }


        void swap(lithe::entity_id a, lithe::entity_id b);
    };
}


#endif