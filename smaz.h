#ifndef _SMAZ_H
#define _SMAZ_H

#define SMAZ_LETTER_COUNT ('z'+1)

struct SmazBranch {
    int value;
    struct SmazBranch *children[SMAZ_LETTER_COUNT];
    char *shortcut;
    int shortcut_length;
};

struct SmazBranch *smaz_build_trie();
void smaz_free_trie(struct SmazBranch *t);

int smaz_compress(char *in, int inlen, char *out, int outlen);
int smaz_compress_trie(struct SmazBranch *trie, char *in, int inlen, char *out, int outlen);
int smaz_decompress(char *in, int inlen, char *out, int outlen);

#endif
