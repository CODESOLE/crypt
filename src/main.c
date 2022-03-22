/* main.c
 *
 * Copyright 2022 CODESOLE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tinycrypt/aes.h"
#include "tinycrypt/sha256.h"
#include "tui.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

_Bool flag_version = 0, flag_help = 0, flag_file = 0, flag_mode = 0,
      flag_output = 0, flag_tui = 0;

char file[128], out_file[128];

enum crypt_mode { NONE = 0, SHA256, AES128 };

static enum crypt_mode mode = NONE;

void usage(void) {
  puts("Usage: crypt [OPTION...] [FILE...]\n"
       "simple hashing and encrypting/decrypting program.\n"
       "\n"
       "-v       print version\n"
       "-t       run in TUI mode\n"
       "-h       print help\n"
       "-m <arg> specify mode which can be either sha256 or aes128\n"
       "-o <arg> specify out file to write aes encrypted/decrypted file, if "
       "not specified, it will print to stdout\n"
       "-f <arg> specify file to encrypt/decrypt");
}

void handle_parsed_arguments(void) {
  if (flag_version) {
    printf("VERSION: v%d.%d\n", 0, 1);
    exit(EXIT_SUCCESS);
  }

  if (flag_help) {
    usage();
    exit(EXIT_FAILURE);
  }

  if (!flag_file) {
    perror("Wrong usage specify file to encrypt/decrypt or hash");
    usage();
    exit(EXIT_FAILURE);
  }

  if (!flag_mode) {
    perror("Wrong usage specify mode");
    usage();
    exit(EXIT_FAILURE);
  }
}

void parse_cmd_arguments(int argc, char **argv, const char *optstring) {
  if (!optstring)
    return;
  int option = -1;

  while ((option = getopt(argc, argv, optstring)) != -1) {
    switch (option) {
    case 'm':
      flag_mode = 1;
      if (strncmp(optarg, "sha256", 6) == 0)
        mode = SHA256;
      else if (strncmp(optarg, "aes128", 6) == 0)
        mode = AES128;
      else {
        perror("Wrong mode, it can be either sha256 or aes128");
        usage();
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      flag_version = 1;
      break;
    case 't':
      flag_tui = 1;
      break;
    case 'o':
      flag_output = 1;
      strncpy(out_file, optarg, 128);
      out_file[128 - 1] = '\0';
      break;
    case 'h':
      flag_help = 1;
      break;
    case 'f':
      flag_file = 1;
      strncpy(file, optarg, 128);
      file[128 - 1] = '\0';
      break;
    case ':':
      printf("Option %c needs a value\n", optopt);
      exit(EXIT_FAILURE);
    case '?':
      printf("Unknown option used: -%c, type -h to get help\n", optopt);
      exit(EXIT_FAILURE);
    default:
      break;
    }
  }

  for (; optind < argc; optind++) {
    perror("Unknown argument entered, type -h to get help");
    exit(EXIT_FAILURE);
  }

  handle_parsed_arguments();
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("give a argument type -h to get help");
    exit(EXIT_FAILURE);
  }

  parse_cmd_arguments(argc, argv, ":vthf:m:o:");

  if (flag_tui) {
    draw_tui();
  } else {
    FILE *in_file = fopen(file, "rb");
    if (!in_file) {
      perror("fopen failed");
      exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (stat(file, &sb) == -1) {
      perror("stat failed");
      exit(EXIT_FAILURE);
    }

    uint8_t *file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, in_file);

    if (mode == SHA256) { /* SHA mode */
      struct tc_sha256_state *sha256_ctx =
          malloc(sizeof(struct tc_sha256_state));
      if (!tc_sha256_init(sha256_ctx))
        puts("fail_sha_init");

      if (!tc_sha256_update(sha256_ctx, file_contents, sb.st_size))
        perror("fail_sha_update");

      uint8_t digest[32] = {0};
      if (!tc_sha256_final(digest, sha256_ctx))
        perror("fail_sha_final");

      for (size_t i = 0; i < 32; i++)
        printf("%02x", digest[i]);
      printf("\t%s\n", file);

      free(sha256_ctx);
    } else if (mode == AES128) { /* AES mode */
      uint8_t remaining = sb.st_size % 16;
      size_t i = (remaining != 0) ? sb.st_size + (16 - remaining) : sb.st_size;

      srand(time(NULL));
      uint8_t encrypt_key[16] = {0};
      uint8_t decrypt_key[16] = {0};

      struct stat keys_file;
      if (stat(".aes_keys", &keys_file) == 0) { /* key exist */
        uint8_t remain = 0;
        FILE *keys = fopen(".aes_keys", "rb");
        if (!keys)
          puts("opening keys file failed");
        fread(encrypt_key, 16, 1, keys);
        fseek(keys, 16, SEEK_SET);
        fread(decrypt_key, 16, 1, keys);
        fseek(keys, 32, SEEK_SET);
        fread(&remain, 1, 1, keys);
        remaining = remain == remaining ? remaining : remain;
        fclose(keys);
      } else { /* key not exist */
        for (size_t i = 0; i < 16; ++i)
          encrypt_key[i] = (uint8_t)rand();
        for (size_t i = 0; i < 16; ++i)
          decrypt_key[i] = (uint8_t)rand();
        FILE *keys = fopen(".aes_keys", "wb");
        if (!keys)
          puts("opening keys file failed");
        fwrite(encrypt_key, 16, 1, keys);
        fseek(keys, 16, SEEK_SET);
        fwrite(decrypt_key, 16, 1, keys);
        fseek(keys, 32, SEEK_SET);
        fwrite(&remaining, 1, 1, keys);
        fclose(keys);
      }

      struct tc_aes_key_sched *aes_ctx =
          malloc(sizeof(struct tc_aes_key_sched));

      if (!tc_aes128_set_encrypt_key(aes_ctx, encrypt_key))
        puts("fail_aes_key_encrypt");
      if (!tc_aes128_set_decrypt_key(aes_ctx, decrypt_key))
        puts("fail_aes_key_decrypt");

      if (strncmp(file + strlen(file) - 4, ".aes", 4) == 0) {
        uint8_t *decrypted_file = calloc(1, i);
        for (size_t k = 0; k < (i / 16); ++k)
          if (!tc_aes_decrypt(decrypted_file + 16 * k, file_contents + 16 * k,
                              aes_ctx))
            puts("fail_aes_decrypt");

        if (flag_output) {
          FILE *out_decrypted_file = fopen(out_file, "w");
          fwrite(decrypted_file, i - remaining, 1, out_decrypted_file);
          fclose(out_decrypted_file);
        } else {
          printf("decrypted:\n%s\n", (char *)decrypted_file);
        }
      } else {
        uint8_t *encrypted_file = calloc(1, i);
        for (size_t k = 0; k < (i / 16); ++k)
          if (!tc_aes_encrypt(encrypted_file + 16 * k, file_contents + 16 * k,
                              aes_ctx))
            puts("fail_aes_encrypt");

        if (flag_output) {
          strncat(out_file, ".aes", 5);
          FILE *out_encrypted_file = fopen(out_file, "wb");
          fwrite(encrypted_file, i, 1, out_encrypted_file);
          fclose(out_encrypted_file);
        } else {
          printf("encrypted:\n%s\n", (char *)encrypted_file);
        }
      }

      free(aes_ctx);
    }

    fclose(in_file);
    free(file_contents);
  }

  return 0;
}
