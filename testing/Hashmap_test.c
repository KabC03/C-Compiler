#include "Hashmap_test.h"


// hashmap_test.c
// Compile:  cc -O2 -Wall -Wextra -pedantic hashmap_test.c your_hashmap_impl.c -o hmap_tests
// Run:      ./hmap_tests

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// ---- from Hashmap.h (assumed) ----
typedef struct Bucket Bucket;

typedef struct Hashmap {
    size_t keySize;
    size_t valueSize;
    Bucket *buckets;
    size_t numBuckets;
} Hashmap;

// Public API we are testing (assumed signatures)
void        hashmap_init   (Hashmap *hashmap, size_t keySize, size_t valueSize);
void        hashmap_destroy(Hashmap *hashmap);
bool        hashmap_insert (Hashmap *hashmap, void *key, void *value);
bool        hashmap_set    (Hashmap *hashmap, void *key, void *newValue);
const void* hashmap_find   (Hashmap *hashmap, void *key);
bool        hashmap_delete (Hashmap *hashmap, void *key);

// ---------- tiny test framework ----------
#define CHECK(cond) do { if(!(cond)) { \
    fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); exit(1); } \
} while(0)

#define REQUIRE_EQ_INT(a,b) CHECK((a)==(b))
#define REQUIRE_TRUE(a)     CHECK((a))
#define REQUIRE_FALSE(a)    CHECK(!(a))

static void banner(const char *name) {
    printf("\n=== %s ===\n", name);
}

// ---------- deterministic PRNG (xorshift64*) ----------
static uint64_t rng_state = 0x123456789abcdefULL;
static uint64_t xorshift64s(void){
    uint64_t x = rng_state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    rng_state = x;
    return x * 2685821657736338717ULL;
}

static void fill_random(void *p, size_t n) {
    uint8_t *b = (uint8_t*)p;
    for (size_t i=0;i<n;i++) b[i] = (uint8_t)(xorshift64s() & 0xFF);
}

// ---------- simple reference map (sorted vector) ----------
typedef struct {
    size_t keySize, valueSize;
    size_t n, cap;
    uint8_t *data; // [key|value] repeated
} RefMap;

static void ref_init(RefMap *rm, size_t ks, size_t vs) {
    rm->keySize = ks; rm->valueSize = vs; rm->n = 0; rm->cap = 0; rm->data = NULL;
}
static void ref_destroy(RefMap *rm) { free(rm->data); rm->data = NULL; rm->n = rm->cap = 0; }

static int ref_cmp_key(const void *a, const void *b, void *ks_ptr){
    size_t ks = *(size_t*)ks_ptr;
    return memcmp(a,b,ks);
}
static uint8_t* ref_key_at(RefMap *rm, size_t i){ return rm->data + i*(rm->keySize+rm->valueSize); }
static uint8_t* ref_val_at(RefMap *rm, size_t i){ return ref_key_at(rm,i)+rm->keySize; }

static ssize_t ref_lower_bound(RefMap *rm, const void *key) {
    size_t lo=0, hi=rm->n;
    while (lo<hi){
        size_t mid=(lo+hi)/2;
        int c = memcmp(ref_key_at(rm,mid), key, rm->keySize);
        if (c < 0) lo=mid+1; else hi=mid;
    }
    return (ssize_t)lo;
}
static bool ref_find(RefMap *rm, const void *key, size_t *idx_out) {
    ssize_t i = ref_lower_bound(rm, key);
    if ((size_t)i < rm->n && memcmp(ref_key_at(rm,i), key, rm->keySize)==0) {
        if (idx_out) *idx_out=(size_t)i;
        return true;
    }
    return false;
}
static bool ref_insert(RefMap *rm, const void *key, const void *val){
    size_t idx;
    if (ref_find(rm,key,&idx)) return false;
    if (rm->n==rm->cap){
        rm->cap = rm->cap? rm->cap*2 : 8;
        rm->data = (uint8_t*)realloc(rm->data, rm->cap*(rm->keySize+rm->valueSize));
        CHECK(rm->data);
    }
    // move tail
    memmove(ref_key_at(rm,idx)+(rm->keySize+rm->valueSize),
            ref_key_at(rm,idx),
            (rm->n-idx)*(rm->keySize+rm->valueSize));
    memcpy(ref_key_at(rm,idx), key, rm->keySize);
    memcpy(ref_val_at(rm,idx), val, rm->valueSize);
    rm->n++;
    return true;
}
static bool ref_set(RefMap *rm, const void *key, const void *val){
    size_t idx;
    if (!ref_find(rm,key,&idx)) return false;
    memcpy(ref_val_at(rm,idx), val, rm->valueSize);
    return true;
}
static bool ref_delete(RefMap *rm, const void *key){
    size_t idx;
    if (!ref_find(rm,key,&idx)) return false;
    memmove(ref_key_at(rm,idx),
            ref_key_at(rm,idx)+(rm->keySize+rm->valueSize),
            (rm->n-idx-1)*(rm->keySize+rm->valueSize));
    rm->n--;
    return true;
}

