import os
import build


def traverse_directories(directory, project_handler):
    for element in os.listdir(directory):
        joined_path = os.path.join(directory, element)
        if os.path.isdir(joined_path):
            if 'CMakeLists.txt' in os.listdir(joined_path):
                project_abs_path = os.path.abspath(path=joined_path)
                project_handler(project_abs_path)

            traverse_directories(joined_path, project_handler)


projects_found = 0
def count_project(project_abs_path):
    global projects_found

    print('Found sketch in ' + project_abs_path)
    projects_found += 1


current_project_index = 0
def build_project(project_abs_path):
    global projects_found
    global current_project_index
    current_project_index += 1

    print(f"Building the project №{current_project_index} of {projects_found} found")
    result_code = build.build_project(project_abs_path)
    if result_code != 0:
        build.clear_project(project_abs_path)
        result_code = build.build_project(project_abs_path)
        if result_code != 0:
            print(f'Error compiling project: {project_abs_path}')
            exit(-1)


def clear_project(project_abs_path):
    build.clear_project(project_abs_path)


if __name__ == "__main__":
    traverse_directories(os.curdir, count_project)
    print(f"Found {projects_found} projects\n")

    print("Started building projects")
    traverse_directories(os.curdir, build_project)
    print("Ended building projects")

    traverse_directories(os.curdir, clear_project)
    print("Cleared all projects")
