#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct VEBNode {
    int universe;
    int min;
    int max;
    struct VEBNode *summary;
    struct VEBNode **cluster;
} VEBNode;

int veb_high(int x, int upper_sqrt) { return x / upper_sqrt; }
int veb_low(int x, int lower_sqrt)  { return x % lower_sqrt; }
int veb_index(int high, int low, int lower_sqrt) { return high * lower_sqrt + low; }

VEBNode* veb_create(int u) {
    VEBNode *node = malloc(sizeof(VEBNode));
    node->universe = u;
    node->min = -1;
    node->max = -1;

    if (u <= 2) {
        node->summary = NULL;
        node->cluster = NULL;
    } else {
        int upper_sqrt = (int)ceil(sqrt(u));
        int lower_sqrt = (int)floor(sqrt(u));

        node->summary = veb_create(upper_sqrt);
        node->cluster = malloc(sizeof(VEBNode*) * upper_sqrt);
        for (int i = 0; i < upper_sqrt; i++) {
            node->cluster[i] = veb_create(lower_sqrt);
        }
    }
    return node;
}

void veb_insert(VEBNode *v, int x) {
    if (v->min == -1) {
        v->min = v->max = x;
        return;
    }

    if (x < v->min) {
        int temp = x;
        x = v->min;
        v->min = temp;
    }

    if (v->universe > 2) {
        int upper_sqrt = (int)ceil(sqrt(v->universe));
        int lower_sqrt = (int)floor(sqrt(v->universe));
        int high = veb_high(x, lower_sqrt);
        int low = veb_low(x, lower_sqrt);

        if (v->cluster[high]->min == -1) {
            veb_insert(v->summary, high);
            v->cluster[high]->min = low;
            v->cluster[high]->max = low;
        } else {
            veb_insert(v->cluster[high], low);
        }
    }

    if (x > v->max) {
        v->max = x;
    }
}

bool veb_member(VEBNode *v, int x) {
    if (x == v->min || x == v->max) return true;
    if (v->universe <= 2) return false;

    int lower_sqrt = (int)floor(sqrt(v->universe));
    return veb_member(v->cluster[veb_high(x, lower_sqrt)], veb_low(x, lower_sqrt));
}

void veb_free(VEBNode *v) {
    if (!v) return;
    if (v->universe > 2) {
        int upper_sqrt = (int)ceil(sqrt(v->universe));
        for (int i = 0; i < upper_sqrt; i++) {
            veb_free(v->cluster[i]);
        }
        free(v->cluster);
        veb_free(v->summary);
    }
    free(v);
}

int main(void) {
    int universe_size = 16;
    VEBNode *veb = veb_create(universe_size);

    printf("--- Van Emde Boas (vEB) Tree ---\n");
    veb_insert(veb, 2);
    veb_insert(veb, 3);
    veb_insert(veb, 7);
    veb_insert(veb, 14);

    printf("Member 7 : %s\n", veb_member(veb, 7) ? "FOUND" : "NOT FOUND");
    printf("Member 9 : %s\n", veb_member(veb, 9) ? "FOUND" : "NOT FOUND");
    printf("Minimum  : %d\n", veb->min);
    printf("Maximum  : %d\n", veb->max);

    veb_free(veb);
    return 0;
}
