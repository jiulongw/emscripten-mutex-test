#include <iostream>
#include <mutex>
#include <thread>

#include <emscripten/threading.h>

std::recursive_mutex g_mutex;
std::mutex g_log;

void log(std::string msg) {
  std::scoped_lock lk(g_log);
  std::cout << "[" << std::this_thread::get_id() << "] " << msg << std::endl;
}

void run_in_main_thread() {
  log("Enter run_in_main_thread.");

  {
    log("accquiring lock...");
    std::scoped_lock lk(g_mutex);
    log("lock accquired.");
  }
  log("lock released.");
}

void worker_proc() {
  log("Enter worker_proc.");

  emscripten_async_run_in_main_runtime_thread(EM_FUNC_SIG_V,
                                              &run_in_main_thread);
}

int main() {
  log("Enter main.");
  std::thread worker(&worker_proc);

  std::chrono::milliseconds sleep_time(10);
  {
    log("accquiring lock...");
    std::scoped_lock lk(g_mutex);
    log("lock accquired.");

    std::this_thread::sleep_for(sleep_time);
  }
  log("lock released.");

  worker.join();
  return 0;
}
