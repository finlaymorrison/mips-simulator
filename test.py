import os
import glob
import filecmp
from tabnanny import check

def get_io(test_path):
    in_files = glob.glob("{}/in.txt".format(test_path))
    out_files = glob.glob("{}/out.txt".format(test_path))
    if len(in_files) == 1 and len(out_files) == 1:
        return in_files[0], out_files[0]
    else:
        return None, None

def test_example(test_path):
    program_name = test_path.split('/')[1]
    test_name = test_path.split('/')[2]
    input, output = get_io(test_path)
    if input and output:
        os.system("./bin/{} {} tmp/out.txt".format(program_name, input))
        if filecmp.cmp("tmp/out.txt", output):
            print("\tsuccess: {}".format(test_name))
        else:
            os.system("mkdir -p tmp/{}".format(program_name))
            os.system("mv tmp/out.txt tmp/{}/{}.out".format(program_name, test_name))
            print("\tfail: {} : output saved at tmp/{}/{}.out".format(test_name, program_name, test_name))

def test_program(tests_path):
    print("testing {}".format(tests_path.split('/')[1]))
    for test in os.listdir(tests_path):
        test_example("{}/{}".format(tests_path,test))

def main():
    for prog_test in os.listdir("test_examples"):
        test_program("test_examples/{}".format(prog_test))

if __name__ == "__main__":
    main()