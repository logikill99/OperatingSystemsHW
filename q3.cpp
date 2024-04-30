#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

// Structure for the request
struct RequestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};

// Global variables
queue<RequestStructure> msg_queue;
mutex mtx;
condition_variable cv;
int request_counter = 0;
string webPages[10] = {"google.com", "yahoo.com", "stackoverflow.com", "github.com", "amazon.com", "ebay.com",
                       "wikipedia.org", "reddit.com", "linkedin.com", "twitter.com"};

// Function to simulate request handling
void listen() {
    while (true) {
        // Sleep for random duration between 1-3 seconds
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        // Create a request
        RequestStructure req;
        req.request_id = ++request_counter;
        req.ip_address = "";
        req.page_requested = webPages[rand() % 10];

        // Lock the mutex before modifying shared data
        unique_lock<mutex> lock(mtx);
        msg_queue.push(req);
        lock.unlock();

        // Notify one thread to consume the request
        cv.notify_one();
    }
}

// Function to handle requests
void do_request(int thread_id) {
    while (true) {
        // Lock the mutex before accessing shared data
        unique_lock<mutex> lock(mtx);

        // Wait if the queue is empty
        cv.wait(lock, [] { return !msg_queue.empty(); }); // macos g++ hates this

        // Get the request from the queue
        RequestStructure req = msg_queue.front();
        msg_queue.pop();

        // Unlock the mutex
        lock.unlock();

        // Display the message
        cout << "Thread " << thread_id << " completed request " << req.request_id << " requesting webpage "
             << req.page_requested << endl;
    }
}

int main() {
    // Create n threads for listening to requests
    const int num_listener_threads = 3; // Change this value as needed
    thread listener_threads[num_listener_threads];
    for (int i = 0; i < num_listener_threads; ++i) {
        listener_threads[i] = thread(listen);
    }

    // Create some threads to handle requests
    const int num_request_threads = 5;
    thread request_threads[num_request_threads];
    cout << num_request_threads;
    for (int i = 0; i < num_request_threads; ++i) {
        request_threads[i] = thread(do_request, i + 1); // this shit doesnt like to compile sometimes idk why
    }

    // Join the listening threads with the main thread
    for (int i = 0; i < num_listener_threads; ++i) {
        listener_threads[i].join();
    }

    // Join the request handling threads with the main thread
    for (int i = 0; i < num_request_threads; ++i) {
        request_threads[i].join();
    }

    return 0;
}