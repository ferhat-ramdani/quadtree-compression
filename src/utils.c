#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>

static char* win32_file_dialog(const char* default_path, int is_save) {
    OPENFILENAMEA ofn;
    char szFile[512] = {0};
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0PNG Images (*.png)\0*.png\0JPEG Images (*.jpg;*.jpeg)\0*.jpg;*.jpeg\0Bitmap Images (*.bmp)\0*.bmp\0Binary Formats (*.qtn;*.qtc)\0*.qtn;*.qtc\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    
    // Set initial directory if provided
    if (default_path) {
        ofn.lpstrInitialDir = default_path;
    } else {
        ofn.lpstrInitialDir = NULL;
    }
    
    int success;
    if (is_save) {
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
        success = GetSaveFileNameA(&ofn);
    } else {
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        success = GetOpenFileNameA(&ofn);
    }

    if (success) {
        char* result = malloc(strlen(szFile) + 1);
        if (result) {
            strcpy(result, szFile);
        }
        return result;
    }
    return NULL;
}

char* open_file_dialog(const char* default_path) {
    return win32_file_dialog(default_path, 0);
}

char* save_file_dialog(const char* default_path) {
    return win32_file_dialog(default_path, 1);
}

#elif defined(__APPLE__)

static char* mac_file_dialog(const char* default_path, int is_save) {
    char cmd[512];
    if (is_save) {
        snprintf(cmd, sizeof(cmd), "osascript -e 'POSIX path of (choose file name default location \"%s\")'", default_path ? default_path : "");
    } else {
        snprintf(cmd, sizeof(cmd), "osascript -e 'POSIX path of (choose file default location \"%s\")'", default_path ? default_path : "");
    }

    FILE* f = popen(cmd, "r");
    if (!f) return NULL;

    char buffer[512];
    if (fgets(buffer, sizeof(buffer), f) != NULL) {
        pclose(f);
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        
        if (strlen(buffer) == 0) return NULL; // cancelled

        char* result = malloc(strlen(buffer) + 1);
        if (result) strcpy(result, buffer);
        return result;
    }
    pclose(f);
    return NULL;
}

char* open_file_dialog(const char* default_path) {
    return mac_file_dialog(default_path, 0);
}

char* save_file_dialog(const char* default_path) {
    return mac_file_dialog(default_path, 1);
}

#elif defined(__linux__)

static char* linux_file_dialog(const char* default_path, int is_save) {
    char cmd[512];
    if (is_save) {
        snprintf(cmd, sizeof(cmd), "zenity --file-selection --save --confirm-overwrite --filename=\"%s/\"", default_path ? default_path : "");
    } else {
        snprintf(cmd, sizeof(cmd), "zenity --file-selection --filename=\"%s/\"", default_path ? default_path : "");
    }

    FILE* f = popen(cmd, "r");
    if (!f) return NULL;

    char buffer[512];
    if (fgets(buffer, sizeof(buffer), f) != NULL) {
        pclose(f);
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        
        if (strlen(buffer) == 0) return NULL; // cancelled
        
        char* result = malloc(strlen(buffer) + 1);
        if (result) strcpy(result, buffer);
        return result;
    }
    pclose(f);
    return NULL;
}

char* open_file_dialog(const char* default_path) {
    return linux_file_dialog(default_path, 0);
}

char* save_file_dialog(const char* default_path) {
    return linux_file_dialog(default_path, 1);
}

#endif
