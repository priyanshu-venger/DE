#include<bits/stdc++.h>
using namespace std;

bool check_antipodal(vector<vector<long long>>&u,vector<vector<long long>>&v,int i,int j){
    long long cmp1=u[i][0]*u[j][1]-u[i][1]*u[j][0];
    cmp1=(cmp1==0?0:cmp1/abs(cmp1));
    long long cmp2=u[i][0]*v[j][1]-u[i][1]*v[j][0];
    cmp2=(cmp2==0?0:cmp2/abs(cmp2));
    if(cmp1*cmp2>=0){
        return 1;
    }
    cmp1=u[j][0]*u[i][1]-u[j][1]*u[i][0];
    cmp1=(cmp1==0?0:cmp1/abs(cmp1));
    cmp2=u[j][0]*v[j][1]-u[j][1]*v[j][0];
    cmp2=(cmp2==0?0:cmp2/abs(cmp2));
    if(cmp1*cmp2>0){
        return 0;
    }
    return 1;
}
int main(){
    ifstream h("a3in.txt");
    int n;
    h>>n;
    vector<vector<long long>> a(n,vector<long long>(2,0)),u(n,vector<long long>(2,0)),v(n,vector<long long>(2,0));
    map<int,map<int,bool>> mp;
    vector<pair<int,int>> res;
    long long x,x1,y,y1;
    cin>>x>>y>>x1>>y1;
    for(auto &i:a) h>>i[0]>>i[1],mp[i[0]][i[1]]=1;
    if(!mp[x][y] || !mp[x1][y1]){
        cout<<"0\n";
        return 0;
    }
    for(int i=0;i<n;i++){
        v[i][0]=a[(i+1)%n][0]-a[i][0];
        v[i][1]=a[(i+1)%n][1]-a[i][1];
        u[i][0]=a[(i+n-1)%n][0]-a[i][0];
        u[i][1]=a[(i+n-1)%n][1]-a[i][1];
    }
    long long ans=0;
    int idx=1,last=1;
    for(int i=0;i<n;i++){
        idx=max(i+1,last);
        bool flag=0;
        while(idx<n){
            bool cmp1=check_antipodal(u,v,i,idx);
            bool cmp2=check_antipodal(v,u,i,idx);
            if(cmp1==cmp2){
                res.push_back({i+1,idx+1});
                ans=min(ans,(a[i][0]-a[idx][0])*(a[i][0]-a[idx][0])+(a[i][1]-a[idx][1])*(a[i][1]-a[idx][1]));
            }
            if(flag && cmp1!=cmp2) break;
            else if(!flag && cmp1==cmp2) last=idx,flag=1;
            idx++;
        }
    }
    if((x-x1)*(x-x1)+(y-y1)*(y-y1)==ans) cout<<"1\n";
    else cout<<"0\n";
    return 0;

}

/*
7
100 0
130 22 
165 55
175 65
123 97
100 98
85 98

7
100 0
115 98
100 98
77 97
25 65
35 55
70 22

4
40 0
100 0
70 4
60 3
*/

import os
import subprocess
import argparse
import sys
import platform
import shutil

def display_and_confirm(file_path):
    """
    Opens a file in a text editor for review and then asks the user for
    confirmation to run it.

    Args:
        file_path (str): The path to the source file.

    Returns:
        bool: True if the user confirms, False otherwise.
    """
    print("\n" + "="*60)
    print(f"Preparing to review code from '{file_path}'...")
    print("="*60)

    editor = os.environ.get('EDITOR')
    if not editor:
        if shutil.which('xdg-open'):
            editor = 'xdg-open'
        elif shutil.which('vim'):
            editor = 'vim'
        elif shutil.which('nano'):
            editor = 'nano'
    
    if editor:
        print(f"--- Opening file in '{editor}'. Please review the code. ---")
        print("--- Close the editor to continue... ---")
        try:
            subprocess.run([editor, file_path])
        except Exception as e:
            print(f"[ERROR] Failed to open the editor: {e}")
            print("[INFO] Please review the file manually.")
    else:
        print("[WARNING] Could not find a default text editor (checked $EDITOR, xdg-open, vim, nano).")
        print(f"[INFO] Please review the file manually: {file_path}")

    print("="*60)
    
    while True:
        try:
            choice = input(f"Do you want to execute the code from '{os.path.basename(file_path)}'? (y/n): ").lower().strip()
            if choice in ['y', 'yes']:
                return True
            elif choice in ['n', 'no']:
                print(f"[INFO] Execution of {file_path} skipped by user.")
                return False
            else:
                print("Invalid input. Please enter 'y' or 'n'.")
        except EOFError:
            print("\n[INFO] No input received. Skipping execution.")
            return False


