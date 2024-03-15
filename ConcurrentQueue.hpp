#include <mutex>
#include <queue>
#include <thread>

namespace FostyMath
{
    template<typename T>
    class ConcurrentQueue
    {
        private:
        std::queue<T> _q;
        std::mutex _qMutex;

        public:
        T pop()
        {
            _qMutex.lock();
            T val = _q.front();
            _q.pop();
            _qMutex.unlock();
            return val;
        }

        void push(T val)
        {
            _qMutex.lock();
            _q.push(val);
            _qMutex.unlock();
        }

        bool empty()
        {
            return _q.empty();
        }
    };

    template<typename T>
    class ConcurrentQueueMessage
    {
        public:
        std::thread::id tid;
        T message;

        ConcurrentQueueMessage(std::thread::id tid, T message) : tid(tid), message(message)
        {}
    };
}