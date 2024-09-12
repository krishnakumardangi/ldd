import subprocess

def main():
    # Run a command and write messege from user space to kernel
    # Define your variable
    message = str(input())
    # Construct the command with the variable
    command = f'echo "{message}" > /proc/ldd_driver'
    result = subprocess.run(['bash', '-c', command], capture_output=True, text=True)
    print(result.stdout)
    # Check if the command was successful
    if result.returncode == 0:
        print("Command succeeded.\n")
    else:
        print("Command failed.\n")
        return 0

    # Run a command and get the output    
    driver_handle = open('/proc/ldd_driver')
    message_from_kernel_space = driver_handle.readline()
    print(message_from_kernel_space)
    driver_handle.close()
    return 0


main()
