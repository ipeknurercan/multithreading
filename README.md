# Multithreading in C: Data vs Task Parallelism

This project demonstrates two common multithreading techniques in C using **POSIX Threads (pthreads)**:  
🔹 **Data Parallelism**  
🔹 **Task Parallelism**

These techniques are essential for optimizing performance in CPU-bound programs, especially in **Linux-based systems** where pthreads are natively supported and widely used in kernel modules, servers, and parallel computation.

---

## 🧠 What is Multithreading?

Multithreading allows a program to execute multiple parts of a task concurrently by dividing it into threads. This helps:
- Improve performance on multi-core CPUs
- Increase responsiveness
- Utilize system resources more efficiently

---

## 🧵 Techniques Used

### 1. Data Parallelism

In **data parallelism**, the same operation is performed in parallel on separate chunks of a data set.

📁 `data_parallelism.c`:  
- The input array is divided among threads  
- Each thread computes partial results (e.g., sum, square, etc.)  
- Results are combined at the end  

🧪 **Example use case:** Summing 1 million integers by splitting work among 4 threads.

---

### 2. Task Parallelism

In **task parallelism**, different threads perform different operations, possibly on the same or related data.

📁 `task_parallelism.c`:  
- Each thread performs a distinct task (e.g., read, process, write)  
- Useful in pipeline-style programs or multitasking applications  

🧪 **Example use case:** One thread reads data, another filters, another stores.

---

## 🐧 Why It Matters in Linux?

- Linux relies heavily on `pthreads` for multithreaded kernel modules and user-space tools  
- Critical in embedded systems, network daemons, parallel builds (make -j)  
- Understanding threading is essential for systems-level C programming

---

## ⚙️ How to Compile and Run

### Compile:

```bash
gcc -pthread data_parallelism.c -o data_parallelism
gcc -pthread task_parallelism.c -o task_parallelism
