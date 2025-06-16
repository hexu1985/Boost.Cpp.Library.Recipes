#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <unistd.h>

const char* kSharedMemPath = "/sample_point";
const size_t kPayloadSize = 16;

using namespace std::literals;
using namespace boost::interprocess;

template<class T>
class SharedMem {
    shared_memory_object shdmem;
    mapped_region region;
    T* ptr;
    const char* name;

public:
    SharedMem(const char* name, bool owner=false) {
        permissions perms(0600);
        shdmem = shared_memory_object{open_or_create, name, read_write, perms};
        shdmem.truncate(sizeof(T));

        region = mapped_region{shdmem, read_write};
        ptr = static_cast<T*>(region.get_address());
        this->name = owner ? name : nullptr;
        std::cout << "Opened shared mem instance " << name << std::endl;
    }

    ~SharedMem() {
        if (name) {
            std::cout << "Remove shared mem instance " << name << std::endl;
            shared_memory_object::remove(name);
        }
    }

    T& get() const {
        return *ptr;
    }
};


struct Payload {
    uint32_t index;
    uint8_t raw[kPayloadSize];
};


void producer() {
    SharedMem<Payload> writer(kSharedMemPath);
    Payload& pw = writer.get();
    for (int i = 0; i < 5; i++) {
        pw.index = i;
        std::fill_n(pw.raw, sizeof(pw.raw) - 1, 'a' + i);
        pw.raw[sizeof(pw.raw) - 1] = '\0';
        std::this_thread::sleep_for(150ms);
    }
}

void consumer() {
    SharedMem<Payload> point_reader(kSharedMemPath, true);
    Payload& pr = point_reader.get();
    for (int i = 0; i < 10; i++) {
        std::cout << "Read data frame " << pr.index << ": " << pr.raw << std::endl;
        std::this_thread::sleep_for(100ms);
    }
}

int main() {

    if (fork()) {
        consumer();
    } else {
        producer();
    }
}
