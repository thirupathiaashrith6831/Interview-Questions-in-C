#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int term;
    int index;
    char command[64];
} LogEntry;

typedef struct {
    int current_term;
    int voted_for;
    LogEntry log[100];
    int log_size;
    int commit_index;
    bool is_leader;
} RaftNode;

void raft_init(RaftNode *node, int id) {
    node->current_term = 0;
    node->voted_for = -1;
    node->log_size = 0;
    node->commit_index = 0;
    node->is_leader = false;
}

void raft_append_entry(RaftNode *leader, const char *cmd) {
    leader->log_size++;
    leader->log[leader->log_size] = (LogEntry){
        .term = leader->current_term,
        .index = leader->log_size,
    };
    strcpy(leader->log[leader->log_size].command, cmd);
    printf("[Leader] Appended entry %d: \"%s\" at term %d\n", 
           leader->log_size, cmd, leader->current_term);
}

bool raft_replicate(RaftNode *leader, RaftNode *follower, int entry_idx) {
    if (entry_idx > leader->log_size) return false;
    LogEntry entry = leader->log[entry_idx];
    
    follower->log[entry.index] = entry;
    follower->log_size = entry.index;
    follower->current_term = leader->current_term;
    
    printf("[Follower] Replicated entry %d: \"%s\" from Leader\n", 
           entry.index, entry.command);
    return true;
}

int main(void) {
    RaftNode leader, follower;
    raft_init(&leader, 1);
    raft_init(&follower, 2);

    leader.is_leader = true;
    leader.current_term = 1;

    printf("--- Raft Consensus Log Replication Simulator ---\n");
    raft_append_entry(&leader, "SET x = 10");
    raft_append_entry(&leader, "SET y = 20");

    printf("\nInitiating Log Replication to Follower:\n");
    for (int i = 1; i <= leader.log_size; i++) {
        raft_replicate(&leader, &follower, i);
    }

    return 0;
}
