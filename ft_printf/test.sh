gcc -w ft_printf.c main.c -o printf
./printf | cat -e > yours.res
gcc -w -D REAL ft_printf.c main.c -o printf
./printf | cat -e > original.res
diff -y original.res yours.res
rm -rf original.res yours.res printf