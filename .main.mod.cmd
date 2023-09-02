cmd_/home/user/Desktop/KernelProject/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/home/user/Desktop/KernelProject/"$$0) }' > /home/user/Desktop/KernelProject/main.mod
