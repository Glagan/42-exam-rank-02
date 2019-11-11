Assignment name  : inter
Expected files   : inter.c
Allowed functions: write
--------------------------------------------------------------------------------

Escriba un programa que reciba como parámetros dos cadenas de caracteres y que
muestre, sin duplicados, los caracteres comunes a las dos cadenas. 

Se mostrarán en el orden de aparición dentro de la primera cadena.
La visualización se debe terminar con un salto de línea.

Si el número de parámetros transmitidos es distinto de 2, el programa mostrará
'\n'.

Ejemplos:
$>./inter "padinton" "paqefwtdjetyiytjneytjoeyjnejeyj" | cat -e
padinto$
$>./inter ddf6vewg64f gtwthgdwthdwfteewhrtag6h4ffdhsd | cat -e
df6ewg4$
$>./inter "algo" "no crea que esta frase esconde algo" | cat -e
algo$
$>./inter | cat -e
$
