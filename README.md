# License Plate Recognition (번호판 인식)

OpenCV **C++** team project for Korean license plates. It is a course-style prototype (detect → recognize → optional member check), **not** production ANPR.

팀 프로젝트 / team project: **조은수, 이승민, 윤도현**

Git history does not record who owned which part, so this README does not assign roles.

There is no Python code and no `requirements.txt`. Dependencies are CMake, a C++11 compiler, and OpenCV (the checked-in CLion cache was generated against OpenCV 4.4.0).

## What the pipeline does

Entry point: `License_Plate_Recognition/main.cpp`.

1. **Load models and one test image**
   - Trains k-NN from character sprite sheets (`KNNtrain()` in `kNN.cpp`) and writes `Knn_number.xml` / `Knn_text.xml`.
   - Loads a **pre-trained** linear SVM (`SVMtrain.xml`) that scores plate vs. non-plate. This repo has SVM *inference* only (`SVM.cpp`); it does not contain SVM training code.
   - Prompts `차량번호(0~20)` and reads `/home/seungmin/사진/test_car/%02d.jpg`.

2. **Detect plate candidates** (`preprocessing.cpp`, `refine.cpp`)
   - Grayscale → blur → Sobel (x) → binary threshold → morphological close.
   - Contours whose min-area rectangle passes size / aspect-ratio checks become candidates.
   - Each candidate is flood-fill refined, deskewed, cropped, converted to gray, and resized to **144×28**.

3. **Pick a real plate** (`SVM.cpp`)
   - Flatten each 144×28 crop and run SVM. The first candidate predicted as class `1` is used. If none match, it prints `번호판 X`.

4. **Recognize characters** (`classify_objects.cpp`, `kNN.cpp`)
   - Resize the plate to 180×35, Otsu-threshold, and crop a small border.
   - Find digit/Hangul blobs (area and x-position heuristics), merge Hangul pieces, sort left-to-right.
   - **Exactly 7** blobs are required (classic Korean layout: two digits, one Hangul, four digits). Otherwise: `숫자(문자) 객체 검출 x`.
   - k-NN: positions 0,1,3–6 as digits (10 classes); position 2 as Hangul (40 labels such as 가, 나, …, 주).

5. **Member list** (`compare_plate.cpp`)
   - Compare the string to `Licenses.xml`. Match → `회원 확인`. Miss → `외부 차량` and optional `회원 추가? y or n`.

Windows show the Sobel/morph images, the plate with red boxes, and the car with a green rotated rectangle.

## Layout

```
License_Plate_Recognition/
  main.cpp                 # pipeline glue
  preprocessing.cpp / .h   # Sobel + contours → plate candidates
  refine.cpp / .h          # flood-fill refine, rotate, crop
  SVM.cpp / .h             # SVM: which candidate is a plate
  classify_objects.cpp / .h
  kNN.cpp / .h             # train/run k-NN for digits and Hangul
  compare_plate.cpp / .h   # Licenses.xml member lookup
  CMakeLists.txt
  cmake-build-debug/       # CLion artifacts + sample SVMtrain.xml, Licenses.xml
```

## Build and run

Install OpenCV 4.x with the `ml` module, then:

```bash
cd License_Plate_Recognition
mkdir -p build && cd build
cmake ..
cmake --build .
./License_Plate_Recognition
```

At runtime the program asks for a car index `0`–`20` and opens OpenCV windows (needs a display). Press any key after the result.

**Paths are hardcoded** to one developer machine (`/home/seungmin/...`). Edit them in `main.cpp`, `kNN.cpp`, and `compare_plate.cpp` before it will run elsewhere. You need:

| Path | Role |
| --- | --- |
| `.../사진/test_car/%02d.jpg` | Test photos `00.jpg`–`20.jpg` |
| `.../사진/trainimage/train_numbers3.png` | Digit training sprite |
| `.../사진/trainimage/train_texts_f.png` | Hangul training sprite |
| `/home/seungmin/SVMtrain.xml` | Saved SVM (a copy exists under `cmake-build-debug/`) |
| `/home/seungmin/Licenses.xml` | Member plates (sample copy in `cmake-build-debug/`) |

k-NN XML files are written on each run. The original test/train images are **not** in this repository.

## Limitations (not production ANPR)

- Tuned for a small set of Korean plates with **seven** characters in one layout; newer plate formats are not handled.
- Geometry, thresholds, and blob rules are fixed for the original photos—not general lighting, angles, or cameras.
- No evaluation set, no CLI flags, no packaged models/images besides CLion build leftovers.
- Retraining k-NN from disk images on every start; SVM training is outside this tree.
- Member XML I/O is a demo, not a real access-control system.

## License

No project license file is included. OpenCV is used under its own license.
