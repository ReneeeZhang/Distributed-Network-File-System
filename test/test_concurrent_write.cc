/*
 * This testfile issue large concurrent write requests to target file, then
 * check whether the write operation is atomic.
 */

#include <fstream>
#include <future>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "threadpool.h"
#define ROUND_CNT 10
#define THREAD_CNT 32
#define CONSECUTIVE_CNT 1000
#define TARGET "../mountdir/write.txt"
using namespace std;

// Ensure the target file exists and empty.
void trunc_file() {
  if (access(TARGET, F_OK) != -1) {
    truncate(TARGET, 0);
  } else {
    fstream fs(TARGET, fstream::out);
    fs.close();
  }
}

void write_file(fstream& fs) {
  for (uint ii = 0; ii < ROUND_CNT; ++ii) {
    vector<char> buffer(CONSECUTIVE_CNT, '0' + ii);
    fs.write(buffer.data(), CONSECUTIVE_CNT);
  }
}

// Every thread write ROUND_CNT of CONSECUTIVE_CNT '0' ~ '9', and THREAD_CNT 
// threads in total.
void issue_concurrent_write() {
  fstream fs(TARGET, fstream::out);
  ThreadPool tp(THREAD_CNT);
  vector<future<void>> futs;
  for (uint ii = 0; ii < THREAD_CNT; ++ii) {
    futs.emplace_back(tp.execute(write_file, ref(fs)));
  }

  // Synchronize.
  for (auto& fut : futs) {
    fut.get();
  }
  fs.close();
}

void check_write_atomicity() {
  fstream fs(TARGET, fstream::in);
  vector<char> buffer(CONSECUTIVE_CNT);
  for (uint32 ii = 0; ii < THREAD_CNT; ++ii) {
    fs.read(buffer.data(), CONSECUTIVE_CNT);
    char expected = buffer[0];
    for (char c : buffer) {
      if (c != expected) {
        cerr << "Atomic write test failed" << endl;
        fs.close();
        return;
      }
    }
  }
  cout << "Atomic write test passed" << endl;
  fs.close();
}

int main() {
  issue_concurrent_write();
  cout << "Write completes" << endl;
  check_write_atomicity();
  return 0;
}