// ---------- helpers ----------
static void must_find_int(Hashmap *hm, const void *key, int expect){
    const int *p = (const int*)hashmap_find(hm, (void*)key);
    CHECK(p != NULL);
    REQUIRE_EQ_INT(*p, expect);
}
static void must_absent(Hashmap *hm, const void *key){
    const void *p = hashmap_find(hm, (void*)key);
    CHECK(p == NULL);
}

// ---------- Tests ----------
static void test_basic_ints(void){
    banner("basic: insert/find/update/delete with ints");
    Hashmap hm = {0};
    hm.numBuckets = 64; // typical
    hashmap_init(&hm, sizeof(int), sizeof(int));

    for (int i=0;i<1000;i++){
        int k=i, v=i*10;
        REQUIRE_TRUE(hashmap_insert(&hm, &k, &v));
    }
    // duplicate insert should fail
    for (int i=0;i<1000;i++){
        int k=i, v=12345;
        REQUIRE_FALSE(hashmap_insert(&hm, &k, &v));
    }
    // find
    for (int i=0;i<1000;i++){
        int k=i;
        must_find_int(&hm, &k, i*10);
    }
    // set update
    for (int i=0;i<1000;i+=3){
        int k=i, v=-i;
        REQUIRE_TRUE(hashmap_set(&hm, &k, &v));
    }
    for (int i=0;i<1000;i++){
        int k=i;
        int expect = (i%3==0)? -i : i*10;
        must_find_int(&hm, &k, expect);
    }
    // delete half
    for (int i=0;i<1000;i+=2){
        int k=i;
        REQUIRE_TRUE(hashmap_delete(&hm, &k));
    }
    // ensure deletes stuck, odds remain
    for (int i=0;i<1000;i++){
        int k=i;
        if (i%2==0) must_absent(&hm, &k);
        else        must_find_int(&hm, &k, (i%3==0)? -i : i*10);
    }
    hashmap_destroy(&hm);
    printf("[ok] basic\n");
}

typedef struct {
    // deliberately awkward/padded key (24 bytes)
    uint64_t a;
    uint32_t b;
    uint8_t  tag[12];
} BigKey;

typedef struct {
    double   x;
    int64_t  y;
} BigVal;

static void test_big_struct_keys_and_values(void){
    banner("struct keys/values (copy semantics, padding, alignment)");
    Hashmap hm = (Hashmap){0};
    hm.numBuckets = 97;
    hashmap_init(&hm, sizeof(BigKey), sizeof(BigVal));

    // Insert 200 unique items
    for (int i=0;i<200;i++){
        BigKey k={0}; BigVal v={0};
        k.a = 0xABCDEF0000000000ULL + (uint64_t)i;
        k.b = (uint32_t)(i*i + 7);
        memset(k.tag, (uint8_t)i, sizeof k.tag);

        v.x = (double)i * 1.25;
        v.y = (int64_t)i * (int64_t)i * -13;

        REQUIRE_TRUE(hashmap_insert(&hm, &k, &v));

        // mutate the local key+value buffers to ensure the map made deep copies
        memset(&k, 0xEE, sizeof k);
        memset(&v, 0xCC, sizeof v);
    }

    // Query those keys again (rebuild the same keys)
    for (int i=0;i<200;i++){
        BigKey k={0};
        k.a = 0xABCDEF0000000000ULL + (uint64_t)i;
        k.b = (uint32_t)(i*i + 7);
        memset(k.tag, (uint8_t)i, sizeof k.tag);

        const BigVal *pv = (const BigVal*)hashmap_find(&hm, &k);
        CHECK(pv != NULL);
        CHECK(pv->x == (double)i * 1.25);
        CHECK(pv->y == (int64_t)i * (int64_t)i * -13);
    }

    // set() a few
    for (int i=0;i<200;i+=17){
        BigKey k={0};
        k.a = 0xABCDEF0000000000ULL + (uint64_t)i;
        k.b = (uint32_t)(i*i + 7);
        memset(k.tag, (uint8_t)i, sizeof k.tag);

        BigVal nv = { .x = 3.14159*i, .y = 42-i };
        REQUIRE_TRUE(hashmap_set(&hm, &k, &nv));
        // verify immediately
        const BigVal *pv = (const BigVal*)hashmap_find(&hm, &k);
        CHECK(pv && pv->x == nv.x && pv->y == nv.y);
    }

    hashmap_destroy(&hm);
    printf("[ok] struct keys/values\n");
}

