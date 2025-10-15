/*
    args.h - v1.0.0
    More command-line argument utilities.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    History:
        v1.0.0  First version

    args.h is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
*/

#ifndef ARGS_H
#define ARGS_H

#define args_arg_len(ARGS) (sizeof(ARGS) / sizeof(char*))

#define args_need_all_args(ARGS) \
    do { \
        if (argc - optind < args_arg_len(ARGS)) { \
            fputs("Need to inform ", stderr); \
            for (i = argc - optind; i < args_arg_len(ARGS); i++) { \
                fputs(ARGS[i], stderr); \
                if (i < args_arg_len(ARGS) - 2) \
                    fputs(", ", stderr); \
                else if (i == args_arg_len(ARGS) - 2) \
                    fputs(" and ", stderr); \
            } \
            putc('\n', stderr); \
            return 2; \
        } \
    } while (0)

#endif
