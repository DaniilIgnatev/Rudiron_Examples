import os
import shutil
import subprocess
from distutils.dir_util import copy_tree


def clear_cmake_project(project_abs_path: str):
    build_path = f"{project_abs_path}/build"
    if os.path.exists(build_path):
        shutil.rmtree(build_path)

    build_debug_path = f"{project_abs_path}/build-debug"
    if os.path.exists(build_debug_path):
        shutil.rmtree(build_debug_path)


def _build_cmake_project(project_path: str):
    build_path = f"{project_path}/build"
    if not os.path.exists(build_path):
        os.mkdir(build_path)

    cmake_command = [
        "cmake",
        "-DCMAKE_BUILD_TYPE=Release",
        "-G",
        "Ninja",
        "-S",
        project_path,
        "-B",
        build_path,
    ]
    result_code = subprocess.call(cmake_command)

    if result_code == 0:
        cmake_command = ["cmake", "--build", build_path]
        result_code = subprocess.call(cmake_command)

    return result_code


def traverse_directories(directory, project_handler):
    for element in os.listdir(directory):
        joined_path = os.path.join(directory, element)
        if os.path.isdir(joined_path):
            if "CMakeLists.txt" in os.listdir(joined_path):
                project_abs_path = os.path.abspath(path=joined_path)
                project_handler(project_abs_path)

            traverse_directories(joined_path, project_handler)


projects_found = 0


def count_project(project_abs_path):
    global projects_found

    print("Found sketch in " + project_abs_path)
    projects_found += 1


current_project_index = 0


def build_cmake_project(project_abs_path):
    global projects_found
    global current_project_index
    current_project_index += 1

    print(f"Building the project №{current_project_index} of {projects_found} found")
    result_code = _build_cmake_project(project_abs_path)
    if result_code != 0:
        clear_cmake_project(project_abs_path)
        result_code = _build_cmake_project(project_abs_path)
        if result_code != 0:
            print(f"Error compiling project: {project_abs_path}")
            exit(-1)


reference_sketch = os.path.abspath("templates/basic")
reference_cmakelists = os.path.join(reference_sketch, "CMakeLists.txt")

def fix_cmakelists(project_abs_path):
    print("Found sketch in " + project_abs_path)
    if project_abs_path == reference_sketch:
        return
    
    cmakelists_path = os.path.join(project_abs_path, "CMakeLists.txt")
    if os.path.exists(cmakelists_path):
        os.remove(cmakelists_path)
    
    shutil.copy(reference_cmakelists, cmakelists_path)

    libraries_path = os.path.join(project_abs_path, "libraries.txt")
    if not os.path.exists(libraries_path):
        open(libraries_path, 'a').close()


reference_vscode = os.path.join(reference_sketch, ".vscode")
def fix_vscode(project_abs_path):
    print("Found sketch in " + project_abs_path)
    if project_abs_path == reference_sketch:
        return

    vscode_path = os.path.join(project_abs_path, ".vscode")
    if os.path.exists(vscode_path):
        shutil.rmtree(vscode_path)
    
    copy_tree(reference_vscode, vscode_path)


def rename_cpp_sketch(project_abs_path):
    print("Found sketch in " + project_abs_path)

    sketch_name = os.path.basename(project_abs_path)
    path_cpp = os.path.join(project_abs_path, sketch_name + ".cpp")

    path_sketch = os.path.join(project_abs_path, "sketch.cpp")
    if not os.path.exists(path_cpp):
        if os.path.exists(path_sketch):
            shutil.copy(path_sketch, path_cpp)

    if os.path.exists(path_sketch):
        os.remove(path_sketch)


def generate_ino_project(project_abs_path):
    print("Found sketch in " + project_abs_path)

    sketch_name = os.path.basename(project_abs_path)
    path_cpp = os.path.join(project_abs_path, sketch_name + ".cpp")

    path_ino_folder = os.path.join(project_abs_path, sketch_name)
    if os.path.exists(path_ino_folder):
        shutil.rmtree(path_ino_folder)
    shutil.copytree(project_abs_path, path_ino_folder)

    path_ino = os.path.join(path_ino_folder, sketch_name + ".ino")
    shutil.copy(path_cpp, path_ino)

    path_ino_cpp = os.path.join(path_ino_folder, sketch_name + ".cpp")
    if os.path.exists(path_ino_cpp):
        os.remove(path_ino_cpp)
    
    path_CMakeLists = os.path.join(path_ino_folder, "CMakeLists.txt")
    if os.path.exists(path_CMakeLists):
        os.remove(path_CMakeLists)


def build_ino_project(project_abs_path):
    sketch_name = os.path.basename(project_abs_path)

    path_ino_folder = os.path.join(project_abs_path, sketch_name)
    path_ino = os.path.join(path_ino_folder, sketch_name + ".ino")
    print("Building ino file: " + path_ino)
    # arduino-cli compile C:\Users\Daniil\Documents\GitHub\Rudiron_Projects\basic  -v

    command = [
        "arduino-cli",
        "compile",
        path_ino_folder,
        "-b",
        "Rudiron:MDR32F9Qx:buterbrodR916",
        "-v"
    ]
    result_code = subprocess.call(command)

    if result_code != 0:
        print(f"Error compiling ino project: {path_ino_folder}")
        exit(-1)


def clear_ino_project(project_abs_path):
    sketch_name = os.path.basename(project_abs_path)
    path_ino_folder = os.path.join(project_abs_path, sketch_name)
    if os.path.exists(path_ino_folder):
        shutil.rmtree(path_ino_folder)


if __name__ == "__main__":
    traverse_directories(os.curdir, count_project)
    print(f"Found {projects_found} projects\n")

    traverse_directories(os.curdir, fix_cmakelists)
    traverse_directories(os.curdir, fix_vscode)

    # print("Started renaming projects")
    # traverse_directories(os.curdir, rename_cpp_sketch)
    # print("Ended renaming projects")

    print("Started building cmake projects")
    traverse_directories(os.curdir, build_cmake_project)
    print("Ended buildin cmake projects")

    print("Started clear cmake projects")
    traverse_directories(os.curdir, clear_cmake_project)
    print("Ended clear cmake projects")

    print("Started generating ino projects")
    traverse_directories(os.curdir, generate_ino_project)
    print("Ended generating ino projects")

    print("Started building arduino projects")
    traverse_directories(os.curdir, build_ino_project)
    print("Ended building arduino projects")

    # print("Started clear arduino projects")
    # traverse_directories(os.curdir, clear_ino_project)
    # print("Ended clear arduino projects")
