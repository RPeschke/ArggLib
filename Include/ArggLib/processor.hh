#ifndef processor_h__
#define processor_h__
#include "ArggLib/reactive_programming/reactive_processor.hh"
#define  Pnew(x) x << new 
#define  gPnew gProcessHandler<0>() << new 
#define  gP_run()  gProcessHandler<0>().run()
#define  giPnew(i) gProcessHandler<i>() << new 
#define  giP_run(i)  gProcessHandler<i>().run()

namespace ArggLib {

  class processHandler {
  public:
    inline react_proc_return next() {
      react_proc_return ret = react_proc_return::success;
      for (auto& e: m_processors){
        ret = e();
        if (ret != react_proc_return::success) {
          break;
        }
      }
      return ret;
    }
    inline react_proc_return run() {
      react_proc_return ret = react_proc_return::success;
      while (ret == react_proc_return::success){
        ret = next();
      }
      return ret;
    }
    inline void push_back(reactive_fun_t pro) {
      m_processors.push_back(std::move(pro));
    }
    inline void clear() {
      m_processors.clear();
    }
  private:
    std::vector<reactive_fun_t> m_processors;
  };

  template <typename T>
  react_proc_return _next(T& pr) {
    return pr.next();
  }

  template <typename T>
  std::shared_ptr<T> operator<<(processHandler& proHandler, T* pr) {
    std::shared_ptr<T> ret(pr);
    proHandler.push_back([ret]() {
      return _next(*ret);
    });

    return ret;
  }

  template< int N>
  processHandler& gProcessHandler() {
    static  processHandler ret;
    return ret;
  }
}

#endif // processor_h__
