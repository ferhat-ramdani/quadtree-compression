#ifndef UTILS_H
#define UTILS_H

/* Opens a native file dialog to select an existing file.
 * Returns a malloc'd string containing the path, or NULL if cancelled.
 * default_path can be NULL.
 */
char* open_file_dialog(const char* default_path);

/* Opens a native file dialog to select a save destination.
 * Returns a malloc'd string containing the path, or NULL if cancelled.
 * default_path can be NULL.
 */
char* save_file_dialog(const char* default_path);

#endif
