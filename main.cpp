#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h> 
#include <string>
#include <algorithm>
#include <io.h>

using namespace cv;
using namespace std;

bool createDirectory(string& dirPath) {
    return _mkdir(dirPath.c_str()) == 0;
}

struct DefectInfo {
    Rect boundingRect;
    double area;
    Point center;
    string filename;
    bool isAllowed;
    string defectType;
    vector<string> violatedStandards;
};


Mat advancedAlignImages(Mat& templateImage, Mat& defectImage) {
    Ptr<SIFT> detector = SIFT::create(1000);
    vector<KeyPoint> kpTemplate, kpDefect;
    Mat descTemplate, descDefect;

    detector->detectAndCompute(templateImage, noArray(), kpTemplate, descTemplate);
    detector->detectAndCompute(defectImage, noArray(), kpDefect, descDefect);

    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descTemplate, descDefect, matches);

    vector<DMatch> goodMatches;
    for (const auto& match : matches) {
        if (match.distance < 5) {
            goodMatches.push_back(match);
        }
    }
    if (goodMatches.size() < 4) {
        return Mat();
    }
    vector<Point2f> ptsTemplate, ptsDefect;
    for (const auto& match : goodMatches) {
        ptsTemplate.push_back(kpTemplate[match.queryIdx].pt);
        ptsDefect.push_back(kpDefect[match.trainIdx].pt);
    }
    Mat homography = findHomography(ptsDefect, ptsTemplate, RANSAC, 5.0);
    Mat alignedDefect;
    warpPerspective(defectImage, alignedDefect, homography, templateImage.size());
    return alignedDefect;
}

