# Rotating Triangle Interactions

## Project Overview
This project is designed to handle physically accurate interactions of rotating triangles using C++. It simulates the dynamics of triangular plates as they interact under rotation and torque, implementing realistic physics behavior.

## Key Features
- **Physical Accuracy**: Simulates the torque and rotation interactions among triangular plates.
- **Dynamic Interactions**: Supports dynamic collision and interaction between elements.
- **Visualization**: Provides real-time graphical representation of triangle interactions.

## Photos from Simulations
![Initial Interaction Setup](https://github.com/mihneaLovesLife/Rotating-triangle-interactions/blob/main/84A23FE2-20C6-47CD-A563-ABF723B84875.png)
![During Interaction](https://github.com/mihneaLovesLife/Rotating-triangle-interactions/blob/main/BDE7E170-17ED-44F2-B058-589D1FD66B77.png)

## Technical Details
The implementation involves calculating forces, torques, and movements of triangular plates, adjusting their positions and orientations based on physical laws. The system uses SFML for rendering the visual output of the simulations.

## How It Works
The main components include:
- `TrianglePlate`: Handles the properties and behaviors of each triangular plate.
- `Vector`: Manages vector operations essential for physical calculations.
- `InputManager`: Processes input for interactive simulations.

The simulation updates in real-time, responding to physical interactions among triangles which are visualized in a 2D space.

