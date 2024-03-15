#include <iostream>
#include <set>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>
#include <sstream>

#include "ConcurrentQueue.hpp"

#ifdef DEBUG_LOG
const bool DEBUG = true;
#else
const bool DEBUG = false;
#endif

#define PROGRESS_BAR_WIDTH 150

using namespace FostyMath;

std::vector<uint64_t[2]> getChunks(int numThreads);
void checkNum(uint64_t);

std::mutex coutMutex;

int main(int argc, char ** argv)
{
    if (argc < 2)
        exit(-1);

    int numThreads = atoi(argv[1]);

    ConcurrentQueue<ConcurrentQueueMessage<uint64_t>> q;
    auto chunks = getChunks(numThreads);

    std::map<std::thread::id, uint64_t *> threadToChunk;
    for (auto chunk : chunks)
    {
        auto t = std::thread(
            [chunk, &q] {
                auto start = chunk[0];
                auto end = chunk[1];

                if (DEBUG)
                {
                    coutMutex.lock();
                    std::cout << std::this_thread::get_id() << " : " << start << " -> " << end << std::endl;
                    coutMutex.unlock();
                }

                for(uint64_t i = start; i <= end; i++)
                {                
                    checkNum(i); 
                    q.push({std::this_thread::get_id(), i});
                }
        });
        threadToChunk.insert({t.get_id(), chunk});
        t.detach();
    }

    double progress = 0.0;
    while (progress < 1.0)
    {
        if (q.empty())
            continue;

        auto message = q.pop();

        auto chunk = threadToChunk[message.tid];

        double totalNums = chunk[1] - chunk[0];
        double numFinished = message.message - chunk[0];

        progress += numFinished / totalNums;

        std::cout << "[";
        int pos = PROGRESS_BAR_WIDTH * progress;
        for (int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << float(progress * 100.0) << " %\r";
        std::cout.flush();
    }
}

std::vector<uint64_t[2]> getChunks(int numThreads)
{
    std::vector<uint64_t[2]> chunks = std::vector<uint64_t[2]>(numThreads);

    uint64_t max = std::numeric_limits<uint64_t>::max();
    uint64_t step = max / numThreads;

    for (int i = 0; i < numThreads; i++)
    {
        chunks[i][0] = (i * step) + 1;
        chunks[i][1] = chunks[i][0] + step;
    }

    return chunks;
}

void checkNum(uint64_t num)
{
    uint64_t orig = num;
    std::set<uint64_t> seen;
    while (num != 1)
    {
        seen.insert(num);
        if (num % 2 == 0)
        {
            num /= 2;
        }
        else 
        {
            num *= 3;
            num += 1;
        }

        if (seen.count(num) != 0)
        {
            coutMutex.lock();
            std::cout << std::this_thread::get_id() << " : " << "Loop detected for number: " << orig << std::endl;
            coutMutex.unlock();
            exit(1);
        }
    }
}
