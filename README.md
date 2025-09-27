The system of automatic detection of external defects of parts using computer vision

Development and testing of an automatic detection system for external defects of parts to improve the efficiency of production control.

The system has been tested on:
- 3D models created in Blender
- Physical prototypes printed on a 3D printer
- Conveyor system with automatic production of 6 projections of a part in 4 seconds

The system demonstrates high accuracy in detecting defects in the contour and surface of parts, which allows:
- Reduce the number of defective products
- Speed up the quality control process
- Eliminate the subjective human factor

Development of an algorithm for detecting defects directly on 3D models without using multiple projections.

The developed program is used in the conveyor industry to reduce the number of defective parts. It uses computer vision algorithms to analyze images and camera data. The cameras scan the part in all planes, sending the images for further processing by the algorithm. Then, the reference image previously uploaded to the system is compared with the received part image obtained after scanning. 
Methods that were used in the program: ORB, BFMatcher, RANSAC.
To run the program, you need to insert a folder with images of a specific part into the program folder, and also change the name of the folder in the program itself to the name of the folder you are using.

As a result of the program's operation, an "output" folder is created in the project repository, containing a report, snapshots of defects, and deviations from the norm according to GOST.

the block diagram of the program looks like this:
<img width="595" height="312" alt="{B34EC3A7-D5A4-491E-936F-79288505B87A}" src="https://github.com/user-attachments/assets/525e4831-df4d-4974-adfb-1abded8146fd" />

An example of how the program works:
<img width="608" height="292" alt="{958260EF-714B-4873-B3E7-C8525BB8242F}" src="https://github.com/user-attachments/assets/8d293481-4333-4279-b53d-76e16a5d1aa3" />

Below is a variant of the part scanner, for which the program was developed.
<img width="618" height="412" alt="image" src="https://github.com/user-attachments/assets/8cd31f12-b303-40ef-a176-a02eb0886048" />

- Written in C++
- Uses the C++11/14/17 standard
