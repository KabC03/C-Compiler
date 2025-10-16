#include "Vector_test.h"



#define CHECK(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "CHECK FAILED: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        abort(); \
    } \
} while (0)

#define REQUIRE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "REQUIRE FAILED: %s (%s) at %s:%d\n", #cond, msg, __FILE__, __LINE__); \
        abort(); \
    } \
} while (0)

typedef struct {
    int32_t  id;
    char     tag[8];
    uint64_t sum;
} Elem;

static uint64_t checksum_for(int32_t id, const char *tag) {
    // simple deterministic checksum
    uint64_t s = 1469598103934665603ull;
    for (const unsigned char *p = (const unsigned char*)&id; p < (const unsigned char*)(&id)+sizeof(id); ++p) {
        s ^= *p; s *= 1099511628211ull;
    }
    for (const unsigned char *p = (const unsigned char*)tag; *p; ++p) {
        s ^= *p; s *= 1099511628211ull;
    }
    return s;
}

static Elem make_elem(int32_t id) {
    Elem e;
    e.id = id;
    snprintf(e.tag, sizeof(e.tag), "E%06d", id % 1000000);
    e.sum = checksum_for(e.id, e.tag);
    return e;
}

static void assert_elem_eq(const Elem *a, const Elem *b) {
    CHECK(a->id == b->id);
    CHECK(0 == memcmp(a->tag, b->tag, sizeof(a->tag)));
    CHECK(a->sum == b->sum);
}

static void test_push_and_growth_pattern(void) {
    Vector v;
    vector_init(&v, sizeof(Elem));
    REQUIRE(v.top == 0 && v.capacity == 0 && v.data == NULL, "init fields");

    // Push a bunch; verify size & contents
    const size_t N = 5000;
    for (size_t i = 0; i < N; ++i) {
        Elem e = make_elem((int32_t)i);
        CHECK(vector_push_back(&v, &e));
        CHECK(vector_get_size(&v) == i + 1);

        // spot-check a few recent elements
        if (i == 0 || i == 1 || i == 2 || i % 777 == 0 || i == N-1) {
            const Elem *got = (const Elem*)vector_get_index(&v, i);
            REQUIRE(got != NULL, "get_index must not be NULL for valid index");
            assert_elem_eq(got, &e);
        }

        // capacity monotonic non-decreasing
        if (i > 0) CHECK(v.capacity >= v.top);
    }

    // Bounds checks
    CHECK(vector_get_index(&v, N) == NULL);
    CHECK(vector_get_index(&v, (size_t)-1) == NULL); // huge index → out of range

    vector_destroy(&v);
    CHECK(v.top == 0 && v.capacity == 0 && v.data == NULL);
}

static void test_set_and_get(void) {
    Vector v;
    vector_init(&v, sizeof(Elem));

    const size_t N = 256;
    for (size_t i = 0; i < N; ++i) {
        Elem e = make_elem((int32_t)(1000 + (int)i));
        CHECK(vector_push_back(&v, &e));
    }
    CHECK(v.top == N);

    // In-range set
    for (size_t i = 0; i < N; i += 5) {
        Elem e = make_elem((int32_t)(200000 + (int)i));
        CHECK(vector_set_index(&v, i, &e));
        const Elem *got = (const Elem*)vector_get_index(&v, i);
        REQUIRE(got != NULL, "set target must exist");
        assert_elem_eq(got, &e);
    }

    // Out-of-range set must fail (index == top)
    Elem dummy = make_elem(42);
    CHECK(!vector_set_index(&v, N, &dummy));
    CHECK(!vector_set_index(&v, N + 1000, &dummy));

    vector_destroy(&v);
}

static void test_resize_shrink_and_grow(void) {
    Vector v;
    vector_init(&v, sizeof(Elem));

    // Start with 100
    for (int i = 0; i < 100; ++i) {
        Elem e = make_elem(i);
        CHECK(vector_push_back(&v, &e));
    }
    CHECK(v.top == 100);
    CHECK(v.capacity >= 100);

    // Shrink to 37 → top must become 37; items after 36 inaccessible
    CHECK(vector_resize(&v, 37));
    CHECK(v.capacity == 37);
    CHECK(v.top == 37);
    CHECK(vector_get_index(&v, 36) != NULL);
    CHECK(vector_get_index(&v, 37) == NULL);

    // Grow to 500; existing values preserved
    CHECK(vector_resize(&v, 500));
    CHECK(v.capacity == 500);
    CHECK(v.top == 37);
    for (int i = 0; i < 37; ++i) {
        Elem expected = make_elem(i);
        const Elem *got = (const Elem*)vector_get_index(&v, (size_t)i);
        REQUIRE(got != NULL, "existing element must persist after grow");
        assert_elem_eq(got, &expected);
    }

    // Resize to zero; semantics of realloc(0) vary (may NULL data),
    // but capacity/top must be 0 and push_back must still work afterwards.
    CHECK(vector_resize(&v, 0));
    CHECK(v.capacity == 0);
    CHECK(v.top == 0);
    // We won't assume v.data is NULL here—C allows either—just ensure API still works.
    Elem e = make_elem(777);
    CHECK(vector_push_back(&v, &e));
    CHECK(v.top == 1);
    const Elem *got = (const Elem*)vector_get_index(&v, 0);
    REQUIRE(got != NULL, "first element after zero-resize");
    assert_elem_eq(got, &e);

    vector_destroy(&v);
}

