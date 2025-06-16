#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <unistd.h>

using namespace boost::interprocess;

const char* kSharedMemPath = "/sample_point";

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
    std::atomic_bool data_ready;
    std::atomic_bool data_processed;
    int index;
};


void producer() {
    SharedMem<Payload> writer(kSharedMemPath);
    Payload& pw = writer.get();
    if (!pw.data_ready.is_lock_free()) {
        throw std::runtime_error("Timestamp is not lock-free");
    }
    for (int i = 0; i < 10; i++) {
        pw.data_processed.store(false);
        pw.index = i;
        pw.data_ready.store(true);
        while(!pw.data_processed.load());
    }
}

void consumer() {
    SharedMem<Payload> point_reader(kSharedMemPath, true);
    Payload& pr = point_reader.get();
    if (!pr.data_ready.is_lock_free()) {
        throw std::runtime_error("Timestamp is not lock-free");
    }
    for (int i = 0; i < 10; i++) {
        while(!pr.data_ready.load());
        pr.data_ready.store(false);
        std::cout << "Processing data chunk " << pr.index << std::endl;
        pr.data_processed.store(true);
    }
}

int main() {

    if (fork()) {
        consumer();
    } else {
        producer();
    }
}
