# Automatic Defect Detection System for Parts Using Computer Vision

## Project Overview

Development and testing of an automatic external defect detection system for mechanical parts to enhance production quality control efficiency. The system employs advanced computer vision algorithms to analyze part images and compare them with reference templates.

## Key Features

- **Automatic Image Alignment** using SIFT algorithm for precise template matching
- **Defect Detection** through difference analysis with reference images
- **Defect Classification** according to GOST standards for various materials
- **Comprehensive Reporting** with defect visualization and detailed descriptions
- **Multi-material Support**: steel, aluminum, titanium, magnesium

## Technology Stack

- **Programming Language**: C++ (C++11/14/17 standard)
- **Core Libraries**:
  - OpenCV (computer vision algorithms)
  - Standard C++ Library

## System Architecture

### Program Flowchart
![Program Flowchart](https://github.com/user-attachments/assets/525e4831-df4d-4974-adfb-1abded8146fd)

### Processing Pipeline:
1. **Data Input**: Material type selection and image loading
2. **Preprocessing**: Alignment of defect image with reference template
3. **Defect Detection**: Image differencing and thresholding
4. **Classification**: Defect type and severity determination per GOST standards
5. **Output Generation**: Report creation and visualization

## Testing & Validation

The system has been rigorously tested on:
- 3D models created in Blender
- Physical prototypes produced via 3D printing
- Conveyor systems capable of generating 6 part projections in 4 seconds

## System Advantages

- **High Accuracy** in detecting contour and surface defects
- **Reduction** of defective products in production
- **Accelerated** quality control process
- **Elimination** of subjective human factor

## Program Example
![Program Example](https://github.com/user-attachments/assets/8d293481-4333-4279-b53d-76e16a5d1aa3)

## Part Scanner Design
![Part Scanner](https://github.com/user-attachments/assets/8cd31f12-b303-40ef-a176-a02eb0886048)

## Implemented Algorithms

1. **SIFT (Scale-Invariant Feature Transform)** - Feature detection and matching
2. **FLANN-based Matcher** - Descriptor matching
3. **RANSAC** - Homography estimation and outlier rejection
4. **Morphological Operations** - Binary mask refinement
5. **Contour Analysis** - Defect extraction and classification

## Supported GOST Standards

### Steel:
- GOST 23901-88 (area > 1000 pixels)
- GOST 15975-85 (pore size > 5 pixels)

### Aluminum:
- GOST R 53442-2009 (area > 800 pixels)

### Titanium:
- GOST R 53450-2009 (area > 600 pixels, size > 8 pixels)

### Magnesium:
- GOST R 54815-2011 (area > 400 pixels)
- GOST 23974-2017 (size > 6 pixels)

## Project Structure

```
project/
├── 1 деталь/              # Input directory with images
│   ├── part_1/
│   │   ├── эталонное.png  # reference.png (template)
│   │   └── дефектное.png  # defective.png
│   └── part_2/
│       ├── reference.png
│       └── defective.png
├── output/                # Output directory (auto-generated)
│   ├── part_1/
│   │   ├── detected_defects/
│   │   ├── gost_report.txt
│   │   ├── aligned_defect.png
│   │   ├── defect_mask.png
│   │   └── visualization.png
│   └── part_2/
└── main.cpp              # Main program file
```

## Installation & Setup

### Prerequisites

- C++ compiler with C++11 or higher support
- OpenCV library (version 4.x recommended)
- Operating System: Windows/Linux/macOS

### Building from Source

1. **Install OpenCV** and configure your development environment
2. **Compile the program**:
   ```bash
   g++ -o defect_detector main.cpp `pkg-config --cflags --libs opencv4` -std=c++11
   ```
   For Windows with Visual Studio, ensure OpenCV paths are correctly configured.

3. **Prepare folder structure**:
   - Create a folder named "1 деталь" in the program directory
   - Create subfolders for each part to analyze
   - In each subfolder, place two files:
     - `reference.png` - defect-free template image
     - `defective.png` - image to analyze for defects

### Running the Program

1. **Execute the compiled binary**:
   ```bash
   ./defect_detector
   ```

2. **Select material type** from the interactive menu:
   ```
   Choose material type:
   1. Steel
   2. Aluminum
   3. Titanium
   4. Magnesium
   Enter number or name:
   ```

## Input Requirements

### Image Specifications:
- Format: PNG or JPG
- Color mode: Grayscale (automatic conversion implemented)
- Dimensions: Images should have identical sizes for comparison
- Naming convention: Consistent naming within subfolders

## Output Files

For each processed part folder, the system generates:

1. **`gost_report.txt`** - Detailed defect analysis according to GOST standards
2. **`aligned_defect.png`** - Preprocessed defect image after alignment
3. **`defect_mask.png`** - Binary mask highlighting detected defects
4. **`visualization.png`** - Annotated image with defect 