static void test_byte_elements_and_contiguity(void) {
    Vector vb;
    vector_init(&vb, sizeof(uint8_t));

    const size_t N = 4096;
    // Mirror buffer to verify contiguous content
    uint8_t *mirror = malloc(N);
    REQUIRE(mirror, "malloc mirror");

    for (size_t i = 0; i < N; ++i) {
        uint8_t val = (uint8_t)(i & 0xFF);
        mirror[i] = val;
        CHECK(vector_push_back(&vb, &val));
    }
    CHECK(vb.top == N);

    // Verify contiguous memory equals mirror (byte-for-byte)
    REQUIRE(vb.data != NULL, "data must exist");
    CHECK(memcmp(vb.data, mirror, N) == 0);

    // Modify some indices via set and re-compare slices
    for (size_t i = 0; i < N; i += 97) {
        uint8_t val = (uint8_t)(0xA5 ^ (uint8_t)i);
        mirror[i] = val;
        CHECK(vector_set_index(&vb, i, &val));
    }
    CHECK(memcmp(vb.data, mirror, N) == 0);

    // Shrink, then ensure the truncated tail is inaccessible and memcpy matches
    size_t newN = 1234;
    CHECK(vector_resize(&vb, newN));
    CHECK(vb.top == newN && vb.capacity == newN);
    CHECK(memcmp(vb.data, mirror, newN) == 0);
    CHECK(vector_get_index(&vb, newN) == NULL);

    free(mirror);
    vector_destroy(&vb);
}

static void fuzz_against_mirror(void) {
    // Random but reproducible
    srand(1234567);

    Vector v;
    vector_init(&v, sizeof(Elem));

    // Simple mirror using plain malloc/realloc and top count
    Elem *mirror = NULL;
    size_t mcap = 0, mtop = 0;

    const int STEPS = 20000;
    for (int step = 0; step < STEPS; ++step) {
        int op = rand() % 4; // 0: push, 1: set, 2: resize grow, 3: resize shrink
        switch (op) {
            case 0: { // push
                Elem e = make_elem(step);
                // mirror grow (double+1 policy for speed; independent of Vector’s policy)
                if (mtop == mcap) {
                    size_t nc = mcap ? (mcap * 2) : 1;
                    Elem *tmp = realloc(mirror, nc * sizeof(Elem));
                    REQUIRE(tmp, "mirror realloc");
                    mirror = tmp; mcap = nc;
                }
                mirror[mtop] = e; mtop++;
                CHECK(vector_push_back(&v, &e));
                CHECK(v.top == mtop);
            } break;
            case 1: { // set
                if (mtop == 0) break;
                size_t i = (size_t)(rand() % (int)mtop);
                Elem e = make_elem(step ^ 0x55AA);
                mirror[i] = e;
                CHECK(vector_set_index(&v, i, &e));
                const Elem *got = (const Elem*)vector_get_index(&v, i);
                REQUIRE(got != NULL, "set target should exist in fuzz");
                assert_elem_eq(got, &e);
            } break;
            case 2: { // resize grow
                size_t growTo = (size_t)(mtop + (rand() % 50));
                // mirror grow capacity only; top unchanged
                if (growTo > mcap) {
                    Elem *tmp = realloc(mirror, growTo * sizeof(Elem));
                    REQUIRE(tmp, "mirror growth realloc");
                    mirror = tmp; mcap = growTo;
                }
                CHECK(vector_resize(&v, growTo));
                CHECK(v.capacity == growTo);
                CHECK(v.top == mtop);
            } break;
            case 3: { // resize shrink
                size_t shrinkTo = (mtop == 0) ? 0 : (size_t)(rand() % (int)mtop);
                mtop = shrinkTo;
                CHECK(vector_resize(&v, shrinkTo));
                CHECK(v.capacity == shrinkTo);
                CHECK(v.top == shrinkTo);
            } break;
        }

        // After each step, compare prefix of contents (up to v.top == mtop)
        for (size_t i = 0; i < v.top; ++i) {
            const Elem *got = (const Elem*)vector_get_index(&v, i);
            REQUIRE(got != NULL, "prefix compare - get must succeed");
            assert_elem_eq(got, &mirror[i]);
        }

        // Bounds must fail
        CHECK(vector_get_index(&v, v.top) == NULL);
    }

    free(mirror);
    vector_destroy(&v);
}

void vector_test(void) {
    printf("[1/5] push & growth…\n");
    test_push_and_growth_pattern();
    printf("[2/5] set & get…\n");
    test_set_and_get();
    printf("[3/5] resize shrink/grow/zero…\n");
    test_resize_shrink_and_grow();
    printf("[4/5] byte elements & contiguity…\n");
    test_byte_elements_and_contiguity();
    printf("[5/5] randomized fuzz vs mirror…\n");
    fuzz_against_mirror();

    printf("All harsh tests passed ✅\n");
    return ;
}