void classifyDefects(vector<DefectInfo>& defects, const string& materialType) {
    for (auto& def : defects) {
        if (materialType == "steel") {
            if (def.area > 1000) {
                def.violatedStandards.push_back("ГОСТ 23901-88 (площадь > 1000 пикселей)");
                def.defectType = "Large defect";
            }
            else if (def.boundingRect.width > 5 || def.boundingRect.height > 5) {
                def.defectType = "Pores";
            }
            else {
                def.defectType = "Small defect";
            }

            if (def.defectType == "Pores" && (def.boundingRect.width > 5 || def.boundingRect.height > 5)) {
                def.violatedStandards.push_back("ГОСТ 15975-85 (размер пор > 5 пикселей)");
            }

            def.isAllowed = def.violatedStandards.empty();
        }
        else if (materialType == "aluminum") {
            if (def.area > 800) {
                def.violatedStandards.push_back("ГОСТ Р 53442-2009 (площадь > 800 пикселей)");
                def.defectType = "Large defect";
            }
            else {
                def.defectType = "Small defect";
            }

            def.isAllowed = def.violatedStandards.empty();
        }
        else if (materialType == "titanium") {
            if (def.area > 600) {
                def.violatedStandards.push_back("ГОСТ Р 53450-2009 (площадь > 600 пикселей)");
                def.defectType = "Large defect";
            }
            else if (def.boundingRect.width > 4 || def.boundingRect.height > 4) {
                def.defectType = "Pores";
            }
            else {
                def.defectType = "Small defect";
            }

            if (def.boundingRect.width > 8 || def.boundingRect.height > 8) {
                def.violatedStandards.push_back("ГОСТ Р 53450-2009 (размер > 8 пикселей)");
            }

            def.isAllowed = def.violatedStandards.empty();
        }
        else if (materialType == "magnesium") {
            if (def.area > 400) {
                def.violatedStandards.push_back("ГОСТ Р 54815-2011 (площадь > 400 пикселей)");
                def.defectType = "Large defect";
            }
            else if (def.boundingRect.width > 3 || def.boundingRect.height > 3) {
                def.defectType = "Pores";
            }
            else {
                def.defectType = "Small defect";
            }

            if (def.boundingRect.width > 6 || def.boundingRect.height > 6) {
                def.violatedStandards.push_back("ГОСТ 23974-2017 (размер > 6 пикселей)");
            }
            def.isAllowed = def.violatedStandards.empty();
        }
        else {
            def.isAllowed = false;
            def.violatedStandards.push_back("Не определен стандарт для материала");
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    string inputDir = "1 деталь";
    string outputDir = "output";
    createDirectory(outputDir);
    string materialType;
    do {
        cout << "Выберите тип материала:\n";
        cout << "1. Сталь\n";
        cout << "2. Алюминий\n";
        cout << "3. Титан\n";
        cout << "4. Магний\n";
        cout << "Введите номер или название: ";
        string input;
        cin >> input;

        if (input == "1" || input == "steel") materialType = "steel";
        else if (input == "2" || input == "aluminum") materialType = "aluminum";
        else if (input == "3" || input == "titanium") materialType = "titanium";
        else if (input == "4" || input == "magnesium") materialType = "magnesium";
        else {
            cout << "Ошибка: Некорректный ввод. Попробуйте еще раз.\n";
            continue;
        }
        break;
    } while (true);

    if (materialType != "steel" && materialType != "aluminum" && materialType != "titanium" && materialType != "magnesium") {
        cout << "Ошибка: Недопустимый тип материала\n";
        return -1;
    }
    vector<string> subfolders;
    intptr_t hFile;
    struct _finddata_t fileData;
    string searchPath = inputDir + "\\*";

    if ((hFile = _findfirst(searchPath.c_str(), &fileData)) != -1L) {
        do {
            if (fileData.attrib & _A_SUBDIR && string(fileData.name) != "." && string(fileData.name) != "..") {
                subfolders.push_back(fileData.name);
            }
        } while (_findnext(hFile, &fileData) == 0);
        _findclose(hFile);
    }

    for (string folderName : subfolders) {
        string fullInputPath = inputDir + "/" + folderName;
        string fullOutputPath = outputDir + "/" + folderName;

        Mat templateImage = imread(fullInputPath + "/эталонное.png", IMREAD_GRAYSCALE);
        Mat defectImage = imread(fullInputPath + "/дефектное.png", IMREAD_GRAYSCALE);

        if (templateImage.empty() || defectImage.empty()) {
            cout << "Ошибка: Не найдены файлы в папке " << folderName << endl;
            continue;
        }

        createDirectory(fullOutputPath);

        Mat alignedDefect = advancedAlignImages(templateImage, defectImage);
        if (alignedDefect.empty()) {
            alignedDefect = defectImage.clone();
            cout << "Успешно обработанны изображения из папки " << folderName << "!" << endl;
        }

        Mat difference;
        absdiff(templateImage, alignedDefect, difference);
        threshold(difference, difference, 0, 255, THRESH_BINARY | THRESH_OTSU);

        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(difference, difference, MORPH_OPEN, kernel);
        morphologyEx(difference, difference, MORPH_CLOSE, kernel);

        vector<vector<Point>> contours;
        findContours(difference, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        vector<DefectInfo> defects;
        for (auto cnt : contours) {
            Rect rect = boundingRect(cnt);
            double area = contourArea(cnt);

            if (area < 20) continue;

            Moments mu = moments(cnt);
            Point center = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
            defects.push_back({ rect, area, center, "", true, "", {} });
        }

        classifyDefects(defects, materialType);

        string defectsDir = fullOutputPath + "/обнаруженные дефекты";
        createDirectory(defectsDir);

        for (size_t i = 0; i < defects.size(); i++) {
            Rect rect = defects[i].boundingRect;
            Mat defectROI = alignedDefect(rect);
            string filename = defectsDir + "/дефект_" + to_string(i + 1) + "_" + to_string(rect.width) + "x" + to_string(rect.height) + ".jpg";
            imwrite(filename, defectROI);
            defects[i].filename = filename;
        }
        Mat visImage;
        cvtColor(alignedDefect, visImage, COLOR_GRAY2BGR);

        for (auto def : defects) {
            Scalar color = def.isAllowed ? Scalar(0, 255, 0) : Scalar(0, 0, 255);
            rectangle(visImage, def.boundingRect, color, 2);
            putText(visImage, def.defectType, Point(def.boundingRect.x, def.boundingRect.y - 5), FONT_HERSHEY_COMPLEX, 0.6, color, 1);
        }

        ofstream report(fullOutputPath + "/отчет по гостам.txt");
        report << "Отчет по дефектам согласно ГОСТ:\n";
        report << "---------------------------------\n";
        report << "Материал: " << materialType << "\n";
        report << "Обнаружено дефектов: " << defects.size() << "\n\n";

        for (size_t i = 0; i < defects.size(); i++) {
            report << "Дефект #" << (i + 1) << ":\n";
            report << "  Позиция: (" << defects[i].boundingRect.x << ", " << defects[i].boundingRect.y << ")\n";
            report << "  Размер: " << defects[i].boundingRect.width << "x" << defects[i].boundingRect.height << "\n";
            report << "  Площадь: " << defects[i].area << "\n";
            report << "  Тип: " << defects[i].defectType << "\n";
            report << "  Статус: " << (defects[i].isAllowed ? "Допустим" : "Не допустим") << "\n";
            report << "  Нарушения ГОСТов:\n";
            if (defects[i].violatedStandards.empty()) {
                report << "    Нет\n";
            }
            else {
                for (auto std : defects[i].violatedStandards) {
                    report << "    - " << std << "\n";
                }
            }
            report << "  Файл: " << defects[i].filename << "\n\n";
        }
        report.close();

        imwrite(fullOutputPath + "/дефектное изображение.png", alignedDefect);
        imwrite(fullOutputPath + "/маска дефектов.png", difference);
        imwrite(fullOutputPath + "/визуализация дефектов.png", visImage);

    }

    cout << "Обработка завершена. Результаты сохранены в папке output";
}