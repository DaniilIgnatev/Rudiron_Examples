import os
import shutil
import subprocess

def clear_project(project_abs_path: str):
    build_path = f'{project_abs_path}/build'
    if os.path.exists(build_path):
        shutil.rmtree(build_path)

    build_debug_path = f'{project_abs_path}/build-debug'
    if os.path.exists(build_debug_path):
        shutil.rmtree(build_debug_path)


def build_project(project_path: str):
    build_path = f'{project_path}/build'
    if not os.path.exists(build_path):
        os.mkdir(build_path)
        
    cmake_command = ['cmake', '-DCMAKE_BUILD_TYPE=Release',
                     '-G', 'Ninja', '-S', project_path, '-B', build_path]
    result_code = subprocess.call(cmake_command)

    if result_code == 0:
        cmake_command = ['cmake', '--build', build_path]
        result_code = subprocess.call(cmake_command)

    return result_code

if __name__ == "__main__":
    project_dir = os.path.abspath(os.curdir)
    clear_project(project_dir)
    build_project(project_dir)