def compile_c_cpp(source_path):
    """
    Compiles a C or C++ source file.

    Args:
        source_path (str): The full path to the source file.

    Returns:
        str or None: The path to the compiled executable if successful, otherwise None.
    """
    if not os.path.exists(source_path):
        print(f"[ERROR] Source file not found: {source_path}")
        return None

    base_name = os.path.splitext(source_path)[0]
    executable_name = f"{base_name}_exec"
    if platform.system() == "Windows":
        executable_name += ".exe"

    compiler = 'g++' if source_path.endswith('.cpp') else 'gcc'
    compile_command = [compiler, source_path, '-o', executable_name, '-lm']
    
    print(f"--- Compiling: {' '.join(compile_command)} ---")
    try:
        result = subprocess.run(
            compile_command, capture_output=True, text=True, check=False
        )
        if result.returncode != 0:
            print(f"[COMPILE ERROR] Failed to compile {source_path}.")
            if result.stderr:
                print("Compiler Error Output:\n" + result.stderr)
            return None
        print(f"[COMPILE SUCCESS] Created executable: {executable_name}")
        return executable_name
    except FileNotFoundError:
        print(f"[ERROR] Compiler '{compiler}' not found. Please install it and ensure it's in your system's PATH.")
        return None
    except Exception as e:
        print(f"[CRITICAL ERROR] An unexpected error occurred during compilation: {e}")
        return None

def run_pa3_with_input(pa3_executable, input_file_path):
    """
    Runs the compiled pa3.cpp executable with a given input file.

    Args:
        pa3_executable (str): The path to the compiled pa3 executable.
        input_file_path (str): The path to the file to be used as stdin.

    Returns:
        float: The numerical output of the pa3 executable, or 0.0 on failure.
    """
    if not os.path.exists(input_file_path):
        print(f"    [ERROR] Input file for pa3.cpp '{input_file_path}' not found. Skipping.")
        return 0.0

    try:
        with open(input_file_path, 'r') as stdin_f:
            run_command = [f'./{pa3_executable}'] if platform.system() != "Windows" else [pa3_executable]
            result = subprocess.run(
                run_command,
                stdin=stdin_f,
                capture_output=True,
                text=True,
                check=True,
                timeout=5.0
            )
            try:
                output_value = float(result.stdout.strip())
                return output_value
            except (ValueError, TypeError):
                print(f"    [WARNING] pa3.cpp did not return a number. Output was: '{result.stdout.strip()}'")
                return 0.0
    except subprocess.CalledProcessError as e:
        print(f"    [RUNTIME ERROR] pa3.cpp failed with exit code {e.returncode}. Skipping.")
        return 0.0
    except Exception as e:
        print(f"    [CRITICAL ERROR] An unexpected error occurred while running pa3.cpp: {e}")
        return 0.0

