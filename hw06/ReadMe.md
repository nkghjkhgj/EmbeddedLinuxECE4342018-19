1. She works for National Instruments.
2. PREEMT_RT is a package which makes linux kernel to a real-time system.
3. Mixed criticality is to mix real-time and non-real-time tasks together to get higher performance.
4. Because the driver stacks are shared between RT-tasks and non-RT tasks.
5. Latency.
6. It's to measure the difference between a thread's intended wake up time and practical wake up time to measure latency.
7. It's a historgram latency samples of rt and non-rt systems.
8. Dispatch latency: the amount of time that it takes between the hardware is firing to the actual wake-up. Scheduling latency: time it takes between the scheduler has been awared that some task needs to be run and it actually tells CPU to run it.
9. Mainline is the thread being run before any interrupts are detected.
10. Because it needs to wait for the current thread to be finished first without real-time patch.

========================
Professor Yoder's Comments

Score:  10/10

Try searching "linux mainline kernel"