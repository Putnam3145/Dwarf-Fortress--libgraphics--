#ifndef MAIL_H
#define MAIL_H
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <queue>

template<typename T>
class Chan {
  std::mutex mtx;
  std::condition_variable cv;
  std::queue<T> vals;
public:
  bool try_read(T &r) {
    std::unique_lock lk(mtx,std::try_to_lock);
    if (lk && !vals.empty()) {
      r=std::move(vals.front());
      vals.pop();
      return true;
    } else
    return false;
  }
  bool try_read_for_millis(T &r,uint32_t millis) {
    std::unique_lock lk(mtx);
    if((lk && !vals.empty()) || cv.wait_for(lk,std::chrono::milliseconds(millis))==std::cv_status::no_timeout)
    {
      r=std::move(vals.front());
      vals.pop();
      return true;
    } 
    else 
    return false;
  }
  void read(T &r) {
    std::unique_lock lk(mtx);
    if(vals.empty())
      cv.wait(lk);
    r = std::move(vals.front());
    vals.pop();
  }
  void write(const T &w) {
    std::unique_lock lk(mtx);
    vals.push(std::move(w));
    cv.notify_one();
  }
};

template<typename L, typename R>
struct Either {
  bool isL;
  union {
    L left;
    R right;
  };
  Either(const L &l) {
    isL = true;
    left = l;
  }
  Either(const R &r) {
    isL = false;
    right = r;
  }
};

#endif
