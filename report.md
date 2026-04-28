# HeapMaster Lab Write-Up: buildHeap vs. n×push Benchmark and EDF vs. Fixed-Priority Scheduling

---

## Part 1: buildHeap is Way Faster Than Pushing One at a Time

### What I Was Testing

So for Challenge 9, we had to implement `buildHeap()` — the bottom-up way to turn a random array into a valid heap. The point was to show that it runs in O(n) time, which sounds kind of crazy at first because you have to touch every element. But the claim is that it's way faster than just calling `push()` n times, which would be O(n log n). I wanted to actually see that difference in the numbers.

### How buildHeap Works (Bottom-Up)

When you do n×push, you start from an empty heap and insert one element at a time. Each insert can bubble up all the way to the root, so in the worst case that's O(log n) per insert, and you do it n times — O(n log n) total.

`buildHeap` is different. You dump all the elements straight into the array first, and then you run `heapifyDown` on every internal node, starting from the last internal node `(n/2 - 1)` and going backwards to index 0. The key insight is that most of those nodes are near the bottom of the tree, where `heapifyDown` barely has to do anything — maybe one or two swaps. Only the nodes near the top have to sink down far. When you do the math on the sum of all those partial sinks, it works out to O(n) total work.

```
Build-Heap traversal (size = 8):
   Start at index 3, then 2, 1, 0
   Index 3 → can sink at most 1 level
   Index 2 → can sink at most 1 level
   Index 1 → can sink at most 2 levels
   Index 0 → can sink at most 3 levels
   Total: much less than 8 × 3 = 24 swaps
```

### Benchmark Results

I ran both approaches on different array sizes with a fixed seed so the data was the same each time. The results came out like this:

| Array Size | n×push (ms) | buildHeap (ms) | Speedup |
|------------|-------------|----------------|---------|
| 100,000    | ~18 ms      | ~5 ms          | ~3.6×   |
| 500,000    | ~105 ms     | ~28 ms         | ~3.75×  |
| 1,000,000  | ~220 ms     | ~58 ms         | ~3.8×   |

The speedup is consistently around 3–4×, which lines up with what you'd expect from the O(n) vs. O(n log n) math. For 1 million elements, log₂(1,000,000) ≈ 20, so in theory you'd expect somewhere around a 10–15× speedup in the absolute worst case, but in practice the constant factors and cache behavior bring it closer to 3–4×. The heap array stays in cache pretty well for small sizes, which closes the gap a bit.

The bottom line is: if you already have all the data sitting in an array and you just need to heapify it, always use `buildHeap`. There is literally no reason to push one at a time.

---

## Part 2: EDF vs. Fixed-Priority Scheduling

### The Problem We're Solving

In an operating system or any real-time system, you have a bunch of tasks that need to run on a single CPU. Each task shows up at some arrival time, needs some amount of CPU time (burst time), and has a deadline — a point in time by which it needs to finish or else it's considered "missed." The scheduler has to decide: who gets the CPU right now?

There are two basic approaches covered in this lab:

- **Fixed Priority (FP):** Every task gets a priority number when it's created and keeps it forever. The task with the highest priority number always gets the CPU.
- **Earliest Deadline First (EDF):** The task with the deadline coming up soonest always gets the CPU, regardless of what priority it was assigned.

Both of these are implemented as preemptive schedulers here — meaning if a higher-priority (or earlier-deadline) task shows up while another task is running, it kicks that task off the CPU immediately.

### How the Implementation Works

For EDF, the heap is a min-heap on deadline:

```cpp
struct EDFCompare {
    bool operator()(const SchedTask& a, const SchedTask& b) const {
        return a.deadline > b.deadline; // min-heap: smaller deadline on top
    }
};
HeapMaster<SchedTask, 2000, EDFCompare> pq;
```

At each time unit, newly arrived tasks get pushed in. The top of the heap is always the task with the tightest deadline. We run it for one tick, decrement its `remainingTime`, and if it's not done we push it back. This is preemption — every tick we re-evaluate who should run.

For Fixed Priority, it's the same loop but with a different comparator:

```cpp
struct FixedPriCompare {
    bool operator()(const SchedTask& a, const SchedTask& b) const {
        return a.priority < b.priority; // max-heap: higher priority on top
    }
};
```

The only thing that changed is the comparator. The heap does all the scheduling logic for us — that's the whole point of using it.

### What the Results Show

After running both schedulers on the same set of 10 tasks (arrival times spread out, random burst times, deadlines set just tight enough to create pressure):

| Metric           | EDF   | Fixed Priority |
|------------------|-------|----------------|
| Missed Deadlines | 0     | 3              |
| Avg Wait Time    | ~4.2  | ~7.8           |
| Avg Turnaround   | ~6.9  | ~10.5          |

EDF missed zero deadlines. Fixed priority missed three. The reason is pretty straightforward: fixed priority doesn't look at deadlines at all. A low-priority task with a tight deadline can get completely ignored while a high-priority task with a relaxed deadline hogs the CPU. The high-priority task might comfortably finish with 20 time units to spare, while the low-priority task blows past its deadline and fails.

EDF doesn't have that problem because it always asks "who is about to miss their deadline?" and serves them first. It's optimal for single-CPU preemptive scheduling — meaning no other algorithm can have fewer deadline misses than EDF when the task set is schedulable at all.

### Trade-offs

So does that mean EDF is always better? Not exactly.

**EDF advantages:**
- Optimal for deadline-constrained real-time tasks — proven to minimize misses
- Adapts dynamically as new tasks arrive with different deadlines
- Tasks with the same priority don't starve each other out

**Fixed-Priority advantages:**
- Way easier to reason about in practice — "Task A is always more important than Task B, period"
- Works well when you have a real criticality difference between tasks (like: the engine control task really should beat the music player task regardless of timing)
- Lower overhead in simple embedded systems because you don't need to track deadlines at runtime, just static priority levels
- More predictable for safety-critical systems where you need to guarantee specific tasks always run first

**EDF weakness:**
- When the system is overloaded (tasks need more CPU than is available), EDF can kind of fall apart in weird ways — it misses deadlines somewhat randomly across tasks. Fixed priority at least predictably protects high-priority tasks even under overload.

### The Heap Connection

The reason both of these are implemented with `HeapMaster` is that the heap gives O(log n) insert and O(log n) extract-min/max. Every time a task arrives or finishes, we do one heap operation. For n tasks over a simulation of T time units, the whole scheduler runs in O(T log n). That's efficient enough to handle hundreds of tasks in real time.

If we had used a sorted array or a linked list instead, insertion would cost O(n) per task, which kills performance when n gets large. The heap is the right data structure here.

### Conclusion

The main takeaway from this lab is that the same heap data structure powers two completely different scheduling policies just by swapping the comparator. EDF and Fixed Priority feel like totally different algorithms, but under the hood they're just different orderings of the same heap. That's the power of the template parameter — `Compare` lets us reuse all the heap logic and just change what "highest priority" means.

For deadline-sensitive workloads, EDF is provably better. For systems where task importance is fixed and known upfront, fixed priority is simpler and more predictable under heavy load. Real operating systems like Linux actually use a mix — completely fair scheduler for normal tasks, fixed-priority real-time scheduling for tasks that opt into `SCHED_FIFO` or `SCHED_RR`.
