#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <chrono>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

ofstream logFile("log.txt", ios::app);

void organizeByType(fs::path basePath, bool preview) {

    fs::create_directory(basePath / "Images");
    fs::create_directory(basePath / "Documents");
    fs::create_directory(basePath / "Videos");
    fs::create_directory(basePath / "Others");

    for (const auto &file : fs::recursive_directory_iterator(basePath)) {

        if (fs::is_directory(file))
            continue;

        string ext = file.path().extension().string();
        fs::path destination;

        if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
            destination = basePath / "Images" / file.path().filename();

        else if (ext == ".pdf" || ext == ".docx" || ext == ".txt")
            destination = basePath / "Documents" / file.path().filename();

        else if (ext == ".mp4" || ext == ".mkv" || ext == ".avi")
            destination = basePath / "Videos" / file.path().filename();

        else
            destination = basePath / "Others" / file.path().filename();

        try {

            if (preview) {
                cout << file.path().filename() << " -> " << destination.filename() << endl;
            }
            else {
                fs::rename(file.path(), destination);
                logFile << "Moved " << file.path().filename() << " to " << destination.filename() << endl;
            }

        }
        catch (fs::filesystem_error &e) {

            cout << "Error moving file: " << file.path().filename() << endl;
            logFile << "Error: " << e.what() << endl;
        }
    }

    if (!preview)
        cout << "Files organized by type.\n";
}

void organizeByDate(fs::path basePath) {

    for (const auto &file : fs::recursive_directory_iterator(basePath)) {

        if (fs::is_directory(file))
            continue;

        try {

            auto ftime = fs::last_write_time(file);

            auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now()
                + chrono::system_clock::now()
            );

            time_t cftime = chrono::system_clock::to_time_t(sctp);

            tm *timeinfo = localtime(&cftime);

            int year = timeinfo->tm_year + 1900;

            fs::path yearFolder = basePath / to_string(year);

            fs::create_directory(yearFolder);

            fs::rename(file.path(), yearFolder / file.path().filename());

            logFile << "Moved " << file.path().filename() << " to " << year << endl;
        }
        catch (fs::filesystem_error &e) {

            cout << "Error processing file: " << file.path().filename() << endl;
            logFile << "Error: " << e.what() << endl;
        }
    }

    cout << "Files sorted by year.\n";
}

void findDuplicates(fs::path basePath) {

    unordered_set<string> seen;

    fs::create_directory(basePath / "Duplicates");

    for (const auto &file : fs::recursive_directory_iterator(basePath)) {

        if (fs::is_directory(file))
            continue;

        string name = file.path().filename().string();

        try {

            if (seen.count(name)) {

                fs::rename(file.path(), basePath / "Duplicates" / name);

                cout << "Duplicate found: " << name << endl;

                logFile << "Duplicate detected: " << name << endl;
            }
            else {
                seen.insert(name);
            }

        }
        catch (fs::filesystem_error &e) {

            cout << "Error handling duplicate file: " << name << endl;
            logFile << "Error: " << e.what() << endl;
        }
    }

    cout << "Duplicate scan completed.\n";
}

int main() {

    string folderPath;
    int choice;
    char previewChoice;

    cout << "Enter folder path: ";
    getline(cin, folderPath);

    fs::path basePath(folderPath);

    while (true) {

        cout << "\nSmart File Organizer\n";
        cout << "1. Organize by file type\n";
        cout << "2. Organize by date\n";
        cout << "3. Detect duplicates\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1) {

            cout << "Preview mode? (y/n): ";
            cin >> previewChoice;

            bool preview = (previewChoice == 'y');

            organizeByType(basePath, preview);
        }

        else if (choice == 2)
            organizeByDate(basePath);

        else if (choice == 3)
            findDuplicates(basePath);

        else if (choice == 4)
            break;

        else
            cout << "Invalid choice\n";
    }

    return 0;
}