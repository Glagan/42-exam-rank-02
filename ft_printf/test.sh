gcc -Wall -Werror -Wextra ft_printf.c main.c -o ft_printf
./ft_printf > yy
cat -e yy > y
gcc -Wall -Werror -Wextra -D REAL main.c -o printf
./printf | cat -e > r
diff -y --suppress-common-lines r y
rm -rf yy y r ft_printf printf