static void test_forced_collisions_and_resizing(void){
    banner("forced collisions (numBuckets=1) and bucket resizing");
    Hashmap hm = {0};
    hm.numBuckets = 1; // every key collides into the same bucket
    hashmap_init(&hm, sizeof(uint64_t), sizeof(uint64_t));

    const int N = 5000;
    for (int i=0;i<N;i++){
        uint64_t k = (uint64_t)i ^ 0x9E3779B97F4A7C15ULL;
        uint64_t v = ~k;
        REQUIRE_TRUE(hashmap_insert(&hm, &k, &v));
    }
    for (int i=0;i<N;i++){
        uint64_t k = (uint64_t)i ^ 0x9E3779B97F4A7C15ULL;
        const uint64_t *pv = (const uint64_t*)hashmap_find(&hm, &k);
        CHECK(pv && *pv == ~k);
    }
    // delete many, leave a pattern
    for (int i=0;i<N;i+=3){
        uint64_t k = (uint64_t)i ^ 0x9E3779B97F4A7C15ULL;
        REQUIRE_TRUE(hashmap_delete(&hm, &k));
        const void *p = hashmap_find(&hm, &k);
        CHECK(p == NULL);
    }
    // remaining ones still present?
    for (int i=0;i<N;i++){
        uint64_t k = (uint64_t)i ^ 0x9E3779B97F4A7C15ULL;
        const void *p = hashmap_find(&hm, &k);
        if (i%3==0) CHECK(p==NULL); else CHECK(p!=NULL);
    }

    hashmap_destroy(&hm);
    printf("[ok] collisions+resize\n");
}

static void test_set_nonexistent_and_find_absent(void){
    banner("set() on nonexistent; find() absent");
    Hashmap hm = {0};
    hm.numBuckets = 32;
    hashmap_init(&hm, sizeof(int), sizeof(int));

    int k=123, v=999;
    REQUIRE_FALSE(hashmap_set(&hm, &k, &v)); // not present yet
    must_absent(&hm, &k);
    REQUIRE_FALSE(hashmap_delete(&hm, &k));
    must_absent(&hm, &k);

    hashmap_destroy(&hm);
    printf("[ok] set/find/delete on absent keys\n");
}

// A heavy randomized differential test vs RefMap
static void test_fuzz_against_reference(void){
    banner("randomized fuzz vs reference implementation (100k ops)");
    const size_t KS = 16, VS = 8;
    Hashmap hm = {0};
    hm.numBuckets = 257;
    hashmap_init(&hm, KS, VS);

    RefMap ref; ref_init(&ref, KS, VS);

    const int OPS = 100000;
    uint8_t key[KS], val[VS];

    for (int i=0;i<OPS;i++){
        int op = (int)(xorshift64s() % 100);
        fill_random(key, KS);
        fill_random(val, VS);

        if (op < 45) { // insert
            bool ok1 = hashmap_insert(&hm, key, val);
            bool ok2 = ref_insert(&ref, key, val);
            CHECK(ok1 == ok2);
        } else if (op < 70) { // set
            bool ok1 = hashmap_set(&hm, key, val);
            bool ok2 = ref_set(&ref, key, val);
            CHECK(ok1 == ok2);
        } else if (op < 90) { // delete
            bool ok1 = hashmap_delete(&hm, key);
            bool ok2 = ref_delete(&ref, key);
            CHECK(ok1 == ok2);
        } else { // find
            const void *p1 = hashmap_find(&hm, key);
            size_t idx;
            bool f2 = ref_find(&ref, key, &idx);
            if (!f2) {
                CHECK(p1 == NULL);
            } else {
                CHECK(p1 != NULL);
                CHECK(memcmp(p1, ref_val_at(&ref, idx), VS) == 0);
            }
        }
    }

    // final sweep: every ref key must be found with exact value
    for (size_t i=0;i<ref.n;i++){
        const void *p = hashmap_find(&hm, ref_key_at(&ref,i));
        CHECK(p && memcmp(p, ref_val_at(&ref,i), VS)==0);
    }

    ref_destroy(&ref);
    hashmap_destroy(&hm);
    printf("[ok] fuzz\n");
}

static void test_copy_semantics(void){
    banner("copy semantics: changing caller buffers after insert must not affect stored values");
    Hashmap hm = {0};
    hm.numBuckets = 11;
    hashmap_init(&hm, 8, 8);

    uint8_t key[8] = {0,1,2,3,4,5,6,7};
    uint8_t val[8] = {10,20,30,40,50,60,70,80};

    REQUIRE_TRUE(hashmap_insert(&hm, key, val));
    // mutate originals
    memset(key, 0xAA, sizeof key);
    memset(val, 0xBB, sizeof val);

    // Rebuild the original key to find (since we mutated it)
    uint8_t origKey[8] = {0,1,2,3,4,5,6,7};
    const uint8_t *found = (const uint8_t*)hashmap_find(&hm, origKey);
    CHECK(found != NULL);

    // Value inside map must still be the original {10,20,...,80}
    uint8_t expect[8] = {10,20,30,40,50,60,70,80};
    CHECK(memcmp(found, expect, 8) == 0);

    hashmap_destroy(&hm);
    printf("[ok] copy semantics\n");
}

int main(void){
    // Make tests deterministic across runs
    rng_state = 0xDEADFEEDCAFEBEEF;

    test_basic_ints();
    test_big_struct_keys_and_values();
    test_forced_collisions_and_resizing();
    test_set_nonexistent_and_find_absent();
    test_copy_semantics();
    test_fuzz_against_reference();

    printf("\nALL TESTS PASSED ✔️\n");
    return 0;
}


