#pragma once
#include "entity.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace common::ecs {

template <typename Component>
class ComponentStorage {
public:
    Component& emplace(Entity e, const Component& value = Component{}) {
        return storage_[e] = value;
    }

    bool has(Entity e) const { return storage_.find(e) != storage_.end(); }

    Component* get(Entity e) {
        auto it = storage_.find(e);
        return it != storage_.end() ? &it->second : nullptr;
    }

    const Component* get(Entity e) const {
        auto it = storage_.find(e);
        return it != storage_.end() ? &it->second : nullptr;
    }

    void remove(Entity e) { storage_.erase(e); }

    template <typename Fn>
    void for_each(Fn&& fn) {
        for (auto& [entity, component] : storage_) {
            fn(entity, component);
        }
    }

private:
    std::unordered_map<Entity, Component> storage_{};
};

class Registry {
public:
    Entity create() {
        return ++lastEntity_;
    }

    void destroy(Entity e) {
        for (auto& remover : removers_) {
            remover(e);
        }
    }

    template <typename Component>
    ComponentStorage<Component>& storage() {
        const auto typeId = type_id<Component>();
        if (typeId >= storages_.size()) {
            storages_.resize(typeId + 1);
        }
        if (!storages_[typeId]) {
            storages_[typeId] = std::make_unique<StorageHolder<Component>>();
            removers_.push_back([this](Entity entity) {
                this->storage<Component>().remove(entity);
            });
        }
        return static_cast<StorageHolder<Component>*>(storages_[typeId].get())->storage;
    }

private:
    template <typename Component>
    static std::size_t type_id() {
        static std::size_t id = counter_++;
        return id;
    }

    struct StorageHolderBase {
        virtual ~StorageHolderBase() = default;
    };

    template <typename Component>
    struct StorageHolder : StorageHolderBase {
        ComponentStorage<Component> storage;
    };

    std::vector<std::unique_ptr<StorageHolderBase>> storages_{};
    std::vector<std::function<void(Entity)>> removers_{};
    Entity lastEntity_{0};
    inline static std::size_t counter_{0};
};

} // namespace common::ecs
