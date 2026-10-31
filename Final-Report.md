# OS Fundamentals Design Note

**Student:** Mile Vaganan V
**Course:** Operating Systems  
**Date:** 31 october 2026

## 1. CPU Scheduling: Gantt-Chart Problems

The following illustrative process set is used because the course brief does not specify numerical inputs.

| Process | Arrival time (AT) | Burst time (BT) |
|---|---:|---:|
| P1 | 0 | 5 |
| P2 | 1 | 3 |
| P3 | 2 | 8 |
| P4 | 3 | 6 |

Completion time (CT) is when a process finishes. Turnaround time is `TAT = CT − AT`. Waiting time is `WT = TAT − BT`.

### First-Come, First-Served (FCFS)

FCFS is non-preemptive. Processes execute in arrival order.

```text
0       5       8               16              22
|  P1   |  P2   |      P3       |      P4       |
```

| Process | AT | BT | CT | TAT | WT |
|---|---:|---:|---:|---:|---:|
| P1 | 0 | 5 | 5 | 5 | 0 |
| P2 | 1 | 3 | 8 | 7 | 4 |
| P3 | 2 | 8 | 16 | 14 | 6 |
| P4 | 3 | 6 | 22 | 19 | 13 |
| **Average** | | | | **11.25** | **5.75** |

For P3, `TAT = 16 − 2 = 14` and `WT = 14 − 8 = 6`.

### Round Robin (RR), time quantum = 2

Each ready process receives up to two CPU time units. An unfinished process returns to the back of the ready queue. If a new process arrives exactly when a time slice ends, this example enqueues the arrival before re-queuing the preempted process.

```text
0   2   4   6   8   10  11  13  14  16  18  20  22
|P1 |P2 |P3 |P1 |P4 |P2 |P3 |P1 |P4 |P3 |P4 |P3 |
```

| Process | AT | BT | CT | TAT | WT |
|---|---:|---:|---:|---:|---:|
| P1 | 0 | 5 | 14 | 14 | 9 |
| P2 | 1 | 3 | 11 | 10 | 7 |
| P3 | 2 | 8 | 22 | 20 | 12 |
| P4 | 3 | 6 | 20 | 17 | 11 |
| **Average** | | | | **15.25** | **9.75** |

**Comparison:** FCFS has lower average waiting and turnaround time for this particular input. RR is still valuable for interactive systems because it shares CPU time among ready processes and improves responsiveness. A very small quantum increases context-switch overhead; a very large quantum makes RR behave more like FCFS.

## 2. Page Replacement: FIFO and LRU

Use three page frames and the reference string:

`7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2`

A **fault** occurs when a referenced page is not in memory. A **hit** occurs when it is already present.

### FIFO

FIFO replaces the page that entered memory earliest.

| Step | Page | Frame 1 | Frame 2 | Frame 3 | Result |
|---:|---:|---:|---:|---:|---|
| 1 | 7 | 7 | – | – | Fault |
| 2 | 0 | 7 | 0 | – | Fault |
| 3 | 1 | 7 | 0 | 1 | Fault |
| 4 | 2 | 2 | 0 | 1 | Fault |
| 5 | 0 | 2 | 0 | 1 | Hit |
| 6 | 3 | 2 | 3 | 1 | Fault |
| 7 | 0 | 2 | 3 | 0 | Fault |
| 8 | 4 | 4 | 3 | 0 | Fault |
| 9 | 2 | 4 | 2 | 0 | Fault |
| 10 | 3 | 4 | 2 | 3 | Fault |
| 11 | 0 | 0 | 2 | 3 | Fault |
| 12 | 3 | 0 | 2 | 3 | Hit |
| 13 | 2 | 0 | 2 | 3 | Hit |

**FIFO result: 10 page faults and 3 hits.**

### LRU

LRU replaces the page that has not been used for the longest time.

| Step | Page | Frame 1 | Frame 2 | Frame 3 | Result |
|---:|---:|---:|---:|---:|---|
| 1 | 7 | 7 | – | – | Fault |
| 2 | 0 | 7 | 0 | – | Fault |
| 3 | 1 | 7 | 0 | 1 | Fault |
| 4 | 2 | 2 | 0 | 1 | Fault |
| 5 | 0 | 2 | 0 | 1 | Hit |
| 6 | 3 | 2 | 0 | 3 | Fault |
| 7 | 0 | 2 | 0 | 3 | Hit |
| 8 | 4 | 4 | 0 | 3 | Fault |
| 9 | 2 | 4 | 0 | 2 | Fault |
| 10 | 3 | 4 | 3 | 2 | Fault |
| 11 | 0 | 0 | 3 | 2 | Fault |
| 12 | 3 | 0 | 3 | 2 | Hit |
| 13 | 2 | 0 | 3 | 2 | Hit |

**LRU result: 9 page faults and 4 hits.**

| Algorithm | Faults | Hits |
|---|---:|---:|
| FIFO | 10 | 3 |
| LRU | 9 | 4 |

LRU performs one fewer fault for this reference string because it considers recent use. FIFO is simpler, but may remove a frequently used page simply because it arrived earliest. This example does not prove LRU always performs better; results depend on the reference string.

## 3. Bounded Buffer Using Semaphores

A producer inserts items into a fixed-size shared buffer, while a consumer removes them. The design must prevent buffer overflow, buffer underflow, and race conditions.

For a buffer of size `N`, use:

