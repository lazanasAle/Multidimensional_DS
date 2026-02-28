# Multidimensional Data Structures & Computational Geometry Project

This repository contains the implementation of the academic project/assignement on the course **multidimensional data structures** and **computational geometry**.
The project has a strong focus on efficient querying, geometric algorithms, and performance evaluation on large-scale datasets.

It includes implementations of classic and advanced data structures, geometric algorithms, and a custom-designed hybrid structure, all tested on either **real-world** or **synthetic** datasets (depending on part of the project).

---

## Features & Implementations

### 1. 3D R-Tree for Airplane Trajectory Data
- Implementation of a **3-dimensional R-tree**
- Designed to index and query **airplane trajectory data**
- Supports efficient spatial range queries in 3D space
- Integrated with a **user-friendly TUI** for interactive querying

---

### 2. Interval Tree & Segment Tree
- Full implementations of:
  - **Interval Tree**
  - **Segment Tree**
- Efficient handling of interval-based queries
- Used as standalone structures for comparative evaluation

---

### 3. Convex Hull & Skyline Algorithms
- Implementation of:
  - **Convex Hull algorithm** (Graham-Scan)
  - **Skyline queries** in multidimensional space (using a kd-b-tree for efficient indexing)
---

### 4. Bentley–Ottmann Sweep Line Algorithm
- Implementation of the **Bentley–Ottmann algorithm**
- Efficient detection of line segment intersections
- Designed for large input sizes

---

## A custom **KD-B-Tree** was also built in this project, which:
  - Acts as an auxiliary structure for skyline computation
  - Serves as a **superclass of the R-tree**
  - It is implemented on the secondary memory (disk-based) and supports LRU caching mechanism

---

## Performance Evaluation

- All data structures and algorithms were evaluated using:
  - **Large real-world datasets** (convex-hull, skyline, rtree)
  - **Synthetic datasets** (interval-tree, segment-tree, sweep-line)
- Performance metrics include:
  - Execution time
  - Scalability with respect to dataset size
  - Query efficiency
- Results demonstrate the effectiveness of the chosen data structures for high-dimensional and geometric workloads

---

## Interactive Terminal User Interface (TUI)

The following components provide an interactive **Terminal User Interface**:
- **Skyline Program**
- **3D R-Tree**

### TUI Features
- Dynamic selection of **dimensions** for queries
- Interactive user-defined Query input.

---

## Datasets

- CSV datasets included in the repository (compressed as ZIP files)
- Mix of:
  - Real-world data (e.g., 19k first airplane trajectories (from 900k total, the rest are synthetic), 900k movies dataset)
  - Synthetic data for benchmarking

---

## Usage and Setup

- To use  you can clone this repo execute the script init.sh (or run the top-level Makefile unzip the datasets wherever needed) and test the executables.
  
