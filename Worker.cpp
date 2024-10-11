#include "Worker.h"

void Worker::processTask(const std::string& task) {
    std::cout << "Processing task: " << task << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Finished processing task: " << task << std::endl;
}
