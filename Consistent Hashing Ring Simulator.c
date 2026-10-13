#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RING_CAPACITY 100

typedef struct {
    uint32_t hash;
    int node_id;
} RingNode;

typedef struct {
    RingNode ring[RING_CAPACITY];
    int size;
} ConsistentHashRing;

uint32_t simple_hash(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 1000;
}

void ring_init(ConsistentHashRing *chr) {
    chr->size = 0;
}

void ring_add_node(ConsistentHashRing *chr, int node_id, const char *node_name) {
    if (chr->size >= RING_CAPACITY) return;
    uint32_t h = simple_hash(node_name);
    
    chr->ring[chr->size++] = (RingNode){h, node_id};
    
    // Sort ring by hash value
    for (int i = 0; i < chr->size - 1; i++) {
        for (int j = 0; j < chr->size - i - 1; j++) {
            if (chr->ring[j].hash > chr->ring[j + 1].hash) {
                RingNode temp = chr->ring[j];
                chr->ring[j] = chr->ring[j + 1];
                chr->ring[j + 1] = temp;
            }
        }
    }
}

int ring_get_node(ConsistentHashRing *chr, const char *key) {
    if (chr->size == 0) return -1;
    uint32_t h = simple_hash(key);

    for (int i = 0; i < chr->size; i++) {
        if (h <= chr->ring[i].hash) {
            return chr->ring[i].node_id;
        }
    }
    return chr->ring[0].node_id; // Wrap around to first node
}

int main(void) {
    ConsistentHashRing ring;
    ring_init(&ring);

    printf("--- Consistent Hashing Ring Simulator ---\n");
    ring_add_node(&ring, 101, "Server_A");
    ring_add_node(&ring, 102, "Server_B");
    ring_add_node(&ring, 103, "Server_C");

    const char *keys[] = {"user_session_1", "database_cache", "api_token_xyz", "image_asset_99"};
    for (size_t i = 0; i < 4; i++) {
        int assigned = ring_get_node(&ring, keys[i]);
        printf("Key '%-15s' -> Assigned Node: %d\n", keys[i], assigned);
    }

    return 0;
}
