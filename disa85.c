/*
 * disa85.c
 *
 * 8085 disassembler
 * 
 */

#include    <stdio.h>
#include    <stdlib.h>

#include    "disa85.h"

#define     BANNER      "*\n"                        \
                        "* CPU 8085 disassembler.\n" \
                        "* v1.0\n"                   \
                        "*\n"
#define     SPACE_PAD   "                "


/*
 * main()
 *
 */
int main(int argc, char *argv[])
{
    FILE   *file;
    int     byte, i, padding, param_size;
    int     prog_counter;
    int     opcode_bytes, opcode, param;

    if ( argc < 2 )
    {
        printf("error: missing binary object file.\n");
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("error: opening file\n");
        return 1;
    }

    printf("%s\n", BANNER);

    prog_counter = 0;
    printf("                 %s ORG   $%04x\n\n", SPACE_PAD, prog_counter);

    while ((byte = fgetc(file)) != EOF)
    {
        printf("%04x %02x ", prog_counter, byte);
        
        if ( ( opcode_bytes = op_codes[byte].byte_count) == 0 )
        {
            printf("       | %s DB    $%02x\n", SPACE_PAD, byte);
            prog_counter++;
            continue;
        }
        else
        {
            opcode = byte;
            param = 0;
            param_size = 0;
            padding = 6;

            for ( i = 0; i < (opcode_bytes - 1); i++ )
            {
                byte = fgetc(file);
                printf("%02x ", byte);

                byte = byte << (i * 8);
                param += byte;
                param_size += 2;
                padding -= 3;
            }

            printf("%-*s | %s ", padding, "", SPACE_PAD);
            printf("%s",op_codes[opcode].mnem );
            if ( param_size )
                printf("%0*x", param_size, param);
        }

        printf("\n");
        prog_counter += opcode_bytes;
    }

    fclose(file);

    return 0;
}
