# Simulation Platform

A high-fidelity simulation platform developed in C++ designed for defense-industry applications. The architecture decouples the physical universe from tactical guidance algorithms, ensuring modularity and rigorous compliance with numeric stability standards.

## 🚀 Key Features

* **RK4 Physics Engine:** Implements a 4th-order Runge-Kutta integration method to dynamically compute aerodynamic drag forces and gravity without accumulative Euler drift.
* **Tactical Guidance Core:** Utilizes a Pure Pursuit homing algorithm to calculate intercept trajectories against moving targets.
* **Kinematic Constraints:** Features strict angular velocity regulation (`Turn Rate Limiting`) to respect maximum physical G-force structures.

## 📊 Core Simulation Pipeline

1. **Guidance Phase:** Computes the line-of-sight (LOS) angle and limits the maximum turn rate per step ($dt$).
2. **Physics Phase (RK4):** Evaluates 4 distinct slope vectors ($k_1, k_2, k_3, k_4$) by sampling flight velocities and updating real-time aerodynamic drag before integrating final states.
3. **Target Phase:** Updates the kinematic vector of the aerial target.

## 🛠️ Tech Stack

* **Language:** C++17
* **Numerical Method:** 4th-order Runge-Kutta (RK4)
* **Target Environment:** Windows / Visual Studio (MSVC)

## 📈 Status
Core physics and guidance loop are fully operational. Next milestone includes integrating a **Qt-based Graphical User Interface (GUI)** for real-time telemetry and trajectory plotting.
