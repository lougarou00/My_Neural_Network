#ifndef DLLOADER_HPP
#define DLLOADER_HPP

#include "librairies.hpp"

template <typename T>
class DLLoader {
private:
    void* handle = nullptr;
    T* instance = nullptr;

public:
    explicit DLLoader(const std::string& libraryPath) {
        handle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle) {
            std::cout << "dlopen failed for " << libraryPath << ": " << dlerror() << std::endl;
            exit(1);
        }

        dlerror(); // clear
        using CreateInstanceFunc = T* (*)();
        auto createInstance =
            reinterpret_cast<CreateInstanceFunc>(dlsym(handle, "createInstance"));

        const char* error = dlerror();
        if (error) {
            std::cout << "dlopen failed for " << libraryPath << ": " << dlerror() << std::endl;
            dlclose(handle);
            throw std::runtime_error(error);
        }

        instance = createInstance();
        if (!instance) {
            std::cout << "dlopen failed for " << libraryPath << ": " << dlerror() << std::endl;
            dlclose(handle);
            throw std::runtime_error("createInstance returned null");
        }
    }

    ~DLLoader() noexcept {
        if (handle) {
            dlerror(); // clear
            using DestroyInstanceFunc = void (*)(T*);
            auto destroyInstance =
                reinterpret_cast<DestroyInstanceFunc>(dlsym(handle, "destroyInstance"));

            if (!dlerror() && destroyInstance && instance) {
                destroyInstance(instance);
            }

            dlclose(handle);
        }
    }

    // Non copyable
    DLLoader(const DLLoader&) = delete;
    DLLoader& operator=(const DLLoader&) = delete;

    // Movable
    DLLoader(DLLoader&& other) noexcept
        : handle(other.handle), instance(other.instance)
    {
        other.handle = nullptr;
        other.instance = nullptr;
    }

    T* getInstance() const noexcept {
        return instance;
    }
};

#endif
