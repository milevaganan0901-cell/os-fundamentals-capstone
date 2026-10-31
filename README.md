# OS Fundamentals Capstone

This repository contains the OS Fundamentals design note requested by the course.

## Files
- `Final-Report.md` — complete report with worked examples and conclusions.
- `01-CPU-Scheduling/scheduling.md` — FCFS and Round Robin examples.
- `02-Page-Replacement/page-replacement.md` — FIFO and LRU tables.
- `03-Synchronization/producer-consumer.c` — runnable bounded-buffer example using POSIX semaphores.
- `03-Synchronization/synchronization-notes.md` — semaphore explanation and wait order.
- `04-Thrashing-Analysis/diagnosis.md` — diagnosis and mitigation.
- `05-RAID-Comparison/raid-recommendation.md` — RAID comparison and recommendation.

## Run the C program
Open the VS Code terminal in this folder and run:

```bash
gcc -Wall -Wextra -O2 -pthread 03-Synchronization/producer-consumer.c -o producer-consumer
./producer-consumer
```

The output order can vary because producer and consumer threads execute concurrently.

## Publish to GitHub
1. Create a **public, empty** GitHub repository named `os-fundamentals-capstone`.
2. In the VS Code terminal, run these commands (replace `YOUR-USERNAME`):

```bash
git init
git add .
git commit -m "Complete OS fundamentals capstone"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/os-fundamentals-capstone.git
git push -u origin main
```

3. Open the GitHub repository in a private/incognito browser window to confirm it is public.
4. Copy the repository URL into TeEdu's **Your project link** field and submit.

**Important:** These are illustrative worked examples because the course page does not provide specific numeric inputs. Review the work, understand it, and replace the examples if your instructor supplied different inputs. Fill in your name, course, and date in `Final-Report.md`.