def run_file_against_testcases(file_path, pa3_executable, test_cases, processed_dir):
    """
    Runs a single file against a series of test cases, piping each result
    through the pa3 executable and summing the final outputs.
    """
    print("\n" + "#"*60)
    print(f"# Processing file: {os.path.basename(file_path)}")
    print("#"*60)

    if not display_and_confirm(file_path):
        return

    executable_to_run = None
    is_temp_executable = False

    if file_path.endswith(('.c', '.cpp')):
        executable_to_run = compile_c_cpp(file_path)
        is_temp_executable = True
        if not executable_to_run:
            return
    elif file_path.endswith('.py'):
        executable_to_run = [sys.executable, file_path]
    else:
        print(f"[ERROR] Unsupported file type: {file_path}.")
        return

    total_sum = 0.0
    input_filename = "a3in.txt"
    output_filename = "a3out.txt"

    for test_case_path in test_cases:
        print(f"\n--- Running with test case: {os.path.basename(test_case_path)} ---")
        try:
            shutil.copy(test_case_path, input_filename)
            print(f"    -> Copied test case to '{input_filename}'.")

            with open(input_filename, 'r') as stdin_f, open(output_filename, 'w') as stdout_f:
                run_command = [f'./{executable_to_run}'] if is_temp_executable and platform.system() != "Windows" else executable_to_run
                subprocess.run(
                    run_command,
                    stdin=stdin_f,
                    stdout=stdout_f,
                    check=True,
                    timeout=5.0
                )
            print(f"    -> Successfully generated '{output_filename}'.")
            
            print(f"    -> Passing output to pa3.cpp...")
            pa3_result = run_pa3_with_input(pa3_executable, output_filename)
            print(f"    -> pa3.cpp returned: {pa3_result}")
            total_sum += pa3_result

        except subprocess.CalledProcessError as e:
            print(f"    [RUNTIME ERROR] File failed on this test case with exit code {e.returncode}. Skipping.")
        except subprocess.TimeoutExpired:
            print(f"    [TIMEOUT] File took too long on this test case. Skipping.")
        except Exception as e:
            print(f"    [CRITICAL ERROR] An unexpected error occurred: {e}")
        finally:
            if os.path.exists(input_filename):
                os.remove(input_filename)
            if os.path.exists(output_filename):
                os.remove(output_filename)

    print("\n-------------------------------------------")
    print(f"Final sum for {os.path.basename(file_path)} over all test cases: {total_sum}")
    print("-------------------------------------------")

    # --- NEW: Move the processed file to the 'processed' directory ---
    try:
        print(f"--- Moving processed file to '{processed_dir}' ---")
        shutil.move(file_path, processed_dir)
    except Exception as e:
        print(f"[MOVE ERROR] Could not move {file_path} to processed folder: {e}")

    if is_temp_executable and executable_to_run and os.path.exists(executable_to_run):
        os.remove(executable_to_run)
        print(f"--- Cleaned up {executable_to_run} ---")


def process_directory(root_dir):
    """
    Recursively finds and processes runnable files in a directory.
    """
    if not os.path.isdir(root_dir):
        print(f"Error: Directory not found at '{root_dir}'")
        return

    # --- Define and create necessary folders ---
    current_dir = os.getcwd()
    test_case_dir = os.path.join(current_dir, "testcases")
    processed_dir = os.path.join(current_dir, "processed")
    os.makedirs(processed_dir, exist_ok=True)
    
    if not os.path.isdir(test_case_dir):
        print(f"Error: 'testcases' directory not found at '{test_case_dir}'. Please create it and add input files.")
        return
    
    test_cases = [os.path.join(test_case_dir, f) for f in os.listdir(test_case_dir)]
    if not test_cases:
        print("Warning: 'testcases' directory is empty. No tests to run.")
        return

    print(f"Found {len(test_cases)} test case(s) in '{test_case_dir}'.")

    pa3_file_path = None
    other_files = []

    print(f"\n--- Scanning directory: {os.path.abspath(root_dir)} ---")
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            full_path = os.path.join(dirpath, filename)
            if filename.lower() == 'pa3.cpp':
                pa3_file_path = full_path
            elif filename.lower().endswith(('.c', '.cpp', '.py')):
                other_files.append(full_path)

    if not pa3_file_path:
        print("\n[FATAL] 'pa3.cpp' not found in the directory. Cannot proceed.")
        return

    print("\n--- Compiling pa3.cpp globally for all runs ---")
    pa3_executable = compile_c_cpp(pa3_file_path)
    if not pa3_executable:
        print("[FATAL] Could not compile pa3.cpp. Aborting.")
        return

    print(f"\nFound {len(other_files)} other file(s) to process.")
    for file_path in other_files:
        run_file_against_testcases(file_path, pa3_executable, test_cases, processed_dir)

    if os.path.exists(pa3_executable):
        os.remove(pa3_executable)
        print(f"\n--- Cleaned up global pa3 executable: {pa3_executable} ---")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Recursively run files against test cases and pipe output through pa3.cpp."
    )
    parser.add_argument(
        "directory",
        nargs="?",
        default=".",
        help="The directory to scan for files. Defaults to the current directory."
    )
    
    args = parser.parse_args()
    process_directory(args.directory)

