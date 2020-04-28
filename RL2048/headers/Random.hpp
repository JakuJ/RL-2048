#pragma once

int random(int max) {
    thread_local auto seed = (unsigned int) time(nullptr);
    return rand_r(&seed) % max;
}
