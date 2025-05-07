# 3D Spectrogram Renderer

This C++ project visualizes the frequency and magnitude of an audio signal over time using a 3D spectrogram. It leverages the **Short-Term Fourier Transform (STFT)** to extract time-varying frequency components from an audio file and renders them in a spatial 3D plot.

---

## 🎥 Demo

<div align="center">
  <video src="samples/sample-video.mp4" controls width="720"></video>
</div>

---

## 🖼 Screenshots

| Spectrogram Overview | Magnitude Detail | Frequency Profile |
|----------------------|------------------|-------------------|
| ![](samples/Screenshot%202025-05-04%20at%2014.46.33.png) | ![](samples/Screenshot%202025-05-04%20at%2014.48.56.png) | ![](samples/Screenshot%202025-05-04%20at%2014.50.23.png) |

| Interactive Camera | Surface Detail |
|--------------------|----------------|
| ![](samples/Screenshot%202025-05-04%20at%2015.49.50.png) | ![](samples/Screenshot%202025-05-04%20at%2015.49.59.png) |

---

## 🧠 Features

- Performs Short-Term Fourier Transform (STFT) on audio input
- Extracts time-frequency-magnitude data
- Renders 3D volume based on spectrogram values using a Sparse Voxel Octree
- Interactive camera for orbiting and zooming
- Customizable resolution and FFT parameters
- Custom renderer using Vulkan

---