| Semaphore | Initial value | Purpose |
|---|---:|---|
| `empty_slots` | N | Counts free buffer positions |
| `full_slots` | 0 | Counts occupied positions |
| `mutex` | 1 | Protects the shared buffer and indexes |

### Correct producer order

1. `wait(empty_slots)` — reserve a free slot.
2. `wait(mutex)` — enter the critical section.
3. Insert the item and advance the circular-buffer index.
4. `signal(mutex)` — leave the critical section.
5. `signal(full_slots)` — announce an available item.

### Correct consumer order

1. `wait(full_slots)` — reserve an available item.
2. `wait(mutex)` — enter the critical section.
3. Remove the item and advance the index.
4. `signal(mutex)` — leave the critical section.
5. `signal(empty_slots)` — announce a free slot.

Waiting on the counting semaphore before acquiring the mutex is important: a thread should not hold the mutex while blocked waiting for a slot or item. The mutex protects only the short operation that changes shared state.

The accompanying `03-Synchronization/producer-consumer.c` implements a circular buffer with five slots, one producer, one consumer, and ten data items. It uses a sentinel value to signal that production is complete. Compile and run it using the commands in `README.md`. The exact print order can vary because the threads execute concurrently.

The counting semaphores prevent overflow and underflow. The mutex ensures that only one thread at a time changes the buffer or its indexes. Each produced data item should be consumed exactly once.

## 4. Diagnosing Thrashing on a Busy Server

Thrashing occurs when the operating system spends excessive time moving pages between RAM and secondary storage instead of executing useful application work. It can occur when the combined active working sets of processes exceed available physical memory.

### Symptoms and evidence

- Sustained high **major page-fault** rates.
- Heavy swap/page-file reads and writes.
- Low available memory and memory pressure.
- High application response times and queue lengths.
- Falling throughput; CPU utilization may fall while processes wait for pages.

No single metric proves thrashing. High disk utilization can be normal database activity, and minor page faults are not necessarily harmful. Correlate paging activity with memory pressure, latency, and throughput.

### Diagnosis procedure

1. Record a normal baseline for CPU, memory, swap, disk latency, page faults, and application throughput.
2. During the slowdown, sample the same metrics over the same time window.
3. Identify processes with large resident memory, rapidly changing working sets, or high major-fault rates.
4. Check whether the total active working set exceeds RAM available to applications after reserving memory for the OS and cache.
5. Correlate page-in/page-out activity with application latency to confirm paging is a cause, not just coincident disk traffic.
6. Check recent deployments, increased worker counts, memory leaks, larger datasets, or reduced VM/container memory limits.

### Mitigation

- Reduce excessive concurrency or limit memory-heavy workers.
- Find and fix memory leaks.
- Reduce per-request memory use, batch sizes, or oversized caches.
- Add RAM or move workloads to a host with enough memory when demand is legitimate.
- Use admission control and workload isolation to prevent one service from exhausting host memory.
- Monitor page faults, swap I/O, memory pressure, latency, and throughput after changes.

Adding swap may prevent immediate out-of-memory termination, but it does not replace adequate RAM. If the active working set remains larger than physical memory, thrashing may continue.

## 5. RAID Comparison and Recommendation

Assume a small lab cluster hosts virtual machines, student projects, shared datasets, and services. It needs good mixed read/write performance and protection against a drive failure.

| RAID | Minimum drives | Approx. usable capacity with equal-size drives | Drive-failure tolerance | Main trade-off |
|---|---:|---|---|---|
| RAID 0 | 2 | N × drive size | None | Fast and full capacity, but any drive failure loses the array |
| RAID 1 | 2 | Half of raw capacity for a two-way mirror | One drive in a mirror | Simple and resilient, but uses half the capacity |
| RAID 5 | 3 | (N−1) × drive size | One drive | Good capacity efficiency; parity writes and rebuild risk |
| RAID 6 | 4 | (N−2) × drive size | Two drives | Better failure tolerance, with more parity overhead |
| RAID 10 | 4 | About half of raw capacity | At least one; two only if they are in different mirror pairs | Strong random I/O and simpler/faster rebuilds, but 50% capacity efficiency |

### Recommendation: RAID 10 for the primary VM/data volume

For a small lab cluster with virtual machines and mixed random reads/writes, RAID 10 is a strong choice if at least four drives and the budget are available. It stripes data across mirrored pairs, provides good random I/O performance, and rebuilds a failed mirror member without calculating parity across every disk.

For example, four equal 2 TB drives in RAID 10 provide approximately 4 TB of decimal usable capacity before filesystem overhead. The array can survive one drive failure. It can survive two only if the failed drives belong to different mirror pairs.

If capacity and budget matter more than write performance, RAID 6 is a reasonable alternative, particularly with more drives. RAID 5 may be suitable for less write-intensive, non-critical capacity pools.

**Important limitation:** RAID is not a backup. It does not protect against accidental deletion, malware, theft, fire, controller failure, or every kind of data corruption. Maintain separate, tested backups.

## Overall Conclusion

This design note demonstrates how OS mechanisms affect performance, correctness, and reliability. FCFS and Round Robin make different scheduling trade-offs; FIFO and LRU produce different page-fault counts; semaphores coordinate safe access to a bounded buffer; memory-pressure metrics help identify thrashing; and RAID 10 is a practical performance-oriented choice for the assumed lab workload. The best design depends on actual workload, hardware, and service requirements.
