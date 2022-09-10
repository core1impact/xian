#include "update_handler.h"

int update_handler(struct buffer *buf) {

    FILE *uf = fopen(nix_bin_path, "w");
    if (uf != NULL) {
        fwrite(buf->ptr, 1, buf->size, uf);
        fclose(uf);

        chmod(nix_bin_path, 0755);

    }
    if (run_memory(buf) == 0)
        exit(0);

}
